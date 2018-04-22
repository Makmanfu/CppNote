

#include "stdafx.h"
#include "CToolClass.hpp"
#include "JpegCompress.h"


class TestStack
{
public:
    static int main(int argc, char** argv)
    {
        //������ջ������ ̫��ع�Ŷ
        //TestStack sStack;
        //sStack.testshow();
        //�����ڶ������� OK
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

//exe����ں���
int main(int argc, char** argv)
{
    SetConsoleTitleA("SQC");
    system("color de");

    FUNProc fp = NULL;
    CmdProc<FUNProc> cmds(argv[0]);                             //����������
    cmds.DefaultFunAdd(&JpegResize);
    cmds.ComandAdd(string("JpegResize"), &JpegResize);
    string strcmd = argc > 1 ? argv[1] : "default";
    cmds.GetCmdFunProc(strcmd, &fp);
    if (fp != NULL)
        fp();
    system("pause");

    //����ʽ��Ϣѭ��
    //LoopMessage();
}

//���ڽ������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    return 0;
}

//dll����ں���
bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:        //�����DLL��ӳ�䵽�˽��̵ĵ�ַ�ռ�ʱ
        case DLL_THREAD_ATTACH:         //һ���߳����ڱ�����
        case DLL_PROCESS_DETACH:        //���DLL�ӽ��̵ĵ�ַ�ռ��н��ӳ��
            break;
        case DLL_THREAD_DETACH:         //�߳��ս�
            break;
    }
    return true;
}




