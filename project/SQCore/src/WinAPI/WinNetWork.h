//-----------------------------------------------------------------------------
//
//   windows ����ͨ��api�ܽ�
//                                          @2017-03-10��������
//-----------------------------------------------------------------------------
#pragma once
#include <string>
using namespace std;

//ģ��cmd ping
SQC_API bool PingPC_c(char* flpDestIp);
//ping�����(f_quk����ping)
SQC_API bool PingPC(string IP, bool quk = false);
//��ȡ����IP
SQC_API char* GetLocalIP(void);
//����IP����ϵͳ��(ע�ⲻ�����߻�ʧ��)
SQC_API char* GetHostNameFromIP(char* IP);
//����socket����
SQC_API string SockSendData(char* fstrdata, string IP = "127.0.0.1", int fport = 80);
//WM_COPYDATA��Ϣ��ʵ����������֮�䴫������ .
SQC_API int SendExeData(const char* name, char* data);
