
#include "stdafx.h"
#include "Rs232.h"

CRs232::CRs232(void)
{
    memset(&m_osWrite, 0, sizeof(OVERLAPPED));
    memset(&m_osRead, 0, sizeof(OVERLAPPED));
    m_bConnected = FALSE;
    m_hThreadTerm = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CRs232::~CRs232(void)
{
    if (m_bConnected)               //如果已打开串口
        CloseConnection();          //关闭、释放串口
    CloseHandle(m_hThreadTerm);
    CloseHandle(m_osRead.hEvent);
    CloseHandle(m_osWrite.hEvent);
}

int CRs232::OpenConnection(LPCSTR  szComPort, DWORD  dwBaudRate, BYTE  cByteSize,
                           BYTE  cStopBits, BYTE  cParity, BOOL  bDTR_DSR, BOOL  bRTS_CTS, BOOL  bXON_XOFF)
{
    BOOL e_InitializeComm = TRUE;
    BOOL e_OpenComm = FALSE;
    if (m_bConnected)                   //已经打开
        return  0;
    SetEvent(m_osWrite.hEvent);
    m_hComDev = CreateFileA(szComPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (m_hComDev == INVALID_HANDLE_VALUE)
        return  e_OpenComm;             //返回不能打开串口错误代码
    DCB  dcb;
    dcb.DCBlength = sizeof(DCB);
    GetCommState(m_hComDev, &dcb);
    dcb.BaudRate = dwBaudRate;
    dcb.ByteSize = cByteSize;
    dcb.Parity = cParity;
    dcb.StopBits = ONESTOPBIT;
    //硬件流控制.  bDTR_DSR、bRTS_CTS一般取默认值0，有Modem时应取1
    dcb.fOutxDsrFlow = (bDTR_DSR != 0);
    if (bDTR_DSR)
        dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
    else
        dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fOutxCtsFlow = (bRTS_CTS != 0);
    if (bRTS_CTS)
        dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    else
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fInX = dcb.fOutX = (bXON_XOFF != 0);        //软件流控制
    dcb.fBinary = TRUE;
    dcb.fParity = TRUE;
    if (SetCommState(m_hComDev, &dcb))
    {
        //配置串口参数成功
        SetCommMask(m_hComDev, EV_RXCHAR);
        //设置接收、发送缓冲区大小
        SetupComm(m_hComDev, 4096, 4096);
        COMMTIMEOUTS    CommTimeOuts;
        CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
        CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
        CommTimeOuts.ReadTotalTimeoutConstant = 10;
        CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
        CommTimeOuts.WriteTotalTimeoutConstant = 0;
        SetCommTimeouts(m_hComDev, &CommTimeOuts);
        m_bConnected = TRUE;
        return  TRUE;
    }
    m_bConnected = FALSE;
    CloseHandle(m_hComDev);
    return FALSE;
}

int CRs232::ReadCommBlock(unsigned char*  lpszBlock, int  nMaxLength)
{
    COMSTAT  ComStat;                                   //串口状态结构
    DWORD  dwLength, dwErrorFlags;
    ClearCommError(m_hComDev, &dwErrorFlags, &ComStat);
    dwLength = min((DWORD)nMaxLength, ComStat.cbInQue); //将要读取的数据字节数
    //dwLength = nMaxLength;
    if (dwLength > 0)
    {
        if (!ReadFile(m_hComDev, lpszBlock, dwLength, &dwLength, &m_osRead))        //从串口读入数据
            if (GetLastError() == ERROR_IO_PENDING)
                if (!GetOverlappedResult(m_hComDev, &m_osRead, &dwLength, TRUE))    //挂起，直到操作完成
                    ClearCommError(m_hComDev, &dwErrorFlags, &ComStat);
    }
    return dwLength;                                    //返回已读入的数据字节数
}

int CRs232::WriteCommBlock(LPCVOID  lpByte, UINT  nBytesToWrite)
{
    int e_NotConnected(0), e_TransmitError(0), e_TransmitNotFinished(0);
    DWORD dwBytesWritten, dwErrorFlags;
    COMSTAT ComStat;
    if (!m_bConnected)                                      //串口没有打开
        return e_NotConnected;
    if (WaitForSingleObject(m_osWrite.hEvent, 0) != WAIT_OBJECT_0)
        return  e_TransmitNotFinished;
    ResetEvent(m_osWrite.hEvent);
    //往串口发送数据
    if (!WriteFile(m_hComDev, lpByte, nBytesToWrite, &dwBytesWritten, &m_osWrite))
    {
        if (GetLastError() == ERROR_IO_PENDING)             //正在后台发送
            return  0;
        ClearCommError(m_hComDev, &dwErrorFlags, &ComStat); //出现错误，清除错误标志
        return e_TransmitError;                             //发送出错
    }
    return 0;                                               //WriteFile返回１,发送成功}
}

BOOL CRs232::CloseConnection()
{
    if (!m_bConnected)
        return FALSE;
    SetEvent(m_hThreadTerm);
    m_bConnected = FALSE;
    SetCommMask(m_hComDev, 0);
    EscapeCommFunction(m_hComDev, CLRDTR);
    CloseHandle(m_hComDev);             //关闭、释放串口
    return (TRUE);
}

BOOL CRs232::IsConnected()
{
    return m_bConnected;
}


/*

好了，类已经封装好了，我们就可以是使用它了。例如，我们建立了一个基于对话框的工程，在对话框的头文件里我们声明一个改串口类的对象。CRs232 SerialPort; 在程序中，我们可以这样使用
SerialPort.OpenConnection("COM1", 57600, 8, 0, 0, 0, 0, 0); 打开COM1，波特率; 57600, 数据位8，停止位1。无校验
SerialPort.CloseConnection(); 关闭改串口
当然，我们还得创建一个辅助线程，专门侦测串口接受到的数据。线程侦测一旦有数据传来，；立即读到缓冲区里。辅助线程必须声明为类的静态成员函数。
这样声明; static UINT CommThreadProc(LPVOID pParam);//辅助线程要执行的函数，必须声明为静态类函数

UINT YourDlg::CommThreadProc(LPVOID pParam)
{
    YourDlg* hwndTemp = (YourDlg*)(pParam);//强制转换对话框对象指针
    DWORD   dwEvtMask;
    HANDLE  aEvent[2];
    OVERLAPPED  os;
    int i, nLength;
    hwndTemp->Totollen = 0;                                  //声明成局部变量

    unsigned char abIn[MAXBLOCK + 1];
    memset(hwndTemp->TotalBuf, 0, sizeof(hwndTemp->TotalBuf));//初始化
    memset(&os, 0, sizeof(OVERLAPPED));                        //  初始化  
    os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);            //生成事件
    aEvent[0] = os.hEvent;
    aEvent[1] = hwndTemp->SerialPort.m_hThreadTerm;
    while (hwndTemp->SerialPort.m_bConnected)
    {
        WaitCommEvent(hwndTemp->SerialPort.m_hComDev, &dwEvtMask, &os);
        if (WaitForMultipleObjects(2, aEvent, FALSE, INFINITE) == WAIT_OBJECT_0)
        {
            //  等待通讯事件发生      
            if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)  //  是接收字符触发了该事件 
                do{
                    if ((nLength = hwndTemp->SerialPort.ReadCommBlock(abIn, MAXBLOCK)))//
                    {
                        abIn[nLength] = 0;
                        for (i = hwndTemp->Totollen; i < hwndTemp->Totollen + nLength; i++)
                        {
                            hwndTemp->TotalBuf[i] = abIn[i - hwndTemp->Totollen];
                        }
                        hwndTemp->Totollen += nLength;
                        Sleep(300);                //超时控制
                    }
                } while (nLength > 0);

                memset(abIn, 0, sizeof(abIn));
                ResetEvent(os.hEvent);
        }
    }
    CloseHandle(os.hEvent);  //释放事件句柄资源  
    return  0;
}

*/