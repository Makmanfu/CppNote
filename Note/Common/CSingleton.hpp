//*********************************************************************
//
//                  CSingleton 单例设计模式
//                                          @2017-03-10阿甘整理
//*********************************************************************
#ifndef CSINGLETON_HPP
#define CSINGLETON_HPP

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

//互斥体
class CMutex
{
public:
    CMutex(void)
    {
        // 创建临界区对象
#ifdef WIN32
        InitializeCriticalSection(&m_cs);
#else
        pthread_mutex_init(&m_Mutex, NULL);
#endif
    };
    virtual ~CMutex(void)
    {
        // 删除临界区对象
#ifdef WIN32
        DeleteCriticalSection(&m_cs);
#else
        pthread_mutex_destroy(&m_Mutex);
#endif
    };
public:
    void Lock()
    {
#ifdef WIN32
        EnterCriticalSection(&m_cs);
#else
        pthread_mutex_lock(&m_Mutex);
#endif
    };
    void UnLock()
    {
#ifdef WIN32
        LeaveCriticalSection(&m_cs);
#else
        pthread_mutex_unlock(&m_Mutex);
#endif
    };
protected:
#ifdef WIN32
    CRITICAL_SECTION m_cs;    //临界区是一种防止多个线程同时执行一个特定代码节的机制
#else
    pthread_mutex_t m_Mutex;
#endif
    friend class CAutoMutex;
};

//智能互斥体
class CAutoMutex
{
private:
    CMutex* m_pMutex;
private:
    CAutoMutex() {};
public:
    CAutoMutex(CMutex& mutex)
    {
        m_pMutex = &mutex;
        m_pMutex->Lock();
    };
    virtual ~CAutoMutex()
    {
        m_pMutex->UnLock();
    };
};


#ifndef 简单模板实现

template<class T>
class SimpleSingleton
{
private:
    SimpleSingleton();
    static T* pInstance;
public:
    static T& Instance()
    {
        if (!pInstance)
            pInstance = new T;
        return *pInstance;
    };
};

template<class T> T* SimpleSingleton<T>::pInstance = NULL;

#ifdef Singleton1
struct TestMsg
{
    void sayHello()
    {
        std::cout << "hello" << std::endl;
    };
};

void Singleton1(int argc, char** argv)
{
    SimpleSingleton<TestMsg>::Instance().sayHello();
    return 0;
}

#endif
#endif

#ifndef 线程安全的单例模板
#include <new>

template<typename T>
class BaseSingleton
{
private:
    static T *m_Instance;
    static CMutex m_Mutex;
public:
    BaseSingleton(){};
    virtual ~BaseSingleton(){};
public:
    static T *Instance()
    {
        if (NULL != m_Instance)
            return m_Instance;
        m_Mutex.Lock();
        if (NULL != m_Instance)
        {
            m_Mutex.UnLock();
            return m_Instance;
        }
        //m_Instance = new(std::nothrow) T();  
        static T tInstance;  //这里采用静态局部变量，保证程序运行时一直存在，程序结束时，会自动析构这个静态变量  
        m_Instance = &tInstance;
        m_Mutex.UnLock();
        return m_Instance;
    };
};

template<typename T> T *BaseSingleton<T>::m_Instance = NULL;
template<typename T> CMutex BaseSingleton<T>::m_Mutex;

#ifdef Singleton2
class TestSingleton : public BaseSingleton<TestSingleton>
{
private:
    TestSingleton(){}; //不允许在类外创建对象，保证该类只有一个对象
    TestSingleton(const TestSingleton&){};
public:
    ~TestSingleton(){};
    void Show(){ std::cout << "I am a singleton." << std::endl; };
    friend BaseSingleton<TestSingleton>; //由于构造函数为私有，而基类必须调用，所以这里声明基类为友元类
};

void Singleton2(int argc, char** argv)
{
    //TestSingleton ts; //这里编译报错，显然是不允许在类外创建对象 
    TestSingleton* p1 = TestSingleton::Instance();
    p1->Show();
    std::cout << "p1=" << p1 << std::endl;

    //TestSingleton ts(*p1); //这种方式，同样报错
    TestSingleton* p2 = TestSingleton::Instance();
    p2->Show();
    std::cout << "p2=" << p2 << std::endl;

    TestSingleton* p3 = TestSingleton::Instance();
    p3->Show();
    std::cout << "p3=" << p3 << std::endl;
}

#endif
#endif

#ifndef 宏替换的单例模板
#include <assert.h>

#ifndef ASSERT
#ifdef NDEBUG
#if defined(QT_LINUX_LIB)
#define  ASSERT(X) Q_ASSERT(X)
#else
#define  ASSERT(X)
#endif
#else
//#define _ASSERT(X) assert(X)
#define ASSERT(X) assert(X)
#endif
#endif

// 安全删除一个需调用delete释放的对象
#define SAFE_DELETE(pObject) if(pObject) {delete (pObject); (pObject) = NULL;}
// 安全删除一组需调用delete[]释放的对象
#define SAFE_DELETE_GROUP(pObject) if(pObject) {delete[] (pObject); (pObject) = NULL;}

//单例模式的泛型
template<class SingletonClass>
class CSingleton
{
public:
    CSingleton()
    {
        m_s_pSingltonInstance = NULL;
        m_s_bIsDestroyed = false;
    }
    virtual ~CSingleton()
    {
        m_s_pSingltonInstance = NULL;
        m_s_bIsDestroyed = true;
    }
    static SingletonClass* GetSingletonInstance()
    {
        ASSERT(!m_s_bIsDestroyed);
        if (!m_s_pSingltonInstance)
        {
            m_s_cs.Lock();
            if (!m_s_pSingltonInstance)
                m_s_pSingltonInstance = new SingletonClass;
            m_s_cs.UnLock();
        }
        return m_s_pSingltonInstance;
    }
private:
    static SingletonClass* m_s_pSingltonInstance;
    static CMutex m_s_cs;
    static bool m_s_bIsDestroyed;
};

template<class SingletonClass>  SingletonClass*  CSingleton<SingletonClass>::m_s_pSingltonInstance = NULL;
template<class SingletonClass>  CMutex CSingleton<SingletonClass>::m_s_cs;
template<class SingletonClass>  bool CSingleton<SingletonClass>::m_s_bIsDestroyed = false;

//类中定义单键的办法
#define DEFINE_SINGLETON(SingletonClass)\
    public:\
    static SingletonClass* Instance()\
    {\
        ASSERT(!m_s_bIsDestroyed);\
        if(!m_s_pInstance)\
                        {\
            m_s_cs.Lock();\
            if (!m_s_pInstance)\
                                    {\
                m_s_pInstance = new SingletonClass();\
                                    }\
            m_s_cs.UnLock();\
                        }\
        return m_s_pInstance;\
    }\
    static void DeleteInstance(void) \
    {\
        m_s_cs.Lock();\
        SAFE_DELETE(m_s_pInstance);\
        m_s_cs.UnLock();\
    }\
    private:\
    static SingletonClass* m_s_pInstance;\
    static bool m_s_bIsDestroyed;\
    static CMutex m_s_cs;\
    public:

//cpp实现单键
#define IMPLEMENT_SINGLETON(SingletonClass) \
    SingletonClass* SingletonClass::m_s_pInstance = NULL;\
    bool SingletonClass::m_s_bIsDestroyed = false;\
    CMutex SingletonClass::m_s_cs;


#ifdef Singleton3

//测试单键类
class CSingletonTest
{
    DEFINE_SINGLETON(CSingletonTest);
public:
    CSingletonTest(void) {};
    ~CSingletonTest(void) {};
    void Initialize(void) {};   //初始化
    void Release(void) {};      //释放资源
};

//实现单键(注意放在cpp中)
IMPLEMENT_SINGLETON(CSingletonTest);

//实现单键接口获取唯一的控制指针
extern "C" CSingletonTest* GetSingleton()
{
    return CSingletonTest::Instance();
}

void Singleton3(int argc, char** argv)
{
    CSingletonTest* sing = GetSingleton();
    sing->Initialize();
}

#endif









#endif



#endif


