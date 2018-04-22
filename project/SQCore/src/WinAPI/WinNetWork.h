//-----------------------------------------------------------------------------
//
//   windows 网络通信api总结
//                                          @2017-03-10阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include <string>
using namespace std;

//模拟cmd ping
SQC_API bool PingPC_c(char* flpDestIp);
//ping计算机(f_quk快速ping)
SQC_API bool PingPC(string IP, bool quk = false);
//获取本地IP
SQC_API char* GetLocalIP(void);
//根据IP解析系统名(注意不插网线会失败)
SQC_API char* GetHostNameFromIP(char* IP);
//发送socket数据
SQC_API string SockSendData(char* fstrdata, string IP = "127.0.0.1", int fport = 80);
//WM_COPYDATA消息来实现两个进程之间传递数据 .
SQC_API int SendExeData(const char* name, char* data);
