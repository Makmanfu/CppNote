
#include "stdafx.h"
#include "preprocessing.h"


/*****************************************************************

    Find the min box. The min box respect original aspect ratio image
    The image is a binary data and background is white.

*******************************************************************/
void findX(IplImage* imgSrc, int* min, int* max)
{
    int i;
    int minFound = 0;
    CvMat data;
    CvScalar maxVal = cvRealScalar(imgSrc->width * 255);
    CvScalar val = cvRealScalar(0);
    //For each col sum, if sum < width*255 then we find the min
    //then continue to end to search the max, if sum< width*255 then is new max
    for (i = 0; i < imgSrc->width; i++)
    {
        cvGetCol(imgSrc, &data, i);
        val = cvSum(&data);
        if (val.val[0] < maxVal.val[0])
        {
            *max = i;
            if (!minFound)
            {
                *min = i;
                minFound = 1;
            }
        }
    }
}

void findY(IplImage* imgSrc, int* min, int* max)
{
    int i;
    int minFound = 0;
    CvMat data;
    CvScalar maxVal = cvRealScalar(imgSrc->width * 255);
    CvScalar val = cvRealScalar(0);
    //For each col sum, if sum < width*255 then we find the min
    //then continue to end to search the max, if sum< width*255 then is new max
    for (i = 0; i < imgSrc->height; i++)
    {
        cvGetRow(imgSrc, &data, i);
        val = cvSum(&data);
        if (val.val[0] < maxVal.val[0])
        {
            *max = i;
            if (!minFound)
            {
                *min = i;
                minFound = 1;
            }
        }
    }
}

CvRect findBB(IplImage* imgSrc)
{
    CvRect aux;
    int xmin, xmax, ymin, ymax;
    xmin = xmax = ymin = ymax = 0;
    findX(imgSrc, &xmin, &xmax);
    findY(imgSrc, &ymin, &ymax);
    aux = cvRect(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1);    //此处代码为改进后
    return aux;
}

IplImage preprocessing(IplImage* imgSrc, int new_width, int new_height)
{
    IplImage* result;
    IplImage* scaledResult;
    CvMat data;
    CvMat dataA;
    CvRect bb;//bounding box
    CvRect bba;//boundinb box maintain aspect ratio
    //Find bounding box找到边界框
    bb = findBB(imgSrc);
    //Get bounding box data and no with aspect ratio, the x and y can be corrupted
    cvGetSubRect(imgSrc, &data, cvRect(bb.x, bb.y, bb.width, bb.height));
    //Create image with this data with width and height with aspect ratio 1
    //then we get highest size betwen width and height of our bounding box
    int size = (bb.width > bb.height) ? bb.width : bb.height;
    result = cvCreateImage(cvSize(size, size), 8, 1);
    cvSet(result, CV_RGB(255, 255, 255), NULL);
    //将图像放在正中间，大小归一化
    int x = (int) floor((float)(size - bb.width) / 2.0f);
    int y = (int) floor((float)(size - bb.height) / 2.0f);
    cvGetSubRect(result, &dataA, cvRect(x, y, bb.width, bb.height));
    cvCopy(&data, &dataA, NULL);
    //Scale result
    scaledResult = cvCreateImage(cvSize(new_width, new_height), 8, 1);
    cvResize(result, scaledResult, CV_INTER_NN);
    //Return processed data
    return *scaledResult;//直接返回处理以后的图片
}




basicOCR::basicOCR() //构造函数
{
    //initial
    sprintf(file_path, "OCR/");
    train_samples = 50;//训练样本，总共100个，50个训练，50个测试
    classes = 10;//暂时识别十个数字
    size = 128;//
    trainData = cvCreateMat(train_samples * classes, size * size, CV_32FC1);    //训练数据的矩阵
    trainClasses = cvCreateMat(train_samples * classes, 1, CV_32FC1);
    //Get data (get images and process it)
    getData();
    //train
    train();
    //Test
    test();
    printf(" ------------------------------------------------------------------------\n");
    printf("|\t识别结果\t|\t 测试精度\t|\t  准确率\t|\n");
    printf(" ------------------------------------------------------------------------\n");
}

void basicOCR::getData()
{
    IplImage* src_image;
    IplImage prs_image;
    CvMat row, data;
    char file[255];
    int i, j;
    for (i = 0; i < classes; i++)
    {
        for (j = 0; j < train_samples; j++)
        {
            //加载pbm格式图像，作为训练
            if (j < 10)
                sprintf(file, "%s%d/%d0%d.pbm", file_path, i, i, j);
            else
                sprintf(file, "%s%d/%d%d.pbm", file_path, i, i, j);
            src_image = cvLoadImage(file, 0);
            if (!src_image)
            {
                printf("Error: Cant load image %s\n", file);
                //exit(-1);
            }
            //process file
            prs_image = preprocessing(src_image, size, size);
            //Set class label
            cvGetRow(trainClasses, &row, i * train_samples + j);
            cvSet(&row, cvRealScalar(i));
            //Set data
            cvGetRow(trainData, &row, i * train_samples + j);
            IplImage* img = cvCreateImage(cvSize(size, size), IPL_DEPTH_32F, 1);
            //convert 8 bits image to 32 float image
            cvConvertScale(&prs_image, img, 0.0039215, 0);
            cvGetSubRect(img, &data, cvRect(0, 0, size, size));
            CvMat row_header, *row1;
            //convert data matrix sizexsize to vecor
            row1 = cvReshape(&data, &row_header, 0, 1);
            cvCopy(row1, &row, NULL);
        }
    }
}

void basicOCR::train()
{
#if CV_MAJOR_VERSION<3
    knn = new CvKNearest(trainData, trainClasses, 0, false, K);
#else
    //???
    //knn = new cv::ml::KNearest(trainData, trainClasses, 0, false, K);
#endif
}

float basicOCR::classify(IplImage* img, int showResult)    //第二个参数主要用来控制是测试训练样本还是手写识别
{
    IplImage prs_image;
    CvMat data;
    CvMat* nearest = cvCreateMat(1, K, CV_32FC1);
    float result;
    //处理输入的图像
    prs_image = preprocessing(img, size, size);
    //Set data
    IplImage* img32 = cvCreateImage(cvSize(size, size), IPL_DEPTH_32F, 1);
    cvConvertScale(&prs_image, img32, 0.0039215, 0);
    cvGetSubRect(img32, &data, cvRect(0, 0, size, size));
    CvMat row_header, *row1;
    row1 = cvReshape(&data, &row_header, 0, 1);
#if CV_MAJOR_VERSION<3
    result = knn->find_nearest(row1, K, 0, 0, nearest, 0);
#else
    //???
    result = 0;
#endif
    int accuracy = 0;
    for (int i = 0; i < K; i++)
    {
        if ((nearest->data.fl[i]) == result)
            accuracy++;
    }
    float pre = 100 * ((float) accuracy / (float) K);
    if (showResult == 1)
    {
        printf("|\t    %.0f    \t| \t    %.2f%%  \t| \t %d of %d \t| \n", result, pre, accuracy, K);
        printf(" ------------------------------------------------------------------------\n");
    }
    return result;
}

void basicOCR::test()
{
    IplImage* src_image;
    IplImage prs_image;
    CvMat row, data;
    char file[255];
    int i, j;
    int error = 0;
    int testCount = 0;
    for (i = 0; i < classes; i++)
    {
        for (j = 50; j < 50 + train_samples; j++)   //五十个测试样本，计算一下错误率
        {
            sprintf(file, "%s%d/%d%d.pbm", file_path, i, i, j);
            src_image = cvLoadImage(file, 0);
            if (!src_image)
            {
                printf("Error: Cant load image %s\n", file);
                exit(-1);
            }
            //process file
            prs_image = preprocessing(src_image, size, size);
            float r = classify(&prs_image, 0);
            if ((int) r != i)
                error++;
            testCount++;
        }
    }
    float totalerror = 100 * (float) error / (float) testCount;
    printf("测试系统误识率: %.2f%%\n", totalerror);
}











//OCRmain================================================
IplImage* imagen;
int red, green, blue;
IplImage* screenBuffer;
int drawing;
int r, last_x, last_y;

void draw(int x, int y)
{
    //Draw a circle where is the mouse
    cvCircle(imagen, cvPoint(x, y), r, CV_RGB(red, green, blue), -1, 4, 0);
    //Get clean copy of image
    screenBuffer = cvCloneImage(imagen);
    cvShowImage("手写板", screenBuffer);
}

void drawCursor(int x, int y)
{
    //Get clean copy of image
    screenBuffer = cvCloneImage(imagen);
    //Draw a circle where is the mouse
    cvCircle(screenBuffer, cvPoint(x, y), r, CV_RGB(0, 0, 0), 1, 4, 0);
}


/*************************
    Mouse CallBack 在opencv的highgui.h头文件中有以下宏定义
    event:
    #define CV_EVENT_MOUSEMOVE      0
    #define CV_EVENT_LBUTTONDOWN    1
    #define CV_EVENT_RBUTTONDOWN    2
    #define CV_EVENT_MBUTTONDOWN    3
    #define CV_EVENT_LBUTTONUP      4
    #define CV_EVENT_RBUTTONUP      5
    #define CV_EVENT_MBUTTONUP      6
    #define CV_EVENT_LBUTTONDBLCLK  7
    #define CV_EVENT_RBUTTONDBLCLK  8
    #define CV_EVENT_MBUTTONDBLCLK  9

    x, y: mouse position

    flag:
    #define CV_EVENT_FLAG_LBUTTON   1
    #define CV_EVENT_FLAG_RBUTTON   2
    #define CV_EVENT_FLAG_MBUTTON   4
    #define CV_EVENT_FLAG_CTRLKEY   8
    #define CV_EVENT_FLAG_SHIFTKEY  16
    #define CV_EVENT_FLAG_ALTKEY    32

**************************/

void on_mouse(int event, int x, int y, int flags, void* param)
{
    last_x = x;
    last_y = y;
    drawCursor(x, y);
    //Select mouse Event
    if (event == CV_EVENT_LBUTTONDOWN)
    {
        drawing = 1;
        draw(x, y);
    }
    else if (event == CV_EVENT_LBUTTONUP)
    {
        //drawing=!drawing;
        drawing = 0;
    }
    else if (event == CV_EVENT_MOUSEMOVE  &&  flags & CV_EVENT_FLAG_LBUTTON)
    {
        if (drawing)
            draw(x, y);
    }
}

//int OCRmain(int argc, char** argv)
int OCRmain(void)
{
    printf("                                 数字字符识别\n"
           "快捷键: \n"
           "\tr - 重置白版\n"
           "\t+ - 笔迹增粗 ++\n"
           "\t- - 笔迹减细 --\n"
           "\ts - 保存输入为 out.pbm\n"    //输入可以作为样本再次部署进去
           "\tc - 输入分类识别, 结果在console显示\n"
           "\tESC - 退出程序\n");
    drawing = 0;
    r = 10;
    red = green = blue = 0;
    last_x = last_y = red = green = blue = 0;
    //Create image
    imagen = cvCreateImage(cvSize(128, 128), IPL_DEPTH_8U, 1);       //白板图像保存为128*128
    //Set data of image to white
    cvSet(imagen, CV_RGB(255, 255, 255), NULL);
    //Image we show user with cursor and other artefacts we need
    screenBuffer = cvCloneImage(imagen);
    //Create window
    cvNamedWindow("手写板", 0);
    cvResizeWindow("手写板", 512, 512);
    //Create mouse CallBack
    cvSetMouseCallback("手写板", &on_mouse, 0);
    //////////////////
    //生成基础OCR类
    //////////////////
    basicOCR ocr;
    //Main Loop
    for (;;)
    {
        int c;
        cvShowImage("手写板", screenBuffer);
        c = cvWaitKey(10);
        if ((char) c == 27)
            break;
        if ((char) c == '+')
        {
            r++;
            drawCursor(last_x, last_y);
        }
        if (((char) c == '-') && (r > 1))
        {
            r--;
            drawCursor(last_x, last_y);
        }
        if ((char) c == 'r')
        {
            cvSet(imagen, cvRealScalar(255), NULL);
            drawCursor(last_x, last_y);
        }
        if ((char) c == 's')
            cvSaveImage("out.pbm", imagen);
        if ((char) c == 'c')
            ocr.classify(imagen, 1);
    }
    cvDestroyWindow("手写板");
    return 0;
}
