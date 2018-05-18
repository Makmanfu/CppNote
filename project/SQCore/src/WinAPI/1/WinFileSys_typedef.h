//-----------------------------------------------------------------------------
//
//              windows 文件系统类型定义和头文件
//
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <shlobj.h>
//#pragma comment(lib, "shell32.lib")
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//文件遍历方式[0递归全部,1递归文件夹,2递归文件,3当前文件夹,4当前文件]
enum DIRTYPE { DALL = 0, DALLDIR, DALLFILE, DCURRDIR, DCURRFILE };

//文件(夹)处理函数指针
typedef void(*FileProc)(string& pathfile);


