//*********************************************************************
//
//          ToolClass工具类记录 此模块为全局模块修改需谨慎
//                                          @2017-03-10阿甘整理
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

//常用函数指针
typedef int(*WINProc)(HINSTANCE, HINSTANCE, char*, int);
typedef int(WINAPI* WINCBProc)(HINSTANCE, HINSTANCE, char*, int);
typedef int(*CMDProc)(int, char**);
typedef int(CALLBACK* CMDCBProc)(int, char**);
typedef int(*FUNProc)(void);

//程序启动器(命令行参数运行管理)
template <typename FunProc>
class CmdProc
{
public:
    struct FunProcInfo
    {
        FunProc func;                   //函数指针
        void* info;                     //其他信息
        FunProcInfo(FunProc _func, void* _info = NULL) :func(_func), info(_info) {};
        FunProcInfo() :FunProcInfo(NULL, NULL){}
    };
private:
    string ExeName;                     //程序名
    FunProcInfo Defaultfp;              //默认执行函数
    map<string, FunProcInfo> cmds;      //定义函数指针参数表
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
    //禁止拷贝操作
    CmdProc(const CmdProc&);
    CmdProc& operator= (const CmdProc&);
public:
    //设置exename
    void SetExeName(const char* exename)
    {
        ExeName = exename;
    };
    //添加默认执行函数
    void DefaultFunAdd(const FunProcInfo fp)
    {
        Defaultfp = fp;
        cmds.insert(make_pair("default", fp));
    };
    //添加命令到关联表
    void ComandAdd(const string cmd, const FunProcInfo fp, bool bat = false)
    {
        //创建测试批处理
        if (bat)
            CreateBat(ExeName.c_str(), cmd.c_str());
        cmds.insert(make_pair(cmd.c_str(), fp));
    };
    //生成脚本
    void CreateBat(void)
    {
        map<string, FunProcInfo>::const_iterator it = cmds.begin();
        for (map<string, FunProcInfo>::const_iterator it = cmds.begin();
             it != cmds.end(); ++it)
            CreateBat(ExeName.c_str(), it->first.c_str());
    }
    //生成辅助脚本
    void CreateToolBat(void)
    {
        CreateBat(ExeName.c_str(), "help");
        CreateBat(ExeName.c_str(), "clean");
    }
    //删除脚本
    void DeleteBat(void)
    {
        system("del /q *.bat");
    };
    //根据命令取出运行函数
    void GetCmdFunProc(const string cmd, FunProc* fp)
    {
        if (0 == cmd.compare("help") || 0 == cmd.compare("?"))
            return CreateBat();                 //输入help生成脚本
        if (0 == cmd.compare("all") || 0 == cmd.compare("??"))
        {
            CreateToolBat();
            return CreateBat();
        }
        if (0 == cmd.compare("clean"))          //输入clean清理脚本
            return DeleteBat();
        map<string, FunProcInfo>::const_iterator findr = cmds.find(cmd);
        if (findr != cmds.end())
        {
            //cout << findr->first.c_str() << endl;
            *fp = findr->second.func;
            return ;                            //命令表中找到函数指针
        }

        *fp = Defaultfp.func;                   //空命令或未知命令执行默认函数
        return;
    };
    //根据命令取出运行函数信息
    void GetCmdFunProc(const string cmd, FunProcInfo& fp)
    {
        if (0 == cmd.compare("help") || 0 == cmd.compare("?"))
            return CreateBat();                 //输入help生成脚本
        if (0 == cmd.compare("all") || 0 == cmd.compare("??"))
        {
            CreateToolBat();
            return CreateBat();
        }
        if (0 == cmd.compare("clean"))          //输入clean清理脚本
            return DeleteBat();
        map<string, FunProcInfo>::const_iterator findr = cmds.find(cmd);
        if (findr != cmds.end())
        {
            fp = findr->second;
            return ;                            //命令表中找到函数指针
        }
        fp = Defaultfp;                         //空命令或未知命令执行默认函数
        return;
    };
    //取出全部命令名称
    void GetAllCmdName(vector<string>& names)
    {
        names.clear();
        for (map<string, FunProcInfo>::const_iterator it = cmds.begin(); it != cmds.end(); it++)
        {
            if (0 != it->first.compare("default"))
                names.push_back(it->first);
        }
    };
    //消息循环(供测试用)
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
        out.close();  //关闭文件
    };
};

//常用函数类模板特化
typedef CmdProc<WINProc> CWINParam;
typedef CmdProc<CMDProc> CCMDParam;
typedef CmdProc<FUNProc> CFUNParam;

class CToolClassDemo
{
public:
    //windows界面入口函数例子
    int WINAPI CmdProcWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        WINProc fp = NULL, DefaultFun = NULL, DlgWinMain = NULL;
        CWINParam cmds;
        cmds.DefaultFunAdd(DefaultFun);                //添加默认执行函数
        cmds.ComandAdd(string("GUI"), DlgWinMain);     //添加注册函数DlgWinMain
        cmds.GetCmdFunProc(string(lpCmdLine), &fp);
        return (fp != NULL) ? fp(hInstance, hPrevInstance, lpCmdLine, nCmdShow) : 0;
    }
    //命令行入口函数例子
    int CmdProcmain(int argc, char** argv)
    {
        FUNProc fp = NULL, DefaultFun = NULL, CmdMain = NULL;
        CFUNParam cmds(argv[0]);                            //程序名传入
        cmds.DefaultFunAdd(DefaultFun);                     //注册默认函数指针
        cmds.ComandAdd(string("CmdMain"), CmdMain);         //添加注册函数
        string strcmd = argc > 1 ? argv[1] : "default";     //默认指令(argc == 1时)
        cmds.GetCmdFunProc(strcmd, &fp);
        return (fp != NULL) ? fp() : 0;
    }
};

#endif
