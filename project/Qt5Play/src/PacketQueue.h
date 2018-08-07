
#pragma once
#include <queue>

#include <SDL2/SDL.h>
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>
extern "C" {
#include <libavcodec\avcodec.h>
}

class PacketQueue
{
public:
    PacketQueue();
    //包队列入队
    bool enQueue(const AVPacket packet);
    //包队列出队
    AVPacket deQueue();
    //获取包大小
    Uint32 getPacketSize();
    //清空队列
    void queueFlush();
    ~PacketQueue();
private:
    std::queue<AVPacket> queue;
    Uint32    size;
    QMutex mutex;
    QWaitCondition cond;
};

