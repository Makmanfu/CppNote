

#include "stdafx.h"
#include "tinyxml2.h"
#include <new>      // yes, this one new style header, is in the Android SDK.
#include <cstddef>
#include <cstdarg>

#define TIXML_VSCPRINTF _vscprintf
#define TIXML_SSCANF    sscanf_s

static inline int TIXML_SNPRINTF(char* buffer, size_t size, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    int result = vsnprintf_s(buffer, size, _TRUNCATE, format, va);
    va_end(va);
    return result;
}

static inline int TIXML_VSNPRINTF(char* buffer, size_t size, const char* format, va_list va)
{
    int result = vsnprintf_s(buffer, size, _TRUNCATE, format, va);
    return result;
}

static const char LINE_FEED             = (char)0x0a;           // all line endings are normalized to LF
static const char LF = LINE_FEED;
static const char CARRIAGE_RETURN       = (char)0x0d;           // CR gets filtered out
static const char CR = CARRIAGE_RETURN;
static const char SINGLE_QUOTE          = '\'';
static const char DOUBLE_QUOTE          = '\"';

static const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
static const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
static const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

namespace tinyxml2
{
    struct Entity
    {
        const char* pattern;
        int length;
        char value;
    };

    static const int NUM_ENTITIES = 5;
    static const Entity entities[NUM_ENTITIES] =
    {
        { "quot", 4,    DOUBLE_QUOTE },
        { "amp", 3,     '&'  },
        { "apos", 4,    SINGLE_QUOTE },
        { "lt", 2,      '<'  },
        { "gt", 2,      '>'  }
    };


    StrPair::~StrPair()
    {
        Reset();
    }

    void StrPair::TransferTo(StrPair* other)
    {
        if (this == other)
            return;
        // This in effect implements the assignment operator by "moving"
        // ownership (as in auto_ptr).
        TIXMLASSERT(other != 0);
        TIXMLASSERT(other->_flags == 0);
        TIXMLASSERT(other->_start == 0);
        TIXMLASSERT(other->_end == 0);
        other->Reset();
        other->_flags = _flags;
        other->_start = _start;
        other->_end = _end;
        _flags = 0;
        _start = 0;
        _end = 0;
    }

    void StrPair::Reset()
    {
        if (_flags & NEEDS_DELETE)
            delete [] _start;
        _flags = 0;
        _start = 0;
        _end = 0;
    }

    void StrPair::SetStr(const char* str, int flags)
    {
        TIXMLASSERT(str);
        Reset();
        size_t len = strlen(str);
        TIXMLASSERT(_start == 0);
        _start = new char[ len + 1 ];
        memcpy(_start, str, len + 1);
        _end = _start + len;
        _flags = flags | NEEDS_DELETE;
    }

    char* StrPair::ParseText(char* p, const char* endTag, int strFlags, int* curLineNumPtr)
    {
        TIXMLASSERT(p);
        TIXMLASSERT(endTag && *endTag);
        TIXMLASSERT(curLineNumPtr);
        char* start = p;
        char  endChar = *endTag;
        size_t length = strlen(endTag);
        // Inner loop of text parsing.
        while (*p)
        {
            if (*p == endChar && strncmp(p, endTag, length) == 0)
            {
                Set(start, p, strFlags);
                return p + length;
            }
            else if (*p == '\n')
                ++(*curLineNumPtr);
            ++p;
            TIXMLASSERT(p);
        }
        return 0;
    }

    char* StrPair::ParseName(char* p)
    {
        if (!p || !(*p))
            return 0;
        if (!XMLUtil::IsNameStartChar(*p))
            return 0;
        char* const start = p;
        ++p;
        while (*p && XMLUtil::IsNameChar(*p))
            ++p;
        Set(start, p, 0);
        return p;
    }


    void StrPair::CollapseWhitespace()
    {
        // Adjusting _start would cause undefined behavior on delete[]
        TIXMLASSERT((_flags & NEEDS_DELETE) == 0);
        // Trim leading space.
        _start = XMLUtil::SkipWhiteSpace(_start, 0);
        if (*_start)
        {
            const char* p = _start; // the read pointer
            char* q = _start;   // the write pointer
            while (*p)
            {
                if (XMLUtil::IsWhiteSpace(*p))
                {
                    p = XMLUtil::SkipWhiteSpace(p, 0);
                    if (*p == 0)
                    {
                        break;    // don't write to q; this trims the trailing space.
                    }
                    *q = ' ';
                    ++q;
                }
                *q = *p;
                ++q;
                ++p;
            }
            *q = 0;
        }
    }

    const char* StrPair::GetStr()
    {
        TIXMLASSERT(_start);
        TIXMLASSERT(_end);
        if (_flags & NEEDS_FLUSH)
        {
            *_end = 0;
            _flags ^= NEEDS_FLUSH;
            if (_flags)
            {
                const char* p = _start; // the read pointer
                char* q = _start;   // the write pointer
                while (p < _end)
                {
                    if ((_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == CR)
                    {
                        // CR-LF pair becomes LF
                        // CR alone becomes LF
                        // LF-CR becomes LF
                        if (*(p + 1) == LF)
                            p += 2;
                        else
                            ++p;
                        *q = LF;
                        ++q;
                    }
                    else if ((_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == LF)
                    {
                        if (*(p + 1) == CR)
                            p += 2;
                        else
                            ++p;
                        *q = LF;
                        ++q;
                    }
                    else if ((_flags & NEEDS_ENTITY_PROCESSING) && *p == '&')
                    {
                        // Entities handled by tinyXML2:
                        // - special entities in the entity table [in/out]
                        // - numeric character reference [in]
                        //   &#20013; or &#x4e2d;
                        if (*(p + 1) == '#')
                        {
                            const int buflen = 10;
                            char buf[buflen] = { 0 };
                            int len = 0;
                            char* adjusted = const_cast<char*>(XMLUtil::GetCharacterRef(p, buf, &len));
                            if (adjusted == 0)
                            {
                                *q = *p;
                                ++p;
                                ++q;
                            }
                            else
                            {
                                TIXMLASSERT(0 <= len && len <= buflen);
                                TIXMLASSERT(q + len <= adjusted);
                                p = adjusted;
                                memcpy(q, buf, len);
                                q += len;
                            }
                        }
                        else
                        {
                            bool entityFound = false;
                            for (int i = 0; i < NUM_ENTITIES; ++i)
                            {
                                const Entity& entity = entities[i];
                                if (strncmp(p + 1, entity.pattern, entity.length) == 0
                                    && *(p + entity.length + 1) == ';')
                                {
                                    // Found an entity - convert.
                                    *q = entity.value;
                                    ++q;
                                    p += entity.length + 2;
                                    entityFound = true;
                                    break;
                                }
                            }
                            if (!entityFound)
                            {
                                // fixme: treat as error?
                                ++p;
                                ++q;
                            }
                        }
                    }
                    else
                    {
                        *q = *p;
                        ++p;
                        ++q;
                    }
                }
                *q = 0;
            }
            // The loop below has plenty going on, and this
            // is a less useful mode. Break it out.
            if (_flags & NEEDS_WHITESPACE_COLLAPSING)
                CollapseWhitespace();
            _flags = (_flags & NEEDS_DELETE);
        }
        TIXMLASSERT(_start);
        return _start;
    }

    // --------- XMLUtil ----------- //

    const char* XMLUtil::writeBoolTrue  = "true";
    const char* XMLUtil::writeBoolFalse = "false";

    void XMLUtil::SetBoolSerialization(const char* writeTrue, const char* writeFalse)
    {
        static const char* defTrue  = "true";
        static const char* defFalse = "false";
        writeBoolTrue = (writeTrue) ? writeTrue : defTrue;
        writeBoolFalse = (writeFalse) ? writeFalse : defFalse;
    }

    const char* XMLUtil::ReadBOM(const char* p, bool* bom)
    {
        TIXMLASSERT(p);
        TIXMLASSERT(bom);
        *bom = false;
        const unsigned char* pu = reinterpret_cast<const unsigned char*>(p);
        // Check for BOM:
        if (*(pu + 0) == TIXML_UTF_LEAD_0
            && *(pu + 1) == TIXML_UTF_LEAD_1
            && *(pu + 2) == TIXML_UTF_LEAD_2)
        {
            *bom = true;
            p += 3;
        }
        TIXMLASSERT(p);
        return p;
    }

    void XMLUtil::ConvertUTF32ToUTF8(unsigned long input, char* output, int* length)
    {
        const unsigned long BYTE_MASK = 0xBF;
        const unsigned long BYTE_MARK = 0x80;
        const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
        if (input < 0x80)
            *length = 1;
        else if (input < 0x800)
            *length = 2;
        else if (input < 0x10000)
            *length = 3;
        else if (input < 0x200000)
            *length = 4;
        else
        {
            *length = 0;    // This code won't convert this correctly anyway.
            return;
        }
        output += *length;
        // Scary scary fall throughs.
        switch (*length)
        {
            case 4:
                --output;
                *output = (char)((input | BYTE_MARK) & BYTE_MASK);
                input >>= 6;
            case 3:
                --output;
                *output = (char)((input | BYTE_MARK) & BYTE_MASK);
                input >>= 6;
            case 2:
                --output;
                *output = (char)((input | BYTE_MARK) & BYTE_MASK);
                input >>= 6;
            case 1:
                --output;
                *output = (char)(input | FIRST_BYTE_MARK[*length]);
                break;
            default:
                TIXMLASSERT(false);
        }
    }

    const char* XMLUtil::GetCharacterRef(const char* p, char* value, int* length)
    {
        // Presume an entity, and pull it out.
        *length = 0;
        if (*(p + 1) == '#' && *(p + 2))
        {
            unsigned long ucs = 0;
            TIXMLASSERT(sizeof(ucs) >= 4);
            ptrdiff_t delta = 0;
            unsigned mult = 1;
            static const char SEMICOLON = ';';
            if (*(p + 2) == 'x')
            {
                // Hexadecimal.
                const char* q = p + 3;
                if (!(*q))
                    return 0;
                q = strchr(q, SEMICOLON);
                if (!q)
                    return 0;
                TIXMLASSERT(*q == SEMICOLON);
                delta = q - p;
                --q;
                while (*q != 'x')
                {
                    unsigned int digit = 0;
                    if (*q >= '0' && *q <= '9')
                        digit = *q - '0';
                    else if (*q >= 'a' && *q <= 'f')
                        digit = *q - 'a' + 10;
                    else if (*q >= 'A' && *q <= 'F')
                        digit = *q - 'A' + 10;
                    else
                        return 0;
                    TIXMLASSERT(digit < 16);
                    TIXMLASSERT(digit == 0 || mult <= UINT_MAX / digit);
                    const unsigned int digitScaled = mult * digit;
                    TIXMLASSERT(ucs <= ULONG_MAX - digitScaled);
                    ucs += digitScaled;
                    TIXMLASSERT(mult <= UINT_MAX / 16);
                    mult *= 16;
                    --q;
                }
            }
            else
            {
                // Decimal.
                const char* q = p + 2;
                if (!(*q))
                    return 0;
                q = strchr(q, SEMICOLON);
                if (!q)
                    return 0;
                TIXMLASSERT(*q == SEMICOLON);
                delta = q - p;
                --q;
                while (*q != '#')
                {
                    if (*q >= '0' && *q <= '9')
                    {
                        const unsigned int digit = *q - '0';
                        TIXMLASSERT(digit < 10);
                        TIXMLASSERT(digit == 0 || mult <= UINT_MAX / digit);
                        const unsigned int digitScaled = mult * digit;
                        TIXMLASSERT(ucs <= ULONG_MAX - digitScaled);
                        ucs += digitScaled;
                    }
                    else
                        return 0;
                    TIXMLASSERT(mult <= UINT_MAX / 10);
                    mult *= 10;
                    --q;
                }
            }
            // convert the UCS to UTF-8
            ConvertUTF32ToUTF8(ucs, value, length);
            return p + delta + 1;
        }
        return p + 1;
    }

    void XMLUtil::ToStr(int v, char* buffer, int bufferSize)
    {
        TIXML_SNPRINTF(buffer, bufferSize, "%d", v);
    }

    void XMLUtil::ToStr(unsigned v, char* buffer, int bufferSize)
    {
        TIXML_SNPRINTF(buffer, bufferSize, "%u", v);
    }

    void XMLUtil::ToStr(bool v, char* buffer, int bufferSize)
    {
        TIXML_SNPRINTF(buffer, bufferSize, "%s", v ? writeBoolTrue : writeBoolFalse);
    }

    /*
        ToStr() of a number is a very tricky topic.
        https://github.com/leethomason/tinyxml2/issues/106
    */
    void XMLUtil::ToStr(float v, char* buffer, int bufferSize)
    {
        TIXML_SNPRINTF(buffer, bufferSize, "%.8g", v);
    }

    void XMLUtil::ToStr(double v, char* buffer, int bufferSize)
    {
        TIXML_SNPRINTF(buffer, bufferSize, "%.17g", v);
    }

    void XMLUtil::ToStr(int64_t v, char* buffer, int bufferSize)
    {
        // horrible syntax trick to make the compiler happy about %lld
        TIXML_SNPRINTF(buffer, bufferSize, "%lld", (long long)v);
    }

    bool XMLUtil::ToInt(const char* str, int* value)
    {
        if (TIXML_SSCANF(str, "%d", value) == 1)
            return true;
        return false;
    }

    bool XMLUtil::ToUnsigned(const char* str, unsigned* value)
    {
        if (TIXML_SSCANF(str, "%u", value) == 1)
            return true;
        return false;
    }

    bool XMLUtil::ToBool(const char* str, bool* value)
    {
        int ival = 0;
        if (ToInt(str, &ival))
        {
            *value = (ival == 0) ? false : true;
            return true;
        }
        if (StringEqual(str, "true"))
        {
            *value = true;
            return true;
        }
        else if (StringEqual(str, "false"))
        {
            *value = false;
            return true;
        }
        return false;
    }

    bool XMLUtil::ToFloat(const char* str, float* value)
    {
        if (TIXML_SSCANF(str, "%f", value) == 1)
            return true;
        return false;
    }

    bool XMLUtil::ToDouble(const char* str, double* value)
    {
        if (TIXML_SSCANF(str, "%lf", value) == 1)
            return true;
        return false;
    }

    bool XMLUtil::ToInt64(const char* str, int64_t* value)
    {
        long long v = 0;    // horrible syntax trick to make the compiler happy about %lld
        if (TIXML_SSCANF(str, "%lld", &v) == 1)
        {
            *value = (int64_t)v;
            return true;
        }
        return false;
    }

    char* XMLDocument::Identify(char* p, XMLNode** node)
    {
        TIXMLASSERT(node);
        TIXMLASSERT(p);
        char* const start = p;
        int const startLine = _parseCurLineNum;
        p = XMLUtil::SkipWhiteSpace(p, &_parseCurLineNum);
        if (!*p)
        {
            *node = 0;
            TIXMLASSERT(p);
            return p;
        }
        // These strings define the matching patterns:
        static const char* xmlHeader        = { "<?" };
        static const char* commentHeader    = { "<!--" };
        static const char* cdataHeader      = { "<![CDATA[" };
        static const char* dtdHeader        = { "<!" };
        static const char* elementHeader    = { "<" };  // and a header for everything else; check last.
        static const int xmlHeaderLen       = 2;
        static const int commentHeaderLen   = 4;
        static const int cdataHeaderLen     = 9;
        static const int dtdHeaderLen       = 2;
        static const int elementHeaderLen   = 1;
        TIXMLASSERT(sizeof(XMLComment) == sizeof(XMLUnknown));             // use same memory pool
        TIXMLASSERT(sizeof(XMLComment) == sizeof(XMLDeclaration));         // use same memory pool
        XMLNode* returnNode = 0;
        if (XMLUtil::StringEqual(p, xmlHeader, xmlHeaderLen))
        {
            returnNode = CreateUnlinkedNode<XMLDeclaration>(_commentPool);
            returnNode->_parseLineNum = _parseCurLineNum;
            p += xmlHeaderLen;
        }
        else if (XMLUtil::StringEqual(p, commentHeader, commentHeaderLen))
        {
            returnNode = CreateUnlinkedNode<XMLComment>(_commentPool);
            returnNode->_parseLineNum = _parseCurLineNum;
            p += commentHeaderLen;
        }
        else if (XMLUtil::StringEqual(p, cdataHeader, cdataHeaderLen))
        {
            XMLText* text = CreateUnlinkedNode<XMLText>(_textPool);
            returnNode = text;
            returnNode->_parseLineNum = _parseCurLineNum;
            p += cdataHeaderLen;
            text->SetCData(true);
        }
        else if (XMLUtil::StringEqual(p, dtdHeader, dtdHeaderLen))
        {
            returnNode = CreateUnlinkedNode<XMLUnknown>(_commentPool);
            returnNode->_parseLineNum = _parseCurLineNum;
            p += dtdHeaderLen;
        }
        else if (XMLUtil::StringEqual(p, elementHeader, elementHeaderLen))
        {
            returnNode =  CreateUnlinkedNode<XMLElement>(_elementPool);
            returnNode->_parseLineNum = _parseCurLineNum;
            p += elementHeaderLen;
        }
        else
        {
            returnNode = CreateUnlinkedNode<XMLText>(_textPool);
            returnNode->_parseLineNum = _parseCurLineNum; // Report line of first non-whitespace character
            p = start;  // Back it up, all the text counts.
            _parseCurLineNum = startLine;
        }
        TIXMLASSERT(returnNode);
        TIXMLASSERT(p);
        *node = returnNode;
        return p;
    }

    bool XMLDocument::Accept(XMLVisitor* visitor) const
    {
        TIXMLASSERT(visitor);
        if (visitor->VisitEnter(*this))
        {
            for (const XMLNode* node = FirstChild(); node; node = node->NextSibling())
            {
                if (!node->Accept(visitor))
                    break;
            }
        }
        return visitor->VisitExit(*this);
    }


    // --------- XMLNode ----------- //

    XMLNode::XMLNode(XMLDocument* doc) :
        _document(doc),
        _parent(0),
        _parseLineNum(0),
        _firstChild(0), _lastChild(0),
        _prev(0), _next(0),
        _userData(0),
        _memPool(0)
    {
    }

    XMLNode::~XMLNode()
    {
        DeleteChildren();
        if (_parent)
            _parent->Unlink(this);
    }

    const char* XMLNode::Value() const
    {
        // Edge case: XMLDocuments don't have a Value. Return null.
        if (this->ToDocument())
            return 0;
        return _value.GetStr();
    }

    void XMLNode::SetValue(const char* str, bool staticMem)
    {
        if (staticMem)
            _value.SetInternedStr(str);
        else
            _value.SetStr(str);
    }

    void XMLNode::DeleteChildren()
    {
        while (_firstChild)
        {
            TIXMLASSERT(_lastChild);
            DeleteChild(_firstChild);
        }
        _firstChild = _lastChild = 0;
    }

    void XMLNode::Unlink(XMLNode* child)
    {
        TIXMLASSERT(child);
        TIXMLASSERT(child->_document == _document);
        TIXMLASSERT(child->_parent == this);
        if (child == _firstChild)
            _firstChild = _firstChild->_next;
        if (child == _lastChild)
            _lastChild = _lastChild->_prev;
        if (child->_prev)
            child->_prev->_next = child->_next;
        if (child->_next)
            child->_next->_prev = child->_prev;
        child->_parent = 0;
    }

    void XMLNode::DeleteChild(XMLNode* node)
    {
        TIXMLASSERT(node);
        TIXMLASSERT(node->_document == _document);
        TIXMLASSERT(node->_parent == this);
        Unlink(node);
        DeleteNode(node);
    }

    XMLNode* XMLNode::InsertEndChild(XMLNode* addThis)
    {
        TIXMLASSERT(addThis);
        if (addThis->_document != _document)
        {
            TIXMLASSERT(false);
            return 0;
        }
        InsertChildPreamble(addThis);
        if (_lastChild)
        {
            TIXMLASSERT(_firstChild);
            TIXMLASSERT(_lastChild->_next == 0);
            _lastChild->_next = addThis;
            addThis->_prev = _lastChild;
            _lastChild = addThis;
            addThis->_next = 0;
        }
        else
        {
            TIXMLASSERT(_firstChild == 0);
            _firstChild = _lastChild = addThis;
            addThis->_prev = 0;
            addThis->_next = 0;
        }
        addThis->_parent = this;
        return addThis;
    }

    XMLNode* XMLNode::InsertFirstChild(XMLNode* addThis)
    {
        TIXMLASSERT(addThis);
        if (addThis->_document != _document)
        {
            TIXMLASSERT(false);
            return 0;
        }
        InsertChildPreamble(addThis);
        if (_firstChild)
        {
            TIXMLASSERT(_lastChild);
            TIXMLASSERT(_firstChild->_prev == 0);
            _firstChild->_prev = addThis;
            addThis->_next = _firstChild;
            _firstChild = addThis;
            addThis->_prev = 0;
        }
        else
        {
            TIXMLASSERT(_lastChild == 0);
            _firstChild = _lastChild = addThis;
            addThis->_prev = 0;
            addThis->_next = 0;
        }
        addThis->_parent = this;
        return addThis;
    }

    XMLNode* XMLNode::InsertAfterChild(XMLNode* afterThis, XMLNode* addThis)
    {
        TIXMLASSERT(addThis);
        if (addThis->_document != _document)
        {
            TIXMLASSERT(false);
            return 0;
        }
        TIXMLASSERT(afterThis);
        if (afterThis->_parent != this)
        {
            TIXMLASSERT(false);
            return 0;
        }
        if (afterThis->_next == 0)
        {
            // The last node or the only node.
            return InsertEndChild(addThis);
        }
        InsertChildPreamble(addThis);
        addThis->_prev = afterThis;
        addThis->_next = afterThis->_next;
        afterThis->_next->_prev = addThis;
        afterThis->_next = addThis;
        addThis->_parent = this;
        return addThis;
    }

    const XMLElement* XMLNode::FirstChildElement(const char* name) const
    {
        for (const XMLNode* node = _firstChild; node; node = node->_next)
        {
            const XMLElement* element = node->ToElementWithName(name);
            if (element)
                return element;
        }
        return 0;
    }

    const XMLElement* XMLNode::LastChildElement(const char* name) const
    {
        for (const XMLNode* node = _lastChild; node; node = node->_prev)
        {
            const XMLElement* element = node->ToElementWithName(name);
            if (element)
                return element;
        }
        return 0;
    }

    const XMLElement* XMLNode::NextSiblingElement(const char* name) const
    {
        for (const XMLNode* node = _next; node; node = node->_next)
        {
            const XMLElement* element = node->ToElementWithName(name);
            if (element)
                return element;
        }
        return 0;
    }

    const XMLElement* XMLNode::PreviousSiblingElement(const char* name) const
    {
        for (const XMLNode* node = _prev; node; node = node->_prev)
        {
            const XMLElement* element = node->ToElementWithName(name);
            if (element)
                return element;
        }
        return 0;
    }

    char* XMLNode::ParseDeep(char* p, StrPair* parentEndTag, int* curLineNumPtr)
    {
        // This is a recursive method, but thinking about it "at the current level"
        // it is a pretty simple flat list:
        //      <foo/>
        //      <!-- comment -->
        //
        // With a special case:
        //      <foo>
        //      </foo>
        //      <!-- comment -->
        //
        // Where the closing element (/foo) *must* be the next thing after the opening
        // element, and the names must match. BUT the tricky bit is that the closing
        // element will be read by the child.
        //
        // 'endTag' is the end tag for this node, it is returned by a call to a child.
        // 'parentEnd' is the end tag for the parent, which is filled in and returned.
        while (p && *p)
        {
            XMLNode* node = 0;
            p = _document->Identify(p, &node);
            TIXMLASSERT(p);
            if (node == 0)
                break;
            int initialLineNum = node->_parseLineNum;
            StrPair endTag;
            p = node->ParseDeep(p, &endTag, curLineNumPtr);
            if (!p)
            {
                DeleteNode(node);
                if (!_document->Error())
                    _document->SetError(XML_ERROR_PARSING, 0, 0, initialLineNum);
                break;
            }
            XMLDeclaration* decl = node->ToDeclaration();
            if (decl)
            {
                // Declarations are only allowed at document level
                bool wellLocated = (ToDocument() != 0);
                if (wellLocated)
                {
                    // Multiple declarations are allowed but all declarations
                    // must occur before anything else
                    for (const XMLNode* existingNode = _document->FirstChild(); existingNode; existingNode = existingNode->NextSibling())
                    {
                        if (!existingNode->ToDeclaration())
                        {
                            wellLocated = false;
                            break;
                        }
                    }
                }
                if (!wellLocated)
                {
                    _document->SetError(XML_ERROR_PARSING_DECLARATION, decl->Value(), 0, initialLineNum);
                    DeleteNode(node);
                    break;
                }
            }
            XMLElement* ele = node->ToElement();
            if (ele)
            {
                // We read the end tag. Return it to the parent.
                if (ele->ClosingType() == XMLElement::CLOSING)
                {
                    if (parentEndTag)
                        ele->_value.TransferTo(parentEndTag);
                    node->_memPool->SetTracked();   // created and then immediately deleted.
                    DeleteNode(node);
                    return p;
                }
                // Handle an end tag returned to this level.
                // And handle a bunch of annoying errors.
                bool mismatch = false;
                if (endTag.Empty())
                {
                    if (ele->ClosingType() == XMLElement::OPEN)
                        mismatch = true;
                }
                else
                {
                    if (ele->ClosingType() != XMLElement::OPEN)
                        mismatch = true;
                    else if (!XMLUtil::StringEqual(endTag.GetStr(), ele->Name()))
                        mismatch = true;
                }
                if (mismatch)
                {
                    _document->SetError(XML_ERROR_MISMATCHED_ELEMENT, ele->Name(), 0, initialLineNum);
                    DeleteNode(node);
                    break;
                }
            }
            InsertEndChild(node);
        }
        return 0;
    }

    void XMLNode::DeleteNode(XMLNode* node)
    {
        if (node == 0)
            return;
        MemPool* pool = node->_memPool;
        node->~XMLNode();
        pool->Free(node);
    }

    void XMLNode::InsertChildPreamble(XMLNode* insertThis) const
    {
        TIXMLASSERT(insertThis);
        TIXMLASSERT(insertThis->_document == _document);
        if (insertThis->_parent)
            insertThis->_parent->Unlink(insertThis);
        else
            insertThis->_memPool->SetTracked();
    }

    const XMLElement* XMLNode::ToElementWithName(const char* name) const
    {
        const XMLElement* element = this->ToElement();
        if (element == 0)
            return 0;
        if (name == 0)
            return element;
        if (XMLUtil::StringEqual(element->Name(), name))
            return element;
        return 0;
    }

    // --------- XMLText ---------- //
    char* XMLText::ParseDeep(char* p, StrPair*, int* curLineNumPtr)
    {
        const char* start = p;
        if (this->CData())
        {
            p = _value.ParseText(p, "]]>", StrPair::NEEDS_NEWLINE_NORMALIZATION, curLineNumPtr);
            if (!p)
                _document->SetError(XML_ERROR_PARSING_CDATA, start, 0, _parseLineNum);
            return p;
        }
        else
        {
            int flags = _document->ProcessEntities() ? StrPair::TEXT_ELEMENT : StrPair::TEXT_ELEMENT_LEAVE_ENTITIES;
            if (_document->WhitespaceMode() == COLLAPSE_WHITESPACE)
                flags |= StrPair::NEEDS_WHITESPACE_COLLAPSING;
            p = _value.ParseText(p, "<", flags, curLineNumPtr);
            if (p && *p)
                return p - 1;
            if (!p)
                _document->SetError(XML_ERROR_PARSING_TEXT, start, 0, _parseLineNum);
        }
        return 0;
    }

    XMLNode* XMLText::ShallowClone(XMLDocument* doc) const
    {
        if (!doc)
            doc = _document;
        XMLText* text = doc->NewText(Value());   // fixme: this will always allocate memory. Intern?
        text->SetCData(this->CData());
        return text;
    }

    bool XMLText::ShallowEqual(const XMLNode* compare) const
    {
        TIXMLASSERT(compare);
        const XMLText* text = compare->ToText();
        return (text && XMLUtil::StringEqual(text->Value(), Value()));
    }

    bool XMLText::Accept(XMLVisitor* visitor) const
    {
        TIXMLASSERT(visitor);
        return visitor->Visit(*this);
    }


    // --------- XMLComment ---------- //

    XMLComment::XMLComment(XMLDocument* doc) : XMLNode(doc)
    {
    }

    XMLComment::~XMLComment()
    {
    }

    char* XMLComment::ParseDeep(char* p, StrPair*, int* curLineNumPtr)
    {
        // Comment parses as text.
        const char* start = p;
        p = _value.ParseText(p, "-->", StrPair::COMMENT, curLineNumPtr);
        if (p == 0)
            _document->SetError(XML_ERROR_PARSING_COMMENT, start, 0, _parseLineNum);
        return p;
    }

    XMLNode* XMLComment::ShallowClone(XMLDocument* doc) const
    {
        if (!doc)
            doc = _document;
        XMLComment* comment = doc->NewComment(Value());      // fixme: this will always allocate memory. Intern?
        return comment;
    }

    bool XMLComment::ShallowEqual(const XMLNode* compare) const
    {
        TIXMLASSERT(compare);
        const XMLComment* comment = compare->ToComment();
        return (comment && XMLUtil::StringEqual(comment->Value(), Value()));
    }

    bool XMLComment::Accept(XMLVisitor* visitor) const
    {
        TIXMLASSERT(visitor);
        return visitor->Visit(*this);
    }


    // --------- XMLDeclaration ---------- //

    XMLDeclaration::XMLDeclaration(XMLDocument* doc) : XMLNode(doc)
    {
    }

    XMLDeclaration::~XMLDeclaration()
    {
        //printf( "~XMLDeclaration\n" );
    }

    char* XMLDeclaration::ParseDeep(char* p, StrPair*, int* curLineNumPtr)
    {
        // Declaration parses as text.
        const char* start = p;
        p = _value.ParseText(p, "?>", StrPair::NEEDS_NEWLINE_NORMALIZATION, curLineNumPtr);
        if (p == 0)
            _document->SetError(XML_ERROR_PARSING_DECLARATION, start, 0, _parseLineNum);
        return p;
    }

    XMLNode* XMLDeclaration::ShallowClone(XMLDocument* doc) const
    {
        if (!doc)
            doc = _document;
        XMLDeclaration* dec = doc->NewDeclaration(Value());      // fixme: this will always allocate memory. Intern?
        return dec;
    }

    bool XMLDeclaration::ShallowEqual(const XMLNode* compare) const
    {
        TIXMLASSERT(compare);
        const XMLDeclaration* declaration = compare->ToDeclaration();
        return (declaration && XMLUtil::StringEqual(declaration->Value(), Value()));
    }

    bool XMLDeclaration::Accept(XMLVisitor* visitor) const
    {
        TIXMLASSERT(visitor);
        return visitor->Visit(*this);
    }

    // --------- XMLUnknown ---------- //

    XMLUnknown::XMLUnknown(XMLDocument* doc) : XMLNode(doc)
    {
    }

    XMLUnknown::~XMLUnknown()
    {
    }

    char* XMLUnknown::ParseDeep(char* p, StrPair*, int* curLineNumPtr)
    {
        // Unknown parses as text.
        const char* start = p;
        p = _value.ParseText(p, ">", StrPair::NEEDS_NEWLINE_NORMALIZATION, curLineNumPtr);
        if (!p)
            _document->SetError(XML_ERROR_PARSING_UNKNOWN, start, 0, _parseLineNum);
        return p;
    }

    XMLNode* XMLUnknown::ShallowClone(XMLDocument* doc) const
    {
        if (!doc)
            doc = _document;
        XMLUnknown* text = doc->NewUnknown(Value());     // fixme: this will always allocate memory. Intern?
        return text;
    }

    bool XMLUnknown::ShallowEqual(const XMLNode* compare) const
    {
        TIXMLASSERT(compare);
        const XMLUnknown* unknown = compare->ToUnknown();
        return (unknown && XMLUtil::StringEqual(unknown->Value(), Value()));
    }

    bool XMLUnknown::Accept(XMLVisitor* visitor) const
    {
        TIXMLASSERT(visitor);
        return visitor->Visit(*this);
    }

    // --------- XMLAttribute ---------- //

    const char* XMLAttribute::Name() const
    {
        return _name.GetStr();
    }

    const char* XMLAttribute::Value() const
    {
        return _value.GetStr();
    }

    char* XMLAttribute::ParseDeep(char* p, bool processEntities, int* curLineNumPtr)
    {
        // Parse using the name rules: bug fix, was using ParseText before
        p = _name.ParseName(p);
        if (!p || !*p)
            return 0;
        // Skip white space before =
        p = XMLUtil::SkipWhiteSpace(p, curLineNumPtr);
        if (*p != '=')
            return 0;
        ++p;    // move up to opening quote
        p = XMLUtil::SkipWhiteSpace(p, curLineNumPtr);
        if (*p != '\"' && *p != '\'')
            return 0;
        char endTag[2] = { *p, 0 };
        ++p;    // move past opening quote
        p = _value.ParseText(p, endTag, processEntities ? StrPair::ATTRIBUTE_VALUE : StrPair::ATTRIBUTE_VALUE_LEAVE_ENTITIES, curLineNumPtr);
        return p;
    }

    void XMLAttribute::SetName(const char* n)
    {
        _name.SetStr(n);
    }

    XMLError XMLAttribute::QueryIntValue(int* value) const
    {
        if (XMLUtil::ToInt(Value(), value))
            return XML_SUCCESS;
        return XML_WRONG_ATTRIBUTE_TYPE;
    }

    XMLError XMLAttribute::QueryUnsignedValue(unsigned int* value) const
    {
        if (XMLUtil::ToUnsigned(Value(), value))
            return XML_SUCCESS;
        return XML_WRONG_ATTRIBUTE_TYPE;
    }

    XMLError XMLAttribute::QueryInt64Value(int64_t* value) const
    {
        if (XMLUtil::ToInt64(Value(), value))
            return XML_SUCCESS;
        return XML_WRONG_ATTRIBUTE_TYPE;
    }

    XMLError XMLAttribute::QueryBoolValue(bool* value) const
    {
        if (XMLUtil::ToBool(Value(), value))
            return XML_SUCCESS;
        return XML_WRONG_ATTRIBUTE_TYPE;
    }

    XMLError XMLAttribute::QueryFloatValue(float* value) const
    {
        if (XMLUtil::ToFloat(Value(), value))
            return XML_SUCCESS;
        return XML_WRONG_ATTRIBUTE_TYPE;
    }

    XMLError XMLAttribute::QueryDoubleValue(double* value) const
    {
        if (XMLUtil::ToDouble(Value(), value))
            return XML_SUCCESS;
        return XML_WRONG_ATTRIBUTE_TYPE;
    }

    void XMLAttribute::SetAttribute(const char* v)
    {
        _value.SetStr(v);
    }

    void XMLAttribute::SetAttribute(int v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        _value.SetStr(buf);
    }

    void XMLAttribute::SetAttribute(unsigned v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        _value.SetStr(buf);
    }

    void XMLAttribute::SetAttribute(int64_t v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        _value.SetStr(buf);
    }

    void XMLAttribute::SetAttribute(bool v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        _value.SetStr(buf);
    }

    void XMLAttribute::SetAttribute(double v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        _value.SetStr(buf);
    }

    void XMLAttribute::SetAttribute(float v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        _value.SetStr(buf);
    }


    // --------- XMLElement ---------- //
    XMLElement::XMLElement(XMLDocument* doc) : XMLNode(doc),
        _closingType(OPEN),
        _rootAttribute(0)
    {
    }

    XMLElement::~XMLElement()
    {
        while (_rootAttribute)
        {
            XMLAttribute* next = _rootAttribute->_next;
            DeleteAttribute(_rootAttribute);
            _rootAttribute = next;
        }
    }

    const XMLAttribute* XMLElement::FindAttribute(const char* name) const
    {
        for (XMLAttribute* a = _rootAttribute; a; a = a->_next)
        {
            if (XMLUtil::StringEqual(a->Name(), name))
                return a;
        }
        return 0;
    }

    const char* XMLElement::Attribute(const char* name, const char* value) const
    {
        const XMLAttribute* a = FindAttribute(name);
        if (!a)
            return 0;
        if (!value || XMLUtil::StringEqual(a->Value(), value))
            return a->Value();
        return 0;
    }

    int XMLElement::IntAttribute(const char* name, int defaultValue) const
    {
        int i = defaultValue;
        QueryIntAttribute(name, &i);
        return i;
    }

    unsigned XMLElement::UnsignedAttribute(const char* name, unsigned defaultValue) const
    {
        unsigned i = defaultValue;
        QueryUnsignedAttribute(name, &i);
        return i;
    }

    int64_t XMLElement::Int64Attribute(const char* name, int64_t defaultValue) const
    {
        int64_t i = defaultValue;
        QueryInt64Attribute(name, &i);
        return i;
    }

    bool XMLElement::BoolAttribute(const char* name, bool defaultValue) const
    {
        bool b = defaultValue;
        QueryBoolAttribute(name, &b);
        return b;
    }

    double XMLElement::DoubleAttribute(const char* name, double defaultValue) const
    {
        double d = defaultValue;
        QueryDoubleAttribute(name, &d);
        return d;
    }

    float XMLElement::FloatAttribute(const char* name, float defaultValue) const
    {
        float f = defaultValue;
        QueryFloatAttribute(name, &f);
        return f;
    }

    const char* XMLElement::GetText() const
    {
        if (FirstChild() && FirstChild()->ToText())
            return FirstChild()->Value();
        return 0;
    }

    void    XMLElement::SetText(const char* inText)
    {
        if (FirstChild() && FirstChild()->ToText())
            FirstChild()->SetValue(inText);
        else
        {
            XMLText*    theText = GetDocument()->NewText(inText);
            InsertFirstChild(theText);
        }
    }

    void XMLElement::SetText(int v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        SetText(buf);
    }

    void XMLElement::SetText(unsigned v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        SetText(buf);
    }

    void XMLElement::SetText(int64_t v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        SetText(buf);
    }

    void XMLElement::SetText(bool v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        SetText(buf);
    }

    void XMLElement::SetText(float v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        SetText(buf);
    }

    void XMLElement::SetText(double v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        SetText(buf);
    }

    XMLError XMLElement::QueryIntText(int* ival) const
    {
        if (FirstChild() && FirstChild()->ToText())
        {
            const char* t = FirstChild()->Value();
            if (XMLUtil::ToInt(t, ival))
                return XML_SUCCESS;
            return XML_CAN_NOT_CONVERT_TEXT;
        }
        return XML_NO_TEXT_NODE;
    }

    XMLError XMLElement::QueryUnsignedText(unsigned* uval) const
    {
        if (FirstChild() && FirstChild()->ToText())
        {
            const char* t = FirstChild()->Value();
            if (XMLUtil::ToUnsigned(t, uval))
                return XML_SUCCESS;
            return XML_CAN_NOT_CONVERT_TEXT;
        }
        return XML_NO_TEXT_NODE;
    }

    XMLError XMLElement::QueryInt64Text(int64_t* ival) const
    {
        if (FirstChild() && FirstChild()->ToText())
        {
            const char* t = FirstChild()->Value();
            if (XMLUtil::ToInt64(t, ival))
                return XML_SUCCESS;
            return XML_CAN_NOT_CONVERT_TEXT;
        }
        return XML_NO_TEXT_NODE;
    }

    XMLError XMLElement::QueryBoolText(bool* bval) const
    {
        if (FirstChild() && FirstChild()->ToText())
        {
            const char* t = FirstChild()->Value();
            if (XMLUtil::ToBool(t, bval))
                return XML_SUCCESS;
            return XML_CAN_NOT_CONVERT_TEXT;
        }
        return XML_NO_TEXT_NODE;
    }

    XMLError XMLElement::QueryDoubleText(double* dval) const
    {
        if (FirstChild() && FirstChild()->ToText())
        {
            const char* t = FirstChild()->Value();
            if (XMLUtil::ToDouble(t, dval))
                return XML_SUCCESS;
            return XML_CAN_NOT_CONVERT_TEXT;
        }
        return XML_NO_TEXT_NODE;
    }

    XMLError XMLElement::QueryFloatText(float* fval) const
    {
        if (FirstChild() && FirstChild()->ToText())
        {
            const char* t = FirstChild()->Value();
            if (XMLUtil::ToFloat(t, fval))
                return XML_SUCCESS;
            return XML_CAN_NOT_CONVERT_TEXT;
        }
        return XML_NO_TEXT_NODE;
    }

    int XMLElement::IntText(int defaultValue) const
    {
        int i = defaultValue;
        QueryIntText(&i);
        return i;
    }

    unsigned XMLElement::UnsignedText(unsigned defaultValue) const
    {
        unsigned i = defaultValue;
        QueryUnsignedText(&i);
        return i;
    }

    int64_t XMLElement::Int64Text(int64_t defaultValue) const
    {
        int64_t i = defaultValue;
        QueryInt64Text(&i);
        return i;
    }

    bool XMLElement::BoolText(bool defaultValue) const
    {
        bool b = defaultValue;
        QueryBoolText(&b);
        return b;
    }

    double XMLElement::DoubleText(double defaultValue) const
    {
        double d = defaultValue;
        QueryDoubleText(&d);
        return d;
    }

    float XMLElement::FloatText(float defaultValue) const
    {
        float f = defaultValue;
        QueryFloatText(&f);
        return f;
    }

    XMLAttribute* XMLElement::FindOrCreateAttribute(const char* name)
    {
        XMLAttribute* last = 0;
        XMLAttribute* attrib = 0;
        for (attrib = _rootAttribute;
             attrib;
             last = attrib, attrib = attrib->_next)
        {
            if (XMLUtil::StringEqual(attrib->Name(), name))
                break;
        }
        if (!attrib)
        {
            attrib = CreateAttribute();
            TIXMLASSERT(attrib);
            if (last)
            {
                TIXMLASSERT(last->_next == 0);
                last->_next = attrib;
            }
            else
            {
                TIXMLASSERT(_rootAttribute == 0);
                _rootAttribute = attrib;
            }
            attrib->SetName(name);
        }
        return attrib;
    }

    void XMLElement::DeleteAttribute(const char* name)
    {
        XMLAttribute* prev = 0;
        for (XMLAttribute* a = _rootAttribute; a; a = a->_next)
        {
            if (XMLUtil::StringEqual(name, a->Name()))
            {
                if (prev)
                    prev->_next = a->_next;
                else
                    _rootAttribute = a->_next;
                DeleteAttribute(a);
                break;
            }
            prev = a;
        }
    }

    char* XMLElement::ParseAttributes(char* p, int* curLineNumPtr)
    {
        const char* start = p;
        XMLAttribute* prevAttribute = 0;
        // Read the attributes.
        while (p)
        {
            p = XMLUtil::SkipWhiteSpace(p, curLineNumPtr);
            if (!(*p))
            {
                _document->SetError(XML_ERROR_PARSING_ELEMENT, start, Name(), _parseLineNum);
                return 0;
            }
            // attribute.
            if (XMLUtil::IsNameStartChar(*p))
            {
                XMLAttribute* attrib = CreateAttribute();
                TIXMLASSERT(attrib);
                attrib->_parseLineNum = _document->_parseCurLineNum;
                int attrLineNum = attrib->_parseLineNum;
                p = attrib->ParseDeep(p, _document->ProcessEntities(), curLineNumPtr);
                if (!p || Attribute(attrib->Name()))
                {
                    DeleteAttribute(attrib);
                    _document->SetError(XML_ERROR_PARSING_ATTRIBUTE, start, p, attrLineNum);
                    return 0;
                }
                // There is a minor bug here: if the attribute in the source xml
                // document is duplicated, it will not be detected and the
                // attribute will be doubly added. However, tracking the 'prevAttribute'
                // avoids re-scanning the attribute list. Preferring performance for
                // now, may reconsider in the future.
                if (prevAttribute)
                {
                    TIXMLASSERT(prevAttribute->_next == 0);
                    prevAttribute->_next = attrib;
                }
                else
                {
                    TIXMLASSERT(_rootAttribute == 0);
                    _rootAttribute = attrib;
                }
                prevAttribute = attrib;
            }
            // end of the tag
            else if (*p == '>')
            {
                ++p;
                break;
            }
            // end of the tag
            else if (*p == '/' && *(p + 1) == '>')
            {
                _closingType = CLOSED;
                return p + 2; // done; sealed element.
            }
            else
            {
                _document->SetError(XML_ERROR_PARSING_ELEMENT, start, p, _parseLineNum);
                return 0;
            }
        }
        return p;
    }

    void XMLElement::DeleteAttribute(XMLAttribute* attribute)
    {
        if (attribute == 0)
            return;
        MemPool* pool = attribute->_memPool;
        attribute->~XMLAttribute();
        pool->Free(attribute);
    }

    XMLAttribute* XMLElement::CreateAttribute()
    {
        TIXMLASSERT(sizeof(XMLAttribute) == _document->_attributePool.ItemSize());
        XMLAttribute* attrib = new(_document->_attributePool.Alloc()) XMLAttribute();
        TIXMLASSERT(attrib);
        attrib->_memPool = &_document->_attributePool;
        attrib->_memPool->SetTracked();
        return attrib;
    }

    //
    //  <ele></ele>
    //  <ele>foo<b>bar</b></ele>
    //
    char* XMLElement::ParseDeep(char* p, StrPair* parentEndTag, int* curLineNumPtr)
    {
        // Read the element name.
        p = XMLUtil::SkipWhiteSpace(p, curLineNumPtr);
        // The closing element is the </element> form. It is
        // parsed just like a regular element then deleted from
        // the DOM.
        if (*p == '/')
        {
            _closingType = CLOSING;
            ++p;
        }
        p = _value.ParseName(p);
        if (_value.Empty())
            return 0;
        p = ParseAttributes(p, curLineNumPtr);
        if (!p || !*p || _closingType != OPEN)
            return p;
        p = XMLNode::ParseDeep(p, parentEndTag, curLineNumPtr);
        return p;
    }

    XMLNode* XMLElement::ShallowClone(XMLDocument* doc) const
    {
        if (!doc)
            doc = _document;
        XMLElement* element = doc->NewElement(Value());                      // fixme: this will always allocate memory. Intern?
        for (const XMLAttribute* a = FirstAttribute(); a; a = a->Next())
        {
            element->SetAttribute(a->Name(), a->Value());                    // fixme: this will always allocate memory. Intern?
        }
        return element;
    }

    bool XMLElement::ShallowEqual(const XMLNode* compare) const
    {
        TIXMLASSERT(compare);
        const XMLElement* other = compare->ToElement();
        if (other && XMLUtil::StringEqual(other->Name(), Name()))
        {
            const XMLAttribute* a = FirstAttribute();
            const XMLAttribute* b = other->FirstAttribute();
            while (a && b)
            {
                if (!XMLUtil::StringEqual(a->Value(), b->Value()))
                    return false;
                a = a->Next();
                b = b->Next();
            }
            if (a || b)
            {
                // different count
                return false;
            }
            return true;
        }
        return false;
    }

    bool XMLElement::Accept(XMLVisitor* visitor) const
    {
        TIXMLASSERT(visitor);
        if (visitor->VisitEnter(*this, _rootAttribute))
        {
            for (const XMLNode* node = FirstChild(); node; node = node->NextSibling())
            {
                if (!node->Accept(visitor))
                    break;
            }
        }
        return visitor->VisitExit(*this);
    }


    // --------- XMLDocument ----------- //

    // Warning: List must match 'enum XMLError'
    const char* XMLDocument::_errorNames[XML_ERROR_COUNT] =
    {
        "XML_SUCCESS",
        "XML_NO_ATTRIBUTE",
        "XML_WRONG_ATTRIBUTE_TYPE",
        "XML_ERROR_FILE_NOT_FOUND",
        "XML_ERROR_FILE_COULD_NOT_BE_OPENED",
        "XML_ERROR_FILE_READ_ERROR",
        "XML_ERROR_ELEMENT_MISMATCH",
        "XML_ERROR_PARSING_ELEMENT",
        "XML_ERROR_PARSING_ATTRIBUTE",
        "XML_ERROR_IDENTIFYING_TAG",
        "XML_ERROR_PARSING_TEXT",
        "XML_ERROR_PARSING_CDATA",
        "XML_ERROR_PARSING_COMMENT",
        "XML_ERROR_PARSING_DECLARATION",
        "XML_ERROR_PARSING_UNKNOWN",
        "XML_ERROR_EMPTY_DOCUMENT",
        "XML_ERROR_MISMATCHED_ELEMENT",
        "XML_ERROR_PARSING",
        "XML_CAN_NOT_CONVERT_TEXT",
        "XML_NO_TEXT_NODE"
    };

    XMLDocument::XMLDocument(bool processEntities, Whitespace whitespaceMode) :
        XMLNode(0),
        _writeBOM(false),
        _processEntities(processEntities),
        _errorID(XML_SUCCESS),
        _whitespaceMode(whitespaceMode),
        _errorLineNum(0),
        _charBuffer(0),
        _parseCurLineNum(0)
    {
        // avoid VC++ C4355 warning about 'this' in initializer list (C4355 is off by default in VS2012+)
        _document = this;
    }

    XMLDocument::~XMLDocument()
    {
        Clear();
    }

    void XMLDocument::Clear()
    {
        DeleteChildren();
#ifdef DEBUG
        const bool hadError = Error();
#endif
        ClearError();
        delete [] _charBuffer;
        _charBuffer = 0;
#if 0
        _textPool.Trace("text");
        _elementPool.Trace("element");
        _commentPool.Trace("comment");
        _attributePool.Trace("attribute");
#endif
#ifdef DEBUG
        if (!hadError)
        {
            TIXMLASSERT(_elementPool.CurrentAllocs()   == _elementPool.Untracked());
            TIXMLASSERT(_attributePool.CurrentAllocs() == _attributePool.Untracked());
            TIXMLASSERT(_textPool.CurrentAllocs()      == _textPool.Untracked());
            TIXMLASSERT(_commentPool.CurrentAllocs()   == _commentPool.Untracked());
        }
#endif
    }

    XMLElement* XMLDocument::NewElement(const char* name)
    {
        XMLElement* ele = CreateUnlinkedNode<XMLElement>(_elementPool);
        ele->SetName(name);
        return ele;
    }

    XMLComment* XMLDocument::NewComment(const char* str)
    {
        XMLComment* comment = CreateUnlinkedNode<XMLComment>(_commentPool);
        comment->SetValue(str);
        return comment;
    }

    XMLText* XMLDocument::NewText(const char* str)
    {
        XMLText* text = CreateUnlinkedNode<XMLText>(_textPool);
        text->SetValue(str);
        return text;
    }

    XMLDeclaration* XMLDocument::NewDeclaration(const char* str)
    {
        XMLDeclaration* dec = CreateUnlinkedNode<XMLDeclaration>(_commentPool);
        dec->SetValue(str ? str : "xml version=\"1.0\" encoding=\"UTF-8\"");
        return dec;
    }

    XMLUnknown* XMLDocument::NewUnknown(const char* str)
    {
        XMLUnknown* unk = CreateUnlinkedNode<XMLUnknown>(_commentPool);
        unk->SetValue(str);
        return unk;
    }

    static FILE* callfopen(const char* filepath, const char* mode)
    {
        TIXMLASSERT(filepath);
        TIXMLASSERT(mode);
#if defined(_MSC_VER) && (_MSC_VER >= 1400 ) && (!defined WINCE)
        FILE* fp = 0;
        errno_t err = fopen_s(&fp, filepath, mode);
        if (err)
            return 0;
#else
        FILE* fp = fopen(filepath, mode);
#endif
        return fp;
    }

    void XMLDocument::DeleteNode(XMLNode* node)
    {
        TIXMLASSERT(node);
        TIXMLASSERT(node->_document == this);
        if (node->_parent)
            node->_parent->DeleteChild(node);
        else
        {
            // Isn't in the tree.
            // Use the parent delete.
            // Also, we need to mark it tracked: we 'know'
            // it was never used.
            node->_memPool->SetTracked();
            // Call the static XMLNode version:
            XMLNode::DeleteNode(node);
        }
    }

    XMLError XMLDocument::LoadFile(const char* filename)
    {
        Clear();
        FILE* fp = callfopen(filename, "rb");
        if (!fp)
        {
            SetError(XML_ERROR_FILE_NOT_FOUND, filename, 0, 0);
            return _errorID;
        }
        LoadFile(fp);
        fclose(fp);
        return _errorID;
    }

    // This is likely overengineered template art to have a check that unsigned long value incremented
    // by one still fits into size_t. If size_t type is larger than unsigned long type
    // (x86_64-w64-mingw32 target) then the check is redundant and gcc and clang emit
    // -Wtype-limits warning. This piece makes the compiler select code with a check when a check
    // is useful and code with no check when a check is redundant depending on how size_t and unsigned long
    // types sizes relate to each other.
    template
    < bool = (sizeof(unsigned long) >= sizeof(size_t)) >
    struct LongFitsIntoSizeTMinusOne
    {
        static bool Fits(unsigned long value)
        {
            return value < (size_t) - 1;
        }
    };

    template <>
    struct LongFitsIntoSizeTMinusOne<false>
    {
        static bool Fits(unsigned long)
        {
            return true;
        }
    };

    XMLError XMLDocument::LoadFile(FILE* fp)
    {
        Clear();
        fseek(fp, 0, SEEK_SET);
        if (fgetc(fp) == EOF && ferror(fp) != 0)
        {
            SetError(XML_ERROR_FILE_READ_ERROR, 0, 0, 0);
            return _errorID;
        }
        fseek(fp, 0, SEEK_END);
        const long filelength = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if (filelength == -1L)
        {
            SetError(XML_ERROR_FILE_READ_ERROR, 0, 0, 0);
            return _errorID;
        }
        TIXMLASSERT(filelength >= 0);
        if (!LongFitsIntoSizeTMinusOne<>::Fits(filelength))
        {
            // Cannot handle files which won't fit in buffer together with null terminator
            SetError(XML_ERROR_FILE_READ_ERROR, 0, 0, 0);
            return _errorID;
        }
        if (filelength == 0)
        {
            SetError(XML_ERROR_EMPTY_DOCUMENT, 0, 0, 0);
            return _errorID;
        }
        const size_t size = filelength;
        TIXMLASSERT(_charBuffer == 0);
        _charBuffer = new char[size + 1];
        size_t read = fread(_charBuffer, 1, size, fp);
        if (read != size)
        {
            SetError(XML_ERROR_FILE_READ_ERROR, 0, 0, 0);
            return _errorID;
        }
        _charBuffer[size] = 0;
        Parse();
        return _errorID;
    }

    XMLError XMLDocument::SaveFile(const char* filename, bool compact)
    {
        FILE* fp = callfopen(filename, "w");
        if (!fp)
        {
            SetError(XML_ERROR_FILE_COULD_NOT_BE_OPENED, filename, 0, 0);
            return _errorID;
        }
        SaveFile(fp, compact);
        fclose(fp);
        return _errorID;
    }

    XMLError XMLDocument::SaveFile(FILE* fp, bool compact)
    {
        // Clear any error from the last save, otherwise it will get reported
        // for *this* call.
        ClearError();
        XMLPrinter stream(fp, compact);
        Print(&stream);
        return _errorID;
    }

    XMLError XMLDocument::Parse(const char* p, size_t len)
    {
        Clear();
        if (len == 0 || !p || !*p)
        {
            SetError(XML_ERROR_EMPTY_DOCUMENT, 0, 0, 0);
            return _errorID;
        }
        if (len == (size_t)(-1))
            len = strlen(p);
        TIXMLASSERT(_charBuffer == 0);
        _charBuffer = new char[ len + 1 ];
        memcpy(_charBuffer, p, len);
        _charBuffer[len] = 0;
        Parse();
        if (Error())
        {
            // clean up now essentially dangling memory.
            // and the parse fail can put objects in the
            // pools that are dead and inaccessible.
            DeleteChildren();
            _elementPool.Clear();
            _attributePool.Clear();
            _textPool.Clear();
            _commentPool.Clear();
        }
        return _errorID;
    }

    void XMLDocument::Print(XMLPrinter* streamer) const
    {
        if (streamer)
            Accept(streamer);
        else
        {
            XMLPrinter stdoutStreamer(stdout);
            Accept(&stdoutStreamer);
        }
    }

    void XMLDocument::SetError(XMLError error, const char* str1, const char* str2, int lineNum)
    {
        TIXMLASSERT(error >= 0 && error < XML_ERROR_COUNT);
        _errorID = error;
        _errorStr1.Reset();
        _errorStr2.Reset();
        _errorLineNum = lineNum;
        if (str1)
            _errorStr1.SetStr(str1);
        if (str2)
            _errorStr2.SetStr(str2);
    }

    /*static*/ const char* XMLDocument::ErrorIDToName(XMLError errorID)
    {
        TIXMLASSERT(errorID >= 0 && errorID < XML_ERROR_COUNT);
        const char* errorName = _errorNames[errorID];
        TIXMLASSERT(errorName && errorName[0]);
        return errorName;
    }

    const char* XMLDocument::ErrorName() const
    {
        return ErrorIDToName(_errorID);
    }

    void XMLDocument::PrintError() const
    {
        if (Error())
        {
            static const int LEN = 20;
            char buf1[LEN] = { 0 };
            char buf2[LEN] = { 0 };
            if (!_errorStr1.Empty())
                TIXML_SNPRINTF(buf1, LEN, "%s", _errorStr1.GetStr());
            if (!_errorStr2.Empty())
                TIXML_SNPRINTF(buf2, LEN, "%s", _errorStr2.GetStr());
            // Should check INT_MIN <= _errorID && _errorId <= INT_MAX, but that
            // causes a clang "always true" -Wtautological-constant-out-of-range-compare warning
            TIXMLASSERT(0 <= _errorID && XML_ERROR_COUNT - 1 <= INT_MAX);
            printf("XMLDocument error id=%d '%s' str1=%s str2=%s line=%d\n",
                   static_cast<int>(_errorID), ErrorName(), buf1, buf2, _errorLineNum);
        }
    }

    void XMLDocument::Parse()
    {
        TIXMLASSERT(NoChildren());   // Clear() must have been called previously
        TIXMLASSERT(_charBuffer);
        _parseCurLineNum = 1;
        _parseLineNum = 1;
        char* p = _charBuffer;
        p = XMLUtil::SkipWhiteSpace(p, &_parseCurLineNum);
        p = const_cast<char*>(XMLUtil::ReadBOM(p, &_writeBOM));
        if (!*p)
        {
            SetError(XML_ERROR_EMPTY_DOCUMENT, 0, 0, 0);
            return;
        }
        ParseDeep(p, 0, &_parseCurLineNum);
    }

    XMLPrinter::XMLPrinter(FILE* file, bool compact, int depth) :
        _elementJustOpened(false),
        _firstElement(true),
        _fp(file),
        _depth(depth),
        _textDepth(-1),
        _processEntities(true),
        _compactMode(compact)
    {
        for (int i = 0; i < ENTITY_RANGE; ++i)
        {
            _entityFlag[i] = false;
            _restrictedEntityFlag[i] = false;
        }
        for (int i = 0; i < NUM_ENTITIES; ++i)
        {
            const char entityValue = entities[i].value;
            TIXMLASSERT(((unsigned char)entityValue) < ENTITY_RANGE);
            _entityFlag[(unsigned char)entityValue ] = true;
        }
        _restrictedEntityFlag[(unsigned char)'&'] = true;
        _restrictedEntityFlag[(unsigned char)'<'] = true;
        _restrictedEntityFlag[(unsigned char)'>'] = true;   // not required, but consistency is nice
        _buffer.Push(0);
    }

    void XMLPrinter::Print(const char* format, ...)
    {
        va_list     va;
        va_start(va, format);
        if (_fp)
            vfprintf(_fp, format, va);
        else
        {
            const int len = TIXML_VSCPRINTF(format, va);
            // Close out and re-start the va-args
            va_end(va);
            TIXMLASSERT(len >= 0);
            va_start(va, format);
            TIXMLASSERT(_buffer.Size() > 0 && _buffer[_buffer.Size() - 1] == 0);
            char* p = _buffer.PushArr(len) - 1;      // back up over the null terminator.
            TIXML_VSNPRINTF(p, len + 1, format, va);
        }
        va_end(va);
    }

    void XMLPrinter::PrintSpace(int depth)
    {
        for (int i = 0; i < depth; ++i)
            Print("    ");
    }

    void XMLPrinter::PrintString(const char* p, bool restricted)
    {
        // Look for runs of bytes between entities to print.
        const char* q = p;
        if (_processEntities)
        {
            const bool* flag = restricted ? _restrictedEntityFlag : _entityFlag;
            while (*q)
            {
                TIXMLASSERT(p <= q);
                // Remember, char is sometimes signed. (How many times has that bitten me?)
                if (*q > 0 && *q < ENTITY_RANGE)
                {
                    // Check for entities. If one is found, flush
                    // the stream up until the entity, write the
                    // entity, and keep looking.
                    if (flag[(unsigned char)(*q)])
                    {
                        while (p < q)
                        {
                            const size_t delta = q - p;
                            // %.*s accepts type int as "precision"
                            const int toPrint = (INT_MAX < delta) ? INT_MAX : (int)delta;
                            Print("%.*s", toPrint, p);
                            p += toPrint;
                        }
                        bool entityPatternPrinted = false;
                        for (int i = 0; i < NUM_ENTITIES; ++i)
                        {
                            if (entities[i].value == *q)
                            {
                                Print("&%s;", entities[i].pattern);
                                entityPatternPrinted = true;
                                break;
                            }
                        }
                        if (!entityPatternPrinted)
                        {
                            // TIXMLASSERT( entityPatternPrinted ) causes gcc -Wunused-but-set-variable in release
                            TIXMLASSERT(false);
                        }
                        ++p;
                    }
                }
                ++q;
                TIXMLASSERT(p <= q);
            }
        }
        // Flush the remaining string. This will be the entire
        // string if an entity wasn't found.
        TIXMLASSERT(p <= q);
        if (!_processEntities || (p < q))
            Print("%s", p);
    }

    void XMLPrinter::PushHeader(bool writeBOM, bool writeDec)
    {
        if (writeBOM)
        {
            static const unsigned char bom[] = { TIXML_UTF_LEAD_0, TIXML_UTF_LEAD_1, TIXML_UTF_LEAD_2, 0 };
            Print("%s", bom);
        }
        if (writeDec)
            PushDeclaration("xml version=\"1.0\"");
    }

    void XMLPrinter::OpenElement(const char* name, bool compactMode)
    {
        SealElementIfJustOpened();
        _stack.Push(name);
        if (_textDepth < 0 && !_firstElement && !compactMode)
            Print("\n");
        if (!compactMode)
            PrintSpace(_depth);
        Print("<%s", name);
        _elementJustOpened = true;
        _firstElement = false;
        ++_depth;
    }

    void XMLPrinter::PushAttribute(const char* name, const char* value)
    {
        TIXMLASSERT(_elementJustOpened);
        Print(" %s=\"", name);
        PrintString(value, false);
        Print("\"");
    }

    void XMLPrinter::PushAttribute(const char* name, int v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        PushAttribute(name, buf);
    }

    void XMLPrinter::PushAttribute(const char* name, unsigned v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        PushAttribute(name, buf);
    }

    void XMLPrinter::PushAttribute(const char* name, int64_t v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        PushAttribute(name, buf);
    }

    void XMLPrinter::PushAttribute(const char* name, bool v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        PushAttribute(name, buf);
    }

    void XMLPrinter::PushAttribute(const char* name, double v)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(v, buf, BUF_SIZE);
        PushAttribute(name, buf);
    }

    void XMLPrinter::CloseElement(bool compactMode)
    {
        --_depth;
        const char* name = _stack.Pop();
        if (_elementJustOpened)
            Print("/>");
        else
        {
            if (_textDepth < 0 && !compactMode)
            {
                Print("\n");
                PrintSpace(_depth);
            }
            Print("</%s>", name);
        }
        if (_textDepth == _depth)
            _textDepth = -1;
        if (_depth == 0 && !compactMode)
            Print("\n");
        _elementJustOpened = false;
    }

    void XMLPrinter::SealElementIfJustOpened()
    {
        if (!_elementJustOpened)
            return;
        _elementJustOpened = false;
        Print(">");
    }

    void XMLPrinter::PushText(const char* text, bool cdata)
    {
        _textDepth = _depth - 1;
        SealElementIfJustOpened();
        if (cdata)
            Print("<![CDATA[%s]]>", text);
        else
            PrintString(text, true);
    }

    void XMLPrinter::PushText(int64_t value)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(value, buf, BUF_SIZE);
        PushText(buf, false);
    }

    void XMLPrinter::PushText(int value)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(value, buf, BUF_SIZE);
        PushText(buf, false);
    }

    void XMLPrinter::PushText(unsigned value)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(value, buf, BUF_SIZE);
        PushText(buf, false);
    }

    void XMLPrinter::PushText(bool value)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(value, buf, BUF_SIZE);
        PushText(buf, false);
    }

    void XMLPrinter::PushText(float value)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(value, buf, BUF_SIZE);
        PushText(buf, false);
    }

    void XMLPrinter::PushText(double value)
    {
        char buf[BUF_SIZE];
        XMLUtil::ToStr(value, buf, BUF_SIZE);
        PushText(buf, false);
    }

    void XMLPrinter::PushComment(const char* comment)
    {
        SealElementIfJustOpened();
        if (_textDepth < 0 && !_firstElement && !_compactMode)
        {
            Print("\n");
            PrintSpace(_depth);
        }
        _firstElement = false;
        Print("<!--%s-->", comment);
    }

    void XMLPrinter::PushDeclaration(const char* value)
    {
        SealElementIfJustOpened();
        if (_textDepth < 0 && !_firstElement && !_compactMode)
        {
            Print("\n");
            PrintSpace(_depth);
        }
        _firstElement = false;
        Print("<?%s?>", value);
    }

    void XMLPrinter::PushUnknown(const char* value)
    {
        SealElementIfJustOpened();
        if (_textDepth < 0 && !_firstElement && !_compactMode)
        {
            Print("\n");
            PrintSpace(_depth);
        }
        _firstElement = false;
        Print("<!%s>", value);
    }

    bool XMLPrinter::VisitEnter(const XMLDocument& doc)
    {
        _processEntities = doc.ProcessEntities();
        if (doc.HasBOM())
            PushHeader(true, false);
        return true;
    }

    bool XMLPrinter::VisitEnter(const XMLElement& element, const XMLAttribute* attribute)
    {
        const XMLElement* parentElem = 0;
        if (element.Parent())
            parentElem = element.Parent()->ToElement();
        const bool compactMode = parentElem ? CompactMode(*parentElem) : _compactMode;
        OpenElement(element.Name(), compactMode);
        while (attribute)
        {
            PushAttribute(attribute->Name(), attribute->Value());
            attribute = attribute->Next();
        }
        return true;
    }

    bool XMLPrinter::VisitExit(const XMLElement& element)
    {
        CloseElement(CompactMode(element));
        return true;
    }

    bool XMLPrinter::Visit(const XMLText& text)
    {
        PushText(text.Value(), text.CData());
        return true;
    }

    bool XMLPrinter::Visit(const XMLComment& comment)
    {
        PushComment(comment.Value());
        return true;
    }

    bool XMLPrinter::Visit(const XMLDeclaration& declaration)
    {
        PushDeclaration(declaration.Value());
        return true;
    }

    bool XMLPrinter::Visit(const XMLUnknown& unknown)
    {
        PushUnknown(unknown.Value());
        return true;
    }

}   // namespace tinyxml2





//???????????????????????????????????????????????????????????????????????????
#include <ctime>
#if defined( _MSC_VER )
#include <crtdbg.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
_CrtMemState startMemState;
#endif

using namespace tinyxml2;
using namespace std;
int gPass = 0;
int gFail = 0;

bool XMLTest(const char* testString, const char* expected, const char* found, bool echo = true, bool extraNL = false)
{
    bool pass;
    if (!expected && !found)
        pass = true;
    else if (!expected || !found)
        pass = false;
    else
        pass = !strcmp(expected, found);
    if (pass)
        printf("[pass]");
    else
        printf("[fail]");
    if (!echo)
        printf(" %s\n", testString);
    else
    {
        if (extraNL)
        {
            printf(" %s\n", testString);
            printf("%s\n", expected);
            printf("%s\n", found);
        }
        else
            printf(" %s [%s][%s]\n", testString, expected, found);
    }
    if (pass)
        ++gPass;
    else
        ++gFail;
    return pass;
}

bool XMLTest(const char* testString, XMLError expected, XMLError found, bool echo = true, bool extraNL = false)
{
    return XMLTest(testString, tinyxml2::XMLDocument::ErrorIDToName(expected), tinyxml2::XMLDocument::ErrorIDToName(found), echo, extraNL);
}

bool XMLTest(const char* testString, bool expected, bool found, bool echo = true, bool extraNL = false)
{
    return XMLTest(testString, expected ? "true" : "false", found ? "true" : "false", echo, extraNL);
}

template< class T > bool XMLTest(const char* testString, T expected, T found, bool echo = true)
{
    bool pass = (expected == found);
    if (pass)
        printf("[pass]");
    else
        printf("[fail]");
    if (!echo)
        printf(" %s\n", testString);
    else
        printf(" %s [%d][%d]\n", testString, static_cast<int>(expected), static_cast<int>(found));
    if (pass)
        ++gPass;
    else
        ++gFail;
    return pass;
}


void NullLineEndings(char* p)
{
    while (p && *p)
    {
        if (*p == '\n' || *p == '\r')
        {
            *p = 0;
            return;
        }
        ++p;
    }
}

int example_1()
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile("resources/dream.xml");
    return doc.ErrorID();
}
/** @page Example-1 Load an XML File
@dontinclude ./xmltest.cpp
Basic XML file loading.
The basic syntax to load an XML file from
disk and check for an error. (ErrorID()
will return 0 for no error.)
@skip example_1()
@until }
*/


int example_2()
{
    static const char* xml = "<element/>";
    tinyxml2::XMLDocument doc;
    doc.Parse(xml);
    return doc.ErrorID();
}
/** @page Example-2 Parse an XML from char buffer
@dontinclude ./xmltest.cpp
Basic XML string parsing.
The basic syntax to parse an XML for
a char* and check for an error. (ErrorID()
will return 0 for no error.)
@skip example_2()
@until }
*/


int example_3()
{
    static const char* xml =
        "<?xml version=\"1.0\"?>"
        "<!DOCTYPE PLAY SYSTEM \"play.dtd\">"
        "<PLAY>"
        "<TITLE>A Midsummer Night's Dream</TITLE>"
        "</PLAY>";
    tinyxml2::XMLDocument doc;
    doc.Parse(xml);
    XMLElement* titleElement = doc.FirstChildElement("PLAY")->FirstChildElement("TITLE");
    const char* title = titleElement->GetText();
    printf("Name of play (1): %s\n", title);
    XMLText* textNode = titleElement->FirstChild()->ToText();
    title = textNode->Value();
    printf("Name of play (2): %s\n", title);
    return doc.ErrorID();
}
/** @page Example-3 Get information out of XML
@dontinclude ./xmltest.cpp
In this example, we navigate a simple XML
file, and read some interesting text. Note
that this example doesn't use error
checking; working code should check for null
pointers when walking an XML tree, or use
XMLHandle.

(The XML is an excerpt from "dream.xml").

@skip example_3()
@until </PLAY>";

The structure of the XML file is:

<ul>
<li>(declaration)</li>
<li>(dtd stuff)</li>
<li>Element "PLAY"</li>
<ul>
<li>Element "TITLE"</li>
<ul>
<li>Text "A Midsummer Night's Dream"</li>
</ul>
</ul>
</ul>

For this example, we want to print out the
title of the play. The text of the title (what
we want) is child of the "TITLE" element which
is a child of the "PLAY" element.

We want to skip the declaration and dtd, so the
method FirstChildElement() is a good choice. The
FirstChildElement() of the Document is the "PLAY"
Element, the FirstChildElement() of the "PLAY" Element
is the "TITLE" Element.

@until ( "TITLE" );

We can then use the convenience function GetText()
to get the title of the play.

@until title );

Text is just another Node in the XML DOM. And in
fact you should be a little cautious with it, as
text nodes can contain elements.

@verbatim
Consider: A Midsummer Night's <b>Dream</b>
@endverbatim

It is more correct to actually query the Text Node
if in doubt:

@until title );

Noting that here we use FirstChild() since we are
looking for XMLText, not an element, and ToText()
is a cast from a Node to a XMLText.
*/


bool example_4()
{
    static const char* xml =
        "<information>"
        "	<attributeApproach v='2' />"
        "	<textApproach>"
        "		<v>2</v>"
        "	</textApproach>"
        "</information>";
    tinyxml2::XMLDocument doc;
    doc.Parse(xml);
    int v0 = 0;
    int v1 = 0;
    XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement("attributeApproach");
    attributeApproachElement->QueryIntAttribute("v", &v0);
    XMLElement* textApproachElement = doc.FirstChildElement()->FirstChildElement("textApproach");
    textApproachElement->FirstChildElement("v")->QueryIntText(&v1);
    printf("Both values are the same: %d and %d\n", v0, v1);
    return !doc.Error() && (v0 == v1);
}
/** @page Example-4 Read attributes and text information.
@dontinclude ./xmltest.cpp

There are fundamentally 2 ways of writing a key-value
pair into an XML file. (Something that's always annoyed
me about XML.) Either by using attributes, or by writing
the key name into an element and the value into
the text node wrapped by the element. Both approaches
are illustrated in this example, which shows two ways
to encode the value "2" into the key "v":

@skip example_4()
@until "</information>";

TinyXML-2 has accessors for both approaches.

When using an attribute, you navigate to the XMLElement
with that attribute and use the QueryIntAttribute()
group of methods. (Also QueryFloatAttribute(), etc.)

@skip XMLElement* attributeApproachElement
@until &v0 );

When using the text approach, you need to navigate
down one more step to the XMLElement that contains
the text. Note the extra FirstChildElement( "v" )
in the code below. The value of the text can then
be safely queried with the QueryIntText() group
of methods. (Also QueryFloatText(), etc.)

@skip XMLElement* textApproachElement
@until &v1 );
*/


int xmltest(int argc, char** argv)
{
#if defined( _MSC_VER ) && defined( DEBUG )
    _CrtMemCheckpoint(&startMemState);
    // Enable MS Visual C++ debug heap memory leaks dump on exit
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif
    {
        TIXMLASSERT(true);
    }
    if (argc > 1)
    {
        tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
        clock_t startTime = clock();
        doc->LoadFile(argv[1]);
        clock_t loadTime = clock();
        int errorID = doc->ErrorID();
        delete doc;
        doc = 0;
        clock_t deleteTime = clock();
        printf("Test file '%s' loaded. ErrorID=%d\n", argv[1], errorID);
        if (!errorID)
        {
            printf("Load time=%u\n", (unsigned)(loadTime - startTime));
            printf("Delete time=%u\n", (unsigned)(deleteTime - loadTime));
            printf("Total time=%u\n", (unsigned)(deleteTime - startTime));
        }
        exit(0);
    }
    FILE* fp = fopen("resources/dream.xml", "r");
    if (!fp)
    {
        printf("Error opening test file 'dream.xml'.\n"
            "Is your working directory the same as where \n"
            "the xmltest.cpp and dream.xml file are?\n\n"
#if defined( _MSC_VER )
            "In windows Visual Studio you may need to set\n"
            "Properties->Debugging->Working Directory to '..'\n"
#endif
            );
        exit(1);
    }
    fclose(fp);
    XMLTest("Example-1", 0, example_1());
    XMLTest("Example-2", 0, example_2());
    XMLTest("Example-3", 0, example_3());
    XMLTest("Example-4", true, example_4());
    /* ------ Example 2: Lookup information. ---- */
    {
        static const char* test[] = { "<element />",
            "<element></element>",
            "<element><subelement/></element>",
            "<element><subelement></subelement></element>",
            "<element><subelement><subsub/></subelement></element>",
            "<!--comment beside elements--><element><subelement></subelement></element>",
            "<!--comment beside elements, this time with spaces-->  \n <element>  <subelement> \n </subelement> </element>",
            "<element attrib1='foo' attrib2=\"bar\" ></element>",
            "<element attrib1='foo' attrib2=\"bar\" ><subelement attrib3='yeehaa' /></element>",
            "<element>Text inside element.</element>",
            "<element><b></b></element>",
            "<element>Text inside and <b>bolded</b> in the element.</element>",
            "<outer><element>Text inside and <b>bolded</b> in the element.</element></outer>",
            "<element>This &amp; That.</element>",
            "<element attrib='This&lt;That' />",
            0
        };
        for (int i = 0; test[i]; ++i)
        {
            tinyxml2::XMLDocument doc;
            doc.Parse(test[i]);
            doc.Print();
            printf("----------------------------------------------\n");
        }
    }
#if 1
    {
        static const char* test = "<!--hello world\n"
            "          line 2\r"
            "          line 3\r\n"
            "          line 4\n\r"
            "          line 5\r-->";
        tinyxml2::XMLDocument doc;
        doc.Parse(test);
        doc.Print();
    }
    {
        static const char* test = "<element>Text before.</element>";
        tinyxml2::XMLDocument doc;
        doc.Parse(test);
        XMLElement* root = doc.FirstChildElement();
        XMLElement* newElement = doc.NewElement("Subelement");
        root->InsertEndChild(newElement);
        doc.Print();
    }
    {
        tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
        static const char* test = "<element><sub/></element>";
        doc->Parse(test);
        delete doc;
    }
    {
        // Test: Programmatic DOM
        // Build:
        //      <element>
        //          <!--comment-->
        //          <sub attrib="1" />
        //          <sub attrib="2" />
        //          <sub attrib="3" >& Text!</sub>
        //      <element>
        tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
        XMLNode* element = doc->InsertEndChild(doc->NewElement("element"));
        XMLElement* sub[3] = { doc->NewElement("sub"), doc->NewElement("sub"), doc->NewElement("sub") };
        for (int i = 0; i < 3; ++i)
            sub[i]->SetAttribute("attrib", i);
        element->InsertEndChild(sub[2]);
        XMLNode* comment = element->InsertFirstChild(doc->NewComment("comment"));
        comment->SetUserData((void*)2);
        element->InsertAfterChild(comment, sub[0]);
        element->InsertAfterChild(sub[0], sub[1]);
        sub[2]->InsertFirstChild(doc->NewText("& Text!"));
        doc->Print();
        XMLTest("Programmatic DOM", "comment", doc->FirstChildElement("element")->FirstChild()->Value());
        XMLTest("Programmatic DOM", "0", doc->FirstChildElement("element")->FirstChildElement()->Attribute("attrib"));
        XMLTest("Programmatic DOM", 2, doc->FirstChildElement()->LastChildElement("sub")->IntAttribute("attrib"));
        XMLTest("Programmatic DOM", "& Text!",
            doc->FirstChildElement()->LastChildElement("sub")->FirstChild()->ToText()->Value());
        XMLTest("User data", (void*)2 == comment->GetUserData(), true, false);
        // And now deletion:
        element->DeleteChild(sub[2]);
        doc->DeleteNode(comment);
        element->FirstChildElement()->SetAttribute("attrib", true);
        element->LastChildElement()->DeleteAttribute("attrib");
        XMLTest("Programmatic DOM", true, doc->FirstChildElement()->FirstChildElement()->BoolAttribute("attrib"));
        int value1 = 10;
        int value2 = doc->FirstChildElement()->LastChildElement()->IntAttribute("attrib", 10);
        int result = doc->FirstChildElement()->LastChildElement()->QueryIntAttribute("attrib", &value1);
        XMLTest("Programmatic DOM", result, (int)XML_NO_ATTRIBUTE);
        XMLTest("Programmatic DOM", value1, 10);
        XMLTest("Programmatic DOM", value2, 10);
        doc->Print();
        {
            XMLPrinter streamer;
            doc->Print(&streamer);
            printf("%s", streamer.CStr());
        }
        {
            XMLPrinter streamer(0, true);
            doc->Print(&streamer);
            XMLTest("Compact mode", "<element><sub attrib=\"true\"/><sub/></element>", streamer.CStr(), false);
        }
        doc->SaveFile("./resources/out/pretty.xml");
        doc->SaveFile("./resources/out/compact.xml", true);
        delete doc;
    }
    {
        // Test: Dream
        // XML1 : 1,187,569 bytes   in 31,209 allocations
        // XML2 :   469,073 bytes   in    323 allocations
        //int newStart = gNew;
        tinyxml2::XMLDocument doc;
        doc.LoadFile("resources/dream.xml");
        doc.SaveFile("resources/out/dreamout.xml");
        doc.PrintError();
        XMLTest("Dream", "xml version=\"1.0\"",
            doc.FirstChild()->ToDeclaration()->Value());
        XMLTest("Dream", true, doc.FirstChild()->NextSibling()->ToUnknown() ? true : false);
        XMLTest("Dream", "DOCTYPE PLAY SYSTEM \"play.dtd\"",
            doc.FirstChild()->NextSibling()->ToUnknown()->Value());
        XMLTest("Dream", "And Robin shall restore amends.",
            doc.LastChild()->LastChild()->LastChild()->LastChild()->LastChildElement()->GetText());
        XMLTest("Dream", "And Robin shall restore amends.",
            doc.LastChild()->LastChild()->LastChild()->LastChild()->LastChildElement()->GetText());
        tinyxml2::XMLDocument doc2;
        doc2.LoadFile("resources/out/dreamout.xml");
        XMLTest("Dream-out", "xml version=\"1.0\"",
            doc2.FirstChild()->ToDeclaration()->Value());
        XMLTest("Dream-out", true, doc2.FirstChild()->NextSibling()->ToUnknown() ? true : false);
        XMLTest("Dream-out", "DOCTYPE PLAY SYSTEM \"play.dtd\"",
            doc2.FirstChild()->NextSibling()->ToUnknown()->Value());
        XMLTest("Dream-out", "And Robin shall restore amends.",
            doc2.LastChild()->LastChild()->LastChild()->LastChild()->LastChildElement()->GetText());
        //gNewTotal = gNew - newStart;
    }
    {
        const char* error = "<?xml version=\"1.0\" standalone=\"no\" ?>\n"
            "<passages count=\"006\" formatversion=\"20020620\">\n"
            "    <wrong error>\n"
            "</passages>";
        tinyxml2::XMLDocument doc;
        doc.Parse(error);
        XMLTest("Bad XML", doc.ErrorID(), XML_ERROR_PARSING_ATTRIBUTE);
    }
    {
        const char* str = "<doc attr0='1' attr1='2.0' attr2='foo' />";
        tinyxml2::XMLDocument doc;
        doc.Parse(str);
        XMLElement* ele = doc.FirstChildElement();
        int iVal, result;
        double dVal;
        result = ele->QueryDoubleAttribute("attr0", &dVal);
        XMLTest("Query attribute: int as double", result, (int)XML_SUCCESS);
        XMLTest("Query attribute: int as double", (int)dVal, 1);
        XMLTest("Query attribute: int as double", (int)ele->DoubleAttribute("attr0"), 1);
        result = ele->QueryDoubleAttribute("attr1", &dVal);
        XMLTest("Query attribute: double as double", result, (int)XML_SUCCESS);
        XMLTest("Query attribute: double as double", dVal, 2.0);
        XMLTest("Query attribute: double as double", ele->DoubleAttribute("attr1"), 2.0);
        result = ele->QueryIntAttribute("attr1", &iVal);
        XMLTest("Query attribute: double as int", result, (int)XML_SUCCESS);
        XMLTest("Query attribute: double as int", iVal, 2);
        result = ele->QueryIntAttribute("attr2", &iVal);
        XMLTest("Query attribute: not a number", result, (int)XML_WRONG_ATTRIBUTE_TYPE);
        XMLTest("Query attribute: not a number", ele->DoubleAttribute("attr2", 4.0), 4.0);
        result = ele->QueryIntAttribute("bar", &iVal);
        XMLTest("Query attribute: does not exist", result, (int)XML_NO_ATTRIBUTE);
        XMLTest("Query attribute: does not exist", ele->BoolAttribute("bar", true), true);
    }
    {
        const char* str = "<doc/>";
        tinyxml2::XMLDocument doc;
        doc.Parse(str);
        XMLElement* ele = doc.FirstChildElement();
        int iVal, iVal2;
        double dVal, dVal2;
        ele->SetAttribute("str", "strValue");
        ele->SetAttribute("int", 1);
        ele->SetAttribute("double", -1.0);
        const char* cStr = ele->Attribute("str");
        ele->QueryIntAttribute("int", &iVal);
        ele->QueryDoubleAttribute("double", &dVal);
        ele->QueryAttribute("int", &iVal2);
        ele->QueryAttribute("double", &dVal2);
        XMLTest("Attribute match test", ele->Attribute("str", "strValue"), "strValue");
        XMLTest("Attribute round trip. c-string.", "strValue", cStr);
        XMLTest("Attribute round trip. int.", 1, iVal);
        XMLTest("Attribute round trip. double.", -1, (int)dVal);
        XMLTest("Alternate query", true, iVal == iVal2);
        XMLTest("Alternate query", true, dVal == dVal2);
        XMLTest("Alternate query", true, iVal == ele->IntAttribute("int"));
        XMLTest("Alternate query", true, dVal == ele->DoubleAttribute("double"));
    }
    {
        tinyxml2::XMLDocument doc;
        doc.LoadFile("resources/utf8test.xml");
        // Get the attribute "value" from the "Russian" element and check it.
        XMLElement* element = doc.FirstChildElement("document")->FirstChildElement("Russian");
        const unsigned char correctValue[] = { 0xd1U, 0x86U, 0xd0U, 0xb5U, 0xd0U, 0xbdU, 0xd0U, 0xbdU,
            0xd0U, 0xbeU, 0xd1U, 0x81U, 0xd1U, 0x82U, 0xd1U, 0x8cU, 0
        };
        XMLTest("UTF-8: Russian value.", (const char*)correctValue, element->Attribute("value"));
        const unsigned char russianElementName[] = { 0xd0U, 0xa0U, 0xd1U, 0x83U,
            0xd1U, 0x81U, 0xd1U, 0x81U,
            0xd0U, 0xbaU, 0xd0U, 0xb8U,
            0xd0U, 0xb9U, 0
        };
        const char russianText[] = "<\xD0\xB8\xD0\xBC\xD0\xB5\xD0\xB5\xD1\x82>";
        XMLText* text = doc.FirstChildElement("document")->FirstChildElement((const char*)russianElementName)->FirstChild()->ToText();
        XMLTest("UTF-8: Browsing russian element name.",
            russianText,
            text->Value());
        // Now try for a round trip.
        doc.SaveFile("resources/out/utf8testout.xml");
        // Check the round trip.
        int okay = 0;
        FILE* saved = fopen("resources/out/utf8testout.xml", "r");
        FILE* verify = fopen("resources/utf8testverify.xml", "r");
        if (saved && verify)
        {
            okay = 1;
            char verifyBuf[256];
            while (fgets(verifyBuf, 256, verify))
            {
                char savedBuf[256];
                fgets(savedBuf, 256, saved);
                NullLineEndings(verifyBuf);
                NullLineEndings(savedBuf);
                if (strcmp(verifyBuf, savedBuf))
                {
                    printf("verify:%s<\n", verifyBuf);
                    printf("saved :%s<\n", savedBuf);
                    okay = 0;
                    break;
                }
            }
        }
        if (saved)
            fclose(saved);
        if (verify)
            fclose(verify);
        XMLTest("UTF-8: Verified multi-language round trip.", 1, okay);
    }
    // --------GetText()-----------
    {
        const char* str = "<foo>This is  text</foo>";
        tinyxml2::XMLDocument doc;
        doc.Parse(str);
        const XMLElement* element = doc.RootElement();
        XMLTest("GetText() normal use.", "This is  text", element->GetText());
        str = "<foo><b>This is text</b></foo>";
        doc.Parse(str);
        element = doc.RootElement();
        XMLTest("GetText() contained element.", element->GetText() == 0, true);
    }
    // --------SetText()-----------
    {
        const char* str = "<foo></foo>";
        tinyxml2::XMLDocument doc;
        doc.Parse(str);
        XMLElement* element = doc.RootElement();
        element->SetText("darkness.");
        XMLTest("SetText() normal use (open/close).", "darkness.", element->GetText());
        element->SetText("blue flame.");
        XMLTest("SetText() replace.", "blue flame.", element->GetText());
        str = "<foo/>";
        doc.Parse(str);
        element = doc.RootElement();
        element->SetText("The driver");
        XMLTest("SetText() normal use. (self-closing)", "The driver", element->GetText());
        element->SetText("<b>horses</b>");
        XMLTest("SetText() replace with tag-like text.", "<b>horses</b>", element->GetText());
        //doc.Print();
        str = "<foo><bar>Text in nested element</bar></foo>";
        doc.Parse(str);
        element = doc.RootElement();
        element->SetText("wolves");
        XMLTest("SetText() prefix to nested non-text children.", "wolves", element->GetText());
        str = "<foo/>";
        doc.Parse(str);
        element = doc.RootElement();
        element->SetText("str");
        XMLTest("SetText types", "str", element->GetText());
        element->SetText(1);
        XMLTest("SetText types", "1", element->GetText());
        element->SetText(1U);
        XMLTest("SetText types", "1", element->GetText());
        element->SetText(true);
        XMLTest("SetText types", "true", element->GetText());
        element->SetText(1.5f);
        XMLTest("SetText types", "1.5", element->GetText());
        element->SetText(1.5);
        XMLTest("SetText types", "1.5", element->GetText());
    }
    // ---------- Attributes ---------
    {
        static const int64_t BIG = -123456789012345678;
        tinyxml2::XMLDocument doc;
        XMLElement* element = doc.NewElement("element");
        doc.InsertFirstChild(element);
        {
            element->SetAttribute("attrib", int(-100));
            int v = 0;
            element->QueryIntAttribute("attrib", &v);
            XMLTest("Attribute: int", -100, v, true);
            element->QueryAttribute("attrib", &v);
            XMLTest("Attribute: int", -100, v, true);
            XMLTest("Attribute: int", -100, element->IntAttribute("attrib"), true);
        }
        {
            element->SetAttribute("attrib", unsigned(100));
            unsigned v = 0;
            element->QueryUnsignedAttribute("attrib", &v);
            XMLTest("Attribute: unsigned", unsigned(100), v, true);
            element->QueryAttribute("attrib", &v);
            XMLTest("Attribute: unsigned", unsigned(100), v, true);
            XMLTest("Attribute: unsigned", unsigned(100), element->UnsignedAttribute("attrib"), true);
        }
        {
            element->SetAttribute("attrib", BIG);
            int64_t v = 0;
            element->QueryInt64Attribute("attrib", &v);
            XMLTest("Attribute: int64_t", BIG, v, true);
            element->QueryAttribute("attrib", &v);
            XMLTest("Attribute: int64_t", BIG, v, true);
            XMLTest("Attribute: int64_t", BIG, element->Int64Attribute("attrib"), true);
        }
        {
            element->SetAttribute("attrib", true);
            bool v = false;
            element->QueryBoolAttribute("attrib", &v);
            XMLTest("Attribute: bool", true, v, true);
            element->QueryAttribute("attrib", &v);
            XMLTest("Attribute: bool", true, v, true);
            XMLTest("Attribute: bool", true, element->BoolAttribute("attrib"), true);
        }
        {
            element->SetAttribute("attrib", true);
            const char* result = element->Attribute("attrib");
            XMLTest("Bool true is 'true'", "true", result);
            XMLUtil::SetBoolSerialization("1", "0");
            element->SetAttribute("attrib", true);
            result = element->Attribute("attrib");
            XMLTest("Bool true is '1'", "1", result);
            XMLUtil::SetBoolSerialization(0, 0);
        }
        {
            element->SetAttribute("attrib", 100.0);
            double v = 0;
            element->QueryDoubleAttribute("attrib", &v);
            XMLTest("Attribute: double", 100.0, v, true);
            element->QueryAttribute("attrib", &v);
            XMLTest("Attribute: double", 100.0, v, true);
            XMLTest("Attribute: double", 100.0, element->DoubleAttribute("attrib"), true);
        }
        {
            element->SetAttribute("attrib", 100.0f);
            float v = 0;
            element->QueryFloatAttribute("attrib", &v);
            XMLTest("Attribute: float", 100.0f, v, true);
            element->QueryAttribute("attrib", &v);
            XMLTest("Attribute: float", 100.0f, v, true);
            XMLTest("Attribute: float", 100.0f, element->FloatAttribute("attrib"), true);
        }
        {
            element->SetText(BIG);
            int64_t v = 0;
            element->QueryInt64Text(&v);
            XMLTest("Element: int64_t", BIG, v, true);
        }
    }
    // ---------- XMLPrinter stream mode ------
    {
        {
            FILE* printerfp = fopen("resources/printer.xml", "w");
            XMLPrinter printer(printerfp);
            printer.OpenElement("foo");
            printer.PushAttribute("attrib-text", "text");
            printer.PushAttribute("attrib-int", int(1));
            printer.PushAttribute("attrib-unsigned", unsigned(2));
            printer.PushAttribute("attrib-int64", int64_t(3));
            printer.PushAttribute("attrib-bool", true);
            printer.PushAttribute("attrib-double", 4.0);
            printer.CloseElement();
            fclose(printerfp);
        }
        {
            tinyxml2::XMLDocument doc;
            doc.LoadFile("resources/printer.xml");
            XMLTest("XMLPrinter Stream mode: load", doc.ErrorID(), XML_SUCCESS, true);
            const tinyxml2::XMLDocument& cdoc = doc;
            const XMLAttribute* attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-text");
            XMLTest("attrib-text", "text", attrib->Value(), true);
            attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-int");
            XMLTest("attrib-int", int(1), attrib->IntValue(), true);
            attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-unsigned");
            XMLTest("attrib-unsigned", unsigned(2), attrib->UnsignedValue(), true);
            attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-int64");
            XMLTest("attrib-int64", int64_t(3), attrib->Int64Value(), true);
            attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-bool");
            XMLTest("attrib-bool", true, attrib->BoolValue(), true);
            attrib = cdoc.FirstChildElement("foo")->FindAttribute("attrib-double");
            XMLTest("attrib-double", 4.0, attrib->DoubleValue(), true);
        }
    }
    // ---------- CDATA ---------------
    {
        const char* str = "<xmlElement>"
            "<![CDATA["
            "I am > the rules!\n"
            "...since I make symbolic puns"
            "]]>"
            "</xmlElement>";
        tinyxml2::XMLDocument doc;
        doc.Parse(str);
        doc.Print();
        XMLTest("CDATA parse.", doc.FirstChildElement()->FirstChild()->Value(),
            "I am > the rules!\n...since I make symbolic puns",
            false);
    }
    // ----------- CDATA -------------
    {
        const char* str = "<xmlElement>"
            "<![CDATA["
            "<b>I am > the rules!</b>\n"
            "...since I make symbolic puns"
            "]]>"
            "</xmlElement>";
        tinyxml2::XMLDocument doc;
        doc.Parse(str);
        doc.Print();
        XMLTest("CDATA parse. [ tixml1:1480107 ]", doc.FirstChildElement()->FirstChild()->Value(),
            "<b>I am > the rules!</b>\n...since I make symbolic puns",
            false);
    }
    // InsertAfterChild causes crash.
    {
        // InsertBeforeChild and InsertAfterChild causes crash.
        tinyxml2::XMLDocument doc;
        XMLElement* parent = doc.NewElement("Parent");
        doc.InsertFirstChild(parent);
        XMLElement* childText0 = doc.NewElement("childText0");
        XMLElement* childText1 = doc.NewElement("childText1");
        XMLNode* childNode0 = parent->InsertEndChild(childText0);
        XMLNode* childNode1 = parent->InsertAfterChild(childNode0, childText1);
        XMLTest("Test InsertAfterChild on empty node. ", (childNode1 == parent->LastChild()), true);
    }
    {
        // Entities not being written correctly.
        // From Lynn Allen
        const char* passages =
            "<?xml version=\"1.0\" standalone=\"no\" ?>"
            "<passages count=\"006\" formatversion=\"20020620\">"
            "<psg context=\"Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
            " It also has &lt;, &gt;, and &amp;, as well as a fake copyright &#xA9;.\"> </psg>"
            "</passages>";
        tinyxml2::XMLDocument doc;
        doc.Parse(passages);
        XMLElement* psg = doc.RootElement()->FirstChildElement();
        const char* context = psg->Attribute("context");
        const char* expected = "Line 5 has \"quotation marks\" and 'apostrophe marks'. It also has <, >, and &, as well as a fake copyright \xC2\xA9.";
        XMLTest("Entity transformation: read. ", expected, context, true);
        FILE* textfile = fopen("resources/out/textfile.txt", "w");
        if (textfile)
        {
            XMLPrinter streamer(textfile);
            psg->Accept(&streamer);
            fclose(textfile);
        }
        textfile = fopen("resources/out/textfile.txt", "r");
        TIXMLASSERT(textfile);
        if (textfile)
        {
            char buf[1024];
            fgets(buf, 1024, textfile);
            XMLTest("Entity transformation: write. ",
                "<psg context=\"Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
                " It also has &lt;, &gt;, and &amp;, as well as a fake copyright \xC2\xA9.\"/>\n",
                buf, false);
            fclose(textfile);
        }
    }
    {
        // Suppress entities.
        const char* passages =
            "<?xml version=\"1.0\" standalone=\"no\" ?>"
            "<passages count=\"006\" formatversion=\"20020620\">"
            "<psg context=\"Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;.\">Crazy &ttk;</psg>"
            "</passages>";
        tinyxml2::XMLDocument doc(false);
        doc.Parse(passages);
        XMLTest("No entity parsing.", doc.FirstChildElement()->FirstChildElement()->Attribute("context"),
            "Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;.");
        XMLTest("No entity parsing.", doc.FirstChildElement()->FirstChildElement()->FirstChild()->Value(),
            "Crazy &ttk;");
        doc.Print();
    }
    {
        const char* test = "<?xml version='1.0'?><a.elem xmi.version='2.0'/>";
        tinyxml2::XMLDocument doc;
        doc.Parse(test);
        XMLTest("dot in names", doc.Error(), false);
        XMLTest("dot in names", doc.FirstChildElement()->Name(), "a.elem");
        XMLTest("dot in names", doc.FirstChildElement()->Attribute("xmi.version"), "2.0");
    }
    {
        const char* test = "<element><Name>1.1 Start easy ignore fin thickness&#xA;</Name></element>";
        tinyxml2::XMLDocument doc;
        doc.Parse(test);
        XMLText* text = doc.FirstChildElement()->FirstChildElement()->FirstChild()->ToText();
        XMLTest("Entity with one digit.",
            text->Value(), "1.1 Start easy ignore fin thickness\n",
            false);
    }
    {
        // DOCTYPE not preserved (950171)
        //
        const char* doctype =
            "<?xml version=\"1.0\" ?>"
            "<!DOCTYPE PLAY SYSTEM 'play.dtd'>"
            "<!ELEMENT title (#PCDATA)>"
            "<!ELEMENT books (title,authors)>"
            "<element />";
        tinyxml2::XMLDocument doc;
        doc.Parse(doctype);
        doc.SaveFile("resources/out/test7.xml");
        doc.DeleteChild(doc.RootElement());
        doc.LoadFile("resources/out/test7.xml");
        doc.Print();
        const XMLUnknown* decl = doc.FirstChild()->NextSibling()->ToUnknown();
        XMLTest("Correct value of unknown.", "DOCTYPE PLAY SYSTEM 'play.dtd'", decl->Value());
    }
    {
        // Comments do not stream out correctly.
        const char* doctype =
            "<!-- Somewhat<evil> -->";
        tinyxml2::XMLDocument doc;
        doc.Parse(doctype);
        XMLComment* comment = doc.FirstChild()->ToComment();
        XMLTest("Comment formatting.", " Somewhat<evil> ", comment->Value());
    }
    {
        // Double attributes
        const char* doctype = "<element attr='red' attr='blue' />";
        tinyxml2::XMLDocument doc;
        doc.Parse(doctype);
        XMLTest("Parsing repeated attributes.", XML_ERROR_PARSING_ATTRIBUTE, doc.ErrorID());     // is an  error to tinyxml (didn't use to be, but caused issues)
        doc.PrintError();
    }
    {
        // Embedded null in stream.
        const char* doctype = "<element att\0r='red' attr='blue' />";
        tinyxml2::XMLDocument doc;
        doc.Parse(doctype);
        XMLTest("Embedded null throws error.", true, doc.Error());
    }
    {
        // Empty documents should return TIXML_XML_ERROR_PARSING_EMPTY, bug 1070717
        const char* str = "";
        tinyxml2::XMLDocument doc;
        doc.Parse(str);
        XMLTest("Empty document error", XML_ERROR_EMPTY_DOCUMENT, doc.ErrorID());
    }
    {
        // Documents with all whitespaces should return TIXML_XML_ERROR_PARSING_EMPTY, bug 1070717
        const char* str = "    ";
        tinyxml2::XMLDocument doc;
        doc.Parse(str);
        XMLTest("All whitespaces document error", XML_ERROR_EMPTY_DOCUMENT, doc.ErrorID());
    }
    {
        // Low entities
        tinyxml2::XMLDocument doc;
        doc.Parse("<test>&#x0e;</test>");
        const char result[] = { 0x0e, 0 };
        XMLTest("Low entities.", doc.FirstChildElement()->GetText(), result);
        doc.Print();
    }
    {
        // Attribute values with trailing quotes not handled correctly
        tinyxml2::XMLDocument doc;
        doc.Parse("<foo attribute=bar\" />");
        XMLTest("Throw error with bad end quotes.", doc.Error(), true);
    }
    {
        // [ 1663758 ] Failure to report error on bad XML
        tinyxml2::XMLDocument xml;
        xml.Parse("<x>");
        XMLTest("Missing end tag at end of input", xml.Error(), true);
        xml.Parse("<x> ");
        XMLTest("Missing end tag with trailing whitespace", xml.Error(), true);
        xml.Parse("<x></y>");
        XMLTest("Mismatched tags", xml.ErrorID(), XML_ERROR_MISMATCHED_ELEMENT);
    }
    {
        // [ 1475201 ] TinyXML parses entities in comments
        tinyxml2::XMLDocument xml;
        xml.Parse("<!-- declarations for <head> & <body> -->"
            "<!-- far &amp; away -->");
        XMLNode* e0 = xml.FirstChild();
        XMLNode* e1 = e0->NextSibling();
        XMLComment* c0 = e0->ToComment();
        XMLComment* c1 = e1->ToComment();
        XMLTest("Comments ignore entities.", " declarations for <head> & <body> ", c0->Value(), true);
        XMLTest("Comments ignore entities.", " far &amp; away ", c1->Value(), true);
    }
    {
        tinyxml2::XMLDocument xml;
        xml.Parse("<Parent>"
            "<child1 att=''/>"
            "<!-- With this comment, child2 will not be parsed! -->"
            "<child2 att=''/>"
            "</Parent>");
        xml.Print();
        int count = 0;
        for (XMLNode* ele = xml.FirstChildElement("Parent")->FirstChild();
            ele;
            ele = ele->NextSibling())
            ++count;
        XMLTest("Comments iterate correctly.", 3, count);
    }
    {
        // trying to repro ]1874301]. If it doesn't go into an infinite loop, all is well.
        unsigned char buf[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?><feed><![CDATA[Test XMLblablablalblbl";
        buf[60] = 239;
        buf[61] = 0;
        tinyxml2::XMLDocument doc;
        doc.Parse((const char*)buf);
    }
    {
        // bug 1827248 Error while parsing a little bit malformed file
        // Actually not malformed - should work.
        tinyxml2::XMLDocument xml;
        xml.Parse("<attributelist> </attributelist >");
        XMLTest("Handle end tag whitespace", false, xml.Error());
    }
    {
        // This one must not result in an infinite loop
        tinyxml2::XMLDocument xml;
        xml.Parse("<infinite>loop");
        XMLTest("Infinite loop test.", true, true);
    }
#endif
    {
        const char* pub = "<?xml version='1.0'?> <element><sub/></element> <!--comment--> <!DOCTYPE>";
        tinyxml2::XMLDocument doc;
        doc.Parse(pub);
        tinyxml2::XMLDocument clone;
        for (const XMLNode* node = doc.FirstChild(); node; node = node->NextSibling())
        {
            XMLNode* copy = node->ShallowClone(&clone);
            clone.InsertEndChild(copy);
        }
        clone.Print();
        int count = 0;
        const XMLNode* a = clone.FirstChild();
        const XMLNode* b = doc.FirstChild();
        for (; a && b; a = a->NextSibling(), b = b->NextSibling())
        {
            ++count;
            XMLTest("Clone and Equal", true, a->ShallowEqual(b));
        }
        XMLTest("Clone and Equal", 4, count);
    }
    {
        // This shouldn't crash.
        tinyxml2::XMLDocument doc;
        if (XML_SUCCESS != doc.LoadFile("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"))
            doc.PrintError();
        XMLTest("Error in snprinf handling.", true, doc.Error());
    }
    {
        // Attribute ordering.
        static const char* xml = "<element attrib1=\"1\" attrib2=\"2\" attrib3=\"3\" />";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLElement* ele = doc.FirstChildElement();
        const XMLAttribute* a = ele->FirstAttribute();
        XMLTest("Attribute order", "1", a->Value());
        a = a->Next();
        XMLTest("Attribute order", "2", a->Value());
        a = a->Next();
        XMLTest("Attribute order", "3", a->Value());
        XMLTest("Attribute order", "attrib3", a->Name());
        ele->DeleteAttribute("attrib2");
        a = ele->FirstAttribute();
        XMLTest("Attribute order", "1", a->Value());
        a = a->Next();
        XMLTest("Attribute order", "3", a->Value());
        ele->DeleteAttribute("attrib1");
        ele->DeleteAttribute("attrib3");
        XMLTest("Attribute order (empty)", false, ele->FirstAttribute() ? true : false);
    }
    {
        // Make sure an attribute with a space in it succeeds.
        static const char* xml0 = "<element attribute1= \"Test Attribute\"/>";
        static const char* xml1 = "<element attribute1 =\"Test Attribute\"/>";
        static const char* xml2 = "<element attribute1 = \"Test Attribute\"/>";
        tinyxml2::XMLDocument doc0;
        doc0.Parse(xml0);
        tinyxml2::XMLDocument doc1;
        doc1.Parse(xml1);
        tinyxml2::XMLDocument doc2;
        doc2.Parse(xml2);
        XMLElement* ele = 0;
        ele = doc0.FirstChildElement();
        XMLTest("Attribute with space #1", "Test Attribute", ele->Attribute("attribute1"));
        ele = doc1.FirstChildElement();
        XMLTest("Attribute with space #2", "Test Attribute", ele->Attribute("attribute1"));
        ele = doc2.FirstChildElement();
        XMLTest("Attribute with space #3", "Test Attribute", ele->Attribute("attribute1"));
    }
    {
        // Make sure we don't go into an infinite loop.
        static const char* xml = "<doc><element attribute='attribute'/><element attribute='attribute'/></doc>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLElement* ele0 = doc.FirstChildElement()->FirstChildElement();
        XMLElement* ele1 = ele0->NextSiblingElement();
        bool equal = ele0->ShallowEqual(ele1);
        XMLTest("Infinite loop in shallow equal.", true, equal);
    }
    // -------- Handles ------------
    {
        static const char* xml = "<element attrib='bar'><sub>Text</sub></element>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLElement* ele = XMLHandle(doc).FirstChildElement("element").FirstChild().ToElement();
        XMLTest("Handle, success, mutable", ele->Value(), "sub");
        XMLHandle docH(doc);
        ele = docH.FirstChildElement("none").FirstChildElement("element").ToElement();
        XMLTest("Handle, dne, mutable", false, ele != 0);
    }
    {
        static const char* xml = "<element attrib='bar'><sub>Text</sub></element>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLConstHandle docH(doc);
        const XMLElement* ele = docH.FirstChildElement("element").FirstChild().ToElement();
        XMLTest("Handle, success, const", ele->Value(), "sub");
        ele = docH.FirstChildElement("none").FirstChildElement("element").ToElement();
        XMLTest("Handle, dne, const", false, ele != 0);
    }
    {
        // Default Declaration & BOM
        tinyxml2::XMLDocument doc;
        doc.InsertEndChild(doc.NewDeclaration());
        doc.SetBOM(true);
        XMLPrinter printer;
        doc.Print(&printer);
        static const char* result = "\xef\xbb\xbf<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        XMLTest("BOM and default declaration", printer.CStr(), result, false);
        XMLTest("CStrSize", printer.CStrSize(), 42, false);
    }
    {
        const char* xml = "<ipxml ws='1'><info bla=' /></ipxml>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLTest("Ill formed XML", true, doc.Error());
    }
    // QueryXYZText
    {
        const char* xml = "<point> <x>1.2</x> <y>1</y> <z>38</z> <valid>true</valid> </point>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        const XMLElement* pointElement = doc.RootElement();
        int intValue = 0;
        unsigned unsignedValue = 0;
        float floatValue = 0;
        double doubleValue = 0;
        bool boolValue = false;
        pointElement->FirstChildElement("y")->QueryIntText(&intValue);
        pointElement->FirstChildElement("y")->QueryUnsignedText(&unsignedValue);
        pointElement->FirstChildElement("x")->QueryFloatText(&floatValue);
        pointElement->FirstChildElement("x")->QueryDoubleText(&doubleValue);
        pointElement->FirstChildElement("valid")->QueryBoolText(&boolValue);
        XMLTest("QueryIntText", intValue, 1, false);
        XMLTest("QueryUnsignedText", unsignedValue, (unsigned)1, false);
        XMLTest("QueryFloatText", floatValue, 1.2f, false);
        XMLTest("QueryDoubleText", doubleValue, 1.2, false);
        XMLTest("QueryBoolText", boolValue, true, false);
    }
    {
        const char* xml = "<element><_sub/><:sub/><sub:sub/><sub-sub/></element>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLTest("Non-alpha element lead letter parses.", doc.Error(), false);
    }
    {
        const char* xml = "<element _attr1=\"foo\" :attr2=\"bar\"></element>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLTest("Non-alpha attribute lead character parses.", doc.Error(), false);
    }
    {
        const char* xml = "<3lement></3lement>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLTest("Element names with lead digit fail to parse.", doc.Error(), true);
    }
    {
        const char* xml = "<element/>WOA THIS ISN'T GOING TO PARSE";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml, 10);
        XMLTest("Set length of incoming data", doc.Error(), false);
    }
    {
        tinyxml2::XMLDocument doc;
        XMLTest("Document is initially empty", doc.NoChildren(), true);
        doc.Clear();
        XMLTest("Empty is empty after Clear()", doc.NoChildren(), true);
        doc.LoadFile("resources/dream.xml");
        XMLTest("Document has something to Clear()", doc.NoChildren(), false);
        doc.Clear();
        XMLTest("Document Clear()'s", doc.NoChildren(), true);
    }
    // ----------- Whitespace ------------
    {
        const char* xml = "<element>"
            "<a> This \nis &apos;  text  &apos; </a>"
            "<b>  This is &apos; text &apos;  \n</b>"
            "<c>This  is  &apos;  \n\n text &apos;</c>"
            "</element>";
        tinyxml2::XMLDocument doc(true, COLLAPSE_WHITESPACE);
        doc.Parse(xml);
        const XMLElement* element = doc.FirstChildElement();
        for (const XMLElement* parent = element->FirstChildElement();
            parent;
            parent = parent->NextSiblingElement())
            XMLTest("Whitespace collapse", "This is ' text '", parent->GetText());
    }
#if 0
    {
        // Passes if assert doesn't fire.
        tinyxml2::XMLDocument xmlDoc;
        xmlDoc.NewDeclaration();
        xmlDoc.NewComment("Configuration file");
        XMLElement* root = xmlDoc.NewElement("settings");
        root->SetAttribute("version", 2);
    }
#endif
    {
        const char* xml = "<element>    </element>";
        tinyxml2::XMLDocument doc(true, COLLAPSE_WHITESPACE);
        doc.Parse(xml);
        XMLTest("Whitespace  all space", true, 0 == doc.FirstChildElement()->FirstChild());
    }
    {
        // An assert should not fire.
        const char* xml = "<element/>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLElement* ele = doc.NewElement("unused");          // This will get cleaned up with the 'doc' going out of scope.
        XMLTest("Tracking unused elements", true, ele != 0, false);
    }
    {
        const char* xml = "<parent><child>abc</child></parent>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLElement* ele = doc.FirstChildElement("parent")->FirstChildElement("child");
        XMLPrinter printer;
        ele->Accept(&printer);
        XMLTest("Printing of sub-element", "<child>abc</child>\n", printer.CStr(), false);
    }
    {
        tinyxml2::XMLDocument doc;
        XMLError error = doc.LoadFile("resources/empty.xml");
        XMLTest("Loading an empty file", XML_ERROR_EMPTY_DOCUMENT, error);
        XMLTest("Loading an empty file and ErrorName as string", "XML_ERROR_EMPTY_DOCUMENT", doc.ErrorName());
        doc.PrintError();
    }
    {
        // BOM preservation
        static const char* xml_bom_preservation = "\xef\xbb\xbf<element/>\n";
        {
            tinyxml2::XMLDocument doc;
            XMLTest("BOM preservation (parse)", XML_SUCCESS, doc.Parse(xml_bom_preservation), false);
            XMLPrinter printer;
            doc.Print(&printer);
            XMLTest("BOM preservation (compare)", xml_bom_preservation, printer.CStr(), false, true);
            doc.SaveFile("resources/bomtest.xml");
        }
        {
            tinyxml2::XMLDocument doc;
            doc.LoadFile("resources/bomtest.xml");
            XMLTest("BOM preservation (load)", true, doc.HasBOM(), false);
            XMLPrinter printer;
            doc.Print(&printer);
            XMLTest("BOM preservation (compare)", xml_bom_preservation, printer.CStr(), false, true);
        }
    }
    {
        // Insertion with Removal
        const char* xml = "<?xml version=\"1.0\" ?>"
            "<root>"
            "<one>"
            "<subtree>"
            "<elem>element 1</elem>text<!-- comment -->"
            "</subtree>"
            "</one>"
            "<two/>"
            "</root>";
        const char* xmlInsideTwo = "<?xml version=\"1.0\" ?>"
            "<root>"
            "<one/>"
            "<two>"
            "<subtree>"
            "<elem>element 1</elem>text<!-- comment -->"
            "</subtree>"
            "</two>"
            "</root>";
        const char* xmlAfterOne = "<?xml version=\"1.0\" ?>"
            "<root>"
            "<one/>"
            "<subtree>"
            "<elem>element 1</elem>text<!-- comment -->"
            "</subtree>"
            "<two/>"
            "</root>";
        const char* xmlAfterTwo = "<?xml version=\"1.0\" ?>"
            "<root>"
            "<one/>"
            "<two/>"
            "<subtree>"
            "<elem>element 1</elem>text<!-- comment -->"
            "</subtree>"
            "</root>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLElement* subtree = doc.RootElement()->FirstChildElement("one")->FirstChildElement("subtree");
        XMLElement* two = doc.RootElement()->FirstChildElement("two");
        two->InsertFirstChild(subtree);
        XMLPrinter printer1(0, true);
        doc.Accept(&printer1);
        XMLTest("Move node from within <one> to <two>", xmlInsideTwo, printer1.CStr());
        doc.Parse(xml);
        subtree = doc.RootElement()->FirstChildElement("one")->FirstChildElement("subtree");
        two = doc.RootElement()->FirstChildElement("two");
        doc.RootElement()->InsertAfterChild(two, subtree);
        XMLPrinter printer2(0, true);
        doc.Accept(&printer2);
        XMLTest("Move node from within <one> after <two>", xmlAfterTwo, printer2.CStr(), false);
        doc.Parse(xml);
        XMLNode* one = doc.RootElement()->FirstChildElement("one");
        subtree = one->FirstChildElement("subtree");
        doc.RootElement()->InsertAfterChild(one, subtree);
        XMLPrinter printer3(0, true);
        doc.Accept(&printer3);
        XMLTest("Move node from within <one> after <one>", xmlAfterOne, printer3.CStr(), false);
        doc.Parse(xml);
        subtree = doc.RootElement()->FirstChildElement("one")->FirstChildElement("subtree");
        two = doc.RootElement()->FirstChildElement("two");
        doc.RootElement()->InsertEndChild(subtree);
        XMLPrinter printer4(0, true);
        doc.Accept(&printer4);
        XMLTest("Move node from within <one> after <two>", xmlAfterTwo, printer4.CStr(), false);
    }
    {
        const char* xml = "<svg width = \"128\" height = \"128\">"
            "	<text> </text>"
            "</svg>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        doc.Print();
    }
    {
        // Test that it doesn't crash.
        const char* xml = "<?xml version=\"1.0\"?><root><sample><field0><1</field0><field1>2</field1></sample></root>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        doc.PrintError();
    }
#if 1
    // the question being explored is what kind of print to use:
    // https://github.com/leethomason/tinyxml2/issues/63
    {
        //const char* xml = "<element attrA='123456789.123456789' attrB='1.001e9' attrC='1.0e-10' attrD='1001000000.000000' attrE='0.1234567890123456789'/>";
        const char* xml = "<element/>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        doc.FirstChildElement()->SetAttribute("attrA-f64", 123456789.123456789);
        doc.FirstChildElement()->SetAttribute("attrB-f64", 1.001e9);
        doc.FirstChildElement()->SetAttribute("attrC-f64", 1.0e9);
        doc.FirstChildElement()->SetAttribute("attrC-f64", 1.0e20);
        doc.FirstChildElement()->SetAttribute("attrD-f64", 1.0e-10);
        doc.FirstChildElement()->SetAttribute("attrD-f64", 0.123456789);
        doc.FirstChildElement()->SetAttribute("attrA-f32", 123456789.123456789f);
        doc.FirstChildElement()->SetAttribute("attrB-f32", 1.001e9f);
        doc.FirstChildElement()->SetAttribute("attrC-f32", 1.0e9f);
        doc.FirstChildElement()->SetAttribute("attrC-f32", 1.0e20f);
        doc.FirstChildElement()->SetAttribute("attrD-f32", 1.0e-10f);
        doc.FirstChildElement()->SetAttribute("attrD-f32", 0.123456789f);
        doc.Print();
        /*  The result of this test is platform, compiler, and library version dependent. :("
        XMLPrinter printer;
        doc.Print( &printer );
        XMLTest( "Float and double formatting.",
        "<element attrA-f64=\"123456789.12345679\" attrB-f64=\"1001000000\" attrC-f64=\"1e+20\" attrD-f64=\"0.123456789\" attrA-f32=\"1.2345679e+08\" attrB-f32=\"1.001e+09\" attrC-f32=\"1e+20\" attrD-f32=\"0.12345679\"/>\n",
        printer.CStr(),
        true );
        */
    }
#endif
    {
        // Issue #184
        // If it doesn't assert, it passes. Caused by objects
        // getting created during parsing which are then
        // inaccessible in the memory pools.
        {
            tinyxml2::XMLDocument doc;
            doc.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\"?><test>");
        }
        {
            tinyxml2::XMLDocument doc;
            doc.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\"?><test>");
            doc.Clear();
        }
    }
    {
        // If this doesn't assert in DEBUG, all is well.
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLElement* pRoot = doc.NewElement("Root");
        doc.DeleteNode(pRoot);
    }
    {
        // Should not assert in DEBUG
        XMLPrinter printer;
    }
    {
        // Issue 291. Should not crash
        const char* xml = "&#0</a>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);
        XMLPrinter printer;
        doc.Print(&printer);
    }
    {
        // Issue 299. Can print elements that are not linked in.
        // Will crash if issue not fixed.
        tinyxml2::XMLDocument doc;
        XMLElement* newElement = doc.NewElement("printme");
        XMLPrinter printer;
        newElement->Accept(&printer);
        // Delete the node to avoid possible memory leak report in debug output
        doc.DeleteNode(newElement);
    }
    {
        // Issue 302. Clear errors from LoadFile/SaveFile
        tinyxml2::XMLDocument doc;
        XMLTest("Issue 302. Should be no error initially", "XML_SUCCESS", doc.ErrorName());
        doc.SaveFile("./no/such/path/pretty.xml");
        XMLTest("Issue 302. Fail to save", "XML_ERROR_FILE_COULD_NOT_BE_OPENED", doc.ErrorName());
        doc.SaveFile("./resources/out/compact.xml", true);
        XMLTest("Issue 302. Subsequent success in saving", "XML_SUCCESS", doc.ErrorName());
    }
    {
        // If a document fails to load then subsequent
        // successful loads should clear the error
        tinyxml2::XMLDocument doc;
        XMLTest("Should be no error initially", false, doc.Error());
        doc.LoadFile("resources/no-such-file.xml");
        XMLTest("No such file - should fail", true, doc.Error());
        doc.LoadFile("resources/dream.xml");
        XMLTest("Error should be cleared", false, doc.Error());
    }
    {
        // Check that declarations are allowed only at beginning of document
        const char* xml0 = "<?xml version=\"1.0\" ?>"
            "   <!-- xml version=\"1.1\" -->"
            "<first />";
        const char* xml1 = "<?xml version=\"1.0\" ?>"
            "<?xml-stylesheet type=\"text/xsl\" href=\"Anything.xsl\"?>"
            "<first />";
        const char* xml2 = "<first />"
            "<?xml version=\"1.0\" ?>";
        const char* xml3 = "<first></first>"
            "<?xml version=\"1.0\" ?>";
        const char* xml4 = "<first><?xml version=\"1.0\" ?></first>";
        tinyxml2::XMLDocument doc;
        doc.Parse(xml0);
        XMLTest("Test that the code changes do not affect normal parsing", doc.Error(), false);
        doc.Parse(xml1);
        XMLTest("Test that the second declaration is allowed", doc.Error(), false);
        doc.Parse(xml2);
        XMLTest("Test that declaration after a child is not allowed", doc.ErrorID(), XML_ERROR_PARSING_DECLARATION);
        doc.Parse(xml3);
        XMLTest("Test that declaration after a child is not allowed", doc.ErrorID(), XML_ERROR_PARSING_DECLARATION);
        doc.Parse(xml4);
        XMLTest("Test that declaration inside a child is not allowed", doc.ErrorID(), XML_ERROR_PARSING_DECLARATION);
    }
    {
        // No matter - before or after successfully parsing a text -
        // calling XMLDocument::Value() causes an assert in debug.
        const char* validXml = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
            "<first />"
            "<second />";
        tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
        XMLTest("XMLDocument::Value() returns null?", NULL, doc->Value());
        doc->Parse(validXml);
        XMLTest("XMLDocument::Value() returns null?", NULL, doc->Value());
        delete doc;
    }
    {
        tinyxml2::XMLDocument doc;
        for (int i = 0; i < XML_ERROR_COUNT; i++)
        {
            doc.SetError((XMLError)i, 0, 0, 0);
            doc.ErrorName();
        }
    }
    // ----------- Line Number Tracking --------------
    {
        struct TestUtil : XMLVisitor
        {
            void TestParseError(const char* testString, const char* docStr, XMLError expected_error, int expectedLine)
            {
                tinyxml2::XMLDocument doc;
                XMLError err = doc.Parse(docStr);
                XMLTest(testString, true, doc.Error());
                XMLTest(testString, expected_error, err);
                XMLTest(testString, expectedLine, doc.GetErrorLineNum());
            };

            void TestStringLines(const char* testString, const char* docStr, const char* expectedLines)
            {
                tinyxml2::XMLDocument doc;
                doc.Parse(docStr);
                XMLTest(testString, false, doc.Error());
                TestDocLines(testString, doc, expectedLines);
            }

            void TestFileLines(const char* testString, const char* file_name, const char* expectedLines)
            {
                tinyxml2::XMLDocument doc;
                doc.LoadFile(file_name);
                XMLTest(testString, false, doc.Error());
                TestDocLines(testString, doc, expectedLines);
            }

        private:
            DynArray<char, 10> str;

            void Push(char type, int lineNum)
            {
                str.Push(type);
                str.Push(char('0' + (lineNum / 10)));
                str.Push(char('0' + (lineNum % 10)));
            }

            bool VisitEnter(const tinyxml2::XMLDocument& doc)
            {
                Push('D', doc.GetLineNum());
                return true;
            }
            bool VisitEnter(const XMLElement& element, const XMLAttribute* firstAttribute)
            {
                Push('E', element.GetLineNum());
                for (const XMLAttribute* attr = firstAttribute; attr != 0; attr = attr->Next())
                    Push('A', attr->GetLineNum());
                return true;
            }
            bool Visit(const XMLDeclaration& declaration)
            {
                Push('L', declaration.GetLineNum());
                return true;
            }
            bool Visit(const XMLText& text)
            {
                Push('T', text.GetLineNum());
                return true;
            }
            bool Visit(const XMLComment& comment)
            {
                Push('C', comment.GetLineNum());
                return true;
            }
            bool Visit(const XMLUnknown& unknown)
            {
                Push('U', unknown.GetLineNum());
                return true;
            }

            void TestDocLines(const char* testString, tinyxml2::XMLDocument& doc, const char* expectedLines)
            {
                str.Clear();
                doc.Accept(this);
                str.Push(0);
                XMLTest(testString, expectedLines, str.Mem());
            }
        } tester;
        tester.TestParseError("ErrorLine-Parsing", "\n<root>\n foo \n<unclosed/>", XML_ERROR_PARSING, 2);
        tester.TestParseError("ErrorLine-Declaration", "<root>\n<?xml version=\"1.0\"?>", XML_ERROR_PARSING_DECLARATION, 2);
        tester.TestParseError("ErrorLine-Mismatch", "\n<root>\n</mismatch>", XML_ERROR_MISMATCHED_ELEMENT, 2);
        tester.TestParseError("ErrorLine-CData", "\n<root><![CDATA[ \n foo bar \n", XML_ERROR_PARSING_CDATA, 2);
        tester.TestParseError("ErrorLine-Text", "\n<root>\n foo bar \n", XML_ERROR_PARSING_TEXT, 3);
        tester.TestParseError("ErrorLine-Comment", "\n<root>\n<!-- >\n", XML_ERROR_PARSING_COMMENT, 3);
        tester.TestParseError("ErrorLine-Declaration", "\n<root>\n<? >\n", XML_ERROR_PARSING_DECLARATION, 3);
        tester.TestParseError("ErrorLine-Unknown", "\n<root>\n<! \n", XML_ERROR_PARSING_UNKNOWN, 3);
        tester.TestParseError("ErrorLine-Element", "\n<root>\n<unclosed \n", XML_ERROR_PARSING_ELEMENT, 3);
        tester.TestParseError("ErrorLine-Attribute", "\n<root>\n<unclosed \n att\n", XML_ERROR_PARSING_ATTRIBUTE, 4);
        tester.TestParseError("ErrorLine-ElementClose", "\n<root>\n<unclosed \n/unexpected", XML_ERROR_PARSING_ELEMENT, 3);
        tester.TestStringLines(
            "LineNumbers-String",
            "<?xml version=\"1.0\"?>\n"                 // 1 Doc, DecL
            "<root a='b' \n"                        // 2 Element Attribute
            "c='d'> d <blah/>  \n"                  // 3 Attribute Text Element
            "newline in text \n"                    // 4 Text
            "and second <zxcv/><![CDATA[\n"         // 5 Element Text
            " cdata test ]]><!-- comment -->\n"     // 6 Comment
            "<! unknown></root>",                   // 7 Unknown
            "D01L01E02A02A03T03E03T04E05T05C06U07");
        tester.TestStringLines(
            "LineNumbers-CRLF",
            "\r\n"                                      // 1 Doc (arguably should be line 2)
            "<?xml version=\"1.0\"?>\n"                 // 2 DecL
            "<root>\r\n"                                // 3 Element
            "\n"                                        // 4
            "text contining new line \n"                // 5 Text
            " and also containing crlf \r\n"            // 6
            "<sub><![CDATA[\n"                          // 7 Element Text
            "cdata containing new line \n"              // 8
            " and also containing cflr\r\n"             // 9
            "]]></sub><sub2/></root>",                  // 10 Element
            "D01L02E03T05E07T07E10");
        tester.TestFileLines(
            "LineNumbers-File",
            "resources/utf8test.xml",
            "D01L01E02E03A03A03T03E04A04A04T04E05A05A05T05E06A06A06T06E07A07A07T07E08A08A08T08E09T09E10T10");
    }
    // ----------- Performance tracking --------------
    {
#if defined( _MSC_VER )
        __int64 start, end, freq;
        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
#endif
        FILE* perfFP = fopen("resources/dream.xml", "r");
        fseek(perfFP, 0, SEEK_END);
        long size = ftell(perfFP);
        fseek(perfFP, 0, SEEK_SET);
        char* mem = new char[size + 1];
        fread(mem, size, 1, perfFP);
        fclose(perfFP);
        mem[size] = 0;
#if defined( _MSC_VER )
        QueryPerformanceCounter((LARGE_INTEGER*)&start);
#else
        clock_t cstart = clock();
#endif
        static const int COUNT = 10;
        for (int i = 0; i < COUNT; ++i)
        {
            tinyxml2::XMLDocument doc;
            doc.Parse(mem);
        }
#if defined( _MSC_VER )
        QueryPerformanceCounter((LARGE_INTEGER*)&end);
#else
        clock_t cend = clock();
#endif
        delete[] mem;
        static const char* note =
#ifdef DEBUG
            "DEBUG";
#else
            "Release";
#endif
#if defined( _MSC_VER )
        printf("\nParsing %s of dream.xml: %.3f milli-seconds\n", note, 1000.0 * (double)(end - start) / ((double)freq * (double)COUNT));
#else
        printf("\nParsing %s of dream.xml: %.3f milli-seconds\n", note, (double)(cend - cstart) / (double)COUNT);
#endif
    }
#if defined( _MSC_VER ) &&  defined( DEBUG )
    //_CrtMemCheckpoint(&endMemState);
    //_CrtMemState diffMemState;
    //_CrtMemDifference(&diffMemState, &startMemState, &endMemState);
    //_CrtMemDumpStatistics(&diffMemState);
#endif
    printf("\nPass %d, Fail %d\n", gPass, gFail);
    return gFail;
}


//字符创建xml文件
void xml_example_01(void)
{
    //字符串创建XML文件
    static const char* xml =
        "<?xml version=\"1.0\"?>"
        "<!DOCTYPE PLAY SYSTEM \"play.dtd\">"
        "<PLAY>"
        "<TITLE>A Midsummer Night's Dream</TITLE>"
        "</PLAY>";
    tinyxml2::XMLDocument doc;
    //doc.LoadFile("example_1.xml");
    doc.Parse(xml);
    doc.SaveFile("example_1.xml");
}

//基本创建文件
void xml_example_02(void)
{
    tinyxml2::XMLDocument doc;
    //doc.SetBOM(true);
    //创建根节点
    XMLElement* rootElement = doc.NewElement("root");
    doc.InsertEndChild(rootElement);
    //创建节点
    XMLElement* oneNode = doc.NewElement("node1");
    //节点挂子节点
    rootElement->InsertEndChild(oneNode);
    //设置标签名
    oneNode->SetValue("value");  //oneNode->SetName("name");
    //设置属性
    oneNode->SetAttribute("att", "attvalue");
    //设置值
    oneNode->SetText("text");

    doc.SaveFile("example_2.xml");
}