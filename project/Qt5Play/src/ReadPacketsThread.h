

#pragma once
#include <QThread>
#include <QMutex>


class ReadPacketsThread : public QThread
{
    Q_OBJECT
public:
    static ReadPacketsThread* getInstance()
    {
        static ReadPacketsThread rpt;
        return &rpt;
    }
    ~ReadPacketsThread();
    //读取音视频包的线程处理函数
    void run();
    //获取视频播放状态
    bool getIsPlaying();
    //设置播放状态
    void setPlaying(bool isPlaying);
public slots:
    //视频跳转位置
    void receivePos(float pos);
public:
    static bool isExit;
    float currentPos = 0;
    bool isSeek = false;
private:
    QMutex mutex;
    ReadPacketsThread();
    bool isPlay = false;
};

