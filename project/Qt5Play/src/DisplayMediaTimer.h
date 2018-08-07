#pragma once
#include <QTimer>
#include <QMutex>

class QImage;

class DisplayMediaTimer: public QObject
{
    Q_OBJECT
public:
    static DisplayMediaTimer* getInstance()
    {
        static DisplayMediaTimer displayMediaThread;
        return &displayMediaThread;
    }
    ~DisplayMediaTimer();
    //获取播放状态
    bool isPlaying();
    //设置播放状态
    void setPlay(bool isPlaying);
    //重置视频大小，窗体最大化最小化时使用
    void resetImage(int width, int height);
public slots:
    //音视频同步设置
    void synchronizeAudioAndVideo();
private:
    //启动视频定时器
    DisplayMediaTimer();
    bool isPlay = false;
    QTimer* timer;//视频定时刷新器
    uchar* buf;
    QImage* img;
    int imageWidth = 800;
    int imageHeight = 600;
    QMutex mutex;
signals:
    //发送到opengl
    void updateFrame(QImage* image);

};

