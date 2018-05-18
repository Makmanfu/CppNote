
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
    if (m_bConnected)               //����Ѵ򿪴���
        CloseConnection();          //�رա��ͷŴ���
    CloseHandle(m_hThreadTerm);
    CloseHandle(m_osRead.hEvent);
    CloseHandle(m_osWrite.hEvent);
}

int CRs232::OpenConnection(LPCSTR  szComPort, DWORD  dwBaudRate, BYTE  cByteSize,
                           BYTE  cStopBits, BYTE  cParity, BOOL  bDTR_DSR, BOOL  bRTS_CTS, BOOL  bXON_XOFF)
{
    BOOL e_InitializeComm = TRUE;
    BOOL e_OpenComm = FALSE;
    if (m_bConnected)                   //�Ѿ���
        return  0;
    SetEvent(m_osWrite.hEvent);
    m_hComDev = CreateFileA(szComPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (m_hComDev == INVALID_HANDLE_VALUE)
        return  e_OpenComm;             //���ز��ܴ򿪴��ڴ������
    DCB  dcb;
    dcb.DCBlength = sizeof(DCB);
    GetCommState(m_hComDev, &dcb);
    dcb.BaudRate = dwBaudRate;
    dcb.ByteSize = cByteSize;
    dcb.Parity = cParity;
    dcb.StopBits = ONESTOPBIT;
    //Ӳ��������.  bDTR_DSR��bRTS_CTSһ��ȡĬ��ֵ0����ModemʱӦȡ1
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
    dcb.fInX = dcb.fOutX = (bXON_XOFF != 0);        //���������
    dcb.fBinary = TRUE;
    dcb.fParity = TRUE;
    if (SetCommState(m_hComDev, &dcb))
    {
        //���ô��ڲ����ɹ�
        SetCommMask(m_hComDev, EV_RXCHAR);
        //���ý��ա����ͻ�������С
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
    COMSTAT  ComStat;                                   //����״̬�ṹ
    DWORD  dwLength, dwErrorFlags;
    ClearCommError(m_hComDev, &dwErrorFlags, &ComStat);
    dwLength = min((DWORD)nMaxLength, ComStat.cbInQue); //��Ҫ��ȡ�������ֽ���
    //dwLength = nMaxLength;
    if (dwLength > 0)
    {
        if (!ReadFile(m_hComDev, lpszBlock, dwLength, &dwLength, &m_osRead))        //�Ӵ��ڶ�������
            if (GetLastError() == ERROR_IO_PENDING)
                if (!GetOverlappedResult(m_hComDev, &m_osRead, &dwLength, TRUE))    //����ֱ���������
                    ClearCommError(m_hComDev, &dwErrorFlags, &ComStat);
    }
    return dwLength;                                    //�����Ѷ���������ֽ���
}

int CRs232::WriteCommBlock(LPCVOID  lpByte, UINT  nBytesToWrite)
{
    int e_NotConnected(0), e_TransmitError(0), e_TransmitNotFinished(0);
    DWORD dwBytesWritten, dwErrorFlags;
    COMSTAT ComStat;
    if (!m_bConnected)                                      //����û�д�
        return e_NotConnected;
    if (WaitForSingleObject(m_osWrite.hEvent, 0) != WAIT_OBJECT_0)
        return  e_TransmitNotFinished;
    ResetEvent(m_osWrite.hEvent);
    //�����ڷ�������
    if (!WriteFile(m_hComDev, lpByte, nBytesToWrite, &dwBytesWritten, &m_osWrite))
    {
        if (GetLastError() == ERROR_IO_PENDING)             //���ں�̨����
            return  0;
        ClearCommError(m_hComDev, &dwErrorFlags, &ComStat); //���ִ�����������־
        return e_TransmitError;                             //���ͳ���
    }
    return 0;                                               //WriteFile���أ�,���ͳɹ�}
}

BOOL CRs232::CloseConnection()
{
    if (!m_bConnected)
        return FALSE;
    SetEvent(m_hThreadTerm);
    m_bConnected = FALSE;
    SetCommMask(m_hComDev, 0);
    EscapeCommFunction(m_hComDev, CLRDTR);
    CloseHandle(m_hComDev);             //�رա��ͷŴ���
    return (TRUE);
}

BOOL CRs232::IsConnected()
{
    return m_bConnected;
}


/*

���ˣ����Ѿ���װ���ˣ����ǾͿ�����ʹ�����ˡ����磬���ǽ�����һ�����ڶԻ���Ĺ��̣��ڶԻ����ͷ�ļ�����������һ���Ĵ�����Ķ���CRs232 SerialPort; �ڳ����У����ǿ�������ʹ��
SerialPort.OpenConnection("COM1", 57600, 8, 0, 0, 0, 0, 0); ��COM1��������; 57600, ����λ8��ֹͣλ1����У��
SerialPort.CloseConnection(); �رոĴ���
��Ȼ�����ǻ��ô���һ�������̣߳�ר����⴮�ڽ��ܵ������ݡ��߳����һ�������ݴ���������������������������̱߳�������Ϊ��ľ�̬��Ա������
��������; static UINT CommThreadProc(LPVOID pParam);//�����߳�Ҫִ�еĺ�������������Ϊ��̬�ຯ��

UINT YourDlg::CommThreadProc(LPVOID pParam)
{
    YourDlg* hwndTemp = (YourDlg*)(pParam);//ǿ��ת���Ի������ָ��
    DWORD   dwEvtMask;
    HANDLE  aEvent[2];
    OVERLAPPED  os;
    int i, nLength;
    hwndTemp->Totollen = 0;                                  //�����ɾֲ�����

    unsigned char abIn[MAXBLOCK + 1];
    memset(hwndTemp->TotalBuf, 0, sizeof(hwndTemp->TotalBuf));//��ʼ��
    memset(&os, 0, sizeof(OVERLAPPED));                        //  ��ʼ��  
    os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);            //�����¼�
    aEvent[0] = os.hEvent;
    aEvent[1] = hwndTemp->SerialPort.m_hThreadTerm;
    while (hwndTemp->SerialPort.m_bConnected)
    {
        WaitCommEvent(hwndTemp->SerialPort.m_hComDev, &dwEvtMask, &os);
        if (WaitForMultipleObjects(2, aEvent, FALSE, INFINITE) == WAIT_OBJECT_0)
        {
            //  �ȴ�ͨѶ�¼�����      
            if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)  //  �ǽ����ַ������˸��¼� 
                do{
                    if ((nLength = hwndTemp->SerialPort.ReadCommBlock(abIn, MAXBLOCK)))//
                    {
                        abIn[nLength] = 0;
                        for (i = hwndTemp->Totollen; i < hwndTemp->Totollen + nLength; i++)
                        {
                            hwndTemp->TotalBuf[i] = abIn[i - hwndTemp->Totollen];
                        }
                        hwndTemp->Totollen += nLength;
                        Sleep(300);                //��ʱ����
                    }
                } while (nLength > 0);

                memset(abIn, 0, sizeof(abIn));
                ResetEvent(os.hEvent);
        }
    }
    CloseHandle(os.hEvent);  //�ͷ��¼������Դ  
    return  0;
}

*/