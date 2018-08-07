#pragma once
#include <QTimer>
#include <QMutex>

class QImage;

class DisplayMediaTimer: public QObject
{
    Q_OBJECT
public:
    static DisplayMediaTimer* getInstance()
    {
        static DisplayMediaTimer displayMediaThread;
        return &displayMediaThread;
    }
    ~DisplayMediaTimer();
    //��ȡ����״̬
    bool isPlaying();
    //���ò���״̬
    void setPlay(bool isPlaying);
    //������Ƶ��С�����������С��ʱʹ��
    void resetImage(int width, int height);
public slots:
    //����Ƶͬ������
    void synchronizeAudioAndVideo();
private:
    //������Ƶ��ʱ��
    DisplayMediaTimer();
    bool isPlay = false;
    QTimer* timer;//��Ƶ��ʱˢ����
    uchar* buf;
    QImage* img;
    int imageWidth = 800;
    int imageHeight = 600;
    QMutex mutex;
signals:
    //���͵�opengl
    void updateFrame(QImage* image);

};

