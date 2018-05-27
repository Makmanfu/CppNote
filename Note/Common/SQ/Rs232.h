//*************************************************************************
//
//              串口通信方法总结
//                                              @2017-05-11阿甘
//
//*************************************************************************
#ifndef RS232_H
#define RS232_H

#define RXQUEUE         4096    // 接收缓冲区大小
#define TXQUEUE         1024    // 发送缓冲区大小
#define MAXBLOCK        1024

//串口通信
class CRs232
{
private:
    //串口设备句柄
    HANDLE m_hComDev;
    OVERLAPPED m_osRead, m_osWrite;
    //事件句柄：控制通讯线程是否终止的同步对象
    HANDLE m_hThreadTerm;
    //串口是否已经打开
    BOOL m_bConnected;
public:
    CRs232();
    ~CRs232();
public:
    //判断串口是否已经打开
    BOOL IsConnected();
    //从串口读数据
    int ReadCommBlock(unsigned char* lpszBlock, int nMaxLength);
    //打开并配置串口
    int OpenConnection(LPCSTR szComPort = "COM1", DWORD dwBaudRate = 9600,
        BYTE cBycByteSize = 8, BYTE cStopBits = ONESTOPBIT, 
        BYTE cParity = NOPARITY, BOOL bDTR_DSR = 0, BOOL bRTS_CTS = 0, BOOL bXON_XOFF = 0);
    //关闭、释放串口
    BOOL CloseConnection();
    //往串口写数据
    int WriteCommBlock(LPCVOID lpByte, UINT nBytesToWrite);
};








#endif