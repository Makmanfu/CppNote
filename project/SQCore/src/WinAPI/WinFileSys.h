//-----------------------------------------------------------------------------
//
//                  windows 文件系统api总结算法
//                                          @2015-05-10阿甘整理
//
//-----------------------------------------------------------------------------
#pragma once
#include "WinFileSys_typedef.h"


class SQCPP_API WinFileSys
{
#define BUFSIZE 1024
public:
    WinFileSys();
    ~WinFileSys();
public:
    //格式化成标准win路径[c:\test] endflg末尾加"\\"
    void PathFormat(string& path, bool endflg = false);
    //检测路径或文件是否合法 ????
    bool CheckPathFile(const string& pathfile);
    //获取当前程序路径信息[idtype:0-全路径,1-程序名,2-路径c:\test ]
    string GetExePath(EXETYPE idtype = PATHALL);
    //得到系统所有驱动器
    void GetAllDisk(vector<string>& AllDrv);
    //根据路径创建文件夹[绝对路径] 等价于MakeSureDirectoryPathExists
    bool CreatePathDir(const string& path);
    //修改文件属性
    void SetAtt(const string& pathfile);
public:
    //根据路径获得下所有内容[0递归全部,1递归文件夹,2递归文件,3当前文件夹,4当前文件]
    int GetDirFiles(const string& path, vector<string>& AllDir, DIRTYPE idtype = DALL);
    //递归删除文件夹所有[路径用c:\test 速度已经优化]
    bool DelPathDir(const string& path);
    //递归删除空文件夹
public:
    //智能递归文件
    bool SmartFile(const string& path, FileProc fp);
    //智能递归文件夹
    bool SmartDir(const string& path, FileProc fp);
};





