
#include "stdafx.h"
#include "ImageFilter.h"
#include <QThread>
#include <QMutexLocker>
#include "QImageHandler.h"

QImage* ImageFilter::filter(QImage*& img)
{
    QMutexLocker locker(&mutex);
    QImageHandler handler(img);
    for (int i = 0; i < tasks.size(); i++)
    {
        switch (tasks[i].type)
        {
        case XTASK_MIRRORLEFTANDRIGHT://×óÓÒ¾µÏñ
            handler.mirrorLeftAndRight();
            break;
        case  XTASK_MIRRORUPANDDOWN://ÉÏÏÂ¾µÏñ
            handler.mirrorUpAndDown();
            break;
        default:
            break;
        }
    }
    return handler.getHandleImage();//·µ»Ø²Ù×÷ºóµÄÍ¼Æ¬
}

void ImageFilter::addTask(XTask task)
{
    QMutexLocker locker(&mutex);
    tasks.push_back(task);
}

void ImageFilter::addColorTask(ColorTask task)
{
    QMutexLocker locker(&mutex);
    colorTasks.push_back(task);
}

QImage* ImageFilter::filterColor(QImage*& img)
{
    QMutexLocker locker(&mutex);
    QImageHandler handler(img);
    for (int i = 0; i < colorTasks.size(); i++)
    {
        switch (colorTasks[i].type)
        {
        case COLRTASK_GRAY_TO_RGBA://»Ò¶ÈÍ¼×ªRGBÍ¼
            handler.gray2RGB();
            break;
        case COLRTASK_RGBA_TO_GRAY://RGBÍ¼×ª»Ò¶ÈÍ¼
            handler.rgb2Gray();
            break;
        default:
            break;
        }
    }
    return handler.getHandleImage();
}

void ImageFilter::clear()
{
    QMutexLocker locker(&mutex);
    tasks.clear();
    colorTasks.clear();
}

ImageFilter::ImageFilter()
{
}

ImageFilter::~ImageFilter()
{
    clear();
}
