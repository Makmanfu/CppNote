
#pragma once
#include <queue>
#include <QMutex>
#include <QWaitCondition>
extern "C" {

#include <libavcodec\avcodec.h>
#include <libavformat/avformat.h>
}

class FrameQueue
{
public:
    static const int capacity = 30;
    FrameQueue();
    //帧队列入队
    bool enQueue(const AVFrame* frame);
    //帧队列
    AVFrame* deQueue();
    //获取队列大小
    int getQueueSize();
    //清空帧队列
    void queueFlush();
private:
    std::queue<AVFrame*> queue;
    QMutex mutex;
    QWaitCondition cond;


};

