//*************************************************************************
//
//              ����ͨ�ŷ����ܽ�
//                                              @2017-05-11����
//
//*************************************************************************
#ifndef RS232_H
#define RS232_H

#define RXQUEUE         4096    // ���ջ�������С
#define TXQUEUE         1024    // ���ͻ�������С
#define MAXBLOCK        1024

//����ͨ��
class CRs232
{
private:
    //�����豸���
    HANDLE m_hComDev;
    OVERLAPPED m_osRead, m_osWrite;
    //�¼����������ͨѶ�߳��Ƿ���ֹ��ͬ������
    HANDLE m_hThreadTerm;
    //�����Ƿ��Ѿ���
    BOOL m_bConnected;
public:
    CRs232();
    ~CRs232();
public:
    //�жϴ����Ƿ��Ѿ���
    BOOL IsConnected();
    //�Ӵ��ڶ�����
    int ReadCommBlock(unsigned char* lpszBlock, int nMaxLength);
    //�򿪲����ô���
    int OpenConnection(LPCSTR szComPort = "COM1", DWORD dwBaudRate = 9600,
        BYTE cBycByteSize = 8, BYTE cStopBits = ONESTOPBIT, 
        BYTE cParity = NOPARITY, BOOL bDTR_DSR = 0, BOOL bRTS_CTS = 0, BOOL bXON_XOFF = 0);
    //�رա��ͷŴ���
    BOOL CloseConnection();
    //������д����
    int WriteCommBlock(LPCVOID lpByte, UINT nBytesToWrite);
};








#endif