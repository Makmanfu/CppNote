
#ifndef NAFILE_H
#define NAFILE_H
#include "NaFileType.h"
#ifndef NATIVE_FILEEXP
    #define NATIVE_FILEEXP extern "C" __declspec(dllexport)
#endif


void test(void);
//原生删除文件例子
int DeleteDemo(void);
//NT系统关机[1重启2关机]
NATIVE_FILEEXP void NaSysShut(int fState = 2);





#endif