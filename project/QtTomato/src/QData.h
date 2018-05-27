//********************************************************************
//
//                  
//
//
//********************************************************************/

#ifndef QDATA_H 
#define QDATA_H

#include "QtConfig.h"
#include <map>
using namespace std;

//Qmap学习类
class CStudyQMap
{
public:
    CStudyQMap(){};
    ~CStudyQMap(){};
private:
    QMap<int, QString> MDat;
    //vector增加使用
    void MapADD_Demo(void)
    {
        MDat.clear();
        MDat.insert(1, "张三");
    };
    //vector显示数据
    void MapShow_Demo(void)
    {
        //用迭代器遍历取map的值
        for (QMap<int, QString> ::const_iterator it = MDat.begin(); it != MDat.end(); it++)
            qDebug() << it.key() << "  " << it.value();
    };
    //vector查找数据
    void MapFind_Demo(void)
    {
        int findnum(2);         //map查找标号为2的项
        QMap<int, QString>::const_iterator findr = MDat.find(findnum);
        if (findr != MDat.end())
            qDebug() << findr.value();
        else
            qDebug() << "没有找到啦";
    };
    void MapDel_Demo(int key)
    {
        QMap<int, QString>::iterator findr = MDat.find(key);
        if (findr != MDat.end())
            findr = MDat.erase(findr);
        else
            return;
    }
};

//map转换QMap
template <typename Key, typename T>
void mapToQMap(std::map<Key, T>& stdmap, QMap<Key, T>& qmap)
{
    if (stdmap.empty())
        return;
    for (std::map<Key, T> ::const_iterator it = stdmap.begin(); it != stdmap.end(); it++)
        qmap.insert(it->first, it->second);
}

//QMap转换map
template <typename Key, typename T>
void QMapTomap(QMap<Key, T>& qmap, std::map<Key, T>& stdmap)
{
    if (qmap.empty())
        return;
    for (QMap<Key, T>::iterator it = qmap.begin(); it != qmap.end(); it++)
        stdmap.insert(std::make_pair(it.key(), it.value()));
}

//void testQMapTomap(void)
//{
//    QMap<string, string> qmap;
//    qmap.insert("as", "asd");
//    qmap.insert("as1", "asd1");
//    qmap.insert("as2", "asd2");
//    qmap.insert("as3", "asd3");
//    std::map<string, string> smap;
//    QMapTomap(qmap, smap);
//}





#endif
