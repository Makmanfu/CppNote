
#pragma once
#include<vector>
#include <QImage>
#include <QMutex>

enum XTaskType
{
    XTASK_MIRRORLEFTANDRIGHT,
    XTASK_MIRRORUPANDDOWN
};
enum  ColorTaskTYPE
{
    COLRTASK_GRAY_TO_RGBA,
    COLRTASK_RGBA_TO_GRAY
};
struct XTask
{
    XTaskType type;
    std::vector<double> para;
};

struct ColorTask
{
    ColorTaskTYPE type;
    std::vector<double> para;
};

class ImageFilter
{
public:
    //过滤图片操作
    QImage* filter(QImage*& img);
    //增加操作任务
    void addTask(XTask task);
    //增加颜色任务
    void addColorTask(ColorTask task);
    //过滤颜色
    QImage* filterColor(QImage*& img);
    //清空任务
    void clear();
    static ImageFilter* getInstance()
    {
        static ImageFilter filter;
        return &filter;
    }

    ~ImageFilter();
private:
    ImageFilter();
    std::vector<XTask> tasks;
    std::vector<ColorTask> colorTasks;
    QMutex mutex;
};

