
#define NAMATH_H
#ifndef NAMATH_H
#define NAMATH_H
#ifndef NATIVE_MATHEXP
    #define NATIVE_MATHEXP extern "C" __declspec(dllexport)
#endif

//����ı�
NATIVE_MATHEXP void NaCout(char* f_txt);
//�����ı�
NATIVE_MATHEXP void NaCin(char** f_txt);
//��װ���Ӳ���ϵͳ��Ϣ
NATIVE_MATHEXP int RecvMessage(void);



#endif