

#include "stdafx.h"
#include "QImageHandler.h"
#include <QMatrix>

QImageHandler::QImageHandler(QImage*& srcImage)
{
    this->desImage = srcImage;
}

QImageHandler::~QImageHandler()
{
}

void QImageHandler::mirrorUpAndDown()
{
    *desImage = desImage->mirrored(false, true);
}

void QImageHandler::mirrorLeftAndRight()
{
    *desImage = desImage->mirrored(true, false);
}

void QImageHandler::rgb2Gray()
{
    int imageWidth = 0;
    int imageHeight = 0;
    if (desImage && desImage->format() != QImage::Format_Grayscale8)
    {
        imageWidth = desImage->width();
        imageHeight = desImage->height();
        if (desImage)
        {
            delete desImage;
            desImage = NULL;
        }
        desImage = new QImage(imageWidth, imageHeight, QImage::Format_Grayscale8);
    }
}

void QImageHandler::gray2RGB()
{
    int imageWidth = 0;
    int imageHeight = 0;
    if (desImage && desImage->format() != QImage::Format_ARGB32)
    {
        imageWidth = desImage->width();
        imageHeight = desImage->height();
        if (desImage)
        {
            delete desImage;
            desImage = NULL;
        }
        desImage = new QImage(imageWidth, imageHeight, QImage::Format_ARGB32);
    }
}
