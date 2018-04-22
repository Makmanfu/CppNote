
#include "stdafx.h"
#include "appcfg.h"

Appcfg* Appcfg::_instance = 0;

Appcfg::Appcfg(void)
{
}

Appcfg::~Appcfg(void)
{
}

bool Appcfg::CreateIni(QString  _name)
{
    inifile = _name;
    QFileInfo fileInfo(inifile);
    //判断文件是否存在
    if (!fileInfo.isFile())
    {
        //创建默认INI
        WriteDataIni();
    }
    ReadDataIni();
    return true;
}

void Appcfg::ReadDataIni(void)
{
    QSettings setting(inifile, QSettings::IniFormat);//生成配置文件
    setting.beginGroup("AppCfg");
    Sdat.SupenShow = setting.value("SuspenShow").toBool();
    Sdat.SuspenPaint = setting.value("SuspenPaint").toInt();
    Sdat.SuspenExit = setting.value("SuspenExit").toBool();
    Sdat.GoodGif = setting.value("GoodGif").toBool();
    Sdat.Clock = setting.value("Clock").toBool();
    Sdat.AxisStyle = setting.value("AxisStyle").toBool();
    setting.endGroup();
}

void Appcfg::WriteDataIni(void)
{
    QSettings setting(inifile, QSettings::IniFormat);//生成配置文件
    setting.beginGroup("AppCfg");
    setting.setValue("SuspenShow", QVariant(int(Sdat.SupenShow)));      //悬浮窗是否显示
    setting.setValue("SuspenPaint", QVariant(Sdat.SuspenPaint));        //悬浮窗效果(0锯齿,1坑锯齿,2动画)
    setting.setValue("SuspenExit", QVariant(int(Sdat.SuspenExit)));     //是否退出到托盘
    setting.setValue("GoodGif", QVariant(int(Sdat.GoodGif)));           //显示彩蛋
    setting.setValue("Clock", QVariant(int(Sdat.Clock)));               //显示Clock
    setting.setValue("AxisStyle", QVariant(int(Sdat.AxisStyle)));
    setting.endGroup();
}

void Appcfg::setSdat(IniDat& _Sdat)
{
    Sdat = _Sdat;
}

