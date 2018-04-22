
#ifndef STDAFX_H
#define STDAFX_H
#define GUIRUN                                //WINDOW窗口运行

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "resource.h"
#pragma comment (lib,"user32.lib")
#pragma comment (lib,"kernel32.lib")
//注意用user32.dll 里的wsprintf替换
#define sprintf wsprintfA


//自己实现new,new[],delete,delete[] 内存分配问题
void* malloc(size_t size);
void free(void* memblock);
void* realloc(void* memblock, size_t size);
void* operator new(size_t count);
void* operator new[](size_t count);
void operator delete(void* _Ptr) throw();
void  operator delete[](void* _Ptr) throw();




#endif