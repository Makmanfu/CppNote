
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
        case XTASK_MIRRORLEFTANDRIGHT://���Ҿ���
            handler.mirrorLeftAndRight();
            break;
        case  XTASK_MIRRORUPANDDOWN://���¾���
            handler.mirrorUpAndDown();
            break;
        default:
            break;
        }
    }
    return handler.getHandleImage();//���ز������ͼƬ
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
        case COLRTASK_GRAY_TO_RGBA://�Ҷ�ͼתRGBͼ
            handler.gray2RGB();
            break;
        case COLRTASK_RGBA_TO_GRAY://RGBͼת�Ҷ�ͼ
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
