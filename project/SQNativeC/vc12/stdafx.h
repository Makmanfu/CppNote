
#ifndef STDAFX_H
#define STDAFX_H
#define GUIRUN                                //WINDOW��������

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "resource.h"
#pragma comment (lib,"user32.lib")
#pragma comment (lib,"kernel32.lib")
//ע����user32.dll ���wsprintf�滻
#define sprintf wsprintfA


//�Լ�ʵ��new,new[],delete,delete[] �ڴ��������
void* malloc(size_t size);
void free(void* memblock);
void* realloc(void* memblock, size_t size);
void* operator new(size_t count);
void* operator new[](size_t count);
void operator delete(void* _Ptr) throw();
void  operator delete[](void* _Ptr) throw();




#endif