
#ifndef NAFILE_H
#define NAFILE_H
#include "NaFileType.h"
#ifndef NATIVE_FILEEXP
    #define NATIVE_FILEEXP extern "C" __declspec(dllexport)
#endif


void test(void);
//ԭ��ɾ���ļ�����
int DeleteDemo(void);
//NTϵͳ�ػ�[1����2�ػ�]
NATIVE_FILEEXP void NaSysShut(int fState = 2);





#endif