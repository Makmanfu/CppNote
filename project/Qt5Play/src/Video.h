

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
    //��Ƶ��֡�̴߳�����
    void run();
    //����ͬ����Ƶ�Ĳ���ʱ��
    double synchronizeVideo(AVFrame*& srcFrame, double& pts);

    //��ȡ/������Ƶ������������
    AVCodecContext* getAVCodecCotext();
    void setAVCodecCotext(AVCodecContext* avCodecContext);
    //��ȡ/�������±�
    int getStreamIndex();
    void setStreamIndex(const int& streamIndex);
    //��ȡ/������Ƶ��
    AVStream* getVideoStream();
    void setVideoStream(AVStream*& stream);
    //��ȡ/����֡ʱ��
    double getFrameTimer();
    void setFrameTimer(const double& frameTimer);
    //��ȡ/������Ƶʱ��
    double getVideoClock();
    void setVideoClock(const double& videoClock);
    //��ȡ/������һ֡����ʱ
    double getFrameLastDelay();
    void setFrameLastDelay(const double& frameLastDelay);
    //��ȡ/������һ֡�Ĳ���ʱ��
    double getFrameLastPts();
    void setFrameLastPts(const double& frameLastPts);
    //��ȡ֡��С/��Ƶ���д�С
    int getVideoFrameSiez();
    int getVideoQueueSize();
    //�����
    void enqueuePacket(const AVPacket& pkt);
    //֡���г���
    AVFrame* dequeueFrame();
    //���֡/������
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

