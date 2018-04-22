

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
    //�ж��ļ��Ƿ����
    if (!fileInfo.isFile())
    {
        //����INI �����Զ���������������
        QSettings setting(inifile, QSettings::IniFormat);   //���������ļ�
        setting.beginGroup("AutoControlSvr");
        setting.setValue("autorunexe", QVariant(0));        //����win����
        setting.setValue("SvrName", QString("Data.db"));    //�����ļ�
        setting.setValue("WebPort", QVariant(80));          //WEB����˿�(Ĭ��80)
        setting.setValue("Setthread", QVariant(0));         //���߳�����(0���߳�)
        setting.setValue("algotype", QVariant(0));          //�㷨����:ʱ������,��������(0 time/1 pro)
        setting.setValue("nettype", QVariant(1));           //ͨ������(0 thread/1 dll/2 socket/3 soap)
        setting.setValue("coreconftype", QVariant(1));      //������������(0 xml/1 json)
        setting.setValue("looptime", QVariant(3));          //ɨ����s
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





