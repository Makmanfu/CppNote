

#include "stdafx.h"
#include "CToolClass.hpp"
#include "JpegCompress.h"


class TestStack
{
public:
    static int main(int argc, char** argv)
    {
        //测试在栈上申请 太大必挂哦
        //TestStack sStack;
        //sStack.testshow();
        //测试在堆上申请 OK
        TestStack* sheap = new TestStack();
        sheap->testshow();
        delete sheap;
    };
#define ARRAYMAX 100000000
public:
    int AA[ARRAYMAX];
    int BB[ARRAYMAX];
    int CC[ARRAYMAX];
    int DD[ARRAYMAX];
public:
    TestStack()
    {
        for (int i = 0; i < ARRAYMAX; i++)
        {
            AA[i] = i + 1;
            BB[i] = i + 1;
            CC[i] = i + 1;
            DD[i] = i + 1;
        }
    };
    ~TestStack()
    {};
    void testshow(void)
    {
        cout << "my god!" << endl;
        for (int i = 0; i < ARRAYMAX; i++)
            cout << AA[i] <<" ";
        cout << endl;
    };
};

//exe的入口函数
int main(int argc, char** argv)
{
    SetConsoleTitleA("SQC");
    system("color de");

    FUNProc fp = NULL;
    CmdProc<FUNProc> cmds(argv[0]);                             //程序名传入
    cmds.DefaultFunAdd(&JpegResize);
    cmds.ComandAdd(string("JpegResize"), &JpegResize);
    string strcmd = argc > 1 ? argv[1] : "default";
    cmds.GetCmdFunProc(strcmd, &fp);
    if (fp != NULL)
        fp();
    system("pause");

    //阻塞式消息循环
    //LoopMessage();
}

//窗口界面程序
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    return 0;
}

//dll的入口函数
bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:        //当这个DLL被映射到了进程的地址空间时
        case DLL_THREAD_ATTACH:         //一个线程正在被创建
        case DLL_PROCESS_DETACH:        //这个DLL从进程的地址空间中解除映射
            break;
        case DLL_THREAD_DETACH:         //线程终结
            break;
    }
    return true;
}




