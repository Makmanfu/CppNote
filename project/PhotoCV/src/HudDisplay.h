//////////////////////////////////////////////////////////////////////////
//          hud����ģ��
//                                                      @2015-08-18 ����
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SQ_C_API
    #define SQ_C_API extern "C" __declspec(dllexport)
#endif

//��ӡMat����
void saveXYZtoMat(const char* filename, const Mat& mat);

//hud���ֻ��ƹ�����ͼ�㴦��
class HudPlay
{
private:
    char* buff;         //��ʱ�ļ���ת��ָ��
    int sendnum;        //�ж��ж�ʹ��   ������bug
private:
    //����������Ϣ��ѯ������Ϣ[direction ת�� signPt ��ʶ��]
    void GetHuddata(char* f_name, int& direction, Rect& signPt, vector<Rect>& carPt);
    //��������Ϣ���Ƶ�Mat
    void HudToMat(Mat& f_img, int f_x, int direction, Rect signPt, vector<Rect> carPt);
    //���Ƽ�ͷ
    void Addlogo(Mat& Lane_output, Mat& f_logo, int f_x);
    //������������ת��ΪԲ�������
    void SquareToCircle(Rect rt, Point* pt, int* r);
    //��Բ�Ĳ���ת��Ϊ����
    void CircleToSquare(Point pt, int r, Rect& rt);
    //����Բ��λ
    Mat DrawCircle(Mat& f_img, Rect rt, Scalar f_color = Scalar(0, 255, 0), float num = 0);
    //���ƶ�λ����
    Mat DrawRect(Mat& f_img, Rect rt, Scalar f_color = Scalar(0, 125, 255), float num = 0);
    //��ӡ����(ͼ,��ӡ����,����)
    Mat DrawDistance(Mat& f_img, int f_lnum, int f_rnum, Point f_lpt, Point f_rpt);
    //�����ļ�
    int GetAllFiles(char* vpath, std::vector<WIN32_FIND_DATAA>& vResult);
public:
    //hudͼ�㴦��
    void HudDataUser(Mat& f_img, int f_x, char* f_name);
    //hudͼ�㴦�� ��f_x�е����
    void HudDataUser(Mat& f_img, Point2d A, Point2d B, Point2d C, Point2d D, int xleft, int xright, string f_name);
    //����4����Ϣ��ת����Ϣ���Ƽ�ͷ
    void ArrowFindMat(Mat& f_img, Point2d A, Point2d B, Point2d C, Point2d D, int det, int xleft, int xright);
    //����ͼƬ
    void PicToPlay(char* f_Doc);
public:
    HudPlay() : sendnum(-1)
    {
        buff = NULL;
    };
    ~HudPlay() {};
};

//��3D��ͷ�͵���Ч��
class CDrawArrow
{
private:
    int kk;
    Mat Userout;
public:
    //��ָʾ��ͷ1(f_ro ��Ϊ����ת f_thetaΪ�ڽ�)
    Mat DrawLoopArrow(Mat& f_img, Point f_pt = Point(200, 50), int longl = 80, int shortl = 20, double f_theta = 1.0 / 3, double f_ro = 0, Scalar f_color = Scalar(0, 255, 0));
    //��ָʾ��ͷ2 (f_pt ���ĵ� longl ������ )
    Mat DrawLoopArrow(Mat& f_img, int longl, Point f_pt = Point(0, 0));
    //���͸�ӱ任����
    Point WrapPoint(Point pt, Mat& R);
    //����R0����
    Mat CalcR0(bool ff = true);
    //ͶӰ�任����ͼ[ff:true ˳ת��]
    Mat ToAirscape(const Mat& f_img, Mat& f_out = Mat(), bool ff = true);
    //���Ź켣����
    void TrackArrowShow(Mat& f_img, vector<Point>& pts);
    //���Ź켣��ָʾ��
    void OneArrowShowQuick(Mat& f_img, vector<Point>& pts, Scalar f_color = Scalar(0, 255, 0));
    void OneArrowShowr(Mat& f_img, vector<Point>& pts, Scalar f_color = Scalar(0, 255, 0));
    //ͼ�����
    void MatAddMat(Mat& f_img, const Mat& f_logo, Point f_begin, float ff = 0.5);
    //�ع�y
    int FunKXtoY(Point A, Point B, int x);
    //�ع�x
    int FunKXtoX(Point A, Point B, int y);
    //�����������
    float CalcABl(Point A, Point B);
    //�������kΪ���ϵ��
    void ResizePoint(vector<Point>& f_pts, float f_k = 0.5);
public:
    CDrawArrow() : kk(5)
    {
        Userout = Mat(600, 800, CV_8UC3, Scalar(255, 255, 255));
    };
    //·��ָʾģ��ѭ����ʾ
    void TestLoopArrowShow(const Mat& f_img);
    //�ڸ���ͼ���ж�Ӧ�ĵ�ͶӰ���Ƶ�ͼ��
    void AddArrow(Mat& f_img, vector<Point>& pts);
    //ͼ���ϵĵ�ֱ��ͶӰ��ͼ����
    void NativeAddArrow(Mat& f_img, vector<Point>& pts);
    //ֱ�ߺ���������ѭ��[f_speed:Ϊ�ȷ��ٶ�]
    void NativeAddbendArrow(Mat& f_img, vector<Point>& pts, int f_speed = 5);
    //ֱ�л���[�ɷŵ��߳��п����ٶ�]
    void CraspedodromeArrow(void);
    //ģ�⾲ָ̬ʾ��ͷ
    void DrawStaticArrow(Mat& f_img, vector<Point>& pts);
};

//hudȫ�ֲ��Թ���
class HudTool
{
public:
    //�״��������
    void CheckDrawRect(Mat& f_img, Point f_pt);
    //�״���������ģ��
    void DrawTarget(Mat& img, Point pt, double dDist);
};


SQ_C_API int TestArrowShow(int argc, char** argv);






