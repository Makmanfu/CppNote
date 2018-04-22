//***********************************************************************
//       ͼ�����ʹ��ѧϰ��¼��
//
//***********************************************************************

#ifndef MATUSETOOL_H__
#define MATUSETOOL_H__

//��������
class MatUseTool
{
public:
    MatUseTool(void);
    ~MatUseTool(void);
public:
    //����ʱ���㷨
    void CalcTime(void);
    //���Ʋ���ͼ��ľֲ�������
    void MatLayers(Rect pt);
    //��ӡMat��������
    void saveXYZMat(const char* filename, const Mat& mat);
    //������ʾ����ʹ�� �ϲ������� [f_numΪ���ż���]
    void MatToNumPanel(vector<Mat>& Imglist, Mat& f_out, int f_num = 1);
};

//BMP���
class BMPimg
{
private:
    unsigned char* pBmpBuf;          //����ͼ�����ݵ�ָ��
    int bmpWidth;                    //ͼ��Ŀ�
    int bmpHeight;                   //ͼ��ĸ�
    RGBQUAD* pColorTable;            //��ɫ��ָ��
    int biBitCount;                  //ͼ�����ͣ�ÿ����λ��
public:
    //��ȡͼƬ[bmpName:�������ļ�]
    bool readBmp(char* bmpName);
    //����ͼƬ[bmpName:�������ļ�]
    bool saveBmp(char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable);
    //pBuffer��ͼ
    Mat BufferMat(BYTE** pBuffer, int f_w, int f_h);
    //BMPתMat[HBITMAP&]
    Mat BmpToMat(HBITMAP hBitmap);
    //��BMPͼƬת��unsigned char *data��ȥ
    void BMPtoBuffer(HBITMAP hBitmap, BYTE** data);

    //����
    int TestSavebmp(void);
    int TestBmpMem(void);
    int TestBmpMem2(void);
};

//��ͼѧϰ
class MatDraw
{
public:
    //�򵥻�ͼ��demo
    void SimpleDraw(void);
    //����ͷ
    void DrawArrow(void);
    //��������ֱ���ͷ(f_xy�������εĶ���)
    void DrawTriangle(Point f_xy = Point(200, 100));
    //������ת���ͷ[���,����,�Ƕ�] Point(100,400),100,1.0/6
    void DrawTkArrow(Point f_xy = Point(100, 400), int f_line = 180, double f_theta = 1.0 / 4);
    //������Point(200,200),90,0.5);
    void DrawCamber(Point center = Point(200, 200), int radius = 90, double f_theta = 0.5);
    //����ͷ������װ
    void cvArrow(Mat& f_img, Point pStart, Point pEnd, int len, int alpha, Scalar& color, int thickness = 2, int lineType = CV_AA);
    //Mat�����ο�(x1 ...Ϊ���ر���)
    void DrawRectangle(Mat& f_img, float f_x1, float f_y1, float f_x2, float f_y2, Scalar& color);
    //ˮӡд����
    Mat TextAddMat(Mat& f_img, const char* f_txt, Point f_pt, double f_theta);
    //��ӡ����(ͼ,��ӡ����,����)
    Mat TestNumDraw(Mat& f_img, int f_num, Point f_pt);
    //��ԲԲ�Ķ�λ����Բ(num=0 ��ӡ����)
    Mat DrawCircle(Mat& f_img, Point pt, int r = 10, float num = 0, Scalar f_color = Scalar(0, 0, 255));
    //��Բ���ζ�λ����Բ(num=0 ��ӡ����)
    Mat DrawCircle(Mat& f_img, Rect rt, float num = 0, Scalar f_color = Scalar(0, 0, 255));
    //�����پ��ζ�λ
    Mat DrawRect(Mat& f_img, Rect rt, float num = 0, Scalar f_color = Scalar(0, 125, 255));
    //�����Ҽ�ͷ(pt��β������ w��ͷ���ȸ�Ϊ��,h�߶�)
    Mat DrawLeftArrow(Mat& f_img, Point pt, int w = -100, int h = 30);
    //��ָʾ��ͷ1(f_ro ��Ϊ����ת )
    Mat DrawLoopArrow(Mat& f_img, Point f_pt = Point(200, 50), int longl = 80, int shortl = 20, double f_theta = 1.0 / 3, double f_ro = 0, Scalar f_color = Scalar(0, 255, 0));
    //��ָʾ��ͷ2 (f_pt ���ĵ� longl ������ )
    Mat DrawLoopArrow(Mat& f_img, int longl, Point f_pt = Point(0, 0));
    //���Ź켣����
    void TrackArrowShow(Mat& f_img, vector<Point>& pts);
    //����������
    void DrawCurve(void);
    //+++++++++++++++++++++++++++++++++++++++++++++++

    //Mat kst =imread("1.jpg");
    //vector<Point> pts;
    //pts.push_back( Point(250,50) );
    //pts.push_back( Point(290, 140) );
    //pts.push_back( Point(305, 235) );
    //pts.push_back( Point(365, 305) );
    //pts.push_back( Point(375,400) );
    //CDrawArrow sd;
    //sd.AddArrow(kst,pts);

    /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
    ////---------------��ͼ����----------------------------------------------------
    //Mat img = imread("lena.jpg") , f_out;
    //MatDraw sds;
    ////��������
    //sds.TestNumDraw(img,80,Point(40,40) );
    ////��Ȧ
    //sds.DrawCircle(img,Rect( Point(90,90),Point(120,120) ),18.0 );
    ////���Ҽ�ͷ
    //sds.DrawLeftArrow(img,Point(150,300),-100);
    //sds.DrawLeftArrow(img,Point(300,300),100);
    //imshow("f_out",img);
    //waitKey(0);
    ////----------------------------------------------------------------------------
    ////��ͷ��ʾ
    //MatDraw().LoopArrowShow(Mat());
    ////----------------------------------------------------------------------------
};

//��Ƶ����
class VideoPlayer
{
private:
    VideoCapture cap;           //��Ƶ�������
    Mat fps_curr;               //һ֡����
    bool thetype;               //0������ͷ(Ĭ��),1���ļ�
public:
    //typedef void(*SetMat)(void);                              //����ָ��
public:
    VideoPlayer();                                              //��������ͷ
    VideoPlayer(const char* ImgName);                           //�����ļ�
    virtual ~VideoPlayer();
    void PlayVideo(int frameStart = 0, int frameStop = 0);      //���Ŵ�����Ƶ
    void OnlyPlayVideo(int frameStart = 0, int frameStop = 0);  //��������Ƶ
    bool CheckCapOK(void);                                      //���cap�Ƿ�ɹ�
    long GetframeNum(void);                                     //��ȡ��Ƶ����֡
    void SetframePoint(int f_fpsnum = 0);                       //��λ������֡
    virtual int OptimizationMat(Mat& f_img);                    //����֡����
};

//��Ƶ¼�ƺ�֡����
class RecVideo
{
public:
    //����ͷ��������
    void CaptureDemo(void);
    //����ع����(��׼�Ҷȷ�)
    void cam_QuckGrayCalc(Mat& f_img, int lMethod);
    //���PID�����ع�����㷨
    void cam_PIDExposure(Mat& f_img);
    //��̬��ֵ�Ҷȿ��ٵ����ع��㷨
    void cam_TuningCalc(Mat& f_img);
    //ͼ���ں�HDR
    void MatAddHDR(const Mat& f_img, const Mat& f_add, Mat& f_out);
    //��Ļ��ͼ
    Mat CutScreenToMat(Mat& f_img, Rect f_rt = Rect(0, 0, 0, 0));
    //��������ͷ����
    void CamRecToAvi(const char* f_name = "video.avi");
    //����Ƶ����ļ���ͼƬ[f_DocΪ��ǰ·���µ��ļ�����]
    void AviToPic(const char* f_name, const char* f_Doc, int frameStart = 0, int frameStop = 0);
};

//��Ƶ����������
class CVideoOptimize : public VideoPlayer
{
public:
    CVideoOptimize() : VideoPlayer() {};
    CVideoOptimize(const char* ImgName) : VideoPlayer(ImgName) {};
    ~CVideoOptimize() {};
public:
    int OptimizationMat(Mat& f_img);
};



#endif