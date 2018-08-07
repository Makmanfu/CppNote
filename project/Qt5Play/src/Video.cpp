
#include "stdafx.h"
#include "Video.h"
#include <QMutexLocker>

static bool isExit = false;

Video::Video()
{
    frameTimer = 0.0;
    frameLastDelay = 0.0;
    frameLastPts = 0.0;
    videoClock = 0.0;
    videoPackets = new PacketQueue;
}

Video::~Video()
{
    QMutexLocker locker(&mutex);
    delete videoPackets;
    isExit = true;
    locker.unlock();
    wait();
}

void Video::run()
{
    AVFrame* frame = av_frame_alloc();
    double pts;
    AVPacket pkt;
    while (!isExit)
    {
        QMutexLocker locker(&mutex);
        if (frameQueue.getQueueSize() >= FrameQueue::capacity)  //视频帧多于30帧就等待消费
        {
            locker.unlock();
            msleep(100);
            continue;
        }
        if (videoPackets->getPacketSize() == 0)  //没帧等待帧入队
        {
            locker.unlock();
            msleep(100);
            continue;
        }
        pkt = videoPackets->deQueue();//出队
        int ret = avcodec_send_packet(videoContext, &pkt);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
            continue;
        ret = avcodec_receive_frame(videoContext, frame);
        if (ret < 0 && ret != AVERROR_EOF)
            continue;
        if ((pts = av_frame_get_best_effort_timestamp(frame)) == AV_NOPTS_VALUE)
            pts = 0;
        pts *= av_q2d(stream->time_base);
        pts = synchronizeVideo(frame, pts);//同步视频播放时间
        frame->opaque = &pts;
        frameQueue.enQueue(frame);//帧入队
        av_frame_unref(frame);
    }
    av_frame_free(&frame);
}

double Video::synchronizeVideo(AVFrame*& srcFrame, double& pts)
{
    double frameDelay;
    if (pts != 0)
        videoClock = pts; // Get pts,then set video clock to it
    else
        pts = videoClock; // Don't get pts,set it to video clock
    frameDelay = av_q2d(stream->codec->time_base);
    //frameDelay = av_q2d(stream->codecpar->time_base);
    //frameDelay = av_q2d(stream->time_base);
    frameDelay += srcFrame->repeat_pict * (frameDelay * 0.5);
    videoClock += frameDelay;
    return pts;
}

AVCodecContext* Video::getAVCodecCotext()
{
    return this->videoContext;
}

void Video::setAVCodecCotext(AVCodecContext* avCodecContext)
{
    this->videoContext = avCodecContext;
}

int Video::getStreamIndex()
{
    return streamIndex;
}

void Video::setStreamIndex(const int& streamIndex)
{
    this->streamIndex = streamIndex;
}

AVStream* Video::getVideoStream()
{
    return stream;
}

void Video::setVideoStream(AVStream*& videoStream)
{
    this->stream = videoStream;
}

double Video::getFrameTimer()
{
    return frameTimer;
}

void Video::setFrameTimer(const double& frameTimer)
{
    this->frameTimer = frameTimer;
}


double Video::getVideoClock()
{
    return videoClock;
}

void Video::setVideoClock(const double& videoClock)
{
    this->videoClock = videoClock;
}

double Video::getFrameLastDelay()
{
    return frameLastDelay;
}

void Video::setFrameLastDelay(const double& frameLastDelay)
{
    this->frameLastDelay = frameLastDelay;
}

double Video::getFrameLastPts()
{
    return frameLastPts;
}

void Video::setFrameLastPts(const double& frameLastPts)
{
    this->frameLastPts = frameLastPts;
}

int Video::getVideoFrameSiez()
{
    return frameQueue.getQueueSize();
}

int Video::getVideoQueueSize()
{
    return videoPackets->getPacketSize();
}

void Video::enqueuePacket(const AVPacket& pkt)
{
    videoPackets->enQueue(pkt);
}

AVFrame* Video::dequeueFrame()
{
    return frameQueue.deQueue();
}

void Video::clearFrames()
{
    frameQueue.queueFlush();
}

void Video::clearPackets()
{
    videoPackets->queueFlush();
}
