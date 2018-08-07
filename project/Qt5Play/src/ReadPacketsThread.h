

#pragma once
#include <QThread>
#include <QMutex>


class ReadPacketsThread : public QThread
{
    Q_OBJECT
public:
    static ReadPacketsThread* getInstance()
    {
        static ReadPacketsThread rpt;
        return &rpt;
    }
    ~ReadPacketsThread();
    //��ȡ����Ƶ�����̴߳�����
    void run();
    //��ȡ��Ƶ����״̬
    bool getIsPlaying();
    //���ò���״̬
    void setPlaying(bool isPlaying);
public slots:
    //��Ƶ��תλ��
    void receivePos(float pos);
public:
    static bool isExit;
    float currentPos = 0;
    bool isSeek = false;
private:
    QMutex mutex;
    ReadPacketsThread();
    bool isPlay = false;
};

