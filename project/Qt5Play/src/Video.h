

#pragma once
#include "PacketQueue.h"
#include "FrameQueue.h"
#include <QThread>

class Video: public QThread
{
public:
    Video();
    ~Video();
public:
    //视频读帧线程处理函数
    void run();
    //计算同步视频的播放时间
    double synchronizeVideo(AVFrame*& srcFrame, double& pts);

    //获取/设置视频解码器上下文
    AVCodecContext* getAVCodecCotext();
    void setAVCodecCotext(AVCodecContext* avCodecContext);
    //获取/设置流下标
    int getStreamIndex();
    void setStreamIndex(const int& streamIndex);
    //获取/设置视频流
    AVStream* getVideoStream();
    void setVideoStream(AVStream*& stream);
    //获取/设置帧时间
    double getFrameTimer();
    void setFrameTimer(const double& frameTimer);
    //获取/设置视频时钟
    double getVideoClock();
    void setVideoClock(const double& videoClock);
    //获取/设置上一帧的延时
    double getFrameLastDelay();
    void setFrameLastDelay(const double& frameLastDelay);
    //获取/设置上一帧的播放时间
    double getFrameLastPts();
    void setFrameLastPts(const double& frameLastPts);
    //获取帧大小/视频队列大小
    int getVideoFrameSiez();
    int getVideoQueueSize();
    //包入队
    void enqueuePacket(const AVPacket& pkt);
    //帧队列出队
    AVFrame* dequeueFrame();
    //清空帧/包队列
    void clearFrames();
    void clearPackets();
public:
    SwsContext* swsContext = NULL;
private:
    double frameTimer;         // Sync fields
    double frameLastPts;
    double frameLastDelay;
    double videoClock;
    PacketQueue* videoPackets;
    FrameQueue frameQueue;
    AVStream* stream;
    int streamIndex = -1;
    QMutex mutex;
    AVCodecContext* videoContext;
};

