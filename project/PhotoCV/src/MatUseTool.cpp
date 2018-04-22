
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
    //�����м��õ��㷨ʱ��
    cout << ((double) cvGetTickCount() - t) / ((double) cvGetTickFrequency() * 1000.) << "ms ";
}

void MatUseTool::MatLayers(Rect pt)
{
    Mat mUser = imread("lena.jpg");
    //��Ĥ��ʹ�÷���
    Mat imageROI = mUser(Rect(pt.x, pt.y, pt.width, pt.height));
    //�ֲ���ת
    flip(imageROI.clone(), imageROI, 1);
    imshow("�ֲ���ת", mUser);
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
            sprintf(szWord, "%.4f,", point);    //���ȵ���
            strcat(szLine, szWord);
        }
        fprintf(fp, "%s\n", szLine);
    }
    fclose(fp);
}

Mat MatAddMat(Mat& InOut_img, const Mat& Inlogo, Point Ptbegin, float ff)
{
    Mat mask;                   //�������Ĥ
    if (-2 == ff)               //png����͸�����Ƿ�ʽ
        cvtColor(Inlogo, mask, CV_BGR2GRAY);
    //ȱ�������ж� ���ܻᱨ��
    Mat imageROI = InOut_img(Rect(Ptbegin.x, Ptbegin.y, Inlogo.cols, Inlogo.rows));
    if (-1 == ff)
        Inlogo.copyTo(imageROI);        //����Ǹ���  ����͸����
    else if (-2 == ff)
        Inlogo.copyTo(imageROI, mask);    //���������Ĥ���� ��͸����
    else
        cv::addWeighted(imageROI, 1.0, Inlogo, ff, 0, imageROI);
    //����� �ںϹ�ʽ f_out = alphaValue*f_img+betaValue*f_add;
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
        �������ƣ�readBmp()
        ����������char *bmpName -�ļ����ּ�·��
        ����ֵ��0Ϊʧ�ܣ�1Ϊ�ɹ�
        ˵���� ����һ��ͼ���ļ�������·������ͼ���λͼ���ݡ����ߡ���ɫ��ÿ����
        λ�������ݽ��ڴ棬�������Ӧ��ȫ�ֱ�����
    ***********************************************************************/
    //�����ƶ���ʽ��ָ����ͼ���ļ�
    FILE* fp = fopen(bmpName, "rb");
    if (fp == 0) return false;
    //����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER
    fseek(fp, sizeof(BITMAPFILEHEADER), 0);
    //����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��
    BITMAPINFOHEADER head;
    fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
    //��ȡͼ����ߡ�ÿ������ռλ������Ϣ
    bmpWidth = head.biWidth;
    bmpHeight = head.biHeight;
    biBitCount = head.biBitCount;
    //�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����
    int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
    //�Ҷ�ͼ������ɫ������ɫ�����Ϊ256
    if (biBitCount == 8)   //������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�
    {
        pColorTable = new RGBQUAD[256];
        fread(pColorTable, sizeof(RGBQUAD), 256, fp);
    }
    //����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�
    pBmpBuf = new unsigned char[lineByte * bmpHeight];
    fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
    //�ر��ļ�
    fclose(fp);
    return true;
}

bool BMPimg::saveBmp(char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable)
{
    /*****************************************
        �������ƣ�saveBmp()
        ����������
         char *bmpName-�ļ����ּ�·��
         unsigned char *imgBuf-�����̵�λͼ����
         int width-������Ϊ��λ������λͼ�Ŀ�
         int height-������Ϊ��λ������λͼ��
         int biBitCount-ÿ������ռλ��
         RGBQUAD *pColorTable-��ɫ��ָ��
        ����ֵ��0Ϊʧ�ܣ�1Ϊ�ɹ�
        ˵��������һ��ͼ��λͼ���ݡ����ߡ���ɫ��ָ�뼰ÿ������ռ��λ������Ϣ��
           ����д��ָ���ļ���
    ***********************************************************************/
    //���λͼ����ָ��Ϊ0����û�����ݴ��룬��������
    if (!imgBuf)
        return 0;
    //��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0
    int colorTablesize = 0;
    if (biBitCount == 8)
        colorTablesize = 1024;
    //���洢ͼ������ÿ���ֽ���Ϊ4�ı���
    int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
    //�Զ�����д�ķ�ʽ���ļ�
    FILE* fp = fopen(bmpName, "wb");
    if (fp == 0) return 0;
    //����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ
    BITMAPFILEHEADER fileHead;
    fileHead.bfType = 0x4D42;//bmp����
    //bfSize��ͼ���ļ�4����ɲ���֮��
    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte * height;
    fileHead.bfReserved1 = 0;
    fileHead.bfReserved2 = 0;
    //bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��
    fileHead.bfOffBits = 54 + colorTablesize;
    //д�ļ�ͷ���ļ�
    fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
    //����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ
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
    //дλͼ��Ϣͷ���ڴ�
    fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);
    //����Ҷ�ͼ������ɫ��д���ļ�
    if (biBitCount == 8)
        fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);
    //дλͼ���ݽ��ļ�
    fwrite(imgBuf, height * lineByte, 1, fp);
    //�ر��ļ�
    fclose(fp);
    return 1;
}

Mat BMPimg::BufferMat(BYTE** pBuffer, int f_w, int f_h)
{
    //ע����pBufferһ�¿�ָ��
    Mat f_img = Mat(f_h, f_w, CV_8UC4, *pBuffer);
    return f_img;
}

cv::Mat BMPimg::BmpToMat(HBITMAP hBitmap)
{
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    //�������
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
    //�������
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
    //����ָ��BMP�ļ����ڴ�
    char readPath[] = "lena.bmp";
    readBmp(readPath);
    //���ͼ�����Ϣ
    printf("width=%d,height=%d, biBitCount=%d\n", bmpWidth, bmpHeight, biBitCount);
    //��ͼ�����ݴ���
    char writePath[] = "lenacpy1.BMP";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
    //�����������pBmpBuf��pColorTable��ȫ�ֱ��������ļ�����ʱ����Ŀռ�
    delete[]pBmpBuf;
    if (biBitCount == 8)
        delete[]pColorTable;
    return 0;
}

int BMPimg::TestBmpMem(void)
{
    //����ָ��BMP�ļ����ڴ�
    char readPath[] = "lena.bmp";
    readBmp(readPath);
    //���ͼ�����Ϣ
    printf("width=%d,height=%d,biBitCount=%d\n", bmpWidth, bmpHeight, biBitCount);
    //ѭ��������ͼ�������
    int i, j;
    //ÿ���ֽ���
    int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
    //ѭ����������Բ�ɫͼ�񣬱���ÿ���ص���������
    int k;
    //��ͼ�����½�1/4�����óɺ�ɫ
    if (biBitCount == 8)            //���ڻҶ�ͼ��
    {
        for (i = 0; i < bmpHeight / 2; i++)
            for (j = 0; j < bmpWidth / 2; j++)
                * (pBmpBuf + i * lineByte + j) = 0;
    }
    else if (biBitCount == 24)      //��ɫͼ��
    {
        for (i = 0; i < bmpHeight / 2; i++)
            for (j = 0; j < bmpWidth / 2; j++)
                for (k = 0; k < 3; k++)   //ÿ����RGB���������ֱ���0�ű�ɺ�ɫ
                    * (pBmpBuf + i * lineByte + j * 3 + k) = 0;
    }
    //��ͼ�����ݴ���
    char writePath[] = "lenacpy2.BMP";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
    //�����������pBmpBuf��pColorTable��ȫ�ֱ��������ļ�����ʱ����Ŀռ�
    delete[]pBmpBuf;
    if (biBitCount == 8)
        delete[]pColorTable;
    return 0;
}

int BMPimg::TestBmpMem2(void)
{
    //����ָ��BMP�ļ����ڴ�
    char readPath[] = "lena.bmp";
    readBmp(readPath);
    //���ͼ�����Ϣ
    printf("width=%d,height=%d,biBitCount=%d\n", bmpWidth, bmpHeight, biBitCount);
    //�ı�Ҷ�ͼ�����ɫ����ɫ������ֵ���鿴ǰ��仯
    if (biBitCount == 8)
        for (int i = 0; i < 256; i++)
            pColorTable[i].rgbBlue = 255 - pColorTable[i].rgbBlue;
    //��ͼ�����ݴ���
    char writePath[] = "lenacpy3.BMP";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
    //�����������pBmpBuf��pColorTable��ȫ�ֱ���,���ļ�����ʱ����Ŀռ�
    delete[]pBmpBuf;
    if (biBitCount == 8)
        delete[]pColorTable;
    return 0;
}

void MatDraw::SimpleDraw(void)
{
    Mat t_Img = Mat(512, 512, CV_8UC3, Scalar(255, 255, 255));
    /* Բ */
    //Բ��
    Point center = Point(255, 255);
    //�뾶
    int r = 100;
    //����ͼ��    //Mat picture(512,512,CV_8UC3,Scalar(255,255,255));
    //����Ϊ�����ص�ͼ��Բ�ġ��뾶����ɫ����ϸ������ ��ϸΪ-1ʱ��ʵ��Բ
    circle(t_Img, center, r, Scalar(0, 255, 0), -1);
    /* ��Բ */
    //����Ϊ�����ص�ͼ��Բ�ġ������ᡢ����нǣ�ˮƽ�浽����ļнǣ�����ʼ�Ƕȣ����ᵽ��ʼ���صļнǣ��������Ƕȣ����ᵽ������ļнǣ�����б�ľ��Σ���ѡ�����ɫ����ϸ�����ԡ�ƫ��
    ellipse(t_Img, center, Size(250, 100), 0, 30, 240, Scalar(0, 0, 0));
    /* ���� */
    Point a = Point(60, 60);
    //����Ϊ�����ص�ͼ����ʼ�㡢�����㡢��ɫ����ϸ������
    line(t_Img, a, center, Scalar(255, 0, 0));
    /* ������  */
    //����Ϊ�����ص�ͼ�񡢶��㡢�Խǵ㡢��ɫ����������ɫ������ϸ����С
    rectangle(t_Img, a, center, Scalar(255, 0, 0));
    /* ��������  */
    //����Ϊ�����ص�ͼƬ����������֣����ֵ�λ�ã��ı������½ǣ������壬��С����ɫ
    string words = "good luck";
    putText(t_Img, words, Point(t_Img.rows / 2, t_Img.cols / 4), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0));
    imshow("��ͼDemo", t_Img);
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
    imshow("����ͷ", t_img);
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
    imshow("��������ֱ���ͷ", t_img);
    waitKey();
}

void MatDraw::DrawTkArrow(Point f_xy, int f_line, double f_theta)
{
    const double PI = 3.1415926;
    Mat t_img = Mat(512, 512, CV_8UC3, Scalar(255, 255, 255));
    int x0 = f_xy.x,  y0 = f_xy.y;
    int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, cx, cy;
    int m_l, n_l, k_l;  //����x0��һ��, cx��һ��,cx��c7����
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
    imshow("������ת���ͷ", t_img);
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
    imshow("������", t_img);
    waitKey();
}

void MatDraw::cvArrow(Mat& f_img, Point pStart, Point pEnd, int len, int alpha, Scalar& color, int thickness /*= 2*/, int lineType /*= CV_AA*/)
{
    //����˵��
    //pStart        ���
    //pEnd          �յ�
    //len             ��ͷǰ�ĳ���
    //alpha         ��ͷ���ſ���
    //color          ��ɫ
    //thickness   ��ϸ
    //lineType     �ߵ�����
    const double PI = 3.1415926;
    Point arrow;
    //���� �� �ǣ���򵥵�һ�����������ͼʾ���Ѿ�չʾ���ؼ����� atan2 ��������������棩
    double angle = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));
    line(f_img, pStart, pEnd, color, thickness, lineType);
    //������Ǳߵ���һ�˵Ķ˵�λ�ã�����Ļ��������Ҫ����ͷ��ָ��Ҳ����pStart��pEnd��λ�ã�
    arrow.x = pEnd.x + len * cos(angle + PI * alpha / 180);
    arrow.y = pEnd.y + len * sin(angle + PI * alpha / 180);
    line(f_img, pEnd, arrow, color, thickness, lineType);
    arrow.x = pEnd.x + len * cos(angle - PI * alpha / 180);
    arrow.y = pEnd.y + len * sin(angle - PI * alpha / 180);
    line(f_img, pEnd, arrow, color, thickness, lineType);
}

void MatDraw::DrawRectangle(Mat& f_img, float f_x1, float f_y1, float f_x2, float f_y2, Scalar& color)
{
    //����Ϊ�����ص�ͼ�񡢶��㡢�Խǵ㡢��ɫ����������ɫ������ϸ����С
    if (!f_img.data)  return;
    Point f_begin(f_img.cols * f_x1, f_img.rows * f_y1);
    Point f_end(f_img.cols * f_x2, f_img.rows * f_y2);
    rectangle(f_img, f_begin, f_end, color, 4);
}

Mat MatDraw::TextAddMat(Mat& f_img, const char* f_txt, Point f_pt, double f_theta)
{
    if (0 == f_theta)       //ֱ�ӻ���ȥ
    {   //������д���ı���
        cv::putText(f_img, f_txt, f_pt, CV_FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 2);
    }
    else                    //��͸��Ч��
    {
        //�ı���
        Mat logo(26, 200, CV_8UC3, Scalar(0, 0, 0));
        //������д���ı���
        cv::putText(logo, f_txt, Point(2, 18), CV_FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 2);
        //�ںϵ�������(ע������Խ��)
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
    //����
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
    //����ͷ����
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
    //��β������ �˴���Ҫ�ú�˼��
    pts.push_back(Point(pts.rbegin()->x, pts.rbegin()->y + 1));
    //�ο���
    //line(f_img, Point(0, f_img.rows / 2), Point(f_img.cols, f_img.rows / 2), Scalar(255, 255, 255), 1);
    //line(f_img, Point(f_img.cols / 2,0), Point(f_img.cols/2, f_img.rows ), Scalar(255, 255, 255), 1);
    //���Ź켣���Ʋ���תͼ��
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
            //line(f_img, t_py, *it, Scalar(0, 0, 255), 1);       //����
            DrawLoopArrow(f_img, t_py, 40, 15, 1.0 / 3, atan((double)(it->x - t_py.x) / (it->y - t_py.y)) / 3.1415926);
            t_py = *it;
        }
        //���һ�������ж�
        //DrawLoopArrow(f_img,*it,40,15,1.0/3,atan( (double)(it->x-t_py.x)/(it->y-t_py.y) ) /3.14159 );
    }
}

void MatDraw::DrawCurve(void)
{
    Mat PanelBox = Mat(800, 800, CV_8UC3, Scalar(0, 0, 0));
    //��������ϵ
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
    cap = VideoCapture(0);      //��Ĭ�ϵ�����ͷ
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
        SetframePoint(frameStart);                      //Ĭ�����ÿ�ʼλ��
        if (0 == frameStop || frameStart > frameStop)   //Ĭ�����ý���λ��
            frameStop = GetframeNum();
        for (int i = 1; i < frameStop - frameStart + 1; ++i)
        {
            if (!cap.read(fps_curr))                 //cap>>fps_curr;
                break;
            cout << i << " " << i + frameStart - 1 << endl;
            k = OptimizationMat(fps_curr);              //ͼ����
            resize(fps_curr, fps_curr, Size(800, 600));       //����������ʾ
            imshow("������Ƶ", fps_curr);
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
            cvNamedWindow("����ͷ", CV_WINDOW_AUTOSIZE);
            k = OptimizationMat(fps_curr);                    //ͼ����
            imshow("����ͷ", fps_curr);
            waitKey(1);
        }
    }
}

void VideoPlayer::OnlyPlayVideo(int frameStart /*= 0*/, int frameStop /*= 0*/)
{
    if (!CheckCapOK()) return;
    if (thetype)
    {
        SetframePoint(frameStart);                          //Ĭ�����ÿ�ʼλ��
        if (0 == frameStop || frameStart > frameStop)       //Ĭ�����ý���λ��
            frameStop = GetframeNum();
        double rate = cap.get(CV_CAP_PROP_FPS);             //��ȡ֡��
        int delay = 1000 / rate;                            //��֡��ļ��ʱ��:
        for (int i = 1; i < frameStop - frameStart + 1; ++i)
        {
            if (!cap.read(fps_curr))                 //cap>>fps_curr;
                break;
            cout << i << " " << i + frameStart - 1 << endl;
            resize(fps_curr, fps_curr, Size(800, 600));       //����������ʾ
            imshow("������Ƶ", fps_curr);
            waitKey(delay);
        }
    }
    else
    {
        while (true)
        {
            cap >> fps_curr;
            if (fps_curr.empty())   return;
            cvNamedWindow("����ͷ", CV_WINDOW_AUTOSIZE);
            imshow("����ͷ", fps_curr);
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
        //��ȡ֡��
        long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
        //��ȡ֡��
        //double rate = capture.get(CV_CAP_PROP_FPS);
        cout << "������Ƶ��" << totalFrameNumber << "֡" << endl;
        return totalFrameNumber;
    }
    else
    {
        return 0;       //����ͷ�Ͳ�����
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
    //  //�ȼ��
    //  StuOpDetect().FaceCheck(f_img,faces);
    //     //�ٻ�Բ
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
    //�жϳߴ��Ӧ
    if ((f_img.cols != f_add.cols) || (f_img.rows != f_add.rows))
        return ;
    double alphaValue = 0.5;
    double betaValue;
    betaValue = (1.0 - alphaValue);
    //�ںϹ�ʽ f_out = alphaValue*f_img+betaValue*f_add;
    addWeighted(f_img, alphaValue, f_add, betaValue, 0.0, f_out);
}

Mat RecVideo::CutScreenToMat(Mat& f_img, Rect f_rt/*=Rect(0,0,0,0) */)
{
    //���������Χ�˴���Ϊȫ��
    int right = GetSystemMetrics(SM_CXSCREEN), bottom = GetSystemMetrics(SM_CYSCREEN);       //��Ļ�ֱ��ʳߴ�
    int left = f_rt.x, top = f_rt.y;        //��������
    int Xend = f_rt.width, Yend = f_rt.height; //�յ�����
    //���������ж�
    if (left > right || left < 0) left = 0;
    if (top > bottom || top < 0) top = 0;
    if (Xend <= left || Xend > right) Xend = right;
    if (Yend <= top || Yend > bottom) Yend = bottom;
    //ͼƬ�ĳߴ�
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
    VideoCapture cap;       //��Ƶ�������
    Mat fps_curr;
    cap.open(f_name);       //����Ƶ
    //��ȡ��֡��
    long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
    cout << "������Ƶ��" << totalFrameNumber << "֡" << endl;
    //Ĭ�����ÿ�ʼλ��
    if (frameStart < totalFrameNumber)
        cap.set(CV_CAP_PROP_POS_FRAMES, frameStart);
    else
        cap.set(CV_CAP_PROP_POS_FRAMES, 0);
    //Ĭ�����ý���λ��
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
        //resize(fps_curr, fps_curr, Size(800, 600));       //����������ʾ
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
        cvNamedWindow("����ͷ", CV_WINDOW_AUTOSIZE);
        imshow("����ͷ", t_outshow);
        waitKey(1);
    }
}

void RecVideo::cam_QuckGrayCalc(Mat& f_img, int lMethod)
{
    return;
    //--------------����˵��
    //m_fGray       //��ǰ�Ҷ�ֵ
    //m_iGrayMin    //��С�Ҷ�ֵ 40
    //m_iGrayMax    //���Ҷ�ֵ 60
    //m_iExp        //�ع�ֵ
    //m_iExpMin     //��С�ع�ֵ  10
    //m_iExpMax     //����ع�ֵ  1000
    //--------------���������ۺ��㷨
    //#define AUTOEXPTEST
#ifdef AUTOEXPTEST
    int lNeedChange = 0;
    if (m_fGray < m_iGrayMin)
        lNeedChange = 1; //INC
    else if (m_fGray > m_iGrayMax)
        lNeedChange = -1; //DEC
    //ͼƬ�Ҷȼ���
    m_fGray = CalcAvgGray(l_FrameInfo);
    //Ŀ��AVG
    double lGrayAvg = (m_iGrayMin + m_iGrayMax) / 2.0;
    //����ԭ��
    if (lMethod == 0)
    {
        //���ݲ�ֵȷ�����ڱ���
        float lGrayGapRatio = lGrayAvg / (m_fGray < 1 ? 1 : m_fGray);
        int lStepChange = (lGrayGapRatio * m_iExp - m_iExp) / 1.5;
        l_iExpToChange = m_iExp + lStepChange;
    }
    else if (lMethod == 1)
    {
        //����
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
            cout << "�ع�: stop" << endl;
            return ;
        }
        m_iExp = l_iExpToChange;
        MVC_SetParameter(0, MVCADJ_INTTIME, m_iExp);
        cout << "�����ع�:   " << m_iExp << "�Ҷ�ֵ:    " << m_fGray << endl;
    }
    else
        cout << "�ع�ֵƽ����:   " << m_iExp << "�Ҷ�ֵ:    " << m_fGray << endl;
#endif
    //--------------������󱶽��ع����
#ifdef  releaseOut
    //ͼƬ�Ҷȼ���
    m_fGray = CalcAvgGray(l_FrameInfo);
    //��������
    if (m_fGray < m_iGrayMin || m_fGray > m_iGrayMax)
    {
        //�����ع�ֵ ���㷽��
        int l_iExpToChange = (m_iGrayMin + m_iGrayMax) * m_iExp / (3.0 * (m_fGray < 1 ? 1 : m_fGray)) + m_iExp / 3;
        if (l_iExpToChange > m_iExpMax || l_iExpToChange < m_iExpMin)  return;
        m_iExp = l_iExpToChange;
        MVC_SetParameter(0, MVCADJ_INTTIME, m_iExp);
    }
#endif
    //--------------�Զ��عⲽ���㷨����
#ifdef AUTOEXPTEST1
    m_fGray = CalcAvgGray(l_FrameInfo);
    //Ŀ��AVG
    double lGrayAvg = (m_iGrayMin + m_iGrayMax) / 2.0;
    //���ݲ�ֵȷ�����ڲ���
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
    //�ع���ڲ���
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
    //--------------�Զ��عⱶ���㷨����
#ifdef AUTOEXPTEST2
    m_fGray = CalcAvgGray(l_FrameInfo);
    //Ŀ��AVG
    double lGrayAvg = (m_iGrayMin + m_iGrayMax) / 2.0;
    //���ݲ�ֵȷ�����ڱ���
    float lGrayGapRatio = lGrayAvg / (m_fGray < 1 ? 1 : m_fGray);
    int lStepChange = (lGrayGapRatio * m_iExp - m_iExp) / 4;
    //�ع���ڲ���
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
    //�ع�����ֵ
    int NowExp;
    //��ʷ�Ҷ�
    static float  grayBefore2 = 0;                          //ǰǰ֡�Ҷ�
    static float  grayBefore = 0;                           //ǰ֡�Ҷ�
    grayBefore2 = grayBefore;
    grayBefore = m_fGray;
    m_fGray = CalcAvgGray(l_FrameInfo);                     //ƽ���Ҷ�
    //��ʷ�ع�ֵ
    static int ExpBefore = 0;                               //ǰ�ع�
    static int ExpBefore2 = 0;                              //ǰǰ�ع�
    ExpBefore2 = ExpBefore;
    ExpBefore = MVC_GetParameter(cardNo, MVCADJ_INTTIME);
    //�ҶȲ���˼���
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
            //����ֵ��Χֱ����Ϣ
            ShowLog(m_fGray, grayBefore, grayBefore2, ExpBefore, ExpBefore2, 2222222);
            Sleep(1000);
            return;
        }
        else if (m_fGray > (m_iGrayMin - m_lrgray / 2) && m_fGray < (m_iGrayMax + m_lrgray / 2))
        {
            //�жϽ���΢��
            if (m_fGray <= m_graySpace)
                NowExp = ExpBefore + abs(0.4 * (m_fGray - grayBefore) + 0.1 * grayBefore2);
            else
                NowExp = ExpBefore - abs(0.4 * (m_fGray - grayBefore) + 0.1 * grayBefore2);
        }
        else      //���ٵ���
            NowExp = m_graySpace * ExpBefore / grayBefore;
    }
    //�����С�ع�ֵ����
    if (NowExp >= m_iExpMax)
        NowExp = m_iExpMax;
    else if (NowExp <= m_iExpMin)
        NowExp = m_iExpMin;
    //�����ع�ֵ
    MVC_SetParameter(0, MVCADJ_INTTIME, NowExp);
    //��ӡ��־
    ShowLog(m_fGray, grayBefore, grayBefore2, ExpBefore, ExpBefore2, NowExp);
#endif
}

void RecVideo::cam_TuningCalc(Mat& f_img)
{
    return;
#ifdef TuningCalc
    //�Ҷȹ���
    m_fGray = CalcAvgGray(l_FrameInfo);
    //��ǰ�ع�
    m_iExp = MVC_GetParameter(0, MVCADJ_INTTIME);
    m_iGrayMin = m_graySpace - m_grayDiff;
    m_iGrayMax = m_graySpace + m_grayDiff;
    //Ŀ��AVG
    double lGrayAvg = m_graySpace;    //�ο��Ҷ� // lGrayAvg =(m_iGrayMin + m_iGrayMax) / 2.0;
    //M0:����     //���ݲ�ֵȷ�����ڱ���
    float lGrayGapRatio = lGrayAvg / (m_fGray < 1 ? 1 : m_fGray);
    int lStepChange = (lGrayGapRatio * m_iExp - m_iExp) / 1.5;
    int l_iExpToChange = m_iExp + lStepChange;
    //�ع���ڲ���
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




















