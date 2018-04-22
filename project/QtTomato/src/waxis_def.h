
#ifndef PADDRAW_DEF_H
#define PADDRAW_DEF_H

//点结构
struct POSTION
{
    double X, Y, Z;
    bool bIsInRect;
    POSTION()
    {
        X = Y = Z = 0;
        bIsInRect = false;
    };
    POSTION(double _x, double _y, double _z = 0)
    {
        X = _x;
        Y = _y;
        Z = _z;
        bIsInRect = false;
    };
};

//经纬度坐标结构
struct GeoPos
{
    qreal target_Lon;
    qreal target_Lat;
    GeoPos() : target_Lon(0.0), target_Lat(0.0) {}
    GeoPos(qreal Lon, qreal Lat) : target_Lon(Lon), target_Lat(Lat) {}
};

//绘制对象数据结构
struct PointObject
{
    int obj_flag;                               //目标id
    GeoPos obj_LonLat;                          //经纬度
    QString obj_Image;                          //图标
    QColor colorline;                           //轨迹颜色
    QVector<GeoPos> FlyLines;                   //全部轨迹点
};





#endif

