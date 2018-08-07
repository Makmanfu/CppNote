#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include "ui_VideoOpenGLWidget.h"


class VideoOpenGLWidget : public QOpenGLWidget 
{
    Q_OBJECT
public:
    static QImage* image;
public: 
    VideoOpenGLWidget(QWidget* parent = Q_NULLPTR);
    ~VideoOpenGLWidget();
    //更新画面
    void paintEvent(QPaintEvent* e);
public slots:
    //槽，调用更新函数
    void setVideoImage(QImage* img);
private:
    Ui::VideoOpenGLWidget ui;
};
