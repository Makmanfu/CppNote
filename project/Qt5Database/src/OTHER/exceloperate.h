#ifndef EXCELOPERATE_H
#define EXCELOPERATE_H

#include "QPublic.h"
#include <windows.h>
#include <ActiveQt/qaxobject.h>
 
//导入和导出数据到Excel类
class ExcelOperate
{
public:
    ExcelOperate();
    ~ExcelOperate();
public:
    //新建一个excel
    void newExcel();
    //打开已经存在的Excel
    void openExcel(const QString& fileName);
    //保存Excel文件
    void saveExcel();
    //释放Excel控件
    void freeExcel();
public:
    //设置Excel名称
    void setExcelName(QString& name);
    //设置当前工作表及名称[工作表序号,名称]
    void setSheetName(uint index, const QString& name);
    //设置单元格的宽和高
    void setWightAndHeight(uint row, uint col, uint wgt, uint hgt);
    //设置单元格对齐方式[行,列 行的对齐方式 列的对齐方式]
    void setCellAlignment(uint row, uint col, int rowAlg, int colAlg);
    //设置单元格背景色[行,列,颜色]
    void setBackgroundColor(uint row, uint col, QColor& color);
    //设置单元格合并[起始行号,列,终止行号,列]
    void setCellsMerge(uint frow, uint fcol, uint srow, uint scol);
    //设置单元格字体大小
    void setCellFontSize(uint row, uint col, int size);
    //设置单元格列宽
    void setColumnWight(uint col, uint wgt);
    //设置行高
    void setRowHeight(int row, int height);
    //设置单元格格式为文本
    void setCellFormat(uint row, uint col);
    //设置单元格格式
    void setRangeFormat(QString& area, QString& format);
    //设置区域边框为黑色[指定的区域]
    void setBorderColor(QString& area);
public:
    //增加1个Worksheet
    void appendSheet(const QString& sheetName);
    //写入一行数据[area 格式类似为A1:B1]
    void setRowData(QString& area, QList<QVariant>& var);
    //向sheet单元格中写入数据
    void setCellValue(uint row, uint col, const QString &value);
    //向工作表中插入图片
    void insertImage(const QString& name, bool f1, bool f2, double left, double top, double right, double down);
    //插入图片到指定的单元格
    void insertPic2Cell(uint row, uint col, QString& name);
    //读取sheet指定区域的数据
    void readRangeData(QVariant& data);
    //写入数据到sheet的指定区域[写入数据的位置,写入的数据]
    void writeRangeData(const QString& area, QList<QList<QVariant>>& data);
    //数据转化 QList<QList<QVariant>>转化为QVariant
    void  castListListVariant2Variant(const QList<QList<QVariant>>& res,QVariant &var);
    //数据转化 QVariant转化为QVariant QList<QList<QVariant>>
    void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant>>& res);
private:
    QAxObject* pApplication;
    QAxObject* pWorkBooks;
    QAxObject* pWorkBook;
    QAxObject* pSheets;
    QAxObject* pSheet;
    QAxObject* pictures;
    QString m_fileName;
};

#endif 


