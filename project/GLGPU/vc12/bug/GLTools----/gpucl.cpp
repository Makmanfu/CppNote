
#include "stdafx.h"
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <glew.h>
#include "gpucl.h"

//------------------------------------------------------------------
struct OpenGL
{
    // Shaders
    GLuint createRuns;
    GLuint gatherNeighbor;
    GLuint updateColumn;
    GLuint scatterBack;
    GLuint updateRoots;
    GLuint updateChildren;
    GLuint blockBorders;
    GLuint computeBBoxes;
    GLuint getIDCount;
    GLuint stencilRoute;

    // Textures and objects
    GLuint fbo;
    GLuint frame;
    GLuint idColorAttachments[2];
    GLuint bboxColorAttachments[2];
    GLuint depthStencil;
    GLuint columnVertsVBO;
    GLuint borderVertsVBO;
    GLuint occlusionQuery;

    // Fields
    int width, height, numBlocks;
    int componentCount;
};

void ParseShaderTextFile(string filename, map<string, string>* shaders);
GLuint LoadShaderFromString(GLenum type, string str);
GLuint CreateProgram(GLint vertexID, GLint fragmentID);
bool LoadShaders(OpenGL* ogl);
bool LoadObjects(OpenGL* ogl, int width, int height, int numBlocks);

typedef void (APIENTRY* PFNCOPYIMAGESUBDATANV)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ,
        GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ,
        GLsizei width, GLsizei height, GLsizei depth);
//PFNCOPYIMAGESUBDATANV glCopyImageSubDataNV = 0;

int logBase2(int n)
{
    int ret = 0;
    while (n >= 1)
    {
        ret++;
        n = n >> 1;
    }
    return ret;
}

//------------------------------------------------------------------
GPUCCL::~GPUCCL()
{
    OpenGL* ogl = (OpenGL*) _internal;
    glDeleteProgram(ogl->createRuns);
    glDeleteProgram(ogl->gatherNeighbor);
    glDeleteProgram(ogl->updateColumn);
    glDeleteProgram(ogl->scatterBack);
    glDeleteProgram(ogl->updateRoots);
    glDeleteProgram(ogl->updateChildren);
    glDeleteProgram(ogl->blockBorders);
    glDeleteProgram(ogl->computeBBoxes);
    glDeleteProgram(ogl->getIDCount);
    glDeleteProgram(ogl->stencilRoute);
    glDeleteTextures(1, &ogl->frame);
    glDeleteTextures(1, &ogl->idColorAttachments[0]);
    glDeleteTextures(1, &ogl->idColorAttachments[1]);
    glDeleteTextures(1, &ogl->bboxColorAttachments[0]);
    glDeleteTextures(1, &ogl->bboxColorAttachments[1]);
    glDeleteRenderbuffers(1, &ogl->depthStencil);
    glDeleteFramebuffers(1, &ogl->fbo);
    delete ogl;
}

//------------------------------------------------------------------
bool GPUCCL::init(int width, int height, int numBlocks)
{
    OpenGL* ogl = new OpenGL();
    ogl->width = width;
    ogl->height = height;
    ogl->numBlocks = numBlocks;
    _internal = (void*) ogl;
    if (glCopyImageSubDataNV == 0)
    {
        glCopyImageSubDataNV = (PFNCOPYIMAGESUBDATANV) wglGetProcAddress("glCopyImageSubDataNV");
        if (glCopyImageSubDataNV == 0)
        {
            printf("ERROR initializing glCopyImageSubDataNV\n");
            return false;
        }
    }
    if (!LoadShaders(ogl)) return false;
    if (!LoadObjects(ogl, width, height, numBlocks)) return false;
    cout << "GPUCCL initialization successfull" << endl;
    return true;
}

//------------------------------------------------------------------
void GPUCCL::begin(unsigned char* image)
{
    OpenGL* ogl = (OpenGL*) _internal;
    // Set OpenGL states
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glClampColorARB(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
    glClampColorARB(GL_CLAMP_READ_COLOR, GL_FALSE);
    glClampColorARB(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glClearDepth(0.0f);
    glClearStencil(0);
    // Upload image to GPU
    glBindTexture(GL_TEXTURE_2D, ogl->frame);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, ogl->width, ogl->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, image);
    // Bind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, ogl->fbo);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Setup projection and viewport
    glViewport(0, 0, ogl->width, ogl->height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, ogl->width, 0, ogl->height, 0.0, 20.0);
    // Set pixel centers at integer values
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.375f, 0.375f, 0.0f);
    // Set active texture unit
    glActiveTextureARB(GL_TEXTURE0);
}

//------------------------------------------------------------------
void GPUCCL::createRuns()
{
    OpenGL* ogl = (OpenGL*) _internal;
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glUseProgram(ogl->createRuns);
    GLuint passLoc = glGetUniformLocation(ogl->createRuns, "pass");
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_ALWAYS);
    for (int i = 0; i < logBase2(ogl->height); i++)
    {
        glUniform1i(passLoc, i);
        if (i == 0)
        {
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
            glBindTexture(GL_TEXTURE_2D, ogl->frame);
        }
        else
        {
            if (i % 2 == 0)
            {
                glDrawBuffer(GL_COLOR_ATTACHMENT0);
                glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[1]);
            }
            else
            {
                glDrawBuffer(GL_COLOR_ATTACHMENT1);
                glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[0]);
            }
        }
        glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(ogl->width, 0);
        glVertex2i(ogl->width, ogl->height);
        glVertex2i(0, ogl->height);
        glEnd();
    }
    glCopyImageSubDataNV(ogl->idColorAttachments[0], GL_TEXTURE_2D, 0, 0, 0, 0,
                         ogl->idColorAttachments[1], GL_TEXTURE_2D, 0, 0, 0, 0,
                         ogl->width, ogl->height, 1);
}

//------------------------------------------------------------------
void GPUCCL::processColumns()
{
    OpenGL* ogl = (OpenGL*) _internal;
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, ogl->columnVertsVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_SHORT, 0, 0);
    // Enable OpenGL states
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_MAX);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDepthMask(GL_FALSE);
    // Process columns from right to left
    int blockSize = (int) ceil(ogl->width / (float) ogl->numBlocks);
    for (int col = blockSize - 2; col >= 0; col--)
    {
        // This is needed if multiple blocks are being processed.
        // It offsets each vertex in the VBO by the correct amount.
        glTranslatef((GLfloat) col, 0.0f, 0.0f);
        // Gat  glTranslatef((GLfloat)col, 0.0f, 0.0f);
        // Gather neighbor
        glUseProgram(ogl->gatherNeighbor);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[1]);
        glDrawArrays(GL_POINTS, 0, ogl->height * ogl->numBlocks);
        // Update columns
        glUseProgram(ogl->updateColumn);
        glDrawBuffer(GL_COLOR_ATTACHMENT1);
        glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[0]);
        glBegin(GL_LINES);
        for (int b = 0; b < ogl->numBlocks; b++)
        {
            glVertex2i(b * blockSize, 0);
            glVertex2i(b * blockSize, ogl->height);
        }
        glEnd();
        // Scatter back
        glDepthMask(GL_TRUE);
        glUseProgram(ogl->scatterBack);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[1]);
        glDrawArrays(GL_POINTS, 0, ogl->height * ogl->numBlocks);
        glDepthMask(GL_FALSE);
        // Undo translation
        glTranslatef(- (GLfloat) col, 0.0f, 0.0f);
    }
    // Undo state changes
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
}

//------------------------------------------------------------------
void GPUCCL::postProcessColumns()
{
    OpenGL* ogl = (OpenGL*) _internal;
    // Update run roots
    glUseProgram(ogl->updateRoots);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[0]);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_EQUAL);
    glDepthMask(GL_FALSE);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(ogl->width, 0);
    glVertex2i(ogl->width, ogl->height);
    glVertex2i(0, ogl->height);
    glEnd();
    glDepthFunc(GL_ALWAYS);
    glDisable(GL_DEPTH_TEST);
    // Update children
    glUseProgram(ogl->updateChildren);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[1]);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(ogl->width, 0);
    glVertex2i(ogl->width, ogl->height);
    glVertex2i(0, ogl->height);
    glEnd();
    // Merge blocks
    if (ogl->numBlocks > 1)
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBindBuffer(GL_ARRAY_BUFFER, ogl->borderVertsVBO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_SHORT, 0, 0);
        int blockSize = (int) ceil(ogl->width / (float) ogl->numBlocks);
        for (int i = 0; i < logBase2(ogl->numBlocks) + 1; i++)
        {
            // Process borders
            glDepthMask(GL_TRUE);
            glUseProgram(ogl->blockBorders);
            glDrawBuffer(GL_COLOR_ATTACHMENT1);
            glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[0]);
            glDrawArrays(GL_POINTS, 0, ogl->height * 2 * (ogl->numBlocks - 1));
            glDepthMask(GL_FALSE);
            // Update children in entire image
            glUseProgram(ogl->updateChildren);
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
            glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[1]);
            glBegin(GL_QUADS);
            glVertex2i(0, 0);
            glVertex2i(ogl->width, 0);
            glVertex2i(ogl->width, ogl->height);
            glVertex2i(0, ogl->height);
            glEnd();
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        // Update roots (again)
        glUseProgram(ogl->updateRoots);
        glDrawBuffer(GL_COLOR_ATTACHMENT1);
        glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[0]);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_EQUAL);
        glDepthMask(GL_FALSE);
        glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(ogl->width, 0);
        glVertex2i(ogl->width, ogl->height);
        glVertex2i(0, ogl->height);
        glEnd();
        glDepthFunc(GL_ALWAYS);
        glDisable(GL_DEPTH_TEST);
        // Update children (again)
        glUseProgram(ogl->updateChildren);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[1]);
        glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(ogl->width, 0);
        glVertex2i(ogl->width, ogl->height);
        glVertex2i(0, ogl->height);
        glEnd();
    }
}

//------------------------------------------------------------------
void GPUCCL::computeBBoxes()
{
    OpenGL* ogl = (OpenGL*) _internal;
    // Reset bbox texture
    GLuint buffers[] = { GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(2, buffers);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Compute bounding boxes
    glUseProgram(ogl->computeBBoxes);
    glDrawBuffer(GL_COLOR_ATTACHMENT2);
    glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[0]);
    glEnable(GL_BLEND);
    glBindBuffer(GL_ARRAY_BUFFER, ogl->columnVertsVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_SHORT, 0, 0);
    glPushMatrix();
    for (int x = 0; x < ogl->width; x++)
    {
        glDrawArrays(GL_POINTS, 0, ogl->height);
        glTranslatef(1.0f, 0.0f, 0.0f);
    }
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//------------------------------------------------------------------
void GPUCCL::stencilRoute()
{
    OpenGL* ogl = (OpenGL*) _internal;
    // Get ID count
    glUseProgram(ogl->getIDCount);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[0]);
    glDisable(GL_BLEND);
    GLuint count;
    glBeginQuery(GL_SAMPLES_PASSED, ogl->occlusionQuery);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(ogl->width, 0);
    glVertex2i(ogl->width, ogl->height);
    glVertex2i(0, ogl->height);
    glEnd();
    glEndQuery(GL_SAMPLES_PASSED);
    glGetOcclusionQueryuivNV(ogl->occlusionQuery, GL_QUERY_RESULT, &count);
    ogl->componentCount = count;
    // Prep stencil buffer
    int stencilWidth = (int) ceil(sqrt((float) count));
    if (stencilWidth % 2 == 0)
        stencilWidth++;
    unsigned char* stencilData = new unsigned char[stencilWidth * stencilWidth];
    for (int i = 0; i < stencilWidth * stencilWidth; i++)
    {
        stencilData[i] = i + 1; // 1, 2, 3, etc.
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, stencilWidth);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glRasterPos2i(0, 0);
    glDrawPixels(stencilWidth, stencilWidth, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencilData);
    delete[] stencilData;
    // Stencil route results
    GLenum attachments[] = { GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(2, attachments);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ogl->bboxColorAttachments[0]);
    glUseProgram(ogl->stencilRoute);
    GLint loc = glGetUniformLocation(ogl->stencilRoute, "stencilCenter");
    glUniform2f(loc, (stencilWidth - 1.0f) * 0.5f, (stencilWidth - 1.0f) * 0.5f);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_DECR, GL_DECR, GL_DECR);
    glPointSize((GLfloat) stencilWidth);
    glBindBuffer(GL_ARRAY_BUFFER, ogl->columnVertsVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_SHORT, 0, 0);
    // Render a vertical line of vertices for each column in the image
    glPushMatrix();
    for (int x = 0; x < ogl->width; x++)
    {
        glDrawArrays(GL_POINTS, 0, ogl->height);
        glTranslatef(1.0f, 0.0f, 0.0f);
    }
    glPopMatrix();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_STENCIL_TEST);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
}

//------------------------------------------------------------------
void GPUCCL::end()
{
    glPopAttrib();
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
}

//------------------------------------------------------------------
int GPUCCL::labeledTextureID()
{
    // If this is called AFTER stencil routing, data will be corrupt
    OpenGL* ogl = (OpenGL*) _internal;
    return ogl->idColorAttachments[0];
}

//------------------------------------------------------------------
float* GPUCCL::labeledFrame()
{
    OpenGL* ogl = (OpenGL*) _internal;
    float* pixels = new float[ogl->width * ogl->height];
    glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[0]);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_FLOAT, pixels);
    return pixels;
}

//------------------------------------------------------------------
vector<GPUCCL::Component> GPUCCL::results()
{
    vector<GPUCCL::Component> list;
    OpenGL* ogl = (OpenGL*) _internal;
    int stencilWidth = (int) ceil(sqrt((float) ogl->componentCount));
    if (stencilWidth % 2 == 0)
        stencilWidth++;
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glPixelStorei(GL_PACK_ROW_LENGTH, stencilWidth);
    // EWidth * stencilWidth];
    float* IDs = new float[stencilWidth * stencilWidth * 4];
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glReadPixels(0, 0, stencilWidth, stencilWidth, GL_LUMINANCE, GL_FLOAT, IDs);
    // Extract bboxes
    float* bboxes = new float[stencilWidth * stencilWidth * 4];
    glReadBuffer(GL_COLOR_ATTACHMENT3);
    glReadPixels(0, 0, stencilWidth, stencilWidth, GL_RGBA, GL_FLOAT, bboxes);
    // Create component objects
    for (int i = 0; i < ogl->componentCount; i++)
    {
        GPUCCL::Component c;
        c.ID = (int) IDs[i];
        c.minX = ogl->width - (int) bboxes[i * 4 + 0];
        c.minY = ogl->height - (int) bboxes[i * 4 + 1];
        c.maxX = (int) bboxes[i * 4 + 2];
        c.maxY = (int) bboxes[i * 4 + 3];
        list.push_back(c);
    }
    return list;
}

//------------------------------------------------------------------
void ParseShaderTextFile(string filename, map<string, string>* shaders)
{
    FILE* fp = fopen(filename.c_str(), "rb");
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = new char[length + 1];
    fread(buffer, sizeof(char), length, fp);
    buffer[length] = 0;
    fclose(fp);
    string textFile = buffer;
    delete[] buffer;
    int startPos = 0;
    string section = "";
    bool readSection = false;
    for (int i = 0; i < textFile.length(); i++)
    {
        if (textFile[i] == '<' && i < textFile.length() - 1 && textFile[i + 1] == '%')
        {
            if (readSection == true)
            {
                string content = textFile.substr(startPos, i - startPos);
                (*shaders) [section] = content;
                readSection = false;
            }
            startPos = i + 2;
        }
        else if (textFile[i] == '%' && i < textFile.length() - 1 && textFile[i + 1] == '>')
        {
            section = textFile.substr(startPos, i - startPos);
            readSection = true;
            startPos = i + 2;
        }
    }
    if (readSection)
    {
        string content = textFile.substr(startPos, textFile.length() - startPos);
        (*shaders) [section] = content;
    }
}

//------------------------------------------------------------------
GLuint LoadShaderFromString(GLenum type, string str)
{
    GLuint id = glCreateShader(type);
    const char* code = str.c_str();
    glShaderSource(id, 1, (const GLchar**) &code, 0);
    glCompileShader(id);
    GLint compiled;
    glGetObjectParameterivARB(id, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
    if (compiled == GL_FALSE)
    {
        int infologLength = 0;
        int charsWritten = 0;
        char* infoLog;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLength);
        if (infologLength > 0)
        {
            infoLog = new char[infologLength];
            glGetShaderInfoLog(id, infologLength, &charsWritten, infoLog);
            printf("%s", infoLog);
            delete[] infoLog;
            return 0;
        }
    }
    return id;
}

//------------------------------------------------------------------
GLuint CreateProgram(GLuint vertexID, GLuint fragmentID)
{
    GLuint id = glCreateProgram();
    glAttachShader(id, vertexID);
    glAttachShader(id, fragmentID);
    glLinkProgram(id);
    GLint linked;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        int length = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        char* log = new char[length];
        int count = 0;
        glGetProgramInfoLog(id, length, &count, log);
        printf("%s", log);
        delete[] log;
        return 0;
    }
    return id;
}

//------------------------------------------------------------------
bool LoadShaders(OpenGL* ogl)
{
    map<string, string> shaders;
    ParseShaderTextFile("shaders.txt", &shaders);
    GLuint vertexID, fragmentID;
    int programID;
    GLuint loc;
    // Create runs
    cout << "Loading CreateRuns shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["CreateRuns.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["CreateRuns.frag"]);
    if (!glIsProgram(ogl->createRuns = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->createRuns);
    loc = glGetUniformLocation(ogl->createRuns, "tex");
    glUniform1i(loc, 0);
    // Gather neighbor
    cout << "Loading GatherNeighbor shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["GatherNeighbor.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["GatherNeighbor.frag"]);
    if (!glIsProgram(ogl->gatherNeighbor = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->gatherNeighbor);
    loc = glGetUniformLocation(ogl->gatherNeighbor, "tex");
    glUniform1i(loc, 0);
    // Update columns
    cout << "Loading UpdateColumns shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["UpdateColumn.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["UpdateColumn.frag"]);
    if (!glIsProgram(ogl->updateColumn = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->updateColumn);
    loc = glGetUniformLocation(ogl->updateColumn, "tex");
    glUniform1i(loc, 0);
    // Scatter back
    cout << "Loading ScatterBack shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["ScatterBack.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["ScatterBack.frag"]);
    if (!glIsProgram(ogl->scatterBack = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->scatterBack);
    loc = glGetUniformLocation(ogl->scatterBack, "tex");
    glUniform1i(loc, 0);
    // Update roots
    cout << "Loading UpdateRoots shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["UpdateRoots.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["UpdateRoots.frag"]);
    if (!glIsProgram(ogl->updateRoots = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->updateRoots);
    loc = glGetUniformLocation(ogl->updateRoots, "tex");
    glUniform1i(loc, 0);
    // Update children
    cout << "Loading UpdateChildren shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["UpdateChildren.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["UpdateChildren.frag"]);
    if (!glIsProgram(ogl->updateChildren = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->updateChildren);
    loc = glGetUniformLocation(ogl->updateChildren, "tex");
    glUniform1i(loc, 0);
    // Block borders
    cout << "Loading BlockBorders shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["BlockBorders.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["BlockBorders.frag"]);
    if (!glIsProgram(ogl->blockBorders = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->blockBorders);
    loc = glGetUniformLocation(ogl->blockBorders, "tex");
    glUniform1i(loc, 0);
    // Compute bboxes
    cout << "Loading ComputeBBoxes shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["ComputeBBoxes.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["ComputeBBoxes.frag"]);
    if (!glIsProgram(ogl->computeBBoxes = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->computeBBoxes);
    loc = glGetUniformLocation(ogl->computeBBoxes, "tex");
    glUniform1i(loc, 0);
    // Get ID count
    cout << "Loading GetIDCount shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["GetIDCount.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["GetIDCount.frag"]);
    if (!glIsProgram(ogl->getIDCount = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->getIDCount);
    loc = glGetUniformLocation(ogl->getIDCount, "tex");
    glUniform1i(loc, 0);
    // Stencil routing
    cout << "Loading StencilRoute shader..." << endl;
    vertexID = LoadShaderFromString(GL_VERTEX_SHADER, shaders["Common.glsl"] + "\n" + shaders["StencilRoute.vert"]);
    fragmentID = LoadShaderFromString(GL_FRAGMENT_SHADER, shaders["Common.glsl"] + "\n" + shaders["StencilRoute.frag"]);
    if (!glIsProgram(ogl->stencilRoute = CreateProgram(vertexID, fragmentID)))
        return false;
    glUseProgram(ogl->stencilRoute);
    loc = glGetUniformLocation(ogl->stencilRoute, "tex");
    glUniform1i(loc, 0);
    loc = glGetUniformLocation(ogl->stencilRoute, "bboxTex");
    glUniform1i(loc, 1);
    glUseProgram(0);
    return true;
}

//------------------------------------------------------------------
bool LoadObjects(OpenGL* ogl, int width, int height, int numBlocks)
{
    // Frame texture
    if (!glIsTexture(ogl->frame))
        glGenTextures(1, &ogl->frame);
    glBindTexture(GL_TEXTURE_2D, ogl->frame);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // ID color attachment textures
    for (int i = 0; i < 2; i++)
    {
        if (!glIsTexture(ogl->idColorAttachments[i]))
            glGenTextures(1, &ogl->idColorAttachments[i]);
        glBindTexture(GL_TEXTURE_2D, ogl->idColorAttachments[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE32F_ARB, width, height, 0, GL_LUMINANCE, GL_FLOAT, 0);
        cout << "Creating GL_LUMINANCE32F_ARB texture..." << glewGetErrorString(glGetError()) << endl;
    }
    // BBox color attachment textures
    for (int i = 0; i < 2; i++)
    {
        if (!glIsTexture(ogl->bboxColorAttachments[i]))
            glGenTextures(1, &ogl->bboxColorAttachments[i]);
        glBindTexture(GL_TEXTURE_2D, ogl->bboxColorAttachments[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        cout << "Creating GL_RGBA16F_ARB texture..." << glewGetErrorString(glGetError()) << endl;
    }
    // Depth and stencil buffers
    if (!glIsRenderbuffer(ogl->depthStencil))
        glGenRenderbuffers(1, &ogl->depthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, ogl->depthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    cout << "Created GL_DEPTH24_STENCIL8 attachment..." << endl;
    // FBO
    if (!glIsFramebuffer(ogl->fbo))
        glGenFramebuffers(1, &ogl->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, ogl->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ogl->idColorAttachments[0], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, ogl->idColorAttachments[1], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, ogl->bboxColorAttachments[0], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, ogl->bboxColorAttachments[1], 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ogl->depthStencil);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ogl->depthStencil);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Creating framebuffer...ERROR CODE " << status << endl;
        return false;
    }
    else
        cout << "Created framebuffer successfully..." << endl;
    // Column verts vbo
    if (!glIsBuffer(ogl->columnVertsVBO))
        glGenBuffers(1, &ogl->columnVertsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ogl->columnVertsVBO);
    int blockWidth = (int) ceil(width / (float) numBlocks);
    unsigned short* data = new unsigned short[width * height * 2];
    for (int b = 0; b < numBlocks; b++)
    {
        for (int y = 0; y < height; y++)
        {
            data[(b * height + y) * 2 + 0] = b * blockWidth;
            data[(b * height + y) * 2 + 1] = y;
        }
    }
    glBufferData(GL_ARRAY_BUFFER, width * height * 2 * sizeof(short), data, GL_STATIC_DRAW);
    delete[] data;
    cout << "Created column VBO" << endl;
    // Block borders vbo
    if (!glIsBuffer(ogl->borderVertsVBO))
        glGenBuffers(1, &ogl->borderVertsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ogl->borderVertsVBO);
    if (numBlocks > 1)
    {
        int count = height * 3 * 2 * (numBlocks - 1);
        short* borderData = new short[count];
        int i = 0;
        for (int b = 0; b < numBlocks - 1; b++)
        {
            for (int y = 0; y < height; y++)
            {
                // column left of the border
                borderData[i * 3 + 0] = (b + 1) * blockWidth - 1;
                borderData[i * 3 + 1] = y;
                borderData[i * 3 + 2] = 1;
                i++;
                // column right of the border
                borderData[i * 3 + 0] = (b + 1) * blockWidth;
                borderData[i * 3 + 1] = y;
                borderData[i * 3 + 2] = -1;
                i++;
            }
        }
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(short), borderData, GL_STATIC_DRAW);
        delete[] borderData;
        cout << "Created block border VBO" << endl;
    }
    // Occlusion query
    glGenQueries(1, &ogl->occlusionQuery);
    cout << "Created occlusion query" << endl;
    // Cleanup
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return true;
}