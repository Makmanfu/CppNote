
#include "stdafx.h"
#include "HudDisplay.h"
#include <io.h>
#ifdef _VIDEO_CHECK_DEMO_
    #include "../Lib/lineDet.hpp"
    #pragma comment(lib,"../lib/Lane.lib")
#endif

string ITS(int f_txt)
{
    char buf[255] = {0};
    sprintf(buf, "%d", f_txt);
    return buf;
}

void saveXYZtoMat(const char* filename, const Mat& mat)
{
    char szLine[20000] = {0};
    char szWord[100] = {0};
    FILE* fp = fopen(filename, "wt");
    for (int x = 0; x < mat.rows; x++)
    {
        strcpy(szLine, "");
        for (int y = 0; y < mat.cols; y++)
        {
            double point = cvGetReal2D(&CvMat(mat), x, y);
            sprintf(szWord, "%.7f,", point);
            strcat(szLine, szWord);
        }
        fprintf(fp, "%s\n", szLine);
    }
    fclose(fp);
}

void HudPlay::GetHuddata(char* f_name, int& direction, Rect& signPt, vector<Rect>& carPt)
{
    //获得转向
    direction = GetPrivateProfileIntA(f_name, "direction", 0, "./data.ini");
    //标识牌
    signPt.x = GetPrivateProfileIntA(f_name, "signPtx", 0, "./data.ini");
    signPt.y = GetPrivateProfileIntA(f_name, "signPty", 0, "./data.ini");
    signPt.width = GetPrivateProfileIntA(f_name, "signPtex", 0, "./data.ini");
    signPt.height = GetPrivateProfileIntA(f_name, "signPtey", 0, "./data.ini");
    //车辆检测
    int carNum = GetPrivateProfileIntA(f_name, "carNum", 0, "./data.ini");
    if (carNum == 0)
        return;
    Rect tmpCar;
    for (int i = 1; i <= carNum; ++i)
    {
        tmpCar.x = GetPrivateProfileIntA(f_name, string("carPtx" + ITS(i)).c_str(), 0, "./data.ini");
        tmpCar.y = GetPrivateProfileIntA(f_name, string("carPty" + ITS(i)).c_str(), 0, "./data.ini");
        tmpCar.width = GetPrivateProfileIntA(f_name, string("carPtex" + ITS(i)).c_str(), 0, "./data.ini");
        tmpCar.height = GetPrivateProfileIntA(f_name, string("carPtey" + ITS(i)).c_str(), 0, "./data.ini");
        carPt.push_back(tmpCar);
    }
}

void HudPlay::HudToMat(Mat& f_img, int f_x, int direction, Rect signPt, vector<Rect> carPt)
{
    Mat logo;
    //计算绘制箭头
    switch (direction)
    {
        case 0:     //直走
            logo = cv::imread("00.png");
            break;
        case 1:     //左转90
            logo = cv::imread("90.png");
            flip(logo, logo, 1);
            break;
        case 2:     //左转60
            logo = cv::imread("60.png");
            flip(logo, logo, 1);
            break;
        case 3:     //左转30
            logo = cv::imread("30.png");
            flip(logo, logo, 1);
            break;
        case 4:     //右转30
            logo = cv::imread("30.png");
            break;
        case 5:     //右转60
            logo = cv::imread("60.png");
            break;
        case 6:     //右转90
            logo = cv::imread("90.png");
            break;
    }
    //叠加图像
    Addlogo(f_img, logo, f_x);
    //绘制标识牌
    DrawCircle(f_img, signPt);          //DrawRect   DrawCircle
    //绘制车辆
    if (carPt.empty())  return ;
    for (vector<Rect>::const_iterator it = carPt.begin(); it != carPt.end(); ++it)
        DrawRect(f_img, *it);
}

void HudPlay::Addlogo(Mat& Lane_output, Mat& f_logo, int f_x)
{
    cv::Mat imageROI = Lane_output(cv::Rect(f_x, 680, f_logo.cols, f_logo.rows));       //imageROI = Lane_output(cv::Rect(650,600,f_logo.cols,f_logo.rows));
    cv::addWeighted(imageROI, 1.0, f_logo, 0.2, 0, imageROI);
}

Mat HudPlay::DrawCircle(Mat& f_img, Rect rt, Scalar f_color /*= Scalar(0, 0, 255)*/, float num /*= 0*/)
{
    int r = rt.width / 2;
    Point pt = Point(rt.width / 2 + rt.x, rt.height / 2 + rt.y);
    if (r < 10) r = 10;
    int thickness = 2, r_l = 3 * r / 2;
    circle(f_img, pt, r, f_color, thickness);
    line(f_img, Point(pt.x - r_l, pt.y), Point(pt.x - r, pt.y), f_color, thickness);
    line(f_img, Point(pt.x + r_l, pt.y), Point(pt.x + r, pt.y), f_color, thickness);
    line(f_img, Point(pt.x, pt.y - r_l), Point(pt.x, pt.y - r), f_color, thickness);
    line(f_img, Point(pt.x, pt.y + r_l), Point(pt.x, pt.y + r), f_color, thickness);
    char buf[255] = { 0 };
    if (0 == num)
    {
        sprintf(buf, "(%d,%d)", pt.x, pt.y);
        cv::putText(f_img, buf, Point(pt.x + r, pt.y - r), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    else
    {
        sprintf(buf, "%.1fm", num);            //    sprintf(buf,"%s : %d",f_name,f_num);
        cv::putText(f_img, buf, Point(pt.x + r, pt.y - r), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    return f_img;
}

Mat HudPlay::DrawRect(Mat& f_img, Rect rt, Scalar f_color /*= Scalar(0, 125, 255)*/, float num /*= 0*/)
{
    rectangle(f_img, Point(rt.x, rt.y), Point(rt.x + rt.width, rt.y + rt.height), f_color, 2);
    char buf[255] = { 0 };
    if (0 == num)
    {
        sprintf(buf, "(%d,%d)", rt.x, rt.y);
        cv::putText(f_img, buf, Point(rt.x, rt.y + rt.height + 20), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    else
    {
        sprintf(buf, "%.1fm", num);
        cv::putText(f_img, buf, Point(rt.x, rt.y + rt.height + 20), CV_FONT_HERSHEY_DUPLEX, 0.5, f_color, 1.2);
    }
    return f_img;
}

void HudPlay::HudDataUser(Mat& f_img, int f_x, char* f_name)
{
    int direction;          //转向
    Rect signPt;
    vector<Rect> carPt;
    GetHuddata(f_name, direction, signPt, carPt);
    HudToMat(f_img, f_x, direction, signPt, carPt);
}

void HudPlay::HudDataUser(Mat& f_img, Point2d A, Point2d B, Point2d C, Point2d D, int xleft, int xright, string f_name)
{
    //绘制数字跟随
    DrawDistance(f_img, xleft , xright, A, B);
    int direction;          //转向
    Rect signPt;
    vector<Rect> carPt;
    //绘制箭头坐标
    int f_x = abs(A.x - B.x) / 2;
    //从文件读取所以绘制信息
    GetHuddata(const_cast<char*>(f_name.c_str()), direction, signPt, carPt);
    //绘制信息
    HudToMat(f_img, f_x, direction, signPt, carPt);
}

Mat HudPlay::DrawDistance(Mat& f_img, int f_lnum, int f_rnum, Point f_lpt, Point f_rpt)
{
    char lbuf[5] = {0}, rbuf[5] = {0};
    sprintf(lbuf, "%dmm", f_lnum);
    sprintf(rbuf, "%dmm", f_rnum);
    cv::putText(f_img, lbuf, f_lpt, CV_FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 1.7);
    cv::putText(f_img, rbuf, f_rpt, CV_FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 1.7);
    return f_img;
}

int HudPlay::GetAllFiles(char* vpath, std::vector<WIN32_FIND_DATAA>& vResult)
{
    int nCount = 0;
    BOOL bRet = FALSE;
    HANDLE hfd;
    WIN32_FIND_DATAA wfd;
    char cdir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, cdir);
    SetCurrentDirectoryA(vpath);
    hfd = FindFirstFileA("*.*", &wfd);
    if (INVALID_HANDLE_VALUE != hfd)
    {
        do
        {
            if (FILE_ATTRIBUTE_DIRECTORY != (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                ++nCount;
                vResult.push_back(wfd);
            }
            bRet = FindNextFileA(hfd, &wfd);
        }
        while (FALSE != bRet);
    }
    SetCurrentDirectoryA(cdir);
    return (nCount);
}

void HudPlay::ArrowFindMat(Mat& f_img, Point2d A, Point2d B, Point2d C, Point2d D, int det, int xleft, int xright)
{
    //绘制箭头坐标
    int f_x = abs(A.x - B.x) / 2;
    //绘制数字跟随
    DrawDistance(f_img, xleft , xright, A, B);    //A B是下面的两个点
    Mat logo;
    switch (det)
    {
        case 0:     //直走
            logo = cv::imread("00.png");
            break;
        case 1:     //左转90
            logo = cv::imread("90.png");
            flip(logo, logo, 1);
            break;
        case 2:     //左转60
            logo = cv::imread("60.png");
            flip(logo, logo, 1);
            break;
        case 3:     //左转30
            logo = cv::imread("30.png");
            flip(logo, logo, 1);
            break;
        case 4:     //右转30
            logo = cv::imread("30.png");
            break;
        case 5:     //右转60
            logo = cv::imread("60.png");
            break;
        case 6:     //右转90
            logo = cv::imread("90.png");
            break;
    }
    Addlogo(f_img, logo, f_x);
}

void HudPlay::PicToPlay(char* f_Doc)
{
    //将文件夹里的所有jpg读取到vector
    std::vector<WIN32_FIND_DATAA> files;        //文件名列表
    std::vector<string> filesNames;             //带绝对路径的文件列表
    int nFileCount = GetAllFiles(f_Doc, files), f_x(0);
    for (std::vector<WIN32_FIND_DATAA>::iterator it = files.begin(); it != files.end(); ++it)
        filesNames.push_back(std::string(it->cFileName));
    Mat fps_curr(1200, 1600, CV_8UC3), Roadimage(960, 480, CV_8UC3);;
    int ffff = 0;
    for (std::vector<string>::iterator itfile = filesNames.begin(); itfile != filesNames.end(); ++itfile)
    {
        cout << ffff++ << endl;
        //判断跳转
        if (-1 != sendnum)   //if (strcmp(buff,"")!=0)
        {
            sprintf(buff, "%d", sendnum);
            std::vector<string>::iterator Itser;
            Itser = find(filesNames.begin(), filesNames.end(), buff);
            if (Itser != filesNames.end())
                itfile = Itser;         //得到迭代器
            sendnum = -1;               //还原
            return ;
        }
        //读图
        fps_curr = imread(string(f_Doc) + *itfile);
        //处理车道线 并计算f_x
#ifdef _VIDEO_CHECK_DEMO_
        stCameraInfo Lane_Message;
        Lanedetect(fps_curr, Roadimage, fps_curr, Lane_Message);
        Point PointL(Lane_Message.LeftDownX + 10, Lane_Message.LeftDownY);
        Point PointR(Lane_Message.RightDownX + 10, Lane_Message.RightDownY);
        DrawDistance(fps_curr, abs(Lane_Message.LeftLineDistance), abs(Lane_Message.RightLineDistance), PointL, PointR);
        f_x = (Lane_Message.LeftUpX + Lane_Message.RightUpX) / 2 - 115;
        if (f_x < 0) f_x = 0;
#else
        f_x = 660;
#endif
        //绘制处理信息
        HudDataUser(fps_curr, f_x, const_cast<char*>(itfile->c_str()));
        //缩放尺寸
        resize(fps_curr, fps_curr, Size(800, 600));       //仅仅缩放显示
        imshow("播放文件夹", fps_curr);
        waitKey(200);       //100
    }
}

void HudPlay::SquareToCircle(Rect rt, Point* pt, int* r)
{
    pt->x = rt.width / 2 + rt.x;
    pt->y = rt.height / 2 + rt.y;
    *r = rt.width / 2;
}

void HudPlay::CircleToSquare(Point pt, int r, Rect& rt)
{
    rt = Rect(pt.x - r, pt.y - r, r, r);
}

Mat CDrawArrow::DrawLoopArrow(Mat& f_img, Point f_pt/*=Point(200,50)*/, int longl/*=80*/, int shortl/*=20*/, double f_theta/*=1.0/3*/, double f_ro/*=0*/, Scalar f_color/*=Scalar(0,255,0)*/)
{
    const double PI = 3.1415926;
    double theta = f_theta * PI, ro = f_ro * PI;    //f_ro 正为左旋转 单位乘以PI
    int xk = sin(ro) * shortl, yk = cos(ro) * shortl;
    int x1(f_pt.x), y1(f_pt.y), x2(f_pt.x + xk), y2(f_pt.y + yk);
    int xl(x1 - sin(theta - ro) *longl) , yl(y1 + cos(theta - ro) *longl), xr(x1 + sin(theta + ro) *longl) , yr(y1 + cos(theta + ro) *longl);
    int xl2(xl + xk), yl2(yl + yk), xr2(xr + xk), yr2(yr + yk);
    //画实心
    Point PointArray[4] = {Point(0, 0), Point(0, 0), Point(0, 0), Point(0, 0) };
    PointArray[0] = Point(x1, y1);
    PointArray[1] = Point(x2, y2);
    PointArray[2] = Point(xl2, yl2);
    PointArray[3] = Point(xl, yl);
    cv::fillConvexPoly(f_img, PointArray, 4, Scalar(0, 255, 0));
    PointArray[0] = Point(x1, y1);
    PointArray[1] = Point(x2, y2);
    PointArray[2] = Point(xr2, yr2);
    PointArray[3] = Point(xr, yr);
    cv::fillConvexPoly(f_img, PointArray, 4, Scalar(0, 255, 0));
    return f_img;
    //Point PointArray[6]={Point(0,0),Point(0,0),Point(0,0),Point(0,0),Point(0,0),Point(0,0)};
    //PointArray[0] = Point(xl,yl);
    //PointArray[1] = Point(x1,y1);
    //PointArray[2] = Point(xr,yr);
    //PointArray[3] = Point(xr2,yr2);
    //PointArray[4] = Point(x2,y2);
    //PointArray[5] = Point(xl2,yl2);
    //cv::fillConvexPoly(f_img,PointArray,6,Scalar(0,255,0) );
    //return f_img;
    //仿画实心
    for (int k = 0; k < shortl; k++)
    {
        line(f_img, Point(x1, y1 + k), Point(xl, yl + k), f_color, 2, 16);
        line(f_img, Point(x1, y1 + k), Point(xr, yr + k), f_color, 2, 16);
    }
    return f_img;
    //下面画框架
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

Mat CDrawArrow::DrawLoopArrow(Mat& f_img, int longl, Point f_pt/*=Point(0,0)*/)
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

Point CDrawArrow::WrapPoint(Point pt, Mat& R)
{
    double mm[3] = {pt.x, pt.y, 1};
    Mat mmm(3, 1, R.type(), mm);
    Mat nn = R * mmm;           //计算
    nn = nn / nn.at<double> (2, 0);
    return cvPoint(nn.at<double> (0, 0), nn.at<double> (1, 0));
}

Mat CDrawArrow::CalcR0(bool ff)
{
    //16001200
    //Point2f src_vertices[4];
    //src_vertices[0] =   Point(621, 629);                 //Point(312, 312);
    //src_vertices[1] = Point(1107,629);                  //Point(551,312);
    //src_vertices[2] = Point(545,709);                   //Point(272,351);
    //src_vertices[3] = Point(1265,709);                  //Point(631,351);
    //Point2f dst_vertices[4];
    //dst_vertices[0] =   Point(545, 1100);                //Point(272, 550);
    //dst_vertices[1] =   Point(1265,1100);                 //Point(631,550);
    //dst_vertices[2] = Point(545,1600);                     //Point(272,820);
    //dst_vertices[3] = Point(1265,1600);                  //Point(631,820);
    //800600
    Point2f src_vertices[4];
    src_vertices[0] =   Point(312, 315);
    src_vertices[1] =   Point(554, 315);
    src_vertices[2] =   Point(273, 355);
    src_vertices[3] =   Point(633, 355);
    Point2f dst_vertices[4];
    dst_vertices[0] = Point(273, 550);
    dst_vertices[1] = Point(633, 550);
    dst_vertices[2] =   Point(273, 800);
    dst_vertices[3] =   Point(633, 800);
    Mat R0;
    if (ff)
        R0 = getPerspectiveTransform(src_vertices, dst_vertices);
    else
        R0 = getPerspectiveTransform(dst_vertices, src_vertices);
    return R0;
    //再计算
    //return WrapPoint(pt,R0);
}

Mat CDrawArrow::ToAirscape(const Mat& f_img, Mat& f_out/*=Mat()*/, bool ff/*=true */)
{
    if (f_out.empty())
        f_out = Mat(ff ? f_img.rows * 2 : f_img.rows / 2, f_img.cols, CV_8UC3, Scalar(255, 255, 255));
    cvWarpPerspective(&CvMat(f_img), &CvMat(f_out), &CvMat(CalcR0(ff)));
    return f_out;
}

void CDrawArrow::TrackArrowShow(Mat& f_img, vector<Point>& pts)
{
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
            DrawLoopArrow(f_img, t_py, 40, 15, 1.0 / 3, it->y == t_py.y ? (it->x > t_py.x ? 1.0 / 2 : -1.0 / 2) :
                          (atan((double)(it->x - t_py.x) / (it->y - t_py.y)) / 3.1415926));
            t_py = *it;
        }
    }
}

void CDrawArrow::OneArrowShowQuick(Mat& f_img, vector<Point>& pts, Scalar f_color)
{
    Point PointArray[4] = {Point(0, 0), Point(0, 0), Point(0, 0), Point(0, 0) };
    int rlinel = 15, fristh = 180, linell = 50, s_linewide = 4, l_linewide = 12; //fristh三角形的高 linell三角形的宽
    if (pts[0].y > 600)
        rlinel = 10, fristh = 80, linell = 30, s_linewide = 2, l_linewide = 4;
    Point t_old(0, 0), t_A(0, 0), t_B(0, 0);
    for (vector<Point>::const_iterator it = pts.begin() ; it != pts.end(); ++it)
    {
        if (it == pts.begin())
        {
            t_old = *it;
            continue;
        }
        else
        {
            if (it == pts.begin() + 1)
            {
                //求偏移值的t_old
                PointArray[3] = Point(t_old.x + (it->x - t_old.x) * fristh / CalcABl(t_old, *it), t_old.y + (it->y - t_old.y) * fristh / CalcABl(t_old, *it));
                PointArray[0] = t_old;
                PointArray[1] = Point(PointArray[3].x - (t_old.y - it->y) * linell / CalcABl(t_old, *it), PointArray[3].y + (t_old.x - it->x) * linell / CalcABl(t_old, *it));
                PointArray[2] = Point(PointArray[3].x + (t_old.y - it->y) * linell / CalcABl(t_old, *it), PointArray[3].y - (t_old.x - it->x) * linell / CalcABl(t_old, *it));
                cv::fillConvexPoly(f_img, PointArray, 3, f_color, l_linewide);
                t_A = Point(PointArray[3].x - (t_old.y - it->y) * rlinel / CalcABl(t_old, *it), PointArray[3].y + (t_old.x - it->x) * rlinel / CalcABl(t_old, *it));
                t_B = Point(PointArray[3].x + (t_old.y - it->y) * rlinel / CalcABl(t_old, *it), PointArray[3].y - (t_old.x - it->x) * rlinel / CalcABl(t_old, *it));
            }
            PointArray[0] = t_A;
            PointArray[1] = t_B;
            t_B = PointArray[2] = Point(it->x + (t_old.y - it->y) * rlinel / CalcABl(t_old, *it), it->y - (t_old.x - it->x) * rlinel / CalcABl(t_old, *it));
            t_A = PointArray[3] = Point(it->x - (t_old.y - it->y) * rlinel / CalcABl(t_old, *it), it->y + (t_old.x - it->x) * rlinel / CalcABl(t_old, *it));
            cv::fillConvexPoly(f_img, PointArray, 4, f_color, l_linewide);
            t_old = *it;
        }
    }
    GaussianBlur(f_img, f_img, Size(3, 3), 1.5);       //滤波处理
    //Point PointArray[4]={Point(0,0),Point(0,0),Point(0,0),Point(0,0)};
    //int rlinel = 15, fristh=180, linell=50,s_linewide=4,l_linewide=12;    //fristh三角形的高 linell三角形的宽
    //if (pts[0].y>600)
    //  rlinel = 10, fristh=80, linell=30,s_linewide=2,l_linewide=4;
    //Point t_old(0,0);
    //vector<Point> ptsL,ptsR;
    //for (vector<Point>::const_iterator it=pts.begin() ;it!=pts.end();++it)
    //{
    //  if (it == pts.begin())
    //  {
    //      t_old = *it;
    //      continue;
    //  }else{
    //      if (it==pts.begin()+1)
    //      {   //求偏移的t_old
    //          Point tt_old( t_old.x+(it->x-t_old.x)*fristh/CalcABl(t_old,*it), t_old.y+(it->y-t_old.y)*fristh/CalcABl(t_old,*it) );
    //          Point Aleft(tt_old.x- (t_old.y-it->y)*linell/CalcABl(t_old,*it),tt_old.y+(t_old.x-it->x)*linell/CalcABl(t_old,*it) );
    //          Point Bright(tt_old.x+ (t_old.y-it->y)*linell/CalcABl(t_old,*it),tt_old.y-(t_old.x-it->x)*linell/CalcABl(t_old,*it) );
    //          //line(f_img,Aleft,t_old,Scalar(0,255,0),l_linewide);
    //          //line(f_img,Bright,t_old,Scalar(0,255,0),l_linewide);
    //          //line(f_img,Aleft,Bright,Scalar(0,255,0),l_linewide);
    //          PointArray[0] = t_old;
    //          PointArray[1] = Aleft;
    //          PointArray[2] = Bright;
    //         cv::fillConvexPoly(f_img,PointArray,3,Scalar(0,255,0),l_linewide);
    //          ptsL.push_back(Point(tt_old.x- (t_old.y-it->y)*rlinel/CalcABl(t_old,*it),tt_old.y+(t_old.x-it->x)*rlinel/CalcABl(t_old,*it) ) );
    //          ptsR.push_back(Point(tt_old.x+ (t_old.y-it->y)*rlinel/CalcABl(t_old,*it),tt_old.y-(t_old.x-it->x)*rlinel/CalcABl(t_old,*it) ) );
    //      } else {
    //          ptsL.push_back(Point(t_old.x- (t_old.y-it->y)*rlinel/CalcABl(t_old,*it),t_old.y+(t_old.x-it->x)*rlinel/CalcABl(t_old,*it) ) );
    //          ptsR.push_back(Point(t_old.x+ (t_old.y-it->y)*rlinel/CalcABl(t_old,*it),t_old.y-(t_old.x-it->x)*rlinel/CalcABl(t_old,*it) ) );
    //      }
    //      //连线
    //      //line(f_img,*it,t_old,Scalar(0,255,0),s_linewide);
    //      //模拟计算最后一个点
    //      if (it==pts.end()-1)
    //      {
    //          ptsL.push_back(Point(it->x- (t_old.y-it->y)*rlinel/CalcABl(t_old,*it),it->y+(t_old.x-it->x)*rlinel/CalcABl(t_old,*it) ) );
    //          ptsR.push_back(Point(it->x+ (t_old.y-it->y)*rlinel/CalcABl(t_old,*it),it->y-(t_old.x-it->x)*rlinel/CalcABl(t_old,*it) ) );
    //          //line(f_img,Point(it->x- abs(t_old.y-it->y)*rlinel/CalcABl(t_old,*it),it->y+abs(t_old.x-it->x)*rlinel/CalcABl(t_old,*it) ),
    //          //  Point(it->x+ abs(t_old.y-it->y)*rlinel/CalcABl(t_old,*it),it->y-abs(t_old.x-it->x)*rlinel/CalcABl(t_old,*it) ),Scalar(0,255,0),s_linewide);
    //      }
    //      t_old = *it;
    //  }
    //}
    //Point L_old,R_old;
    //vector<Point>::const_iterator itL=ptsL.begin();
    //vector<Point>::const_iterator itR=ptsR.begin();
    //for ( ;itL!=ptsL.end();++itL,++itR)
    //{
    //  if (itL == ptsL.begin())
    //  {
    //   L_old = *itL;
    //  R_old = *itR;
    //  //line(f_img,L_old,R_old,Scalar(0,255,0),s_linewide);
    //  continue;
    // }else{
    //  //line(f_img,*itL,L_old,Scalar(0,255,0),s_linewide);
    //  //line(f_img,*itR,R_old,Scalar(0,255,0),s_linewide);
    //  PointArray[0] = *itL;
    //  PointArray[1] = L_old;
    //  PointArray[2] = R_old;
    //  PointArray[3] = *itR;
    //  cv::fillConvexPoly(f_img,PointArray,4,Scalar(0,255,0),l_linewide);
    //  L_old = *itL;
    //  R_old = *itR;
    // }
    //}
    ////line(f_img,L_old,R_old,Scalar(0,255,0),s_linewide);
}

void CDrawArrow::OneArrowShowr(Mat& f_img, vector<Point>& pts, Scalar f_color)
{
    Point PointArray[4] = {Point(0, 0), Point(0, 0), Point(0, 0), Point(0, 0) };
    int rlinel = 15, fristh = 180, linell = 50, s_linewide = 4, l_linewide = 12; //fristh三角形的高 linell三角形的宽
    if (pts[pts.size() - 1].y > 600)
        rlinel = 10, fristh = 80, linell = 30, s_linewide = 2, l_linewide = 4;
    Point t_old(0, 0), t_A(0, 0), t_B(0, 0);
    for (vector<Point>::const_reverse_iterator it = pts.rbegin() ; it != pts.rend(); ++it)
    {
        if (it == pts.rbegin())
        {
            t_old = *it;
            continue;
        }
        else
        {
            if (it == pts.rbegin() + 1)
            {
                PointArray[3] = Point(t_old.x + (it->x - t_old.x) * fristh / CalcABl(t_old, *it), t_old.y + (it->y - t_old.y) * fristh / CalcABl(t_old, *it));
                PointArray[0] = t_old;
                PointArray[1] = Point(PointArray[3].x - (t_old.y - it->y) * linell / CalcABl(t_old, *it), PointArray[3].y + (t_old.x - it->x) * linell / CalcABl(t_old, *it));
                PointArray[2] = Point(PointArray[3].x + (t_old.y - it->y) * linell / CalcABl(t_old, *it), PointArray[3].y - (t_old.x - it->x) * linell / CalcABl(t_old, *it));
                fillConvexPoly(f_img, PointArray, 3, f_color, l_linewide);
                t_A = Point(PointArray[3].x - (t_old.y - it->y) * rlinel / CalcABl(t_old, *it), PointArray[3].y + (t_old.x - it->x) * rlinel / CalcABl(t_old, *it));
                t_B = Point(PointArray[3].x + (t_old.y - it->y) * rlinel / CalcABl(t_old, *it), PointArray[3].y - (t_old.x - it->x) * rlinel / CalcABl(t_old, *it));
            }
            PointArray[0] = t_A;
            PointArray[1] = t_B;
            t_B = PointArray[2] = Point(it->x + (t_old.y - it->y) * rlinel / CalcABl(t_old, *it), it->y - (t_old.x - it->x) * rlinel / CalcABl(t_old, *it));
            t_A = PointArray[3] = Point(it->x - (t_old.y - it->y) * rlinel / CalcABl(t_old, *it), it->y + (t_old.x - it->x) * rlinel / CalcABl(t_old, *it));
            fillConvexPoly(f_img, PointArray, 4, f_color, l_linewide);
            t_old = *it;
        }
    }
    GaussianBlur(f_img, f_img, Size(3, 3), 1.5);       //滤波处理
}

void CDrawArrow::MatAddMat(Mat& f_img, const Mat& f_logo, Point f_begin, float ff /*= 0.5*/)
{
    //f_img += f_logo;
    Mat imageROI = f_img(Rect(f_begin.x, f_begin.y, f_logo.cols, f_logo.rows));
    cv::addWeighted(imageROI, 1.0, f_logo, ff, 0, imageROI);
}

int CDrawArrow::FunKXtoY(Point A, Point B, int x)
{
    //double k=(double)(A.y-B.y)/(A.x-B.x);
    //int b = A.y - k*A.x;
    //return k*x+b;
    return A.x == B.x ? A.y : abs((A.y - B.y) / (A.x - B.x) * (x - A.x) + A.y);
}

int CDrawArrow::FunKXtoX(Point A, Point B, int y)
{
    //double k=(double)(A.y-B.y)/(A.x-B.x);
    //int b = A.y - k*A.x;
    //return (y-b)/k;
    return A.y == B.y ? A.x : abs((A.x - B.x) * (y - A.y) / (A.y - B.y) + A.x);
}

float CDrawArrow::CalcABl(Point A, Point B)
{
    return sqrt(float ((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y)));
}

void CDrawArrow::ResizePoint(vector<Point>& f_pts, float f_k/*=0.5*/)
{
    //......加个过滤K的合法性
    for (vector<Point>::iterator it = f_pts.begin(); it != f_pts.end(); ++it)
        *it = Point(it->x * f_k, it->y * f_k);
}

void CDrawArrow::TestLoopArrowShow(const Mat& f_img)
{
    int img_h(f_img.rows), img_w(f_img.cols);
    Mat img = Mat(img_h * 2, img_w, CV_8UC3, Scalar(0, 0, 0)), t_img, bgimg, loopOut;
    for (; ;)
        for (int j = 0; j <= 50; j += 2)   //50是间距
        {
            img.copyTo(t_img);
            f_img.copyTo(bgimg);                //t_img = f_img.clone();
            for (int i = 0; i < 16; i++)        //绘制个数
                ToAirscape(DrawLoopArrow(t_img, Point(400, 80 * i + j - 50), 40, 15), loopOut, false);          //绘制坐标
            //cv::GaussianBlur(loopOut, loop2, Size(3, 3), 1.5);       //滤波处理
            MatAddMat(bgimg, loopOut, Point(0, 0), 1);
            imshow("原图", t_img);
            imshow("逆投影变换图", loopOut);
            imshow("最后图", bgimg);
            waitKey(1);
        }
    //======================================
    //Mat testImg3 = imread("../DllRes/IMG/girl.jpg");
    //resize(testImg3, testImg3, Size(800, 600));
    //CDrawArrow().TestLoopArrowShow(testImg3);
}

void CDrawArrow::AddArrow(Mat& f_img, vector<Point>& pts)
{
    if (pts.empty()) return ;
    //1-先确定俯视图的尺寸
    Mat loopshow = Mat(f_img.rows * 2, f_img.cols , CV_8UC3, Scalar(0, 0, 0)), loop2;
    TrackArrowShow(loopshow, pts);                       //绘制坐标
    ToAirscape(loopshow, loop2, false);                  //转换坐标
    //GaussianBlur(loop2, loop2, Size(3, 3), 1.5);       //滤波处理
    MatAddMat(f_img, loop2, Point(0, 0), 1);
    imshow("绘制坐标(俯视图)", loopshow);
    imshow("转换坐标(还原原图图层)", loop2);
    imshow("最后图(叠加效果)", f_img);
    waitKey(0);
    //------模拟点效果------------------------------------------------------------
    //Mat kst =imread("lena.jpg");
    //resize( kst,kst,Size(800,600) );
    //vector<Point> pts;
    //pts.push_back( Point(250,50) );
    //pts.push_back( Point(290, 140) );
    //pts.push_back( Point(305, 235) );
    //pts.push_back( Point(365, 305) );
    //pts.push_back( Point(375,400) );
    //CDrawArrow sd;
    //sd.AddArrow(kst,pts);
}

void CDrawArrow::NativeAddArrow(Mat& f_img, vector<Point>& pts)
{
    if (pts.empty()) return ;
    //1-原图点换算到俯视图中
    vector<Point> t_pts(pts);
    vector<Point>::iterator t_it = t_pts.begin();
    for (vector<Point>::const_iterator it = pts.begin(); it != pts.end(); ++it, ++t_it)
        *t_it = WrapPoint(*it, CalcR0(true));       //计算转换映射点
    //2-在俯视图处理图像
    AddArrow(f_img, t_pts);
}

void CDrawArrow::NativeAddbendArrow(Mat& f_img, vector<Point>& pts, int f_speed/*=0*/)
{
    if (pts.empty()) return ;
    //转换到800对应位置
    ResizePoint(pts);
    //计算到俯视图点
    vector<Point> t_pts(pts), pts_new;
    //俯视图转换
    //vector<Point>::iterator t_it = t_pts.begin();
    //for (vector<Point>::const_iterator it=pts.begin();it!=pts.end();++it,++t_it)
    // *t_it = WrapPoint(*it,drawA.CalcR0(true));       //计算转换映射点
    Mat  img = Mat(1200, 800, CV_8UC3, Scalar(0, 0, 0)), loopOut, t_img, bgimg;
    if (f_speed < 0) f_speed = 3;
    for (;;)
    {
        for (int i = 0; i <= f_speed; i++)
        {
            double t = (double) cvGetTickCount();
            Point t_old(0, 0);
            img.copyTo(t_img);          //  t_img = img.clone();        //俯视图
            f_img.copyTo(bgimg);        //  bgimg =f_img.clone();       //这里放背景图
            pts_new.clear();
            for (vector<Point>::const_iterator it2 = t_pts.begin(); it2 != t_pts.end(); ++it2)
            {
                if (it2 == t_pts.begin())
                {
                    t_old = *it2;
                    continue;
                }
                else        //求出推算点
                {
                    pts_new.push_back(it2->y == t_old.y ? Point(t_old.x + abs(it2->x - t_old.x) *i / f_speed, t_old.y) : Point(
                                          FunKXtoX(t_old, *it2, t_old.y + abs(it2->y - t_old.y) *i / f_speed), t_old.y + abs(it2->y - t_old.y) *i / f_speed));
                    t_old = *it2;
                }
            }
            TrackArrowShow(t_img, pts_new);
            ToAirscape(t_img, Userout, false);
            resize(Userout, loopOut, Size(1600, 1200));
            MatAddMat(bgimg, loopOut, Point(0, 0), 1);
            Mat tmp(bgimg);
            resize(tmp, tmp, Size(800, 600));
            imshow("tmp", tmp);
            waitKey(1);
            t = (double) cvGetTickCount() - t;
            printf("detection time = %g ms\n", t / ((double) cvGetTickFrequency() * 1000.));
        }
    }
    return ;
    //=======================================================================
    //模拟转弯
    //vector<Point> w_pts;
    //w_pts.push_back( Point(600,0) );
    //w_pts.push_back( Point(650,500) );
    //w_pts.push_back( Point(700,1000) );
    //w_pts.push_back( Point(750,1500) );
    //w_pts.push_back( Point(850,2000) );
    //w_pts.push_back( Point(850,2500) );
    ////w_pts.push_back( Point(100,0) );
    ////w_pts.push_back( Point(300,500) );
    ////w_pts.push_back( Point(500,1000) );
    ////w_pts.push_back( Point(700,1500) );
    ////w_pts.push_back( Point(900,2000) );
    ////w_pts.push_back( Point(1100,2500) );
    //CDrawArrow().NativeAddbendArrow(imread("../DllRes/IMG/road.jpg"),w_pts,8);
    //=======================================================================
    //vector<Point> pts;
    //pts.push_back( Point(500,0) );
    //pts.push_back( Point(540,300) );
    //pts.push_back( Point(570,600) );
    //pts.push_back( Point(620,1000) );
    //pts.push_back( Point(720,1200) );
    //pts.push_back( Point(780,1500) );
    //pts.push_back( Point(850,1800) );
    //pts.push_back( Point(910,2100) );
    //pts.push_back( Point(950,2400) );
    //CDrawArrow().NativeAddbendArrow(imread("../DllRes/IMG/road.jpg"), pts, 20);
}

void CDrawArrow::CraspedodromeArrow(void)
{
    Mat img = Mat(2400, 1600, CV_8UC3, Scalar(0, 0, 0)), loop2, t_img;
    Mat bg;
    vector<Point> pts;
    for (; ;)
        for (int j = 0; j <= 250; j += kk)   //2 5 10 25 50
        {
            bg = imread("../Resource/girl.jpg");
            img.copyTo(t_img);              //t_img = img.clone();
            pts.clear();
            for (int i = 0; i < 11; i++)
                pts.push_back(Point(800, 250 * (i - 1) + j));
            TrackArrowShow(t_img, pts);
            ToAirscape(t_img, loop2, false);
            MatAddMat(bg, loop2, Point(0, 0), 1);
            imshow("bg", bg);
            waitKey(1);
        }
    //==========================================================
    //for(;;)
    //  CDrawArrow().CraspedodromeArrow();
    //return 0;
    //==========================================================
}

void CDrawArrow::DrawStaticArrow(Mat& f_img, vector<Point>& pts)
{
    Mat img = Mat::zeros(1200, 800, CV_8UC3), Userout, loopOut;
    //Mat  img= Mat(1200, 800, CV_8UC3, Scalar(0, 0, 0) ),Userout,loopOut;
    if (pts.empty()) return ;
    //1600转换到800对应位置
    ResizePoint(pts);
    //计算到俯视图点
    vector<Point> t_pts(pts), pts_new;
    //俯视图转换
    vector<Point>::iterator t_it = t_pts.begin();
    for (vector<Point>::const_iterator it = pts.begin(); it != pts.end(); ++it, ++t_it)
        *t_it = WrapPoint(*it, CalcR0(true));       //计算转换映射点
    //俯视图画出来
    double t = (double) cvGetTickCount();
    OneArrowShowQuick(img, t_pts, Scalar(138, 138, 138));       //Scalar(23,90,226)
    t = (double) cvGetTickCount() - t;
    printf("detection time = %g ms\n", t / ((double) cvGetTickFrequency() * 1000.));
    //imshow("俯视图",img);
    //imwrite("1.jpg",img);
    //俯视图还原
    ToAirscape(img, Userout, false);
    //图层还原
    resize(Userout, loopOut, Size(1600, 1200));
    //GaussianBlur(loopOut, loopOut, Size(3, 3), 1.5);       //滤波处理
    MatAddMat(f_img, loopOut, Point(0, 0), 1);
}

void HudTool::CheckDrawRect(Mat& f_img, Point f_pt)
{
    //计算自动检查越界 1600x1200
    //if (f_pt.y<568||f_pt.y>1190)
    //  return ;
    //int fy =(f_pt.y-569)*0.5 +2;
    //800x600
    f_pt.x /= 2;
    f_pt.y /= 2;
    if (f_pt.y > 570 || f_pt.x < 0 || f_pt.y < 1)
        return ;
    int fy = (f_pt.y - 275) * 12 / 17 + 8 - 21 * 12 / 17;
    line(f_img, Point(f_pt.x - fy, f_pt.y), Point(f_pt.x + fy, f_pt.y), Scalar(0, 125, 255), 2);
    line(f_img, Point(f_pt.x - fy, f_pt.y - 2.6 * fy), Point(f_pt.x + fy, f_pt.y - 2.6 * fy), Scalar(0, 125, 255), 2);
    line(f_img, Point(f_pt.x - fy, f_pt.y - 2.6 * fy), Point(f_pt.x - fy, f_pt.y), Scalar(0, 125, 255), 2);
    line(f_img, Point(f_pt.x + fy, f_pt.y - 2.6 * fy), Point(f_pt.x + fy, f_pt.y), Scalar(0, 125, 255), 2);
    //resize(f_img,f_img,Size(800,600) );
    //imshow("test",f_img);
    //waitKey(0);
}

void HudTool::DrawTarget(Mat& img, Point pt, double dDist)
{
#define MAX_TARGET_PIXEL_SIZE  50
    char szIndex[MAX_PATH] = {0};
    char szDist[MAX_PATH] = {0};
    sprintf(szDist, "%d", (int) dDist);
    double front_scale = 2;
    int nRad = MAX_TARGET_PIXEL_SIZE / (dDist / 10);
    if (nRad >= MAX_TARGET_PIXEL_SIZE)
        nRad = MAX_TARGET_PIXEL_SIZE;
    //  CvScalar color = cvScalar(Rcolor.uniform(0,255),Rcolor.uniform(0,255),Rcolor.uniform(0,255));
    CvScalar color = cvScalar(0, 0, 255);
    rectangle(img, Point(pt.x - nRad, pt.y - nRad), Point(pt.x + nRad, pt.y + nRad), color, 2);
    //cvCircle(&CvMat(img),pt,nRad,color,2,8);
    //cvLine(&CvMat(img),cvPoint(pt.x-1.5*nRad,pt.y),cvPoint(pt.x-0.5*nRad,pt.y),color,2,8);
    //cvLine(&CvMat(img),cvPoint(pt.x,pt.y-1.5*nRad),cvPoint(pt.x,pt.y-0.5*nRad),color,2,8);
    //cvLine(&CvMat(img),cvPoint(pt.x+1.5*nRad,pt.y),cvPoint(pt.x+0.5*nRad,pt.y),color,2,8);
    //cvLine(&CvMat(img),cvPoint(pt.x,pt.y+1.5*nRad),cvPoint(pt.x,pt.y+0.5*nRad),color,2,8);
    //putText(img,szIndex,cvPoint(pt.x,pt.y+2.0*nRad),FONT_HERSHEY_SCRIPT_SIMPLEX,front_scale/(dDist/10),color,2,8);
    //putText(img,szDist,cvPoint(pt.x,pt.y-2.0*nRad),FONT_HERSHEY_SCRIPT_SIMPLEX,front_scale/(dDist/10),color,2,8);
}

#ifndef __tdf____
int kk; //速度
//线程测试代码
void LoopArrowTest(void)
{
    Mat img = Mat(1200, 800, CV_8UC3, Scalar(0, 0, 0)), loop2, t_img;
    Mat bg;// = imread("16001200.jpg");
    vector<Point> pts;
    for (; ;)
        for (int j = 0; j <= 250; j += kk)   //2 5 10 25 50
        {
            img.copyTo(t_img);              //t_img = img.clone();
            bg = imread("../Resource/Speed.jpg");
            resize(bg, bg, Size(800, 600));
            pts.clear();
            for (int i = 0; i < 11; i++)
                pts.push_back(Point(800, 250 * (i - 1) + j));
            CDrawArrow drawA;
            drawA.ResizePoint(pts);
            drawA.TrackArrowShow(t_img, pts);
            drawA.ToAirscape(t_img, loop2, false);
            drawA.MatAddMat(bg, loop2, Point(0, 0), 1);
            imshow("bg", bg);
            waitKey(1);
        }
}

typedef void (*FunThreadProc)(void);
void ThreadProTmp(FunThreadProc f_NewFun)
{
    //全局变量:线程句柄和线程返回ID供下面StartThreadPro使用
    HANDLE StartThreadHandle;
    DWORD StartThreadID;
    //这里使用函数指针NewFun
    StartThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) f_NewFun, NULL, 0, &StartThreadID);
    CloseHandle(StartThreadHandle);    //上面线程完成后自动释放线程
}

void testStraight(void)
{
    //========直走模拟==========================================
    kk = 8;
    cout << "可以输入速度0-25" << endl;
    ThreadProTmp(LoopArrowTest);
    while (true)
    {
        cin >> kk;
        kk *= 5;
    }
    //=======================================================
}
#endif

#ifndef __Contrail____
vector<Point> pts_Contrai;
void onClickBtn(int Event, int x, int y, int flags, void* param)
{
    if (flags == 1 && Event == 1)   //左击
    {
        cout << "onClickBtn: " << x * 2 << "  " << y * 2 << endl;
        pts_Contrai.push_back(Point(x * 2, y * 2));
        //circle( (Mat&)param,Point(x,y),2,Scalar(0,0,255),2);
    }
}

//模拟轨迹
void testContrail(void)
{
    namedWindow("标出3D轨迹", CV_WINDOW_AUTOSIZE);
    pts_Contrai.clear();
    Mat rt_img = Mat(1200, 800, CV_8UC3, Scalar(0, 255, 0));
    rectangle(rt_img, Point(0, 0), Point(800, 600), Scalar(0, 0, 255), -1);
    rectangle(rt_img, Point(0, 1050), Point(800, 1200), Scalar(255, 0, 0), -1);
    Mat img = CDrawArrow().ToAirscape(rt_img, Mat(), false);
    //设置鼠标事件
    setMouseCallback("标出3D轨迹", onClickBtn, NULL);
    imshow("标出3D轨迹", img);
    waitKey(0);
    Mat test_one;// = imread("../DllRes/IMG/road.jpg"); //road      16001200
    rt_img.copyTo(test_one);
    resize(test_one, test_one, Size(1600, 1200));       //注意这是为了保证和上面的图片一样哦
    HudTool().DrawTarget(test_one, Point(500, 500), 10);
    HudTool().DrawTarget(test_one, Point(900, 600), 20);
    HudTool().DrawTarget(test_one, Point(300, 700), 30);
    CDrawArrow().DrawStaticArrow(test_one, pts_Contrai);
    resize(test_one, test_one, Size(800, 600));
    imshow("test_one", test_one);
    waitKey(0);
}

#endif

int TestArrowShow(int argc, char** argv)
{
    Mat testImg3 = imread("../Resource/lena.jpg");
    resize(testImg3, testImg3, Size(800, 600));
    CDrawArrow().TestLoopArrowShow(testImg3);
    //Mat testImg3 = imread("../DllRes/IMG/girl.jpg");
    //resize(testImg3, testImg3, Size(800, 600));
    //CDrawArrow().TestLoopArrowShow(testImg3);
    //testContrail();         //模拟轨迹
    testStraight();       //模拟直走
    return 0;
}



