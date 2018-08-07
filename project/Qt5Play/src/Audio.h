
#pragma once
#include "PacketQueue.h"

class Audio
{
public:
    Audio();
    ~Audio();
public:
    static int audioVolume;
    //计算时间基准
    static double r2d(AVRational r);
    //向设备发送audio数据的回调函数
    static void audioCallback(void* userdata, Uint8* stream, int len);
    //解码Avpacket中的数据填充到缓冲空间
    static int audioDecodeFrame(Audio* audio, uint8_t* audioBuffer, int bufferSize);
public:
    const uint32_t BUFFERSIZE = 192000;// 缓冲区的大小
    //音频播放
    bool audioPlay();
    //音频关闭
    bool audioClose();
    //获取当前音频时钟
    double getCurrentAudioClock();
    //获取/设置流下标
    int getStreamIndex();
    void setStreamIndex(const int streamIndex);
    //获取音频队列大小
    int getAudioQueueSize();
    //音频包入队出队
    void enqueuePacket(const AVPacket pkt);
    AVPacket dequeuePacket();
    //获取设置音频缓冲
    uint8_t* getAudioBuff();
    void setAudioBuff(uint8_t*& audioBuff);
    //获取/设置音频缓冲大小
    uint32_t getAudioBuffSize();
    void setAudioBuffSize(uint32_t audioBuffSize);
    //获取/设置音频缓冲的下标
    uint32_t getAudioBuffIndex();
    void setAudioBuffIndex(uint32_t audioBuffIndex);
    //获取/设置音频时钟
    double getAudioClock();
    void setAudioClock(const double& audioClock);
    //获取/设置音频流
    AVStream* getStream();
    void setStream(AVStream*& stream);
    //获取音频解码器上下文
    AVCodecContext* getAVCodecContext();
    void setAVCodecContext(AVCodecContext* audioContext);
    //获取播放状态
    bool getIsPlaying();
    void setPlaying(bool isPlaying);
    //清理音频包队列
    void clearPacket();
    void setVolume(int volume);
private:
    AVCodecContext* audioContext;//音频解码器上下文
    AVStream* stream;   //音频流
    double audioClock; // audio clock
    PacketQueue audiaPackets;
    uint8_t* audioBuff;       // 解码后数据的缓冲空间
    uint32_t audioBuffSize;  // buffer中的字节数
    uint32_t audioBuffIndex; // buffer中未发送数据的index
    int streamIndex = -1;
    bool isPlay = false;

};


