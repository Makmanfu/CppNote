//////////////////////////////////////////////////////////////////////////
//          hud绘制模块
//                                                      @2015-08-18 阿甘
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SQ_C_API
    #define SQ_C_API extern "C" __declspec(dllexport)
#endif

//打印Mat数据
void saveXYZtoMat(const char* filename, const Mat& mat);

//hud各种绘制工作和图层处理
class HudPlay
{
private:
    char* buff;         //临时文件名转换指针
    int sendnum;        //判断切断使用   有严重bug
private:
    //根据名字信息查询绘制信息[direction 转向 signPt 标识牌]
    void GetHuddata(char* f_name, int& direction, Rect& signPt, vector<Rect>& carPt);
    //将绘制信息绘制到Mat
    void HudToMat(Mat& f_img, int f_x, int direction, Rect signPt, vector<Rect> carPt);
    //绘制箭头
    void Addlogo(Mat& Lane_output, Mat& f_logo, int f_x);
    //将正方形坐标转换为圆区域参数
    void SquareToCircle(Rect rt, Point* pt, int* r);
    //将圆的参数转换为矩形
    void CircleToSquare(Point pt, int r, Rect& rt);
    //绘制圆定位
    Mat DrawCircle(Mat& f_img, Rect rt, Scalar f_color = Scalar(0, 255, 0), float num = 0);
    //绘制定位矩形
    Mat DrawRect(Mat& f_img, Rect rt, Scalar f_color = Scalar(0, 125, 255), float num = 0);
    //打印数据(图,打印数字,坐标)
    Mat DrawDistance(Mat& f_img, int f_lnum, int f_rnum, Point f_lpt, Point f_rpt);
    //搜索文件
    int GetAllFiles(char* vpath, std::vector<WIN32_FIND_DATAA>& vResult);
public:
    //hud图层处理
    void HudDataUser(Mat& f_img, int f_x, char* f_name);
    //hud图层处理 带f_x中点计算
    void HudDataUser(Mat& f_img, Point2d A, Point2d B, Point2d C, Point2d D, int xleft, int xright, string f_name);
    //根据4点信息和转向信息绘制箭头
    void ArrowFindMat(Mat& f_img, Point2d A, Point2d B, Point2d C, Point2d D, int det, int xleft, int xright);
    //播放图片
    void PicToPlay(char* f_Doc);
public:
    HudPlay() : sendnum(-1)
    {
        buff = NULL;
    };
    ~HudPlay() {};
};

//仿3D箭头和叠加效果
class CDrawArrow
{
private:
    int kk;
    Mat Userout;
public:
    //画指示箭头1(f_ro 正为左旋转 f_theta为内角)
    Mat DrawLoopArrow(Mat& f_img, Point f_pt = Point(200, 50), int longl = 80, int shortl = 20, double f_theta = 1.0 / 3, double f_ro = 0, Scalar f_color = Scalar(0, 255, 0));
    //画指示箭头2 (f_pt 中心点 longl 长距离 )
    Mat DrawLoopArrow(Mat& f_img, int longl, Point f_pt = Point(0, 0));
    //点的透视变换计算
    Point WrapPoint(Point pt, Mat& R);
    //计算R0矩阵
    Mat CalcR0(bool ff = true);
    //投影变换俯视图[ff:true 顺转换]
    Mat ToAirscape(const Mat& f_img, Mat& f_out = Mat(), bool ff = true);
    //沿着轨迹绘制
    void TrackArrowShow(Mat& f_img, vector<Point>& pts);
    //沿着轨迹画指示符
    void OneArrowShowQuick(Mat& f_img, vector<Point>& pts, Scalar f_color = Scalar(0, 255, 0));
    void OneArrowShowr(Mat& f_img, vector<Point>& pts, Scalar f_color = Scalar(0, 255, 0));
    //图像相加
    void MatAddMat(Mat& f_img, const Mat& f_logo, Point f_begin, float ff = 0.5);
    //回归y
    int FunKXtoY(Point A, Point B, int x);
    //回归x
    int FunKXtoX(Point A, Point B, int y);
    //计算两点距离
    float CalcABl(Point A, Point B);
    //点的缩放k为相乘系数
    void ResizePoint(vector<Point>& f_pts, float f_k = 0.5);
public:
    CDrawArrow() : kk(5)
    {
        Userout = Mat(600, 800, CV_8UC3, Scalar(255, 255, 255));
    };
    //路面指示模拟循环演示
    void TestLoopArrowShow(const Mat& f_img);
    //在俯视图像中对应的点投影绘制到图像
    void AddArrow(Mat& f_img, vector<Point>& pts);
    //图像上的点直接投影到图像中
    void NativeAddArrow(Mat& f_img, vector<Point>& pts);
    //直线和弯曲绘制循环[f_speed:为等分速度]
    void NativeAddbendArrow(Mat& f_img, vector<Point>& pts, int f_speed = 5);
    //直行绘制[可放到线程中控制速度]
    void CraspedodromeArrow(void);
    //模拟静态指示箭头
    void DrawStaticArrow(Mat& f_img, vector<Point>& pts);
};

//hud全局测试工具
class HudTool
{
public:
    //雷达检测框推算
    void CheckDrawRect(Mat& f_img, Point f_pt);
    //雷达推算框计算模拟
    void DrawTarget(Mat& img, Point pt, double dDist);
};


SQ_C_API int TestArrowShow(int argc, char** argv);






