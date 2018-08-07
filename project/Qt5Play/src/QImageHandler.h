

#pragma once
#include <QImage>

class QImageHandler
{
public:
    QImageHandler(QImage*& srcImage);
    ~QImageHandler();
public:
    //视频上下镜像操作
    void mirrorUpAndDown();
    //视屏左右镜像操作
    void mirrorLeftAndRight();
    //灰度图转rgb
    void gray2RGB();
    //颜色转换，rgb转灰度图
    void rgb2Gray();

    inline QImage* getHandleImage()
    {
        return desImage;
    };
private:
    QImage* desImage;
    uchar* buf;
};

