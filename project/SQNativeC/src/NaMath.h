
#define NAMATH_H
#ifndef NAMATH_H
#define NAMATH_H
#ifndef NATIVE_MATHEXP
    #define NATIVE_MATHEXP extern "C" __declspec(dllexport)
#endif

//输出文本
NATIVE_MATHEXP void NaCout(char* f_txt);
//输入文本
NATIVE_MATHEXP void NaCin(char** f_txt);
//安装钩子捕获系统消息
NATIVE_MATHEXP int RecvMessage(void);



#endif