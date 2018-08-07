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
    //���»���
    void paintEvent(QPaintEvent* e);
public slots:
    //�ۣ����ø��º���
    void setVideoImage(QImage* img);
private:
    Ui::VideoOpenGLWidget ui;
};
