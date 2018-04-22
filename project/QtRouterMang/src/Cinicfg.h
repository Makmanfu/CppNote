//********************************************************************
//
//  �����ļ�
//
//
//********************************************************************/

#ifndef CINICFG_H
#define CINICFG_H

class CInIcfg
{
private:
    static CInIcfg* _instance;
    QString inifile;                    //ini�ļ�
protected:
    CInIcfg();
public:
    static CInIcfg* Instance()
    {
        if (NULL == _instance)
            _instance = new CInIcfg();
        return _instance;
    };
    virtual ~CInIcfg();
//private:
    int autorunexe;                     //����win����
    QString SvrName;                    //�����ļ�
    int WebPort;                        //web�����˿�
    int Setthread;                      //���߳�����(0���߳�)
    int algotype;                       //�㷨����:ʱ������,��������(0 time/1 pro)
    int nettype;                        //ͨ������(0 thread/1 dll/2 socket/3 soap)
    int coreconftype;                   //������������(0 xml/1 json)
    int looptime;                       //ɨ����s

public:
    //����Ĭ�������ļ�
    bool CreateIni(QString _name);
    //���������ļ�
    void ReadDataIni(void);
    void WriteDataIni(void);
};



#endif

