
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
    //���������
    bool enQueue(const AVPacket packet);
    //�����г���
    AVPacket deQueue();
    //��ȡ����С
    Uint32 getPacketSize();
    //��ն���
    void queueFlush();
    ~PacketQueue();
private:
    std::queue<AVPacket> queue;
    Uint32    size;
    QMutex mutex;
    QWaitCondition cond;
};

