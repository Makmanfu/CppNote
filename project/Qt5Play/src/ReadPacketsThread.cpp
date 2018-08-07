
#include "stdafx.h"
#include "ReadPacketsThread.h"
#include "Media.h"
#include <QMutexLocker>

bool ReadPacketsThread::isExit = false;

void ReadPacketsThread::run()
{
    AVPacket packet;
    while (!isExit)
    {
        QMutexLocker locker(&mutex);
        if (!isPlay)  //还没开始播放
        {
            locker.unlock();
            msleep(100);
            continue;
        }
        Media* media = Media::getInstance();
        if (media->audio == nullptr || media->video == nullptr)
            break;
        if (isSeek)  //是否跳转视频的标识，在解压音视频读包时进行跳转
        {
            int64_t stamp = 0;
            stamp = currentPos * media->video->getVideoStream()->duration;
            int ret = av_seek_frame(media->getAVFormatContext(), media->getVideoStreamIndex(),
                                    stamp, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
            media->audio->clearPacket();//要清空所有队列
            media->video->clearPackets();
            media->video->clearFrames();
            isSeek = false;
        }
        bool isInvalid = media->checkMediaSizeValid();//音视队列超过一定量时先不读包，等待包消费
        if (isInvalid)
        {
            locker.unlock();
            msleep(10);
            continue;
        }
        if (!media->getAVFormatContext())
        {
            locker.unlock();
            msleep(10);
            continue;
        }
        int ret = av_read_frame(media->getAVFormatContext(), &packet);//读帧音视频包
        if (ret < 0)
        {
            if (ret == AVERROR_EOF)  //读包出错
                break;
            if (media->getAVFormatContext()->pb->error == 0) // 没有错误就等待下次读
            {
                locker.unlock();
                msleep(100);
                continue;
            }
            else
                break;
        }
        if (media->audio != nullptr && packet.stream_index == media->getAudioStreamIndex()) // 音频包队列此处入队
        {
            locker.unlock();
            media->enqueueAudioPacket(packet);
        }
        else if (media->video != nullptr && packet.stream_index == media->getVideoStreamIndex()) // 视频包队列此处入队
        {
            locker.unlock();
            media->enqueueVideoPacket(packet);
        }
        else
            av_packet_unref(&packet);
    }
    if (packet.size >= 0)
        av_packet_unref(&packet);//包没数据不能释放
}

ReadPacketsThread::ReadPacketsThread()
{
}

ReadPacketsThread::~ReadPacketsThread()
{
    QMutexLocker locker(&mutex);
    isExit = true;
    locker.unlock();
    wait();
}

bool ReadPacketsThread::getIsPlaying()
{
    return isPlay;
}

void ReadPacketsThread::setPlaying(bool isPlaying)
{
    this->isPlay = isPlaying;
}

void ReadPacketsThread::receivePos(float pos)
{
    currentPos = pos;
    isSeek = true;
}