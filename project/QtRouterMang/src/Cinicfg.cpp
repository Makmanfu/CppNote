

#include "stdafx.h"
#include "Cinicfg.h"
#include <QtCore\QSettings>

CInIcfg* CInIcfg::_instance = 0;

CInIcfg::CInIcfg()
{
}

CInIcfg::~CInIcfg()
{
}

bool CInIcfg::CreateIni(QString  _name)
{
    inifile = _name;

    QFileInfo fileInfo(inifile);
    //判断文件是否存在
    if (!fileInfo.isFile())
    {
        //创建INI 智能自动化控制中心配置
        QSettings setting(inifile, QSettings::IniFormat);   //生成配置文件
        setting.beginGroup("AutoControlSvr");
        setting.setValue("autorunexe", QVariant(0));        //程序win自启
        setting.setValue("SvrName", QString("Data.db"));    //数据文件
        setting.setValue("WebPort", QVariant(80));          //WEB服务端口(默认80)
        setting.setValue("Setthread", QVariant(0));         //多线程设置(0单线程)
        setting.setValue("algotype", QVariant(0));          //算法类型:时间优先,流程优先(0 time/1 pro)
        setting.setValue("nettype", QVariant(1));           //通信类型(0 thread/1 dll/2 socket/3 soap)
        setting.setValue("coreconftype", QVariant(1));      //引擎配置类型(0 xml/1 json)
        setting.setValue("looptime", QVariant(3));          //扫描间隔s
        setting.endGroup();
    }
    return true;
}

void CInIcfg::ReadDataIni(void)
{
    QSettings setting(inifile, QSettings::IniFormat);
    setting.beginGroup("AutoControlSvr");
    autorunexe = setting.value("autorunexe").toInt();
    SvrName = setting.value("SvrName").toString();
    WebPort = setting.value("WebPort").toInt();
    Setthread = setting.value("Setthread").toInt();
    algotype = setting.value("algotype").toInt();
    nettype = setting.value("nettype").toInt();
    coreconftype = setting.value("coreconftype").toInt();
    looptime = setting.value("looptime").toInt();
    setting.endGroup();
}

void CInIcfg::WriteDataIni(void)
{
    QSettings setting(inifile, QSettings::IniFormat);
    setting.beginGroup("AutoControlSvr");
    setting.setValue("autorunexe", QVariant(autorunexe));
    setting.setValue("SvrName", QString(SvrName));
    setting.setValue("WebPort", QVariant(WebPort));
    setting.setValue("Setthread", QVariant(Setthread));
    setting.setValue("algotype", QVariant(algotype));
    setting.setValue("nettype", QVariant(nettype));
    setting.setValue("coreconftype", QVariant(coreconftype));
    setting.setValue("looptime", QVariant(looptime));
    setting.endGroup();
}





