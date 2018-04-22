//********************************************************************
//
//                  配置文件管理工具
//                                      @阿甘2016-05-12
//
//********************************************************************/

#ifndef APPCFG_H
#define APPCFG_H

#include "QPublic.h"

//Ini数据字段
struct IniDat
{
    bool SupenShow;                     //悬浮窗显示
    int SuspenPaint;                    //悬浮窗是否坑锯齿
    bool SuspenExit;                    //是否退出到托盘
    bool GoodGif;                       //显示彩蛋
    bool Clock;                         //显示悬浮clock
    float SuspenPtX, SuspenPtY;         //悬浮窗默认位置
    bool AxisStyle;                     //坐标轴风格
    IniDat()
    {
        SupenShow = SuspenPaint = SuspenExit = GoodGif = 0;
        SuspenPtX = SuspenPtY = 0.0;
        Clock = AxisStyle = 1;
    };
};

//配置文件管理
class Appcfg
{
private:
    static Appcfg* _instance;
    QString inifile;                    //ini文件
protected:
    Appcfg(void);
public:
    static Appcfg* Instance()
    {
        if (NULL == _instance)
            _instance = new Appcfg();
        return _instance;
    };
    virtual ~Appcfg();
    bool CreateIni(QString _name);
    void ReadDataIni(void);
    void WriteDataIni(void);
    void setSdat(IniDat& _Sdat);
public:
    IniDat Sdat;                        //数据
};




#endif
