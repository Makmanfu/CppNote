


#include "stdafx.h"
#include "VideoOpenGLWidget.h"
#include "Media.h"
#include "ReadPacketsThread.h"

QImage* VideoOpenGLWidget::image;

VideoOpenGLWidget::VideoOpenGLWidget(QWidget* parent): QOpenGLWidget(parent)
{
    ui.setupUi(this);
}

VideoOpenGLWidget::~VideoOpenGLWidget()
{
}

void VideoOpenGLWidget::setVideoImage(QImage* img)
{
    if (ReadPacketsThread::getInstance()->getIsPlaying())
    {
        image = img;
        this->update();
    }
}

void VideoOpenGLWidget::paintEvent(QPaintEvent* e)
{
    if (image == nullptr)
        return;
    QPainter painter;
    painter.begin(this);
    painter.drawImage(QPoint(0, 0), *image);
    painter.end();
}


