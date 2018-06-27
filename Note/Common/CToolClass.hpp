//*********************************************************************
//
//          ToolClass�������¼ ��ģ��Ϊȫ��ģ���޸������
//                                          @2017-03-10��������
//*********************************************************************
#ifndef CTOOLCLASS_HPP
#define CTOOLCLASS_HPP

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

//���ú���ָ��
typedef int(*WINProc)(HINSTANCE, HINSTANCE, char*, int);
typedef int(WINAPI* WINCBProc)(HINSTANCE, HINSTANCE, char*, int);
typedef int(*CMDProc)(int, char**);
typedef int(CALLBACK* CMDCBProc)(int, char**);
typedef int(*FUNProc)(void);

//����������(�����в������й���)
template <typename FunProc>
class CmdProc
{
public:
    struct FunProcInfo
    {
        FunProc func;                   //����ָ��
        void* info;                     //������Ϣ
        FunProcInfo(FunProc _func, void* _info = NULL) :func(_func), info(_info) {};
        FunProcInfo() :FunProcInfo(NULL, NULL){}
    };
private:
    string ExeName;                     //������
    FunProcInfo Defaultfp;              //Ĭ��ִ�к���
    map<string, FunProcInfo> cmds;      //���庯��ָ�������
public:
    CmdProc()
    {
        Defaultfp = NULL;
        ExeName = GetExeName();
    };
    CmdProc(const char* exename)
    {
        SetExeName(exename);
    };
    ~CmdProc() {};
private:
    //��ֹ��������
    CmdProc(const CmdProc&);
    CmdProc& operator= (const CmdProc&);
public:
    //����exename
    void SetExeName(const char* exename)
    {
        ExeName = exename;
    };
    //���Ĭ��ִ�к���
    void DefaultFunAdd(const FunProcInfo fp)
    {
        Defaultfp = fp;
        cmds.insert(make_pair("default", fp));
    };
    //������������
    void ComandAdd(const string cmd, const FunProcInfo fp, bool bat = false)
    {
        //��������������
        if (bat)
            CreateBat(ExeName.c_str(), cmd.c_str());
        cmds.insert(make_pair(cmd.c_str(), fp));
    };
    //���ɽű�
    void CreateBat(void)
    {
        map<string, FunProcInfo>::const_iterator it = cmds.begin();
        for (map<string, FunProcInfo>::const_iterator it = cmds.begin();
             it != cmds.end(); ++it)
            CreateBat(ExeName.c_str(), it->first.c_str());
    }
    //���ɸ����ű�
    void CreateToolBat(void)
    {
        CreateBat(ExeName.c_str(), "help");
        CreateBat(ExeName.c_str(), "clean");
    }
    //ɾ���ű�
    void DeleteBat(void)
    {
        system("del /q *.bat");
    };
    //��������ȡ�����к���
    void GetCmdFunProc(const string cmd, FunProc* fp)
    {
        if (0 == cmd.compare("help") || 0 == cmd.compare("?"))
            return CreateBat();                 //����help���ɽű�
        if (0 == cmd.compare("all") || 0 == cmd.compare("??"))
        {
            CreateToolBat();
            return CreateBat();
        }
        if (0 == cmd.compare("clean"))          //����clean����ű�
            return DeleteBat();
        map<string, FunProcInfo>::const_iterator findr = cmds.find(cmd);
        if (findr != cmds.end())
        {
            //cout << findr->first.c_str() << endl;
            *fp = findr->second.func;
            return ;                            //��������ҵ�����ָ��
        }

        *fp = Defaultfp.func;                   //�������δ֪����ִ��Ĭ�Ϻ���
        return;
    };
    //��������ȡ�����к�����Ϣ
    void GetCmdFunProc(const string cmd, FunProcInfo& fp)
    {
        if (0 == cmd.compare("help") || 0 == cmd.compare("?"))
            return CreateBat();                 //����help���ɽű�
        if (0 == cmd.compare("all") || 0 == cmd.compare("??"))
        {
            CreateToolBat();
            return CreateBat();
        }
        if (0 == cmd.compare("clean"))          //����clean����ű�
            return DeleteBat();
        map<string, FunProcInfo>::const_iterator findr = cmds.find(cmd);
        if (findr != cmds.end())
        {
            fp = findr->second;
            return ;                            //��������ҵ�����ָ��
        }
        fp = Defaultfp;                         //�������δ֪����ִ��Ĭ�Ϻ���
        return;
    };
    //ȡ��ȫ����������
    void GetAllCmdName(vector<string>& names)
    {
        names.clear();
        for (map<string, FunProcInfo>::const_iterator it = cmds.begin(); it != cmds.end(); it++)
        {
            if (0 != it->first.compare("default"))
                names.push_back(it->first);
        }
    };
    //��Ϣѭ��(��������)
    void Loop(void)
    {
        MSG msg;
        HACCEL hAccelTable = NULL;
        printf("Message loop!\n");
        while (GetMessage(&msg, NULL, 0, 0))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    };
private:
    string GetExeName(void)
    {
        char szAppPath[MAX_PATH];
        GetModuleFileNameA(NULL, szAppPath, MAX_PATH);
        string ret(szAppPath);
        size_t pos0 = ret.find_last_of("\\");
        ret = ret.substr(pos0 + 1, ret.size() - pos0 - 1);
        return ret;
    };
    void CreateBat(const char* name, const char* cmd)
    {
        ofstream out;
        string filename = cmd;
        filename += ".bat";
        out.open(filename, ios::ate | ios::out);
        if (!out.is_open())
            return;
        out << "@" << name << " " << cmd << "\n";
        out.close();  //�ر��ļ�
    };
};

//���ú�����ģ���ػ�
typedef CmdProc<WINProc> CWINParam;
typedef CmdProc<CMDProc> CCMDParam;
typedef CmdProc<FUNProc> CFUNParam;

class CToolClassDemo
{
public:
    //windows������ں�������
    int WINAPI CmdProcWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        WINProc fp = NULL, DefaultFun = NULL, DlgWinMain = NULL;
        CWINParam cmds;
        cmds.DefaultFunAdd(DefaultFun);                //���Ĭ��ִ�к���
        cmds.ComandAdd(string("GUI"), DlgWinMain);     //���ע�ắ��DlgWinMain
        cmds.GetCmdFunProc(string(lpCmdLine), &fp);
        return (fp != NULL) ? fp(hInstance, hPrevInstance, lpCmdLine, nCmdShow) : 0;
    }
    //��������ں�������
    int CmdProcmain(int argc, char** argv)
    {
        FUNProc fp = NULL, DefaultFun = NULL, CmdMain = NULL;
        CFUNParam cmds(argv[0]);                            //����������
        cmds.DefaultFunAdd(DefaultFun);                     //ע��Ĭ�Ϻ���ָ��
        cmds.ComandAdd(string("CmdMain"), CmdMain);         //���ע�ắ��
        string strcmd = argc > 1 ? argv[1] : "default";     //Ĭ��ָ��(argc == 1ʱ)
        cmds.GetCmdFunProc(strcmd, &fp);
        return (fp != NULL) ? fp() : 0;
    }
};

#endif
