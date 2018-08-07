

#pragma once
#include <QImage>

class QImageHandler
{
public:
    QImageHandler(QImage*& srcImage);
    ~QImageHandler();
public:
    //��Ƶ���¾������
    void mirrorUpAndDown();
    //�������Ҿ������
    void mirrorLeftAndRight();
    //�Ҷ�ͼתrgb
    void gray2RGB();
    //��ɫת����rgbת�Ҷ�ͼ
    void rgb2Gray();

    inline QImage* getHandleImage()
    {
        return desImage;
    };
private:
    QImage* desImage;
    uchar* buf;
};

