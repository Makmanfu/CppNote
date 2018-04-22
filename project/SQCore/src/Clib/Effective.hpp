//-----------------------------------------------------------------------------
//
//          类的相关问题
//
//-----------------------------------------------------------------------------
#ifndef EFFECTIVE_HPP
#define EFFECTIVE_HPP
#include <stdint.h>
#include <iostream>
#include <cstring>
using namespace std;

//类的拷贝问题
class ClassCopy
{
    //类的浅拷贝[成员不含指针]
    class copyObjShallow
    {
    public:
        //构造函数
        copyObjShallow(const string& str) : strdata(str), num(str.size()) {};
        //委托构造函数
        copyObjShallow() : copyObjShallow("") {};
        //拷贝构造
        copyObjShallow(const copyObjShallow& cp) : strdata(cp.strdata), num(cp.num) {};
        //拷贝赋值构造函数
        copyObjShallow& operator= (const copyObjShallow& cp)
        {
            strdata = cp.strdata;
            num = cp.num;
        };
        //析构函数 [类和内置类型不需要管]
        ~copyObjShallow() {};
        //测试数据
        void show(void)
        {
            cout << strdata.c_str() << num << endl;
        }
    private:
        string strdata;
        int num;
    };
    //类的深拷贝[成员含指针 不能用浅拷贝]
    class copyObjDeep final //final阻止派生
    {
    public:
        //委托构造函数
        copyObjDeep() : copyObjDeep("") {};
        //构造函数
        copyObjDeep(const string& str) : pData(new string(str)), len(str.size()), num(new size_t (1)) {};
        //拷贝构造
        copyObjDeep(const copyObjDeep& cp) : pData(new string(*cp.pData)), len(cp.len), num(cp.num)
        {
            ++*num;
        };
        //拷贝赋值构造函数
        copyObjDeep& operator= (const copyObjDeep& cp)
        {
            //处理自我赋值
            auto newp = new string(*cp.pData);      //拷贝底层string
            ++*cp.num;                              //递增右侧运算对象的引用计数
            if (--*num == 0)                        //然后递减本对象的引用计数
            {
                delete pData;                       //释放旧内存
                delete num;                         //释放本对象分配的成员
            }
            pData = newp;                           //得到新拷贝
            len = cp.len;
            num = cp.num;
            return *this;
        };
        //析构函数
        ~copyObjDeep()
        {
            if (--*num == 0)
            {
                delete pData;
                delete num;
            }
        };
        //测试数据
        void show(void)
        {
            cout << pData->c_str() << "  " << len << "  " << *num << endl;
        };
    private:
        string* pData;      //指针数据
        int len;            //数量
        size_t* num;        //引用计数
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
        //cout << "和：" << r1.sum() << endl;
        //int e[15] = { 18, 68, 10, 52, 3, 19, 12, 100, 56, 96, 95, 97, 1, 4, 93 };
        //ArrayS r2(e, 15);
        //cout << "和：" << r2.sum() << endl;
        return 1;
    };
};

//虚函数的理解
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

//析构函数理解
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
        A *a = new A(); delete a; cout << endl;  //释放 A
        B *b = new B(); delete b; cout << endl;  //释放 B A
        C *c = new C(); delete c; cout << endl;  //释放 C B A
        A *d = new B(); delete d; cout << endl;  //开始出错 仅释放 A
        A *e = new C(); delete e; cout << endl;  //仅释放 A
        B *f = new C(); delete f; cout << endl;  //仅释放 B A
        system("pause");
        return 0;
    }
};

//模仿string字符例子
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
    //默认构造函数
    StrChar() : data(NULL), len(0) {}
    //构造函数
    StrChar(char* str)
    {
        len = strlen(str);
        //分配data指向的空间，其长度根据s指向的字符串定。为何加1？字符串结束要用\0
        data = new char[len + 1];
        //浅拷贝祸端已经由上一句摘除
        strcpy(data, str);
    }
    //深拷贝构造
    StrChar(const StrChar& cp)
    {
        len = cp.len;
        //同上，这样data就不是野指针了
        data = new char[strlen(cp.data) + 1];
        strcpy(data, cp.data);
    }
    //赋值拷贝构造
    StrChar& operator= (const StrChar& cp)
    {
        auto newdata = new char[strlen(cp.data) + 1];
        delete[] data;
        data = newdata;
        len = cp.len;
        return *this;
    }
    //析构
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
    int len;        //字符长度
    char* data;     //字符数据
};

//面试题STRING
class STRING
{
public:
    //普通构造函数
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
    //拷贝构造函数
    STRING(const STRING& other)
    {
        int length = strlen(other.m_data);
        m_data = new char[length + 1];
        strcpy(m_data, other.m_data);
    };
    //赋值函数
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
    //析构函数
    ~STRING(void)
    {
        delete[] m_data;
    };
private:
    char* m_data;
};

//一个例子说明OOP
class ClassOOP
{
    static double Rand(double dMin, double dMax)
    {
        double dVal = (double)rand() / RAND_MAX;
        return dMin + dVal * (dMax - dMin);
    }
public:
    //面向过程求复数和的方法
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

    //面向结构化求复数和的方法
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

    //面向对象求复数和的方法
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
        cout << "面向过程求复数和的方法:" << endl;
        ComplexTest::main(argc, argv);
        cout << "面向结构化求复数和的方法:" << endl;
        ComplexTest_Structure::main(argc, argv);
        cout << "面向对象求复数和的方法:" << endl;
        ComplexTestOO::main(argc, argv);
        return 1;
    }
};

//求数组的和
class ArrayS
{
private:
    int* arrayAddr;     //保存一个有len个整型元素的数组的首地址
    int len;            //记录动态数组的长度
public:
    ArrayS(int a[], int n)
    {
        len = n;
        arrayAddr = new int[n]; //为指针数据成员分配空间，注意，没有上面例子中加1那回事
        for (int i = 0; i < n; i++)   //逐个地将a指向的值逐个地复制过来
            arrayAddr[i] = a[i];
    };
    //析构函数的类外定义，释放指针型数据a所指向的空间
    ~ArrayS()
    {
        delete [] arrayAddr;
    };
    //获得a指向的数组中下标为i的元素的值
    int sum()
    {
        int s = 0;
        for (int i = 0; i < len; i++)   //逐个地将a指向的值逐个地复制过来
            s += arrayAddr[i];
        return s;
    };
};

//所有数据类型的参数
int TypeSize(int argc, char** argv)
{
    cout << "type: \t\t" << "********************size**********************" << endl;
    cout << "bool: \t\t" << "\t占字节:" << sizeof(bool) << "  最大值:" << (numeric_limits<bool>::max)() << "\t\t最小值:" << (numeric_limits<bool>::min)() << endl;
    cout << "(signed) char: \t" << "\t占字节:" << sizeof(signed char) << "  最大值:" << (int)(numeric_limits<signed char>::max)() << "\t\t最小值:" << (int)(numeric_limits<signed char>::min)() << endl;
    cout << "unsigned char(BYTE): " << "\t占字节:" << sizeof(unsigned char) << "  最大值:" << "255" << "\t\t最小值:" << "0" << endl;
    cout << "wchar_t: \t" << "\t占字节:" << sizeof(wchar_t) << "  最大值:" << (numeric_limits<wchar_t>::max)() << "\t\t最小值:" << (numeric_limits<wchar_t>::min)() << endl;
    cout << "short: \t\t" << "\t占字节:" << sizeof(short) << "  最大值:" << (numeric_limits<short>::max)() << "\t\t最小值:" << (numeric_limits<short>::min)() << endl;
    cout << "int(BOOL): \t" << "\t占字节:" << sizeof(int) << "  最大值:" << (numeric_limits<int>::max)() << "\t最小值:" << (numeric_limits<int>::min)() << endl;
    cout << "long: \t\t" << "\t占字节:" << sizeof(long) << "  最大值:" << (numeric_limits<long>::max)() << "\t最小值:" << (numeric_limits<long>::min)() << endl;
    cout << "unsigned int: \t" << "\t占字节:" << sizeof(unsigned) << "  最大值:" << (numeric_limits<unsigned>::max)() << "\t最小值:" << (numeric_limits<unsigned>::min)() << endl;
    cout << "unsigned long(DWORD): " << "\t占字节:" << sizeof(unsigned long) << "  最大值:" << (numeric_limits<unsigned long>::max)() << "\t最小值:" << (numeric_limits<unsigned long>::min)() << endl;
    cout << "size_t: \t" << "\t占字节:" << sizeof(size_t) << "  最大值:" << (numeric_limits<size_t>::max)() << "\t最小值:" << (numeric_limits<size_t>::min)() << endl;
    cout << "double: \t" << "\t占字节:" << sizeof(double) << "  最大值:" << (numeric_limits<double>::max)() << "\t最小值:" << (numeric_limits<double>::min)() << endl;
    cout << "long double: \t" << "\t占字节:" << sizeof(long double) << "  最大值:" << (numeric_limits<long double>::max)() << "\t最小值:" << (numeric_limits<long double>::min)() << endl;
    cout << "float: \t\t" << "\t占字节:" << sizeof(float) << "  最大值:" << (numeric_limits<float>::max)() << "\t最小值:" << (numeric_limits<float>::min)() << endl;
    cout << "int64: \t\t" << "\t占字节:" << sizeof(__int64) << "  最大值:" << (numeric_limits<__int64>::max)() << "\n\t\t\t\t  最小值:" << (numeric_limits<__int64>::min)() << endl;
    cout << "string: \t" << "\t占字节:" << sizeof(string) << endl;
    return 1;
}







#endif