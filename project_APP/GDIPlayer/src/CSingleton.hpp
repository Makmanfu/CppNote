//*********************************************************************
//
//                  CSingleton �������ģʽ
//                                          @2017-03-10��������
//*********************************************************************
#ifndef CSINGLETON_HPP
#define CSINGLETON_HPP

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

//������
class CMutex
{
public:
    CMutex(void)
    {
        // �����ٽ�������
#ifdef WIN32
        InitializeCriticalSection(&m_cs);
#else
        pthread_mutex_init(&m_Mutex, NULL);
#endif
    };
    virtual ~CMutex(void)
    {
        // ɾ���ٽ�������
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
    CRITICAL_SECTION m_cs;    //�ٽ�����һ�ַ�ֹ����߳�ͬʱִ��һ���ض�����ڵĻ���
#else
    pthread_mutex_t m_Mutex;
#endif
    friend class CAutoMutex;
};

//���ܻ�����
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


#ifndef ��ģ��ʵ��

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

#ifndef �̰߳�ȫ�ĵ���ģ��
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
        static T tInstance;  //������þ�̬�ֲ���������֤��������ʱһֱ���ڣ��������ʱ�����Զ����������̬����  
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
    TestSingleton(){}; //�����������ⴴ�����󣬱�֤����ֻ��һ������
    TestSingleton(const TestSingleton&){};
public:
    ~TestSingleton(){};
    void Show(){ std::cout << "I am a singleton." << std::endl; };
    friend BaseSingleton<TestSingleton>; //���ڹ��캯��Ϊ˽�У������������ã�����������������Ϊ��Ԫ��
};

void Singleton2(int argc, char** argv)
{
    //TestSingleton ts; //������뱨����Ȼ�ǲ����������ⴴ������ 
    TestSingleton* p1 = TestSingleton::Instance();
    p1->Show();
    std::cout << "p1=" << p1 << std::endl;

    //TestSingleton ts(*p1); //���ַ�ʽ��ͬ������
    TestSingleton* p2 = TestSingleton::Instance();
    p2->Show();
    std::cout << "p2=" << p2 << std::endl;

    TestSingleton* p3 = TestSingleton::Instance();
    p3->Show();
    std::cout << "p3=" << p3 << std::endl;
}

#endif
#endif

#ifndef ���滻�ĵ���ģ��
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

// ��ȫɾ��һ�������delete�ͷŵĶ���
#define SAFE_DELETE(pObject) if(pObject) {delete (pObject); (pObject) = NULL;}
// ��ȫɾ��һ�������delete[]�ͷŵĶ���
#define SAFE_DELETE_GROUP(pObject) if(pObject) {delete[] (pObject); (pObject) = NULL;}

//����ģʽ�ķ���
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

//���ж��嵥���İ취
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

//cppʵ�ֵ���
#define IMPLEMENT_SINGLETON(SingletonClass) \
    SingletonClass* SingletonClass::m_s_pInstance = NULL;\
    bool SingletonClass::m_s_bIsDestroyed = false;\
    CMutex SingletonClass::m_s_cs;


#ifdef Singleton3

//���Ե�����
class CSingletonTest
{
    DEFINE_SINGLETON(CSingletonTest);
public:
    CSingletonTest(void) {};
    ~CSingletonTest(void) {};
    void Initialize(void) {};   //��ʼ��
    void Release(void) {};      //�ͷ���Դ
};

//ʵ�ֵ���(ע�����cpp��)
IMPLEMENT_SINGLETON(CSingletonTest);

//ʵ�ֵ����ӿڻ�ȡΨһ�Ŀ���ָ��
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


