

#pragma once
#include <QImage>

class QImageHandler
{
public:
    QImageHandler(QImage*& srcImage);
    ~QImageHandler();
public:
    void mirrorUpAndDown();
    void mirrorLeftAndRight();
    void gray2RGB();
    void rgb2Gray();

    inline QImage* getHandleImage()
    {
        return desImage;
    };
private:
    QImage* desImage;
    uchar* buf;
};

