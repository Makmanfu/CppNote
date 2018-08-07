
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
    //������Ƶ
    Media* config();
    //������Ƶ�ļ�
    Media* setMediaFile(const char* filename);
    //�����Ƶ��С�Ƿ�Ϸ�
    bool checkMediaSizeValid();
    //��ȡ��Ƶ����Ϣ
    int getVideoStreamIndex();
    //��ȡ��Ƶ���±�
    int getAudioStreamIndex();
    //��Ƶ�����
    void enqueueVideoPacket(const AVPacket& packet);
    //��Ƶ�����
    void enqueueAudioPacket(const AVPacket& packet);
    void startAudioPlay();
    //��ȡ����Ƶ�ļ���ʽ������
    AVFormatContext* getAVFormatContext();
    //�رգ�������Դ
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

