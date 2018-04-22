//-----------------------------------------------------------------------------
//
//          ����������
//
//-----------------------------------------------------------------------------
#ifndef EFFECTIVE_HPP
#define EFFECTIVE_HPP
#include <stdint.h>
#include <iostream>
#include <cstring>
using namespace std;

//��Ŀ�������
class ClassCopy
{
    //���ǳ����[��Ա����ָ��]
    class copyObjShallow
    {
    public:
        //���캯��
        copyObjShallow(const string& str) : strdata(str), num(str.size()) {};
        //ί�й��캯��
        copyObjShallow() : copyObjShallow("") {};
        //��������
        copyObjShallow(const copyObjShallow& cp) : strdata(cp.strdata), num(cp.num) {};
        //������ֵ���캯��
        copyObjShallow& operator= (const copyObjShallow& cp)
        {
            strdata = cp.strdata;
            num = cp.num;
        };
        //�������� [����������Ͳ���Ҫ��]
        ~copyObjShallow() {};
        //��������
        void show(void)
        {
            cout << strdata.c_str() << num << endl;
        }
    private:
        string strdata;
        int num;
    };
    //������[��Ա��ָ�� ������ǳ����]
    class copyObjDeep final //final��ֹ����
    {
    public:
        //ί�й��캯��
        copyObjDeep() : copyObjDeep("") {};
        //���캯��
        copyObjDeep(const string& str) : pData(new string(str)), len(str.size()), num(new size_t (1)) {};
        //��������
        copyObjDeep(const copyObjDeep& cp) : pData(new string(*cp.pData)), len(cp.len), num(cp.num)
        {
            ++*num;
        };
        //������ֵ���캯��
        copyObjDeep& operator= (const copyObjDeep& cp)
        {
            //�������Ҹ�ֵ
            auto newp = new string(*cp.pData);      //�����ײ�string
            ++*cp.num;                              //�����Ҳ������������ü���
            if (--*num == 0)                        //Ȼ��ݼ�����������ü���
            {
                delete pData;                       //�ͷž��ڴ�
                delete num;                         //�ͷű��������ĳ�Ա
            }
            pData = newp;                           //�õ��¿���
            len = cp.len;
            num = cp.num;
            return *this;
        };
        //��������
        ~copyObjDeep()
        {
            if (--*num == 0)
            {
                delete pData;
                delete num;
            }
        };
        //��������
        void show(void)
        {
            cout << pData->c_str() << "  " << len << "  " << *num << endl;
        };
    private:
        string* pData;      //ָ������
        int len;            //����
        size_t* num;        //���ü���
    };
public:
    static int main(int argc, char** argv)
    {
        copyObjShallow Shallow1("abcde");
        copyObjShallow Shallow2(Shallow1);
        copyObjShallow Shallow3 = Shallow2;
        Shallow1.show();
        Shallow2.show();
        Shallow3.show();
        copyObjDeep Deep1("xyz");
        Deep1.show();
        copyObjDeep Deep2(Deep1);
        Deep2.show();
        copyObjDeep Deep3 = Deep2;
        Deep1.show();
        Deep2.show();
        Deep3.show();
        Deep3 = Deep3;
        Deep3.show();

        //int d[10] = { 75, 99, 90, 93, 38, 15, 5, 7, 52, 4 };
        //ArrayS r1(d, 10);
        //cout << "�ͣ�" << r1.sum() << endl;
        //int e[15] = { 18, 68, 10, 52, 3, 19, 12, 100, 56, 96, 95, 97, 1, 4, 93 };
        //ArrayS r2(e, 15);
        //cout << "�ͣ�" << r2.sum() << endl;
        return 1;
    };
};

//�麯�������
class ClassVirtual
{
    class CA
    {
    public:
        virtual void Func1(int& i)  { i = 1; };
        void Func2(int& i)          { i = 2; };
        virtual void Func3(int& i)  { i = 3; };
        void Func4(int& i)          { i = 4; };
    };
    class CB : public CA
    {
    public:
        virtual void Func1(int& i)  { i = 5; };
        void Func2(int& i)          { i = 6; };
        void Func3(int i)           { i = 7; };
        virtual void Func4(int i)   { i = 8; };
    };
public:
    static int main(int argc, char** argv)
    {
        int i1(0), i2(0), i3(0), i4(0), i5(0), i6(0), i7(0), i8(0);
        CB* pCB = new CB;
        pCB->Func1(i1);             //5
        ((CA*) pCB)->Func1(i2);     //5
        pCB->Func2(i3);             //6
        ((CA*) pCB)->Func2(i4);     //2
        pCB->Func3(i5);             //0
        ((CA*) pCB)->Func3(i6);     //3
        pCB->Func4(i7);             //0
        ((CA*) pCB)->Func4(i8);     //4
        printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", i1, i2, i3, i4, i5, i6, i7, i8);
        return 1;
    }
};

//�����������
class CVirtualDestructor
{
    struct A
    {
        ~A() { cout << "A::~A" << endl; };
    };

    struct B :public A
    {
        ~B() { cout << "B::~B" << endl; };
    };

    struct C :public B
    {
        ~C() { cout << "C::~C" << endl; };
    };

    static int TEST(int argc, char** argv)
    {
        A *a = new A(); delete a; cout << endl;  //�ͷ� A
        B *b = new B(); delete b; cout << endl;  //�ͷ� B A
        C *c = new C(); delete c; cout << endl;  //�ͷ� C B A
        A *d = new B(); delete d; cout << endl;  //��ʼ���� ���ͷ� A
        A *e = new C(); delete e; cout << endl;  //���ͷ� A
        B *f = new C(); delete f; cout << endl;  //���ͷ� B A
        system("pause");
        return 0;
    }
};

//ģ��string�ַ�����
class StrChar
{
public:
    static int main(int argc, char** argv)
    {
        StrChar a("hello");
        StrChar b(a);
        StrChar c = a;
        a.show();
        b.show();
        c.show();
        return 1;
    };
public:
    //Ĭ�Ϲ��캯��
    StrChar() : data(NULL), len(0) {}
    //���캯��
    StrChar(char* str)
    {
        len = strlen(str);
        //����dataָ��Ŀռ䣬�䳤�ȸ���sָ����ַ�������Ϊ�μ�1���ַ�������Ҫ��\0
        data = new char[len + 1];
        //ǳ���������Ѿ�����һ��ժ��
        strcpy(data, str);
    }
    //�������
    StrChar(const StrChar& cp)
    {
        len = cp.len;
        //ͬ�ϣ�����data�Ͳ���Ұָ����
        data = new char[strlen(cp.data) + 1];
        strcpy(data, cp.data);
    }
    //��ֵ��������
    StrChar& operator= (const StrChar& cp)
    {
        auto newdata = new char[strlen(cp.data) + 1];
        delete[] data;
        data = newdata;
        len = cp.len;
        return *this;
    }
    //����
    ~StrChar()
    {
        delete[] data;
    }
    int getsize()
    {
        return len;
    }
    char* getdata()
    {
        return data;
    }
    void show()
    {
        cout << getdata() << "," << getsize() << endl;
    }
private:
    int len;        //�ַ�����
    char* data;     //�ַ�����
};

//������STRING
class STRING
{
public:
    //��ͨ���캯��
    STRING(const char* str = NULL)
    {
        if (str == NULL)
        {
            m_data = new char[1];
            *m_data = '\0';
        }
        else
        {
            int length = strlen(str);
            m_data = new char[length + 1];
            strcpy(m_data, str);
        }
    };
    //�������캯��
    STRING(const STRING& other)
    {
        int length = strlen(other.m_data);
        m_data = new char[length + 1];
        strcpy(m_data, other.m_data);
    };
    //��ֵ����
    STRING& operator= (const STRING& other)
    {
        if (this == &other)
            return *this;
        delete[] m_data;
        int length = strlen(other.m_data);
        m_data = new char[length + 1];
        strcpy(m_data, other.m_data);
        return *this;
    };
    //��������
    ~STRING(void)
    {
        delete[] m_data;
    };
private:
    char* m_data;
};

//һ������˵��OOP
class ClassOOP
{
    static double Rand(double dMin, double dMax)
    {
        double dVal = (double)rand() / RAND_MAX;
        return dMin + dVal * (dMax - dMin);
    }
public:
    //������������͵ķ���
    struct ComplexTest
    {
        static void ComplexAdd(double dReal1, double dImg1, double dReal2, double dImg2, double& dReal, double& dImg)
        {
            dReal = dReal1 + dReal2;
            dImg = dImg1 + dImg2;
        }

        static int main(int argc, char** argv)
        {
            double dReal1 = Rand(-10, 10);
            double dImg1 = Rand(-10, 10);

            double dReal2 = Rand(-10, 10);
            double dImg2 = Rand(-10, 10);

            double dReal = 0;
            double dImg = 0;

            ComplexAdd(dReal1, dImg1, dReal2, dImg2, dReal, dImg);
            cout << dReal << "+" << dImg << "i" << endl;
            return 1;
        }
    };

    //����ṹ�������͵ķ���
    struct ComplexTest_Structure
    {
        struct SComplex
        {
            double dReal;
            double dImg;
        };

        static SComplex ComplexAdd(SComplex c1, SComplex c2)
        {
            SComplex c;
            c.dReal = c1.dReal + c2.dReal;
            c.dImg = c1.dImg + c2.dImg;
            return c;
        }

        static int main(int argc, char** argv)
        {
            SComplex c1;
            c1.dReal = Rand(-10, 10);
            c1.dImg = Rand(-10, 10);

            SComplex c2;
            c2.dReal = Rand(-10, 10);
            c2.dImg = Rand(-10, 10);

            SComplex c = ComplexAdd(c1, c2);

            cout << c.dReal << "+" << c.dImg << "i" << endl;
            return 1;
        }
    };

    //������������͵ķ���
    struct ComplexTestOO
    {
        class CComplex
        {
        private:
            double dReal;
            double dImg;
        public:
            CComplex()
            {
                dReal = Rand(-10, 10);
                dImg = Rand(-10, 10);
            }
            CComplex(double dReal, double dImg)
            {
                this->dReal = dReal;
                this->dImg = dImg;
            }
            friend CComplex operator+ (const CComplex& c1, const CComplex& c2)
            {
                CComplex c(c1.dReal + c2.dReal, c1.dImg + c2.dImg);
                return c;
            };
            friend ostream& operator<< (ostream& os, const CComplex& c)
            {
                return os << c.dReal << "+" << c.dImg << "i";
            };
        };

        static int main(int argc, char** argv)
        {
            CComplex c1;
            CComplex c2(-2.3, 0.9);
            CComplex c = c1 + c2;
            cout << c << endl;
            return 1;
        }
    };

    static int main(int argc, char** argv)
    {
        cout << "������������͵ķ���:" << endl;
        ComplexTest::main(argc, argv);
        cout << "����ṹ�������͵ķ���:" << endl;
        ComplexTest_Structure::main(argc, argv);
        cout << "������������͵ķ���:" << endl;
        ComplexTestOO::main(argc, argv);
        return 1;
    }
};

//������ĺ�
class ArrayS
{
private:
    int* arrayAddr;     //����һ����len������Ԫ�ص�������׵�ַ
    int len;            //��¼��̬����ĳ���
public:
    ArrayS(int a[], int n)
    {
        len = n;
        arrayAddr = new int[n]; //Ϊָ�����ݳ�Ա����ռ䣬ע�⣬û�����������м�1�ǻ���
        for (int i = 0; i < n; i++)   //����ؽ�aָ���ֵ����ظ��ƹ���
            arrayAddr[i] = a[i];
    };
    //�������������ⶨ�壬�ͷ�ָ��������a��ָ��Ŀռ�
    ~ArrayS()
    {
        delete [] arrayAddr;
    };
    //���aָ����������±�Ϊi��Ԫ�ص�ֵ
    int sum()
    {
        int s = 0;
        for (int i = 0; i < len; i++)   //����ؽ�aָ���ֵ����ظ��ƹ���
            s += arrayAddr[i];
        return s;
    };
};

//�����������͵Ĳ���
int TypeSize(int argc, char** argv)
{
    cout << "type: \t\t" << "********************size**********************" << endl;
    cout << "bool: \t\t" << "\tռ�ֽ�:" << sizeof(bool) << "  ���ֵ:" << (numeric_limits<bool>::max)() << "\t\t��Сֵ:" << (numeric_limits<bool>::min)() << endl;
    cout << "(signed) char: \t" << "\tռ�ֽ�:" << sizeof(signed char) << "  ���ֵ:" << (int)(numeric_limits<signed char>::max)() << "\t\t��Сֵ:" << (int)(numeric_limits<signed char>::min)() << endl;
    cout << "unsigned char(BYTE): " << "\tռ�ֽ�:" << sizeof(unsigned char) << "  ���ֵ:" << "255" << "\t\t��Сֵ:" << "0" << endl;
    cout << "wchar_t: \t" << "\tռ�ֽ�:" << sizeof(wchar_t) << "  ���ֵ:" << (numeric_limits<wchar_t>::max)() << "\t\t��Сֵ:" << (numeric_limits<wchar_t>::min)() << endl;
    cout << "short: \t\t" << "\tռ�ֽ�:" << sizeof(short) << "  ���ֵ:" << (numeric_limits<short>::max)() << "\t\t��Сֵ:" << (numeric_limits<short>::min)() << endl;
    cout << "int(BOOL): \t" << "\tռ�ֽ�:" << sizeof(int) << "  ���ֵ:" << (numeric_limits<int>::max)() << "\t��Сֵ:" << (numeric_limits<int>::min)() << endl;
    cout << "long: \t\t" << "\tռ�ֽ�:" << sizeof(long) << "  ���ֵ:" << (numeric_limits<long>::max)() << "\t��Сֵ:" << (numeric_limits<long>::min)() << endl;
    cout << "unsigned int: \t" << "\tռ�ֽ�:" << sizeof(unsigned) << "  ���ֵ:" << (numeric_limits<unsigned>::max)() << "\t��Сֵ:" << (numeric_limits<unsigned>::min)() << endl;
    cout << "unsigned long(DWORD): " << "\tռ�ֽ�:" << sizeof(unsigned long) << "  ���ֵ:" << (numeric_limits<unsigned long>::max)() << "\t��Сֵ:" << (numeric_limits<unsigned long>::min)() << endl;
    cout << "size_t: \t" << "\tռ�ֽ�:" << sizeof(size_t) << "  ���ֵ:" << (numeric_limits<size_t>::max)() << "\t��Сֵ:" << (numeric_limits<size_t>::min)() << endl;
    cout << "double: \t" << "\tռ�ֽ�:" << sizeof(double) << "  ���ֵ:" << (numeric_limits<double>::max)() << "\t��Сֵ:" << (numeric_limits<double>::min)() << endl;
    cout << "long double: \t" << "\tռ�ֽ�:" << sizeof(long double) << "  ���ֵ:" << (numeric_limits<long double>::max)() << "\t��Сֵ:" << (numeric_limits<long double>::min)() << endl;
    cout << "float: \t\t" << "\tռ�ֽ�:" << sizeof(float) << "  ���ֵ:" << (numeric_limits<float>::max)() << "\t��Сֵ:" << (numeric_limits<float>::min)() << endl;
    cout << "int64: \t\t" << "\tռ�ֽ�:" << sizeof(__int64) << "  ���ֵ:" << (numeric_limits<__int64>::max)() << "\n\t\t\t\t  ��Сֵ:" << (numeric_limits<__int64>::min)() << endl;
    cout << "string: \t" << "\tռ�ֽ�:" << sizeof(string) << endl;
    return 1;
}







#endif