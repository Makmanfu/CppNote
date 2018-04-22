//********************************************************************
//
//  配置文件
//
//
//********************************************************************/

#ifndef CINICFG_H
#define CINICFG_H

class CInIcfg
{
private:
    static CInIcfg* _instance;
    QString inifile;                    //ini文件
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
    int autorunexe;                     //程序win自启
    QString SvrName;                    //数据文件
    int WebPort;                        //web发布端口
    int Setthread;                      //多线程设置(0单线程)
    int algotype;                       //算法类型:时间优先,流程优先(0 time/1 pro)
    int nettype;                        //通信类型(0 thread/1 dll/2 socket/3 soap)
    int coreconftype;                   //引擎配置类型(0 xml/1 json)
    int looptime;                       //扫描间隔s

public:
    //创建默认配置文件
    bool CreateIni(QString _name);
    //解析配置文件
    void ReadDataIni(void);
    void WriteDataIni(void);
};



#endif

