
#ifndef GLCONF_H
#define GLCONF_H

#include <Windows.h>
#include <gl.h>
#include <glu.h>
#include <glaux.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#if defined(_WIN64)
    #pragma comment(lib,"GlAux64.lib")
#else
    #pragma comment(lib,"GlAux.lib")
#endif

bool UPackExeRes(char* f_strFileName, WORD f_wResID, LPCSTR lpType);





#endif