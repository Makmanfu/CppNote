//*************************************************************************
//
//                  windows �����ļ���д����־���Բ���
//
//                                       @���������� 2015-03-10
//
//*************************************************************************
#ifndef miniIni_H
#define miniIni_H
#include <string>
using namespace std;

//-------------------------------���Է���ʹ��--------------------------------------
//��ʾchar*
SQC_API void ShowBox(const char* f_txt);
//��ʾstring
SQC_API void ShowBoxStr(string& f_txt);
//��ʾint
SQC_API void ShowBoxInt(int f_txt);
//---------------------------------------------------------------------------------
//��ini�ļ� �����ַ�[��,��,Ĭ��ֵ,ini�ļ�]
SQC_API string ReadINI(string inode, string ikey, string ivalue, string ipathfile = "./cfg.ini");
//��ini�ļ� ����int
SQC_API int RiniInt(string inode, string ikey, int ivalue, string ipathfile = "./cfg.ini");
//��ini�ļ� ����float
SQC_API float RiniFloat(string inode, string ikey, float ivalue, string ipathfile = "./cfg.ini");
//дINI�ļ�[�ļ�,��,��,ֵ]
SQC_API void WriteINI(string inode, string ikey, string ivalue, string ipathfile = "./cfg.ini");
//����ini�ļ�
SQC_API bool CreateIni(string ipathfile = "./cfg.ini");
//׷��д��־[cmd=1�������� 2����]  (���� д��־)
SQC_API int LogFun(const char* f_txt, const char* f_logpath = "app.log", int cmd = 0);

//INI������
class SQCPP_API Ciniop
{
};





#endif