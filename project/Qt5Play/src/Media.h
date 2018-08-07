
#pragma once
#include "Audio.h"
#include "Video.h"

class Media
{
public:
    static Media* getInstance()
    {
        static Media media;
        return &media;
    }
public:
    ~Media();
    //配置视频
    Media* config();
    //设置视频文件
    Media* setMediaFile(const char* filename);
    //检查视频大小是否合法
    bool checkMediaSizeValid();
    //获取视频流信息
    int getVideoStreamIndex();
    //获取音频流下标
    int getAudioStreamIndex();
    //视频包入队
    void enqueueVideoPacket(const AVPacket& packet);
    //音频包入队
    void enqueueAudioPacket(const AVPacket& packet);
    void startAudioPlay();
    //获取音视频文件格式上下文
    AVFormatContext* getAVFormatContext();
    //关闭，回收资源
    void close();
public:
    Video* video;
    Audio* audio;
    int totalMs = 0;
    int pts = 0;
private:
    AVFormatContext* pFormatCtx;
    Media();
    const char* filename;
    QMutex mutex;
};

