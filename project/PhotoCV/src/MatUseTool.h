//***********************************************************************
//       图像基本使用学习记录集
//
//***********************************************************************

#ifndef MATUSETOOL_H__
#define MATUSETOOL_H__

//辅助工具
class MatUseTool
{
public:
    MatUseTool(void);
    ~MatUseTool(void);
public:
    //计算时间算法
    void CalcTime(void);
    //类似操作图层的局部处理方法
    void MatLayers(Rect pt);
    //打印Mat矩阵数据
    void saveXYZMat(const char* filename, const Mat& mat);
    //多屏显示测试使用 合并到网格 [f_num为横着几个]
    void MatToNumPanel(vector<Mat>& Imglist, Mat& f_out, int f_num = 1);
};

//BMP相关
class BMPimg
{
private:
    unsigned char* pBmpBuf;          //读入图像数据的指针
    int bmpWidth;                    //图像的宽
    int bmpHeight;                   //图像的高
    RGBQUAD* pColorTable;            //颜色表指针
    int biBitCount;                  //图像类型，每像素位数
public:
    //读取图片[bmpName:二进制文件]
    bool readBmp(char* bmpName);
    //保存图片[bmpName:二进制文件]
    bool saveBmp(char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable);
    //pBuffer存图
    Mat BufferMat(BYTE** pBuffer, int f_w, int f_h);
    //BMP转Mat[HBITMAP&]
    Mat BmpToMat(HBITMAP hBitmap);
    //将BMP图片转到unsigned char *data里去
    void BMPtoBuffer(HBITMAP hBitmap, BYTE** data);

    //测试
    int TestSavebmp(void);
    int TestBmpMem(void);
    int TestBmpMem2(void);
};

//画图学习
class MatDraw
{
public:
    //简单画图形demo
    void SimpleDraw(void);
    //画箭头
    void DrawArrow(void);
    //画三角形直向箭头(f_xy是三角形的顶点)
    void DrawTriangle(Point f_xy = Point(200, 100));
    //画任务转向箭头[起点,长度,角度] Point(100,400),100,1.0/6
    void DrawTkArrow(Point f_xy = Point(100, 400), int f_line = 180, double f_theta = 1.0 / 4);
    //画弧线Point(200,200),90,0.5);
    void DrawCamber(Point center = Point(200, 200), int radius = 90, double f_theta = 0.5);
    //画箭头函数封装
    void cvArrow(Mat& f_img, Point pStart, Point pEnd, int len, int alpha, Scalar& color, int thickness = 2, int lineType = CV_AA);
    //Mat画矩形框(x1 ...为像素比例)
    void DrawRectangle(Mat& f_img, float f_x1, float f_y1, float f_x2, float f_y2, Scalar& color);
    //水印写文字
    Mat TextAddMat(Mat& f_img, const char* f_txt, Point f_pt, double f_theta);
    //打印数据(图,打印数字,坐标)
    Mat TestNumDraw(Mat& f_img, int f_num, Point f_pt);
    //画圆圆心定位坐标圆(num=0 打印坐标)
    Mat DrawCircle(Mat& f_img, Point pt, int r = 10, float num = 0, Scalar f_color = Scalar(0, 0, 255));
    //画圆矩形定位坐标圆(num=0 打印坐标)
    Mat DrawCircle(Mat& f_img, Rect rt, float num = 0, Scalar f_color = Scalar(0, 0, 255));
    //画跟踪矩形定位
    Mat DrawRect(Mat& f_img, Rect rt, float num = 0, Scalar f_color = Scalar(0, 125, 255));
    //画左右箭头(pt结尾点坐标 w箭头长度负为左,h高度)
    Mat DrawLeftArrow(Mat& f_img, Point pt, int w = -100, int h = 30);
    //画指示箭头1(f_ro 正为左旋转 )
    Mat DrawLoopArrow(Mat& f_img, Point f_pt = Point(200, 50), int longl = 80, int shortl = 20, double f_theta = 1.0 / 3, double f_ro = 0, Scalar f_color = Scalar(0, 255, 0));
    //画指示箭头2 (f_pt 中心点 longl 长距离 )
    Mat DrawLoopArrow(Mat& f_img, int longl, Point f_pt = Point(0, 0));
    //沿着轨迹绘制
    void TrackArrowShow(Mat& f_img, vector<Point>& pts);
    //贝塞尔曲线
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
    ////---------------画图测试----------------------------------------------------
    //Mat img = imread("lena.jpg") , f_out;
    //MatDraw sds;
    ////画多少米
    //sds.TestNumDraw(img,80,Point(40,40) );
    ////画圈
    //sds.DrawCircle(img,Rect( Point(90,90),Point(120,120) ),18.0 );
    ////左右箭头
    //sds.DrawLeftArrow(img,Point(150,300),-100);
    //sds.DrawLeftArrow(img,Point(300,300),100);
    //imshow("f_out",img);
    //waitKey(0);
    ////----------------------------------------------------------------------------
    ////箭头演示
    //MatDraw().LoopArrowShow(Mat());
    ////----------------------------------------------------------------------------
};

//视频播放
class VideoPlayer
{
private:
    VideoCapture cap;           //视频处理对象
    Mat fps_curr;               //一帧画面
    bool thetype;               //0是摄像头(默认),1是文件
public:
    //typedef void(*SetMat)(void);                              //函数指针
public:
    VideoPlayer();                                              //构造摄像头
    VideoPlayer(const char* ImgName);                           //构造文件
    virtual ~VideoPlayer();
    void PlayVideo(int frameStart = 0, int frameStop = 0);      //播放处理视频
    void OnlyPlayVideo(int frameStart = 0, int frameStop = 0);  //仅播放视频
    bool CheckCapOK(void);                                      //检查cap是否成功
    long GetframeNum(void);                                     //获取视频多少帧
    void SetframePoint(int f_fpsnum = 0);                       //定位到多少帧
    virtual int OptimizationMat(Mat& f_img);                    //处理帧画面
};

//视频录制和帧处理
class RecVideo
{
public:
    //摄像头操作测试
    void CaptureDemo(void);
    //相机曝光调节(基准灰度法)
    void cam_QuckGrayCalc(Mat& f_img, int lMethod);
    //相机PID快速曝光调节算法
    void cam_PIDExposure(Mat& f_img);
    //动态均值灰度快速调整曝光算法
    void cam_TuningCalc(Mat& f_img);
    //图像融合HDR
    void MatAddHDR(const Mat& f_img, const Mat& f_add, Mat& f_out);
    //屏幕截图
    Mat CutScreenToMat(Mat& f_img, Rect f_rt = Rect(0, 0, 0, 0));
    //保存摄像头数据
    void CamRecToAvi(const char* f_name = "video.avi");
    //将视频变成文件夹图片[f_Doc为当前路径下的文件夹名]
    void AviToPic(const char* f_name, const char* f_Doc, int frameStart = 0, int frameStop = 0);
};

//视频操作处理框架
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