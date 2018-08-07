
#pragma once
#include "PacketQueue.h"

class Audio
{
public:
    Audio();
    ~Audio();
public:
    static int audioVolume;
    //����ʱ���׼
    static double r2d(AVRational r);
    //���豸����audio���ݵĻص�����
    static void audioCallback(void* userdata, Uint8* stream, int len);
    //����Avpacket�е�������䵽����ռ�
    static int audioDecodeFrame(Audio* audio, uint8_t* audioBuffer, int bufferSize);
public:
    const uint32_t BUFFERSIZE = 192000;// �������Ĵ�С
    //��Ƶ����
    bool audioPlay();
    //��Ƶ�ر�
    bool audioClose();
    //��ȡ��ǰ��Ƶʱ��
    double getCurrentAudioClock();
    //��ȡ/�������±�
    int getStreamIndex();
    void setStreamIndex(const int streamIndex);
    //��ȡ��Ƶ���д�С
    int getAudioQueueSize();
    //��Ƶ����ӳ���
    void enqueuePacket(const AVPacket pkt);
    AVPacket dequeuePacket();
    //��ȡ������Ƶ����
    uint8_t* getAudioBuff();
    void setAudioBuff(uint8_t*& audioBuff);
    //��ȡ/������Ƶ�����С
    uint32_t getAudioBuffSize();
    void setAudioBuffSize(uint32_t audioBuffSize);
    //��ȡ/������Ƶ������±�
    uint32_t getAudioBuffIndex();
    void setAudioBuffIndex(uint32_t audioBuffIndex);
    //��ȡ/������Ƶʱ��
    double getAudioClock();
    void setAudioClock(const double& audioClock);
    //��ȡ/������Ƶ��
    AVStream* getStream();
    void setStream(AVStream*& stream);
    //��ȡ��Ƶ������������
    AVCodecContext* getAVCodecContext();
    void setAVCodecContext(AVCodecContext* audioContext);
    //��ȡ����״̬
    bool getIsPlaying();
    void setPlaying(bool isPlaying);
    //������Ƶ������
    void clearPacket();
    void setVolume(int volume);
private:
    AVCodecContext* audioContext;//��Ƶ������������
    AVStream* stream;   //��Ƶ��
    double audioClock; // audio clock
    PacketQueue audiaPackets;
    uint8_t* audioBuff;       // ��������ݵĻ���ռ�
    uint32_t audioBuffSize;  // buffer�е��ֽ���
    uint32_t audioBuffIndex; // buffer��δ�������ݵ�index
    int streamIndex = -1;
    bool isPlay = false;

};


