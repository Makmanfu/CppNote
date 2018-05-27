
#include "stdafx.h"
#include "exceloperate.h"
//#include <QFile>
//#include <QDir>
//#include <QFileDialog>
//#include <QStandardPaths>

ExcelOperate::ExcelOperate()
{
    pApplication = NULL;
    pWorkBooks = NULL;
    pWorkBook = NULL;
    pSheets = NULL;
    pSheet = NULL;
    pictures = NULL;
    m_fileName = "";
}

ExcelOperate::~ExcelOperate()
{
}

void ExcelOperate::newExcel()
{
    pApplication = new QAxObject();
    pApplication->setControl("Excel.Application");/// 连接Excel控件
    pApplication->dynamicCall("SetVisible(bool)", false);/// false不显示窗体
    pApplication->setProperty("DisplayAlerts", false);/// 不显示任何警告信息。
    pWorkBooks = pApplication->querySubObject("Workbooks");
    pWorkBooks->dynamicCall("Add");
    pWorkBook = pApplication->querySubObject("ActiveWorkBook");
    pSheets = pWorkBook->querySubObject("Sheets");
    pSheet = pSheets->querySubObject("Item(int)", 1);
    pictures = pSheet->querySubObject("Pictures");
}

void ExcelOperate::openExcel(const QString &fileName)
{
    pApplication = new QAxObject();
    pApplication->setControl("Excel.Application");
    pApplication->dynamicCall("SetVisible(bool)", false);
    pApplication->setProperty("DisplayAlerts", false);

    pWorkBooks = pApplication->querySubObject("Workbooks");
    QFile file(fileName);
    if (file.exists()) {
        pWorkBook = pWorkBooks->querySubObject("Open(const QString &)", fileName);
    }
    else
    {
        pWorkBooks->dynamicCall("Add");
        pWorkBook = pApplication->querySubObject("ActiveWorkBook");
    }
    pSheets = pWorkBook->querySubObject("Sheets");
    pSheet = pSheets->querySubObject("Item(int)", 1);
}

void ExcelOperate::saveExcel()
{
    if (m_fileName != "")
    {
        pWorkBook->dynamicCall("SaveAs(const QString &)",QDir::toNativeSeparators(m_fileName));
    }
}

void  ExcelOperate::freeExcel()
{ 
    if (pApplication != NULL)
    {
        pApplication->dynamicCall("Quit()");
        delete pApplication;
        pApplication = NULL;
    }
}

void ExcelOperate::setExcelName(QString &name)
{
    m_fileName = name;
}

void ExcelOperate::setSheetName(uint index, const QString& name)
{
    uint cnt = pSheets->property("Count").toUInt(); /// 获取工作表数目
    if(index <= cnt)
    {
        pSheet = pSheets->querySubObject("Item(int)", index);
        pSheet->setProperty("Name", name);
    }
    else
    {
        QAxObject *pLastSheet = pSheets->querySubObject("Item(int)", cnt);
        pSheets->dynamicCall("Add(QVariant)", pLastSheet->asVariant());
        pSheet = pSheets->querySubObject("Item(int)", cnt);
        pLastSheet->dynamicCall("Move(QVariant)", pSheet->asVariant());
        pSheet->setProperty("Name", name);
        delete pLastSheet;
    }
}

void ExcelOperate::setWightAndHeight(uint row, uint col, uint wgt, uint hgt)
{
    QAxObject *cell = pSheet->querySubObject("Cells(int,int)", row, col);
    cell->setProperty("ColumnWidth", wgt);  /// 设置单元格列宽
    cell->setProperty("RowHeight", hgt);  /// 设置单元格行高
    ///// 设置边框为黑色
    //QAxObject* border = cell->querySubObject("Borders");
    //border->setProperty("Color", QColor(0, 0, 0));
    //delete border;
    delete cell;
}

void ExcelOperate::setCellAlignment(uint row, uint col, int rowAlg, int colAlg)
{
    QAxObject *cell = pSheet->querySubObject("Cells(int,int)", row, col);
    /// 左对齐（xlLeft）：-4131  居中（xlCenter）：-4108  右对齐（xlRight）：-4152
    cell->setProperty("HorizontalAlignment", rowAlg);
    /// 上对齐（xlTop）-4160 居中（xlCenter）：-4108  下对齐（xlBottom）：-4107
    cell->setProperty("VerticalAlignment", colAlg);
    delete cell;
}

void ExcelOperate::setBackgroundColor(uint row, uint col, QColor &color)
{
     QAxObject *cell = pSheet->querySubObject("Cells(int,int)", row, col);
     QAxObject* interior = cell->querySubObject("Interior");
     interior->setProperty("Color", color);
     /// 设置边框为黑色
     QAxObject* border = cell->querySubObject("Borders");
     border->setProperty("Color", QColor(0, 0, 0));
     delete interior;
     delete cell;
}

void ExcelOperate::setCellsMerge(uint frow, uint fcol, uint srow, uint scol)
{
    QString merge_cell;
    merge_cell.append(QChar(fcol - 1 + 'A'));  /// 初始列
    merge_cell.append(QString::number(frow));  /// 初始行
    merge_cell.append(":");
    merge_cell.append(QChar(scol - 1 + 'A'));  /// 终止列
    merge_cell.append(QString::number(srow));  /// 终止行
    QAxObject *merge_range = pSheet->querySubObject("Range(const QString&)", merge_cell);
    merge_range->setProperty("HorizontalAlignment", -4108);
    merge_range->setProperty("VerticalAlignment", -4108);
    //merge_range->setProperty("WrapText", true);
    merge_range->setProperty("MergeCells", true);  /// 合并单元格
    delete merge_range;
}

void ExcelOperate::setCellFontSize(uint row, uint col, int size)
{
    QAxObject *pCell = pSheet->querySubObject("Cells(int,int)", row, col);
    QAxObject *font = pCell->querySubObject("Font");  //获取单元格字体
    font->setProperty("Size", size);  //设置单元格字体大小
    delete font;
    delete pCell;
}

void ExcelOperate::setColumnWight(uint col, uint wgt)
{
    QString strCol(QChar(col - 1 + 'A'));
    QAxObject *column = pSheet->querySubObject("Columns(const QString&)",strCol);
    column->setProperty("ColumnWidth", wgt);  /// 设置列宽
    delete column;
}

void ExcelOperate::setRowHeight(int row, int height)
{
    QAxObject *range = pSheet->querySubObject("Rows(int)",row);
    range->setProperty("RowHeight", height);  /// 设置行高
    delete range;
}

void ExcelOperate::setCellFormat(uint row, uint col)
{
     QAxObject *pCell = pSheet->querySubObject("Cells(int,int)", row, col);
     pCell->setProperty("NumberFormatLocal", "@");///设置单元格格式为文本
     delete pCell;
}

void ExcelOperate::setRangeFormat(QString &area, QString &format)
{
    QAxObject *pRange = pSheet->querySubObject("Range(const QString&)",area);
    pRange->setProperty("NumberFormatLocal", format);
    delete pRange;
}

void ExcelOperate::setBorderColor(QString &area)
{
    QAxObject *pRange = pSheet->querySubObject("Range(const QString&)",area);
    QAxObject* border = pRange->querySubObject("Borders");
    border->setProperty("Color", QColor(0, 0, 0));
    delete border;
    delete pRange;
}

void ExcelOperate::appendSheet(const QString& sheetName)
{
    int cnt = pSheets->property("Count").toInt(); /// 获取工作表数目
    QAxObject *pLastSheet = pSheets->querySubObject("Item(int)", cnt);
    pSheets->dynamicCall("Add(QVariant)", pLastSheet->asVariant());
    pSheet = pSheets->querySubObject("Item(int)", cnt);
    pLastSheet->dynamicCall("Move(QVariant)", pSheet->asVariant());
    pSheet->setProperty("Name", sheetName);
    delete pLastSheet;
}

void ExcelOperate::setRowData(QString &area, QList<QVariant> &var)
{
    QVariant temp;
    temp = QVariant::fromValue(var);
    QAxObject *pRange = pSheet->querySubObject("Range(const QString&)",area);
    pRange->setProperty("Value", var);
    delete pRange;
}

void ExcelOperate::setCellValue(uint row, uint col, const QString &value)
{
    QAxObject *pRange = pSheet->querySubObject("Cells(int,int)", row, col);
    pRange->dynamicCall("Value", value);
    pRange->setProperty("WrapText", true);
    delete pRange;
}

void ExcelOperate::insertImage(const QString &name, bool f1, bool f2, double left, double top, double right, double down)
{
    //*@param name 名称
    //* @param f1   是否将图片建立为独立的档案副本
    //* @param f2   是否在文件中存放连接信息
    //* @param left 图片左上角位置
    //* @param top  图片顶上角位置
    //* @param right 图片的宽度
    //* @param down  图片的高度
    QAxObject * shapes = pSheet->querySubObject("Shapes");
    QAxObject * shape = shapes->querySubObject("AddPicture( QString&, bool, bool, double, double, double, double",name,f1,f2,left,top,right,down);
    if (shape)
    {
        shape->dynamicCall("ScaleHeight(double,bool)",1.0,true);
        shape->dynamicCall("ScaleWidth(double,bool)",1.0,true);
    }
    delete shape;
    delete shapes;
}

void ExcelOperate::insertPic2Cell(uint row, uint col, QString& name)
{
    QAxObject *cell = pSheet->querySubObject("Cells(int,int)", row, col);
    pictures->dynamicCall("Insert(QString)",name);
    QAxObject *picture = pictures->querySubObject("Item(int)",1);
    int left = cell->property("Left").toInt();
    int top = cell->property("Top").toInt();
    int width = cell->property("Width").toInt();
    int height = cell->property("Height").toInt();

    picture->dynamicCall("SetWidth(double)",width);
    picture->dynamicCall("SetHeight(double)",height);
    picture->dynamicCall("SetLeft(double)",left);
    picture->dynamicCall("SetTop(double)",top);
    delete cell;
}

void ExcelOperate::readRangeData(QVariant& data)
{
    QAxObject* usedRange = pSheet->querySubObject("UsedRange");
    if(usedRange == NULL)
    {
        return;
    }
    data = usedRange->dynamicCall("Value");
    delete usedRange;
}

void ExcelOperate::writeRangeData(const QString& area, QList<QList<QVariant> > &data)
{
    if(data.size() <= 0)
        return;
    QAxObject *pRange = pSheet->querySubObject("Range(const QString&)",area);
    if(pRange == NULL)
        return;
    QVariant var;
    castListListVariant2Variant(data, var);
    pRange->setProperty("Value", var);
    pRange->setProperty("WrapText", true);
    delete pRange;
}

void ExcelOperate::castListListVariant2Variant(const QList<QList<QVariant>> &res, QVariant &var)
{
    QVariant temp = QVariant(QVariantList());
    QVariantList record;

    int listSize = res.size();
    for(int i = 0; i < listSize; ++i)
    {
        temp = res.at(i);
        record << temp;
    }
    temp = record;
    var = temp;
}

void ExcelOperate::castVariant2ListListVariant(const QVariant& var, QList<QList<QVariant>>& res)
{
    QVariantList varRows = var.toList();
    if(varRows.isEmpty())
    {
        return;
    }
    const int rowCount = varRows.size();
    QVariantList rowData;
    for(int i=0;i<rowCount;++i)
    {
        rowData = varRows[i].toList();
        res.push_back(rowData);
    }
}








//ExcelOperate ecl;
//ecl.newExcel();
//ecl.setExcelName(QString("C:\\abc.xls"));
//QList<QVariant> lit;
//lit.push_back("testA");
//lit.push_back("testB");
//ecl.setRowData(QString("A1:B1"), lit);
//ecl.saveExcel();
//ecl.freeExcel();