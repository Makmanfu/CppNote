
#include "stdafx.h"
#include "MatUseTool.h"


MatUseTool::MatUseTool(void)
{
}

MatUseTool::~MatUseTool(void)
{
}

void MatUseTool::CalcTime(void)
{
    double t = (double) cvGetTickCount();
    //计算中间用的算法时间
    cout << ((double) cvGetTickCount() - t) / ((double) cvGetTickFrequency() * 1000.) << "ms ";
}

void MatUseTool::MatLayers(Rect pt)
{
    Mat mUser = imread("lena.jpg");
    //掩膜的使用方法
    Mat imageROI = mUser(Rect(pt.x, pt.y, pt.width, pt.height));
    //局部反转
    flip(imageROI.clone(), imageROI, 1);
    imshow("局部反转", mUser);
    waitKey(0);
}

void MatUseTool::saveXYZMat(const char* filename, const Mat& mat)
{
    char szLine[200000] = { 0 };
    char szWord[10000] = { 0 };
    FILE* fp = fopen(filename, "wt");
    for (int x = 0; x < mat.rows; x++)
    {
        strcpy(szLine, "");
        for (int y = 0; y < mat.cols; y++)
        {
            double point = cvGetReal2D(&CvMat(mat), x, y);
            sprintf(szWord, "%.4f,", point);    //精度调节
            strcat(szLine, szWord);
        }
        fprintf(fp, "%s\n", szLine);
    }
    fclose(fp);
}

Mat MatAddMat(Mat& InOut_img, const Mat& Inlogo, Point Ptbegin, float ff)
{
    Mat mask;                   //这个是掩膜
    if (-2 == ff)               //png特例透明覆盖方式
        cvtColor(Inlogo, mask, CV_BGR2GRAY);
    //缺少区域判断 可能会报错
    Mat imageROI = InOut_img(Rect(Ptbegin.x, Ptbegin.y, Inlogo.cols, Inlogo.rows));
    if (-1 == ff)
        Inlogo.copyTo(imageROI);        //这个是覆盖  不会透明的
    else if (-2 == ff)
        Inlogo.copyTo(imageROI, mask);    //这个是在掩膜区域 会透明的
    else
        cv::addWeighted(imageROI, 1.0, Inlogo, ff, 0, imageROI);
    //这个是 融合公式 f_out = alphaValue*f_img+betaValue*f_add;
    return InOut_img;
}

void MatUseTool::MatToNumPanel(vector<Mat>& Imglist, Mat& f_out, int f_num /*= 1*/)
{
    if (Imglist.empty()) 
        return;
    int imgNum = Imglist.size();
    if (f_num > imgNum)   f_num = imgNum;
    int num2 = imgNum / f_num;
    if (imgNum % f_num != 0) num2 += 1;
    int flagNum = 0;
    f_out = Mat((Imglist[0].rows) * num2 + num2, (Imglist[0].cols + 1) * f_num + f_num, CV_8UC3, Scalar(0, 0, 0));
    for (vector<Mat>::iterator it = Imglist.begin(); it != Imglist.end(); ++it, ++flagNum)
        MatAddMat(f_out, *it, Point(it->cols * (flagNum % f_num), it->rows * (flagNum / f_num)), -1);
}

bool BMPimg::readBmp(char* bmpName)
{
    /***********************************************************************
        函数名称：readBmp()
        函数参数：char *bmpName -文件名字及路径
        返回值：0为失败，1为成功
        说明： 给定一个图像文件名及其路径，读图像的位图数据、宽、高、颜色表及每像素
        位数等数据进内存，存放在相应的全局变量中
    ***********************************************************************/
    //二进制读方式打开指定的图像文件
    FILE* fp = fopen(bmpName, "rb");
    if (fp == 0) return false;
    //跳过位图文件头结构BITMAPFILEHEADER
    fseek(fp, sizeof(BITMAPFILEHEADER), 0);
    //定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
    BITMAPINFOHEADER head;
    fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
    //获取图像宽、高、每像素所占位数等信息
    bmpWidth = head.biWidth;
    bmpHeight = head.biHeight;
    biBitCount = head.biBitCount;
    //定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
    int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
    //灰度图像有颜色表，且颜色表表项为256
    if (biBitCount == 8)   //申请颜色表所需要的空间，读颜色表进内存
    {
        pColorTable = new RGBQUAD[256];
        fread(pColorTable, sizeof(RGBQUAD), 256, fp);
    }
    //申请位图数据所需要的空间，读位图数据进内存
    pBmpBuf = new unsigned char[lineByte * bmpHeight];
    fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
    //关闭文件
    fclose(fp);
    return true;
}

bool BMPimg::saveBmp(char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable)
{
    /*****************************************
        函数名称：saveBmp()
        函数参数：
         char *bmpName-文件名字及路径
         unsigned char *imgBuf-待存盘的位图数据
         int width-以像素为单位待存盘位图的宽
         int height-以像素为单位待存盘位图高
         int biBitCount-每像素所占位数
         RGBQUAD *pColorTable-颜色表指针
        返回值：0为失败，1为成功
        说明：给定一个图像位图数据、宽、高、颜色表指针及每像素所占的位数等信息，
           将其写到指定文件中
    ***********************************************************************/
    //如果位图数据指针为0，则没有数据传入，函数返回
    if (!imgBuf)
        return 0;
    //颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0
    int colorTablesize = 0;
    if (biBitCount == 8)
        colorTablesize = 1024;
    //待存储图像数据每行字节数为4的倍数
    int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
    //以二进制写的方式打开文件
    FILE* fp = fopen(bmpName, "wb");
    if (fp == 0) return 0;
    //申请位图文件头结构变量，填写文件头信息
    BITMAPFILEHEADER fileHead;
    fileHead.bfType = 0x4D42;//bmp类型
    //bfSize是图像文件4个组成部分之和
    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte * height;
    fileHead.bfReserved1 = 0;
    fileHead.bfReserved2 = 0;
    //bfOffBits是图像文件前3个部分所需空间之和
    fileHead.bfOffBits = 54 + colorTablesize;
    //写文件头进文件
    fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
    //申请位图信息头结构变量，填写信息头信息
    BITMAPINFOHEADER head;
    head.biBitCount = biBitCount;
    head.biClrImportant = 0;
    head.biClrUsed = 0;
    head.biCompression = 0;
    head.biHeight = height;
    head.biPlanes = 1;
    head.biSize = 40;
    head.biSizeImage = lineByte * height;
    head.biWidth = width;
    head.biXPelsPerMeter = 0;
    head.biYPelsPerMeter = 0;
    //写位图信息头进内存
    fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);
    //如果灰度图像，有颜色表，写入文件
    if (biBitCount == 8)
        fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);
    //写位图数据进文件
    fwrite(imgBuf, height * lineByte, 1, fp);
    //关闭文件
    fclose(fp);
    return 1;
}

Mat BMPimg::BufferMat(BYTE** pBuffer, int f_w, int f_h)
{
    //注意检查pBuffer一下空指针
    Mat f_img = Mat(f_h, f_w, CV_8UC4, *pBuffer);
    return f_img;
}

cv::Mat BMPimg::BmpToMat(HBITMAP hBitmap)
{
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    //计算参数
    int f_nchannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
    //int f_depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
    int image_width = bmp.bmWidth;
    int image_height = bmp.bmHeight;
    BYTE* pBuffer;
    pBuffer = new BYTE[image_width * image_height * f_nchannels];
    GetBitmapBits(hBitmap, image_height * image_width * f_nchannels, pBuffer);
    return  Mat(image_height, image_width, CV_8UC4, pBuffer);
}

void BMPimg::BMPtoBuffer(HBITMAP hBitmap, BYTE** data)
{
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    //计算参数
    int f_nchannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
    int image_width = bmp.bmWidth;
    int image_height = bmp.bmHeight;
    BYTE* pBuffer;
    pBuffer = new BYTE[image_width * image_height * f_nchannels];
    GetBitmapBits(hBitmap, image_height * image_width * f_nchannels, pBuffer);
    *data = pBuffer;
    //delete pBuffer;
    //std::shared_ptr<BYTE> pBuffer(new BYTE(image_width * image_height*f_nchannels) );
    //GetBitmapBits(hBitmap, image_height * image_width * f_nchannels, (LPVOID)pBuffer);
    //*data = &pBuffer;
    return ;
}

int BMPimg::TestSavebmp(void)
{
    //读入指定BMP文件进内存
    char readPath[] = "lena.bmp";
    readBmp(readPath);
    //输出图像的信息
    printf("width=%d,height=%d, biBitCount=%d\n", bmpWidth, bmpHeight, biBitCount);
    //将图像数据存盘
    char writePath[] = "lenacpy1.BMP";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
    //清除缓冲区，pBmpBuf和pColorTable是全局变量，在文件读入时申请的空间
    delete[]pBmpBuf;
    if (biBitCount == 8)
        delete[]pColorTable;
    return 0;
}

int BMPimg::TestBmpMem(void)
{
    //读入指定BMP文件进内存
    char readPath[] = "lena.bmp";
    readBmp(readPath);
    //输出图像的信息
    printf("width=%d,height=%d,biBitCount=%d\n", bmpWidth, bmpHeight, biBitCount);
    //循环变量，图像的坐标
    int i, j;
    //每行字节数
    int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
    //循环变量，针对彩色图像，遍历每像素的三个分量
    int k;
    //将图像左下角1/4部分置成黑色
    if (biBitCount == 8)            //对于灰度图像
    {
        for (i = 0; i < bmpHeight / 2; i++)
            for (j = 0; j < bmpWidth / 2; j++)
                * (pBmpBuf + i * lineByte + j) = 0;
    }
    else if (biBitCount == 24)      //彩色图像
    {
        for (i = 0; i < bmpHeight / 2; i++)
            for (j = 0; j < bmpWidth / 2; j++)
                for (k = 0; k < 3; k++)   //每像素RGB三个分量分别置0才变成黑色
                    * (pBmpBuf + i * lineByte + j * 3 + k) = 0;
    }
    //将图像数据存盘
    char writePath[] = "lenacpy2.BMP";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
    //清除缓冲区，pBmpBuf和pColorTable是全局变量，在文件读入时申请的空间
    delete[]pBmpBuf;
    if (biBitCount == 8)
        delete[]pColorTable;
    return 0;
}

int BMPimg::TestBmpMem2(void)
{
    //读入指定BMP文件进内存
    char readPath[] = "lena.bmp";
    readBmp(readPath);
    //输出图像的信息
    printf("width=%d,height=%d,biBitCount=%d\n", bmpWidth, bmpHeight, biBitCount);
    //改变灰度图像的颜色表蓝色分量的值，查看前后变化
    if (biBitCount == 8)
        for (int i = 0; i < 256; i++)
            pColorTable[i].rgbBlue = 255 - pColorTable[i].rgbBlue;
    //将图像数据存盘
    char writePath[] = "lenacpy3.BMP";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
    //清除缓冲区，pBmpBuf和pColorTable是全局变量,在文件读入时申请的空间
    delete[]pBmpBuf;
    if (biBitCount == 8)
        delete[]pColorTable;
    return 0;
}

void MatDraw::SimpleDraw(void)
{
    Mat t_Img = Mat(512, 512, CV_8UC3, Scalar(255, 255, 255));
    /* 圆 */
    //圆心
    Point center = Point(255, 255);
    //半径
    int r = 100;
    //承载图像    //Mat picture(512,512,CV_8UC3,Scalar(255,255,255));
    //参数为：承载的图像、圆心、半径、颜色、粗细、线型 粗细为-1时是实心圆
    circle(t_Img, center, r, Scalar(0, 255, 0), -1);
    /* 椭圆 */
    //参数为：承载的图像、圆心、长短轴、径向夹角（水平面到长轴的夹角）、起始角度（长轴到起始边沿的夹角）、结束角度（长轴到结束点的夹角）、倾斜的矩形（可选项）、颜色、粗细、线性、偏移
    ellipse(t_Img, center, Size(250, 100), 0, 30, 240, Scalar(0, 0, 0));
    /* 画线 */
    Point a = Point(60, 60);
    //参数为：承载的图像、起始点、结束点、颜色、粗细、线型
    line(t_Img, a, center, Scalar(255, 0, 0));
    /* 画矩形  */
    //参数为：承载的图像、顶点、对角点、颜色（这里是蓝色）、粗细、大小
    rectangle(t_Img, a, center, Scalar(255, 0, 0));
    /* 插入文字  */
    //参数为：承载的图片，插入的文字，文字的位置（文本框左下角），字体，大小，颜色
    string words = "good luck";
    putText(t_Img, words, Point(t_Img.rows / 2, t_Img.cols / 4), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0));
    imshow("画图Demo", t_Img);
    waitKey(0);
}

void MatDraw::DrawArrow(void)
{
    Mat& t_img = Mat(512, 512, CV_8UC3, Scalar(0, 0, 0));
    Point pStart(380, 100), pEnd(100, 250);
    Scalar lineColor(0, 255, 255);
    cvArrow(t_img, pStart, pEnd, 10, 45, lineColor, 3, 8);
    pStart = Point(100, 100);
    pEnd = Point(320, 190);
    lineColor = Scalar(0, 0, 255);
    cvArrow(t_img, pStart, pEnd, 25, 30, lineColor, 2, CV_AA);
    pStart = Point(200, 420);
    pEnd = Point(370, 170);
    lineColor = Scalar(255, 0, 255);
    cvArrow(t_img, pStart, pEnd, 17, 15, lineColor, 1, 4);
    imshow("画箭头", t_img);
    waitKey();
}

void MatDraw::DrawTriangle(Point f_xy)
{
    Mat t_img = Mat(512, 512, CV_8UC3, Scalar(255, 255, 255));
    //int x0,y0, x1,y1, x2,y2;
    //x0 = f_xy.x;      //x1 = x0 - 70;       //x2 = x0 +70;
    //y0 = f_xy.y;      //y1 = y0 +40;       //y2 = y0 +40;
    //line(picture,Point(x0,y0),Point(x1,y1),Scalar(0,255,0),2 );
    //line(picture,Point(x2,y2),Point(x1,y1),Scalar(0,255,0),2 );
    //line(picture,Point(x0,y0),Point(x2,y2),Scalar(0,255,0),2 );
    int x0, y0;
    x0 = f_xy.x;
    y0 = f_xy.y;
    int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
    x1 = x0 - 52;
    x2 = x0 + 52;
    y1 = y0 + 33;
    y2 = y0 + 33;
    x3 = x0 - 30;
    x4 = x0 + 30;
    y3 = y0 + 33;
    y4 = y0 + 33;
    x5 = x0 - 58;
    x6 = x0 + 58;
    y5 = y0 + 101;
    y6 = y0 + 101;
    line(t_img, Point(x0, y0), Point(x1, y1), Scalar(0, 255, 0), 2);
    line(t_img, Point(x2, y2), Point(x1, y1), Scalar(0, 255, 0), 2);
    line(t_img, Point(x0, y0), Point(x2, y2), Scalar(0, 255, 0), 2);
    line(t_img, Point(x3, y3), Point(x5, y5), Scalar(0, 255, 0), 2);
    line(t_img, Point(x4, y4), Point(x6, y6), Scalar(0, 255, 0), 2);
    line(t_img, Point(x5, y5), Point(x6, y6), Scalar(0, 255, 0), 2);
    imshow("画三角形直向箭头", t_img);
    waitKey();
}

void MatDraw::DrawTkArrow(Point f_xy, int f_line, double f_theta)
{
    const double PI = 3.1415926;
    Mat t_img = Mat(512, 512, CV_8UC3, Scalar(255, 255, 255));
    int x0 = f_xy.x,  y0 = f_xy.y;
    int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, cx, cy;
    int m_l, n_l, k_l;  //下面x0处一半, cx处一半,cx到c7顶点
    m_l = 30;
    n_l = 30, k_l = 30;
    double theta = f_theta * PI;
    x1 = x0 - m_l;
    y1 = y0;
    x2 = x0 + m_l;
    y2 = y0;
    cx = x0 + cos(theta) * f_line;
    x7 = x0 + cos(theta) * (f_line + k_l);
    cy = y0 - sin(theta) * f_line;
    y7 = y0 - sin(theta) * (f_line + k_l);
    x3 = cx - sin(theta) * (m_l * sin(theta));
    x4 = cx + sin(theta) * (m_l * sin(theta));
    y3 = cy - cos(theta) * (m_l * sin(theta));
    y4 = cy + cos(theta) * (m_l * sin(theta));
    x5 = cx - sin(theta) * (m_l * sin(theta) + n_l);
    x6 = cx + sin(theta) * (m_l * sin(theta) + n_l);
    y5 = cy - cos(theta) * (m_l * sin(theta) + n_l);
    y6 = cy + cos(theta) * (m_l * sin(theta) + n_l);
    line(t_img, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2);
    line(t_img, Point(x1, y1), Point(x3, y3), Scalar(0, 255, 0), 2);
    line(t_img, Point(x2, y2), Point(x4, y4), Scalar(0, 255, 0), 2);
    line(t_img, Point(x5, y5), Point(x6, y6), Scalar(0, 255, 0), 2);
    line(t_img, Point(x5, y5), Point(x7, y7), Scalar(0, 255, 0), 2);
    line(t_img, Point(x6, y6), Point(x7, y7), Scalar(0, 255, 0), 2);
    //line(t_img,Point(x0,y0),Point(x7,y7),Scalar(0,255,0),2 );
    imshow("画任务转向箭头", t_img);
    waitKey();
}

void MatDraw::DrawCamber(Point center, int radius, double f_theta)
{
    const double PI = 3.1415926;
    Mat t_img = Mat(512, 512, CV_8UC3, Scalar(255, 255, 255));
    int x0 = center.x, y0 = center.y;
    int x1, y1, x2, y2;
    x1 = x0 - radius;
    y1 = y0;
    double theta = f_theta * PI;
    x2 = x0 - cos(theta) * radius;
    y2 = y0 - sin(theta) * radius;
    circle(t_img, center, radius, Scalar(0, 255, 0), -1);
    t_img.at<uchar> (200, 200) = 0;
    for (int i = x1; i < x2; ++i)
        t_img.at<uchar> (200, i) = 255;
    int tmpy;
    for (int i = x1; i < x2; ++i)
    {
        tmpy = sqrt(pow((double)(radius), 2) - pow((double)(i), 2));
        cout << tmpy << endl;
        //t_Mat.at<uchar>(i,tmpy )  = 255;
    }
    imshow("画弧线", t_img);
    waitKey();
}

void MatDraw::cvArrow(Mat& f_img, Point pStart, Point pEnd, int len, int alpha, Scalar& color, int thickness /*= 2*/, int lineType /*= CV_AA*/)
{
    //参数说明
    //pStart        起点
    //pEnd          终点
    //len             箭头前的长短
    //alpha         箭头的张开度
    //color          颜色
    //thickness   粗细
    //lineType     线的类型
    const double PI = 3.1415926;
    Point arrow;
    //计算 θ 角（最简单的一种情况在下面图示中已经展示，关键在于 atan2 函数，详情见下面）
    double angle = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));
    line(f_img, pStart, pEnd, color, thickness, lineType);
    //计算箭角边的另一端的端点位置（上面的还是下面的要看箭头的指向，也就是pStart和pEnd的位置）
    arrow.x = pEnd.x + len * cos(angle + PI * alpha / 180);
    arrow.y = pEnd.y + len * sin(angle + PI * alpha / 180);
    line(f_img, pEnd, arrow, color, thickness, lineType);
    arrow.x = pEnd.x + len * cos(angle - PI * alpha / 180);
    arrow.y = pEnd.y + len * sin(angle - PI * alpha / 180);
    line(f_img, pEnd, arrow, color, thickness, lineType);
}

void MatDraw::DrawRectangle(Mat& f_img, float f_x1, float f_y1, float f_x2, float f_y2, Scalar& color)
{
    //参数为：承载的图像、顶点、对角点、颜色（这里是蓝色）、粗细、大小
    if (!f_img.data)  return;
    Point f_begin(f_img.cols * f_x1, f_img.rows * f_y1);
    Point f_end(f_img.cols * f_x2, f_img.rows * f_y2);
    rectangle(f_img, f_begin, f_end, color, 4);
}

Mat MatDraw::TextAddMat(Mat& f_img, const char* f_txt, Point f_pt, double f_theta)
{
    if (0 == f_theta)       //直接绘上去
    {   //将文字写入文本框
        cv::putText(f_img, f_txt, f_pt, CV_FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 2);
    }
    else                    //半透明效果
    {
        //文本框
        Mat logo(26, 200, CV_8UC3, Scalar(0, 0, 0));
        //将文字写入文本框
        cv::putText(logo, f_txt, Point(2, 18), CV_FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 2);
        //融合到区域中(注意坐标越界)
        Mat imageROI = f_img(Rect(f_pt.x, f_pt.y, logo.cols, logo.rows));
        addWeighted(imageROI, 1.0, logo, f_theta, 0.0, imageROI);
    }
    return f_img;
}

Mat MatDraw::TestNumDraw(Mat& f_img, int f_num, Point f_pt)
{
    char buf[255] = {0};
    sprintf(buf, "%dmm", f_num);          //    sprintf(buf,"%s : %d",f_name,f_num);
    //    cv::putText( f_img, buf, f_pt,CV_FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 255, 0),1.2 );
    cv::putText(f_img, buf, f_pt, CV_FONT_HERSHEY_DUPLEX, 1.5, Scalar(0, 255, 0), 1.7);
    return f_img;
}

Mat MatDraw::DrawCircle(Mat& f_img, Point pt, int r, float num/*=0*/, Scalar f_color/*=Scalar(0,255,0)*/)
{
    if (r < 10) r = 10;
    int thickness = 2, r_l =  3 * r / 2;
    circle(f_img, pt, r, f_color , thickness);
    //空心
    line(f_img, Point(pt.x - r_l, pt.y), Point(pt.x - r, pt.y), f_color, thickness);
    line(f_img, Point(pt.x + r_l, pt.y), Point(pt.x + r, pt.y), f_color, thickness);
    line(f_img, Point(pt.x, pt.y - r_l), Point(pt.x, pt.y - r), f_color, thickness);
    line(f_img, Point(pt.x, pt.y + r_l), Point(pt.x, pt.y + r), f_color, thickness);
    char buf[255] = {0};
    if (0 == num)
    {
        sprintf(buf, "(%d,%d)", pt.x, pt.y);         //    sprintf(buf,"%s : %d",f_name,f_num);
        cv::putText(f_img, buf, Point(pt.x + r, pt.y - r), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    else
    {
        sprintf(buf, "%.1fm", num);          //    sprintf(buf,"%s : %d",f_name,f_num);
        cv::putText(f_img, buf, Point(pt.x + r, pt.y - r), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    return f_img;
}

Mat MatDraw::DrawCircle(Mat& f_img, Rect rt, float num/*=0*/, Scalar f_color/*=Scalar(0,255,0)*/)
{
    Point pt;
    int r = rt.width / 2;
    pt.x = rt.width / 2 + rt.x;
    pt.y = rt.height / 2 + rt.y;
    if (r < 10) r = 10;
    int thickness = 2, r_l =  3 * r / 2;
    circle(f_img, pt, r, f_color , thickness);
    line(f_img, Point(pt.x - r_l, pt.y), Point(pt.x - r, pt.y), f_color, thickness);
    line(f_img, Point(pt.x + r_l, pt.y), Point(pt.x + r, pt.y), f_color, thickness);
    line(f_img, Point(pt.x, pt.y - r_l), Point(pt.x, pt.y - r), f_color, thickness);
    line(f_img, Point(pt.x, pt.y + r_l), Point(pt.x, pt.y + r), f_color, thickness);
    char buf[255] = {0};
    if (0 == num)
    {
        sprintf(buf, "(%d,%d)", pt.x, pt.y);         //    sprintf(buf,"%s : %d",f_name,f_num);
        cv::putText(f_img, buf, Point(pt.x + r, pt.y - r), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    else
    {
        sprintf(buf, "%.1fm", num);          //    sprintf(buf,"%s : %d",f_name,f_num);
        cv::putText(f_img, buf, Point(pt.x + r, pt.y - r), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    return f_img;
}

Mat MatDraw::DrawRect(Mat& f_img, Rect rt, float num/*=0*/, Scalar f_color/*=Scalar(0,0,255)*/)
{
    rectangle(f_img, Point(rt.x, rt.y), Point(rt.x + rt.width, rt.y + rt.height), f_color, 2);
    char buf[255] = {0};
    if (0 == num)
    {
        sprintf(buf, "(%d,%d)", rt.x, rt.y);         //    sprintf(buf,"%s : %d",f_name,f_num);
        cv::putText(f_img, buf, Point(rt.x, rt.y + rt.height + 20), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    else
    {
        sprintf(buf, "%.1fm", num);          //    sprintf(buf,"%s : %d",f_name,f_num);
        cv::putText(f_img, buf, Point(rt.x, rt.y + rt.height + 20), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    return f_img;
}

Mat MatDraw::DrawLeftArrow(Mat& f_img, Point pt, int w, int h)
{
    int thickness = 2;
    int x1, x2, x3, x4, x5, x6, x7;
    int y1, y2, y3, y4, y5, y6, y7;
    x1 = x2 = pt.x;
    x3 = x4 = x5 = x6 = pt.x + (2 * w / 3);
    x7 = pt.x + w;
    y1 = y3 = pt.y - h / 2;
    y2 = y4 = pt.y + h / 2;
    y5 = y1 - h;
    y6 = y2 + h;
    y7 = pt.y;
    line(f_img, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), thickness);
    line(f_img, Point(x1, y1), Point(x3, y3), Scalar(0, 255, 0), thickness);
    line(f_img, Point(x4, y4), Point(x2, y2), Scalar(0, 255, 0), thickness);
    line(f_img, Point(x3, y3), Point(x5, y5), Scalar(0, 255, 0), thickness);
    line(f_img, Point(x4, y4), Point(x6, y6), Scalar(0, 255, 0), thickness);
    line(f_img, Point(x7, y7), Point(x5, y5), Scalar(0, 255, 0), thickness);
    line(f_img, Point(x7, y7), Point(x6, y6), Scalar(0, 255, 0), thickness);
    return f_img;
}

Mat MatDraw::DrawLoopArrow(Mat& f_img, Point f_pt, int longl, int shortl, double f_theta, double f_ro, Scalar f_color)
{
    const double PI = 3.1415926;
    double theta = f_theta * PI, ro = f_ro * PI;
    //int x1(f_pt.x), y1(f_pt.y), x2(f_pt.x), y2(f_pt.y+shortl),xl,yl,xr,yr,xl2,yl2,xr2,yr2;
    //xl = x1 - sin(theta) * longl;    yl = y1 + cos(theta) * longl;
    //xr = x1 +sin(theta) * longl;   yr = y1 + cos(theta) * longl;
    //xl2 = xl;    yl2 = yl + shortl;
    //xr2 = xr;   yr2 = yr + shortl;
    //line(f_img,Point(x1,y1),Point(xl,yl),Scalar(0,255,0),2 );
    //line(f_img,Point(x1,y1),Point(xr,yr),Scalar(0,255,0),2 );
    //line(f_img,Point(xl,yl),Point(xl2,yl2),Scalar(0,255,0),2 );
    //line(f_img,Point(xr,yr),Point(xr2,yr2),Scalar(0,255,0),2 );
    //line(f_img,Point(x2,y2),Point(xl2,yl2),Scalar(0,255,0),2 );
    //line(f_img,Point(x2,y2),Point(xr2,yr2),Scalar(0,255,0),2 );
    //Mat t_out=Mat(xr2-xl2,yr2-y1,CV_8UC3, Scalar(255, 255, 255) );
    //return t_out;
    int xk = sin(ro) * shortl, yk = cos(ro) * shortl;
    int x1(f_pt.x), y1(f_pt.y), x2(f_pt.x + xk), y2(f_pt.y + yk);
    int xl(x1 - sin(theta - ro) *longl) , yl(y1 + cos(theta - ro) *longl), xr(x1 + sin(theta + ro) *longl) , yr(y1 + cos(theta + ro) *longl);
    int xl2(xl + xk), yl2(yl + yk), xr2(xr + xk), yr2(yr + yk);
    line(f_img, Point(x1, y1), Point(xl, yl), f_color, 2, 16);
    line(f_img, Point(x1, y1), Point(xr, yr), f_color, 2, 16);
    line(f_img, Point(xl, yl), Point(xl2, yl2), f_color, 2, 16);
    line(f_img, Point(xr, yr), Point(xr2, yr2), f_color, 2, 16);
    line(f_img, Point(x2, y2), Point(xl2, yl2), f_color, 2, 16);
    line(f_img, Point(x2, y2), Point(xr2, yr2), f_color, 2, 16);
    return f_img;
    //画箭头测试
    //MatDraw().DrawLoopArrow(img1,Point( 200,200 ),80,20,1.0/3,-1.0/4);
    //MatDraw().DrawLoopArrow(img1,Point( 400,400 ),80,20,1.0/3,1.0/4);
}

Mat MatDraw::DrawLoopArrow(Mat& f_img, int longl, Point f_pt)
{
    int iw, ih, x1, y1, x2, y2, xl, yl, xr, yr, xl2, yl2, xr2, yr2;
    if (0 != longl)
    {
        int shortl = longl / (2 * 1.73205);     //warning 4
        x1 = f_pt.x;
        y1 = f_pt.y;
        x2 = f_pt.x;
        y2 = f_pt.y + shortl;
        xl = x1 - longl / 2;
        yl = y2;
        xr = x1 + longl / 2;
        yr = y2;
        xl2 = x1 - longl / 2;
        yl2 = y2 + shortl;
        xr2 = x1 + longl / 2;
        yr2 = y2 + shortl;
    }
    else
    {
        iw = f_img.cols;
        ih = f_img.rows;
        x1 = iw / 2;
        y1 = 0;
        x2 = iw / 2;
        y2 = ih / 2;
        xl = 2;
        yl = ih / 2;
        xr = iw - 2;
        yr = ih / 2;
        xl2 = 2;
        yl2 = ih;
        xr2 = iw - 2;
        yr2 = ih;
    }
    line(f_img, Point(x1, y1), Point(xl, yl), Scalar(0, 255, 0), 2);
    line(f_img, Point(x1, y1), Point(xr, yr), Scalar(0, 255, 0), 2);
    line(f_img, Point(xl, yl), Point(xl2, yl2), Scalar(0, 255, 0), 2);
    line(f_img, Point(xr, yr), Point(xr2, yr2), Scalar(0, 255, 0), 2);
    line(f_img, Point(x2, y2), Point(xl2, yl2), Scalar(0, 255, 0), 2);
    line(f_img, Point(x2, y2), Point(xr2, yr2), Scalar(0, 255, 0), 2);
    return f_img;
}

void MatDraw::TrackArrowShow(Mat& f_img, vector<Point>& pts)
{
    //结尾点虚拟 此处需要好好思考
    pts.push_back(Point(pts.rbegin()->x, pts.rbegin()->y + 1));
    //参考线
    //line(f_img, Point(0, f_img.rows / 2), Point(f_img.cols, f_img.rows / 2), Scalar(255, 255, 255), 1);
    //line(f_img, Point(f_img.cols / 2,0), Point(f_img.cols/2, f_img.rows ), Scalar(255, 255, 255), 1);
    //沿着轨迹绘制并旋转图像
    Point t_py;
    for (vector<Point>::const_iterator it = pts.begin(); it != pts.end(); ++it)
    {
        if (it == pts.begin())
        {
            t_py = *it;
            continue;
        }
        else
        {
            //line(f_img, t_py, *it, Scalar(0, 0, 255), 1);       //连线
            DrawLoopArrow(f_img, t_py, 40, 15, 1.0 / 3, atan((double)(it->x - t_py.x) / (it->y - t_py.y)) / 3.1415926);
            t_py = *it;
        }
        //最后一个不好判断
        //DrawLoopArrow(f_img,*it,40,15,1.0/3,atan( (double)(it->x-t_py.x)/(it->y-t_py.y) ) /3.14159 );
    }
}

void MatDraw::DrawCurve(void)
{
    Mat PanelBox = Mat(800, 800, CV_8UC3, Scalar(0, 0, 0));
    //画画坐标系
    line(PanelBox, Point(0, PanelBox.rows / 2), Point(PanelBox.cols, PanelBox.rows / 2), Scalar(255, 255, 255), 1);
    line(PanelBox, Point(PanelBox.cols / 2, 0), Point(PanelBox.cols / 2, PanelBox.rows), Scalar(255, 255, 255), 1);
    int y = 0;
    for (int x = 0; x < PanelBox.cols; x++)   //x
    {
        y = -x  + PanelBox.rows;
        PanelBox.at<Vec3b> (y, x) [0] = 0;
        PanelBox.at<Vec3b> (y, x) [1] = 255;
        PanelBox.at<Vec3b> (y, x) [2] = 0;
    }
    imshow("PanelBox", PanelBox);
    waitKey(0);
}

VideoPlayer::VideoPlayer()
{
    thetype = false;
    cap = VideoCapture(0);      //打开默认的摄像头
}

VideoPlayer::VideoPlayer(const char* ImgName)
{
    thetype = true;
    cap.open(ImgName);
}

VideoPlayer::~VideoPlayer()
{
    cap.release();
}

void VideoPlayer::PlayVideo(int frameStart /*= 0*/, int frameStop /*= 0*/)
{
    int k = 0;
    if (!CheckCapOK()) return;
    if (thetype)
    {
        SetframePoint(frameStart);                      //默认设置开始位置
        if (0 == frameStop || frameStart > frameStop)   //默认设置结束位置
            frameStop = GetframeNum();
        for (int i = 1; i < frameStop - frameStart + 1; ++i)
        {
            if (!cap.read(fps_curr))                 //cap>>fps_curr;
                break;
            cout << i << " " << i + frameStart - 1 << endl;
            k = OptimizationMat(fps_curr);              //图像处理
            resize(fps_curr, fps_curr, Size(800, 600));       //仅仅缩放显示
            imshow("播放视频", fps_curr);
            waitKey(1);
            if (k < 1)
                waitKey(1);
            else
                waitKey(0);
        }
    }
    else
    {
        while (true)
        {
            cap >> fps_curr;
            if (fps_curr.empty())   return;
            cvNamedWindow("摄像头", CV_WINDOW_AUTOSIZE);
            k = OptimizationMat(fps_curr);                    //图像处理
            imshow("摄像头", fps_curr);
            waitKey(1);
        }
    }
}

void VideoPlayer::OnlyPlayVideo(int frameStart /*= 0*/, int frameStop /*= 0*/)
{
    if (!CheckCapOK()) return;
    if (thetype)
    {
        SetframePoint(frameStart);                          //默认设置开始位置
        if (0 == frameStop || frameStart > frameStop)       //默认设置结束位置
            frameStop = GetframeNum();
        double rate = cap.get(CV_CAP_PROP_FPS);             //获取帧率
        int delay = 1000 / rate;                            //两帧间的间隔时间:
        for (int i = 1; i < frameStop - frameStart + 1; ++i)
        {
            if (!cap.read(fps_curr))                 //cap>>fps_curr;
                break;
            cout << i << " " << i + frameStart - 1 << endl;
            resize(fps_curr, fps_curr, Size(800, 600));       //仅仅缩放显示
            imshow("播放视频", fps_curr);
            waitKey(delay);
        }
    }
    else
    {
        while (true)
        {
            cap >> fps_curr;
            if (fps_curr.empty())   return;
            cvNamedWindow("摄像头", CV_WINDOW_AUTOSIZE);
            imshow("摄像头", fps_curr);
            waitKey(1);
        }
    }
}

bool VideoPlayer::CheckCapOK(void)
{
    return cap.isOpened();
}

long VideoPlayer::GetframeNum(void)
{
    if (thetype)
    {
        //获取帧数
        long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
        //获取帧率
        //double rate = capture.get(CV_CAP_PROP_FPS);
        cout << "整个视频共" << totalFrameNumber << "帧" << endl;
        return totalFrameNumber;
    }
    else
    {
        return 0;       //摄像头就不用了
    }
}

void VideoPlayer::SetframePoint(int f_fpsnum /*= 0*/)
{
    if (thetype)
    {
        if (f_fpsnum < GetframeNum())
            cap.set(CV_CAP_PROP_POS_FRAMES, f_fpsnum);
        else
            cap.set(CV_CAP_PROP_POS_FRAMES, 0);
    }
}

int VideoPlayer::OptimizationMat(Mat& f_img)
{
    return 0;
    //     vector<Rect> faces;
    //  //先检测
    //  StuOpDetect().FaceCheck(f_img,faces);
    //     //再画圆
    //     if (faces.empty()) return 0;
    //     for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++ )
    //  MatDraw().DrawCircle(f_img, *r);    //MatDraw().DrawCircle(f_img,Rect( Point(r->x,r->y),Point(r->width,r->height) ) );
    //  return faces.size();
}



void RecVideo::CamRecToAvi(const char* f_name)
{
    VideoCapture capture(0);
    VideoWriter writer(f_name, CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(640, 480));
    Mat frame;
    while (true)
    {
        capture >> frame;
        writer << frame;
        imshow("video", frame);
        if (cvWaitKey(20) == 27)
            break;
    }
}

void RecVideo::MatAddHDR(const Mat& f_img, const Mat& f_add, Mat& f_out)
{
    if (!f_img.data)  return ;
    if (!f_add.data)  return ;
    //判断尺寸对应
    if ((f_img.cols != f_add.cols) || (f_img.rows != f_add.rows))
        return ;
    double alphaValue = 0.5;
    double betaValue;
    betaValue = (1.0 - alphaValue);
    //融合公式 f_out = alphaValue*f_img+betaValue*f_add;
    addWeighted(f_img, alphaValue, f_add, betaValue, 0.0, f_out);
}

Mat RecVideo::CutScreenToMat(Mat& f_img, Rect f_rt/*=Rect(0,0,0,0) */)
{
    //定义截屏范围此处设为全屏
    int right = GetSystemMetrics(SM_CXSCREEN), bottom = GetSystemMetrics(SM_CYSCREEN);       //屏幕分辨率尺寸
    int left = f_rt.x, top = f_rt.y;        //顶点坐标
    int Xend = f_rt.width, Yend = f_rt.height; //终点坐标
    //下面区域判断
    if (left > right || left < 0) left = 0;
    if (top > bottom || top < 0) top = 0;
    if (Xend <= left || Xend > right) Xend = right;
    if (Yend <= top || Yend > bottom) Yend = bottom;
    //图片的尺寸
    int nWidth = Xend - top, nHeight = Yend - left;
    HDC hSrcDC = NULL, hMemDC = NULL;
    HBITMAP hBitmap = NULL, hOldBitmap = NULL;
    hSrcDC = CreateDCW(L"DISPLAY", NULL, NULL, NULL);
    hMemDC = CreateCompatibleDC(hSrcDC);
    hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight);
    hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, left, top , SRCCOPY);
    hBitmap = (HBITMAP) SelectObject(hMemDC, hOldBitmap);
    BITMAP bmp;
    int nChannels, depth;
    BYTE* pBuffer;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    int f_nchannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8 ;
    int f_depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
    int image_width = bmp.bmWidth;
    int image_height = bmp.bmHeight;
    pBuffer = new BYTE[image_width * image_height * f_nchannels];
    GetBitmapBits(hBitmap, image_height * image_width * f_nchannels, pBuffer);
    f_img = Mat(image_height, image_width, CV_8UC4, pBuffer);
    f_img = f_img.clone();
    //imshow("clone", f_img);
    //waitKey(0);
    delete[] pBuffer;
    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hOldBitmap);
    DeleteDC(hMemDC);
    SelectObject(hSrcDC, hBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);
    return f_img;
}

void RecVideo::AviToPic(const char* f_name, const char* f_Doc, int frameStart, int frameStop)
{
    VideoCapture cap;       //视频处理对象
    Mat fps_curr;
    cap.open(f_name);       //打开视频
    //获取总帧数
    long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
    cout << "整个视频共" << totalFrameNumber << "帧" << endl;
    //默认设置开始位置
    if (frameStart < totalFrameNumber)
        cap.set(CV_CAP_PROP_POS_FRAMES, frameStart);
    else
        cap.set(CV_CAP_PROP_POS_FRAMES, 0);
    //默认设置结束位置
    if (0 == frameStop || frameStart > frameStop)
        frameStop = totalFrameNumber;
    char buf[255] = {0};
    string sbuf;
    for (int i = 1; i < frameStop - frameStart + 1; ++i)
    {
        if (!cap.read(fps_curr))                 //cap>>fps_curr;
            break;
        cout << i << " " << i + frameStart - 1 << endl;
        sprintf(buf, "Avitojpg_%d.jpg", i);
        sbuf = (string) f_Doc + "/" + (string) buf;
        //resize(fps_curr, fps_curr, Size(800, 600));       //仅仅缩放显示
        imwrite(sbuf, fps_curr);
    }
}

void RecVideo::CaptureDemo(void)
{
    VideoCapture cam(0);
    if (!cam.isOpened()) return;
    Mat t_outshow;
    while (true)
    {
        cam >> t_outshow;
        if (t_outshow.empty()) return;
        cvNamedWindow("摄像头", CV_WINDOW_AUTOSIZE);
        imshow("摄像头", t_outshow);
        waitKey(1);
    }
}

void RecVideo::cam_QuckGrayCalc(Mat& f_img, int lMethod)
{
    return;
    //--------------参数说明
    //m_fGray       //当前灰度值
    //m_iGrayMin    //最小灰度值 40
    //m_iGrayMax    //最大灰度值 60
    //m_iExp        //曝光值
    //m_iExpMin     //最小曝光值  10
    //m_iExpMax     //最大曝光值  1000
    //--------------步进倍进综合算法
    //#define AUTOEXPTEST
#ifdef AUTOEXPTEST
    int lNeedChange = 0;
    if (m_fGray < m_iGrayMin)
        lNeedChange = 1; //INC
    else if (m_fGray > m_iGrayMax)
        lNeedChange = -1; //DEC
    //图片灰度计算
    m_fGray = CalcAvgGray(l_FrameInfo);
    //目标AVG
    double lGrayAvg = (m_iGrayMin + m_iGrayMax) / 2.0;
    //计算原理
    if (lMethod == 0)
    {
        //根据差值确定调节倍进
        float lGrayGapRatio = lGrayAvg / (m_fGray < 1 ? 1 : m_fGray);
        int lStepChange = (lGrayGapRatio * m_iExp - m_iExp) / 1.5;
        l_iExpToChange = m_iExp + lStepChange;
    }
    else if (lMethod == 1)
    {
        //步进
        int lStepRatio = 1;
        int lGrayGap = abs(m_fGray - lGrayAvg);
        if (lGrayGap >= 100)
            lStepRatio = 16;
        else if (lGrayGap >= 50 && lGrayGap < 100)
            lStepRatio = 4;
        else if (lGrayGap >= 20 && lGrayGap < 50)
            lStepRatio = 2;
        else lStepRatio = 1;
        int lStepChange = EXP_STEP * lStepRatio;
        l_iExpToChange = m_iExp + lStepChange * lNeedChange;
    }
    if (lNeedChange)
    {
        if (l_iExpToChange > m_iExpMax || l_iExpToChange < m_iExpMin)
        {
            cout << "曝光: stop" << endl;
            return ;
        }
        m_iExp = l_iExpToChange;
        MVC_SetParameter(0, MVCADJ_INTTIME, m_iExp);
        cout << "调整曝光:   " << m_iExp << "灰度值:    " << m_fGray << endl;
    }
    else
        cout << "曝光值平稳在:   " << m_iExp << "灰度值:    " << m_fGray << endl;
#endif
    //--------------发布最后倍进曝光程序
#ifdef  releaseOut
    //图片灰度计算
    m_fGray = CalcAvgGray(l_FrameInfo);
    //调节条件
    if (m_fGray < m_iGrayMin || m_fGray > m_iGrayMax)
    {
        //倍进曝光值 计算方法
        int l_iExpToChange = (m_iGrayMin + m_iGrayMax) * m_iExp / (3.0 * (m_fGray < 1 ? 1 : m_fGray)) + m_iExp / 3;
        if (l_iExpToChange > m_iExpMax || l_iExpToChange < m_iExpMin)  return;
        m_iExp = l_iExpToChange;
        MVC_SetParameter(0, MVCADJ_INTTIME, m_iExp);
    }
#endif
    //--------------自动曝光步进算法测试
#ifdef AUTOEXPTEST1
    m_fGray = CalcAvgGray(l_FrameInfo);
    //目标AVG
    double lGrayAvg = (m_iGrayMin + m_iGrayMax) / 2.0;
    //根据差值确定调节步进
    int lStepRatio = 1;
    int lGrayGap = abs(m_fGray - lGrayAvg);
    if (lGrayGap >= 100)
        lStepRatio = 16;
    else if (lGrayGap >= 50 && lGrayGap < 100)
        lStepRatio = 4;
    else if (lGrayGap >= 20 && lGrayGap < 50)
        lStepRatio = 2;
    else lStepRatio = 1;
    int lStepChange = EXP_STEP * lStepRatio;
    //曝光调节步骤
    if (m_fGray < m_iGrayMin && (m_iExp + lStepChange <= m_iExpMax))
    {
        m_iExp += lStepChange;
        MVC_SetParameter(0, MVCADJ_INTTIME, m_iExp);
    }
    else if (m_fGray > m_iGrayMax && (m_iExp - lStepChange >= m_iExpMin))
    {
        m_iExp -= lStepChange;
        MVC_SetParameter(0, MVCADJ_INTTIME, m_iExp);
    }
#endif
    //--------------自动曝光倍进算法测试
#ifdef AUTOEXPTEST2
    m_fGray = CalcAvgGray(l_FrameInfo);
    //目标AVG
    double lGrayAvg = (m_iGrayMin + m_iGrayMax) / 2.0;
    //根据差值确定调节倍进
    float lGrayGapRatio = lGrayAvg / (m_fGray < 1 ? 1 : m_fGray);
    int lStepChange = (lGrayGapRatio * m_iExp - m_iExp) / 4;
    //曝光调节步骤
    if ((m_fGray < m_iGrayMin && (m_iExp + lStepChange <= m_iExpMax)) || (m_fGray > m_iGrayMax && (m_iExp + lStepChange >= m_iExpMin)))
    {
        m_iExp += lStepChange;
        MVC_SetParameter(0, MVCADJ_INTTIME, m_iExp);
    }
#endif
}

void RecVideo::cam_PIDExposure(Mat& f_img)
{
    return;
#ifdef PIDExposure
    //曝光设置值
    int NowExp;
    //历史灰度
    static float  grayBefore2 = 0;                          //前前帧灰度
    static float  grayBefore = 0;                           //前帧灰度
    grayBefore2 = grayBefore;
    grayBefore = m_fGray;
    m_fGray = CalcAvgGray(l_FrameInfo);                     //平均灰度
    //历史曝光值
    static int ExpBefore = 0;                               //前曝光
    static int ExpBefore2 = 0;                              //前前曝光
    ExpBefore2 = ExpBefore;
    ExpBefore = MVC_GetParameter(cardNo, MVCADJ_INTTIME);
    //灰度差过滤计算
    if (abs(m_fGray - grayBefore) > m_grayDiff)
    {
        ShowLog(m_fGray, grayBefore, grayBefore2, ExpBefore, ExpBefore2, 1111111);
        Sleep(500);
        return ;
    }
    else
    {
        if (m_fGray > m_iGrayMin && m_fGray < m_iGrayMax)
        {
            //合理值范围直接休息
            ShowLog(m_fGray, grayBefore, grayBefore2, ExpBefore, ExpBefore2, 2222222);
            Sleep(1000);
            return;
        }
        else if (m_fGray > (m_iGrayMin - m_lrgray / 2) && m_fGray < (m_iGrayMax + m_lrgray / 2))
        {
            //判断进行微调
            if (m_fGray <= m_graySpace)
                NowExp = ExpBefore + abs(0.4 * (m_fGray - grayBefore) + 0.1 * grayBefore2);
            else
                NowExp = ExpBefore - abs(0.4 * (m_fGray - grayBefore) + 0.1 * grayBefore2);
        }
        else      //快速调节
            NowExp = m_graySpace * ExpBefore / grayBefore;
    }
    //最大最小曝光值限制
    if (NowExp >= m_iExpMax)
        NowExp = m_iExpMax;
    else if (NowExp <= m_iExpMin)
        NowExp = m_iExpMin;
    //设置曝光值
    MVC_SetParameter(0, MVCADJ_INTTIME, NowExp);
    //打印日志
    ShowLog(m_fGray, grayBefore, grayBefore2, ExpBefore, ExpBefore2, NowExp);
#endif
}

void RecVideo::cam_TuningCalc(Mat& f_img)
{
    return;
#ifdef TuningCalc
    //灰度过滤
    m_fGray = CalcAvgGray(l_FrameInfo);
    //当前曝光
    m_iExp = MVC_GetParameter(0, MVCADJ_INTTIME);
    m_iGrayMin = m_graySpace - m_grayDiff;
    m_iGrayMax = m_graySpace + m_grayDiff;
    //目标AVG
    double lGrayAvg = m_graySpace;    //参考灰度 // lGrayAvg =(m_iGrayMin + m_iGrayMax) / 2.0;
    //M0:倍进     //根据差值确定调节倍进
    float lGrayGapRatio = lGrayAvg / (m_fGray < 1 ? 1 : m_fGray);
    int lStepChange = (lGrayGapRatio * m_iExp - m_iExp) / 1.5;
    int l_iExpToChange = m_iExp + lStepChange;
    //曝光调节步骤
    if ((m_fGray < m_iGrayMin) || (m_fGray > m_iGrayMax))
    {
        if (l_iExpToChange > m_iExpMax)
        {
            if (m_fGray <= m_graySpace) return;
            m_iExp = m_iExpMax;
        }
        if (l_iExpToChange < m_iExpMin)
            return ;
        else
            m_iExp = l_iExpToChange;
        MVC_SetParameter(0, MVCADJ_INTTIME, m_iExp);
    }
#endif
}




















