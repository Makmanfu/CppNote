
#include "stdafx.h"
#include "MatDispose.h"
#include "MatUseTool.h"

bool compare_image_cv(const char* inputFile, const char* outputFile, int resizeID)
{
    if (inputFile == NULL || outputFile == NULL)
        return false;
    if (resizeID < 1)
        return false;
    cv::Mat tmp = cv::imread(inputFile);
    cv::Mat Out;
    cv::resize(tmp, Out, cv::Size(tmp.cols / resizeID, tmp.rows / resizeID));
    vector<int> compression_params;
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(90);
    imwrite(outputFile, Out, compression_params);
}

void MatPixelsTool::TestColorMat(void)
{
    //创建一张颜色卡彩色图
    Mat mMat = Mat(512, 512, CV_8UC3, Scalar(250, 250, 250));
    for (int i = 0; i < mMat.rows; i++)     //行数
        for (int j = 0; j < mMat.cols; j++)
        {
            mMat.at<Vec3b> (i, j) [0] = rand() % 255;      //蓝色
            mMat.at<Vec3b> (i, j) [1] = rand() % 255;      //绿色
            mMat.at<Vec3b> (i, j) [2] = rand() % 255;      //红色
        }
    imshow("mMat", mMat);
    waitKey(0);
    //创建PNG图
    //mMat.isContinuous();     //函数来判断图像数组是否为连续的
    //创建灰度图
    //mMat = imread("1.jpg",0);
    //mMat = Mat(512, 512, CV_8U, Scalar(250, 250, 250) );
    Mat result;
    result.create(mMat.rows, mMat.cols, mMat.type());
    //所有灰度值
    int datavalue = 0;
    Mat gray_img = Mat(16, 16, CV_8U, Scalar(0));
    for (int i = 0; i < gray_img.rows; i++)
        for (int j = 0; j < gray_img.cols; j++)
            gray_img.at<uchar> (i, j) = datavalue++;
    imshow("gray_img", gray_img);
    imwrite("grayimg.jpg", gray_img);
    resize(gray_img, gray_img, cv::Size(640, 640));
    imshow("灰度所有像素0-255", gray_img);
    //所有彩色像素
    Mat Color_img = Mat(4096, 4096, CV_8UC3, Scalar(0, 0, 0));
    int R(0), G(0), B(0), i(0), j(0);
    while (R < 256)
    {
        while (G < 256)
        {
            while (B < 256)
            {
                //cout << R << " " << G << " " << B << "-";
                //cout << i << " " << j << endl;
                Color_img.at<Vec3b> (i, j) [0] = R;
                Color_img.at<Vec3b> (i, j) [1] = G;
                Color_img.at<Vec3b> (i, j) [2] = B;
                ++B;
                j++;
                if (j == 4096)
                {
                    ++i;
                    j = 0;
                }
            }
            ++G;
            B = 0;
        }
        ++R;
        B = G = 0;
    }
    imshow("彩色所有像素0-16777216", Color_img);
    imwrite("Colorimg.jpg", Color_img);
    waitKey(0);
}

Mat MatPixelsTool::AntithesisMat(const Mat& In_img, Mat& Out_img, double f_alpha, int f_beta)
{
    if (!In_img.data)
        return Mat(512, 512, CV_8U, Scalar(0));       //返回白板
    Mat tmp = In_img.clone();
    Out_img = Mat::zeros(tmp.size(), tmp.type());
    for (int i = 0; i < tmp.rows; i++)
        for (int j = 0; j < tmp.cols; j++)
            for (int c = 0; c < 3; c++)
                //由于运算结果可能不是整数，所以需要格式转换
                Out_img.at<Vec3b> (i, j) [c] =
                    saturate_cast<uchar> (f_alpha * (tmp.at<Vec3b> (i, j) [c]) + f_beta);
    return Out_img;
}

Mat MatPixelsTool::ColorFindPixels(const Mat& In_img, Mat& Out_img, Scalar f_color, int ff)
{
    Out_img = Mat(In_img.rows, In_img.cols, CV_8U);
    Mat_<Vec3b>::const_iterator it = In_img.begin<Vec3b>();
    Mat_<uchar>::iterator itout = Out_img.begin<uchar>();
    for (; it != In_img.end<Vec3b>(); ++it, ++itout)
    {
        if (abs(((*it) [0] - f_color[0]) + ((*it) [1] - f_color[1]) + ((*it) [2] - f_color[2])) < ff)                  //颜色距离差 阈值
            //if (static_cast<int>(cv::norm<int, 3>(cv::Vec3i(f_color[0] - (*it)[0], f_color[1] - (*it)[1], f_color[2] - (*it)[2]))) < ff)
            *itout = 255;   //黑色
        else
            *itout = 0;
    }
    return Out_img;
}

Mat MatPixelsTool::ColorReduce(const Mat& In_img, Mat& Out_img, int div)
{
    //// get iterators
    //cv::Mat_<cv::Vec3b>::iterator it= image.begin<cv::Vec3b>();
    //cv::Mat_<cv::Vec3b>::iterator itend= image.end<cv::Vec3b>();
    //for ( ; it!= itend; ++it)
    //{
    //    // process each pixel ---------------------
    //    (*it)[0]= (*it)[0]/div*div + div/2;
    //    (*it)[1]= (*it)[1]/div*div + div/2;
    //    (*it)[2]= (*it)[2]/div*div + div/2;
    //    // end of pixel processing ----------------
    //}
    //int nl= image.rows; // number of lines
    //int nc= image.cols ; // number of columns
    //if (image.isContinuous())
    //{   // then no padded pixels
    //    nc= nc*nl;
    //    nl= 1;  // it is now a 1D array
    //}
    //int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
    //// mask used to round the pixel value
    //uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
    //for (int j=0; j<nl; j++)
    //{
    //    uchar* data= image.ptr<uchar>(j);
    //    for (int i=0; i<nc; i++)
    //    {
    //        // process each pixel ---------------------
    //        *data++= *data&mask + div/2;
    //        *data++= *data&mask + div/2;
    //        *data++= *data&mask + div/2;
    //        // end of pixel processing ----------------
    //    } // end of line
    //}
    Out_img = In_img.clone();
    int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0));             // mask used to round the pixel value
    uchar mask = 0xFF << n; // e.g. for div=16, mask= 0xF0
    Out_img = (Out_img & cv::Scalar(mask, mask, mask)) + cv::Scalar(div / 2, div / 2, div / 2);         // perform color reduction
    return Out_img;
}

Mat MatPixelsTool::MatAddMat(Mat& InOut_img, const Mat& Inlogo, Point Ptbegin, float ff)
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

Mat MatPixelsTool::MatResize(const Mat& In_img, Mat& Out_img, double val)
{
    //f_val 缩放比例
    Size2d dsize(In_img.cols * val, In_img.rows * val);    // 设置新图片的大小
    //f_outimg(dsize,CV_32S);// 创建一个新的Mat（opencv的矩阵数据类型）
    resize(In_img, Out_img, dsize);
    return Out_img;
}

Mat MatPixelsTool::CutRectMat(const Mat& In_img, Mat& Out_img, Point Ptbegin, Point Ptend)
{
    int fx = In_img.cols, fy = In_img.rows;
    if (Ptbegin.x > fx || Ptbegin.x < 0) Ptbegin.x = 0;
    if (Ptbegin.y > fy || Ptbegin.y < 0) Ptbegin.y = 0;
    if (Ptend.x <= Ptbegin.x || Ptend.x > fx) Ptend.x = fx;
    if (Ptend.y <= Ptbegin.y || Ptend.y > fy) Ptend.y = fy;
    Out_img = In_img(Rect(Ptbegin, Ptend));
    return Out_img;
}

Mat MatPixelsTool::CutRectMat(const Mat& In_img, Mat& Out_img, Rect f_rect)
{
    //int fx = In_img.cols, fy = In_img.rows;
    ////起点判断
    //if (f_rect.x > fx || f_rect.x <= 0) f_rect.x = 0;
    //if (f_rect.y > fy || f_rect.y <= 0) f_rect.y = 0;
    ////终点判断
    //int ex(0), ey(0);
    //if (f_rect.width + f_rect.x >fx || f_rect.width) f_rect.width = fx - f_rect.x;
    //if (f_rect.height + f_rect.y> fy || f_rect.height) f_rect.height = fy - f_rect.y;
    //Out_img = In_img(Rect(Point(f_rect.x, f_rect.y), Point(f_rect.width, f_rect.height)));
    Out_img = In_img(Rect(f_rect));
    return Out_img;
}

Mat MatPixelsTool::ColorGetROI(const Mat& In_img, Mat& Out_img, Scalar f_color)
{
    Out_img = In_img.clone();
    int left = In_img.cols, top = In_img.rows, right = 0, bottom = 0, k = 0;
    for (int i = 0; i < In_img.rows; i++)
        for (int j = 0; j < In_img.cols; j++)
            //if (f_img.channels() == 1) if (f_img.at<uchar>(i, j) == f_color[0])
            if (In_img.at<Vec3b> (i, j) [0] == f_color[0] && In_img.at<Vec3b> (i, j) [1]
                == f_color[1] && In_img.at<Vec3b> (i, j) [2] == f_color[2])
                //注上面算法可根据欧拉距离改进 阈值来过滤
            {
                if (left > j) left = j;
                if (right < j) right = j;
                if (top > i) top = i;
                if (bottom < i) bottom = i;
                k++;
            }
    if (k == 0)
        Out_img = In_img;
    else
        Out_img = In_img(Rect(left, top, right - left, bottom - top));
    return Out_img;
}

cv::Mat MatPixelsTool::MosaicPixels(const Mat& In_img, Mat& Out_img, int ff /*= 10*/)
{
    //考虑尺寸问题
    //Out_img = In_img.clone();
    //int W(ff), H(ff);
    ////int wnum = f_img.cols / W + f_img.cols % W;
    ////int hnum = f_img.rows / H + f_img.rows % H;
    //double t_r, t_g, t_b;
    //for (int mi = W; mi < In_img.cols; mi += W)
    //    for (int mj = H; mj < In_img.rows; mj += H)
    //    {
    //        t_r = In_img.at<Vec3b>(mi - W / 2, mj - H / 2)[0];
    //        t_g = In_img.at<Vec3b>(mi - W / 2, mj - H / 2)[1];
    //        t_b = In_img.at<Vec3b>(mi - W / 2, mj - H / 2)[2];
    //        for (int mx = mi - W; mx <= mi; mx++)
    //            for (int my = mj - H; my <= mj; my++)
    //            {
    //                Out_img.at<Vec3b>(mx, my)[0] = t_r;
    //                Out_img.at<Vec3b>(mx, my)[1] = t_g;
    //                Out_img.at<Vec3b>(mx, my)[2] = t_b;
    //            }
    //    }
    return Out_img;
}

cv::Mat MatPixelsTool::ColorMatPixels(int ff /*=0*/)
{
    double t = (double) cvGetTickCount();
    Mat f_img;
    if (ff)
        f_img = Mat(512, 512, CV_8U, Scalar(255));
    else
        f_img = Mat(512, 512, CV_8UC3, Scalar(0, 0, 255));
    if (ff)
    {
        //单通道的处理方法
        //===========================================
        for (int i = 0; i < f_img.rows; i++)     //行数
            for (int j = 0; j < f_img.cols; j++)
                f_img.at<uchar> (i, j) = 0;
        //=========下面指针速度会更快=======================
        for (int i = 0; i < f_img.rows; i++)
        {
            uchar* data = f_img.ptr<uchar> (i);   //每行的指针
            for (int j = 0; j < f_img.cols; j++)
                data[j] = 0;
        }
    }
    else    //彩色多通道图的处理
    {
        //===========================================
        for (int i = 0; i < f_img.rows; i++)     //行数
            for (int j = 0; j < f_img.cols; j++)
            {
                f_img.at<Vec3b> (i, j) [0] = 0;        //蓝色
                f_img.at<Vec3b> (i, j) [1] = 255;      //绿色
                f_img.at<Vec3b> (i, j) [2] = 0;        //红色
            }
        //===========================================
        t = (double) cvGetTickCount() - t;
        printf("time = %g ms\n", t / ((double) cvGetTickFrequency() * 1000.));
        //还有多种方法尝试
    }
    return f_img;
}

void MatPixelsTool::WritePng(const Mat& In_img, const char* f_name)
{
    Mat tmp = In_img.clone();
    for (int i = 0; i < tmp.rows; ++i)
        for (int j = 0; j < tmp.cols; ++j)
        {
            Vec4b& rgba = tmp.at<Vec4b> (i, j);
            if (rgba[0] < 3 && rgba[1] < 3 && rgba[2] < 3)
                rgba[3] = 0;    //透明
        }
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);    //png格式下，默认的参数为3.
    imshow("显示数据", tmp);
    imwrite(f_name, tmp, compression_params);
}


cv::Mat MatFiltration::GetGrayMat(const Mat& In_img)
{
    Mat gray;
    //图像空间转换
    cvtColor(In_img, gray, CV_RGB2GRAY);    //灰度模式
    return gray;
}

cv::Mat MatFiltration::Binarization(const Mat& In_img, Mat& Out_img, int ff /*= 60*/)
{
    //二值图
    //cv::threshold(GetGrayMat(In_img), Out_img, ff, 255, THRESH_BINARY_INV);
    //二值图并取反
    cv::threshold(GetGrayMat(In_img), Out_img, ff, 255, THRESH_BINARY);
    return Out_img;
}

cv::Mat MatFiltration::Sharpen2D(const Mat& f_img, Mat& f_out)
{
    /*  方法一
        //转为灰度图
        Mat img;
        cvtColor(f_img,img,CV_RGB2GRAY);
        f_outimg.create(img.rows,img.cols,img.type());
        //使用3*3滤波器，所以遍历的像素中不能包括图像最外围的一圈
        for(int i = 1;i < img.rows-1;i++)
        {   //前一行、当前行、后一行的指针
        uchar* previous = img.ptr< uchar>(i-1);
        uchar* current  = img.ptr< uchar>(i);
        uchar* next     = img.ptr< uchar>(i+1);
        //输出结果图像的行指针
        uchar* output = f_outimg.ptr<uchar>(i);
        for(int j = 1;j < img.cols - 1;j++)
        {
            //输出图像的遍历指针与当前行的指针同步递增, 以每行的每一个像素点的每一个通道值为一个递增量, 因为要考虑到图像的通道数
             output++= cv::saturate_cast<uchar>(5*current[j]-current[j-1]-current[j+1]-previous[j]-next[j]);
            //saturate_cast<uchar>会将小于0的置零，大于255的改为255
        }
        }
        f_outimg.row(0).setTo(cv::Scalar(0));
        f_outimg.row(f_outimg.rows-1).setTo(cv::Scalar(0));
        f_outimg.col(0).setTo(cv::Scalar(0));
        f_outimg.col(f_outimg.cols-1).setTo(cv::Scalar(0));
    */
    /*  方法2调用滤波函数来完成图像的锐化*/
    cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));       // Construct kernel (all entries initialized to 0)
    // assigns kernel values
    kernel.at<float> (1, 1) = 5.0;      // 分配像素置
    kernel.at<float> (0, 1) = -1.0;
    kernel.at<float> (2, 1) = -1.0;
    kernel.at<float> (1, 0) = -1.0;
    kernel.at<float> (1, 2) = -1.0;
    f_out = f_img.clone();
    cv::filter2D(f_img, f_out, f_out.depth(), kernel);    //调用滤波函数
    return f_out;
    //--------------简写
    //Mat sharpenedLenakernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    //filter2D(f_img,f_out,f_out.depth(),sharpenedLenakernel);  //调用滤波函数
}

void MatFiltration::FiltersImg(const Mat& f_img)
{
    // 高斯滤波
    Mat result;
    GaussianBlur(f_img, result, cv::Size(5, 5), 1.5);
    imshow("高斯滤波", result);
    blur(f_img, result, cv::Size(5, 5));
    imshow("低通滤波", result);
    medianBlur(f_img, result, 5);
    imshow("中值滤波", result);
    waitKey(0);
}

cv::Mat MatFiltration::MatRotate(const Mat& f_img, Mat& f_out, double angle)
{
    // 设置目标图像的大小和类型与源图像一致
    Mat t_dstImg_warp = Mat::zeros(f_img.rows, f_img.cols, f_img.type());
    //【6】对图像进行缩放后再旋转
    // 计算绕图像中点顺时针旋转50度缩放因子为0.6的旋转矩阵
    Point center = Point(t_dstImg_warp.cols / 2, t_dstImg_warp.rows / 2);
    //double angle = -30.0;     //-30.0右旋转30度 30.0左旋转
    double scale = 1.0;         //缩放比例
    // 通过上面的旋转细节信息求得旋转矩阵
    Mat rotMat(30, 30, CV_32FC1);
    rotMat = getRotationMatrix2D(center, angle, scale);
    // 旋转已缩放后的图像
    warpAffine(f_img, f_out, rotMat, t_dstImg_warp.size());
    return f_out;
}

cv::Mat MatFiltration::WarpAffineImg(const Mat& f_img, Mat& f_out)
{
    return f_out;
}

cv::Mat MatFiltration::InverseProjection(const Mat& f_img, Mat& f_out)
{
    long nnn = 60;            //注意取值范围
    long mmm = 60;
    f_out = Mat(f_img.rows - 200, f_img.cols + 2 * mmm, CV_8UC3, Scalar(255, 255, 255));
    Point2f src_vertices[4];
    src_vertices[0] =   Point(0, 0);
    src_vertices[1] =   Point(f_img.cols, 0);
    src_vertices[2] =   Point(0, f_img.rows);
    src_vertices[3] =   Point(f_img.cols, f_img.rows);
    Point2f dst_vertices[4];
    dst_vertices[0] =   Point(0 + mmm + nnn, 0);
    dst_vertices[1] =   Point(f_out.cols - mmm - nnn, 0);
    dst_vertices[2] =   Point(0, f_out.rows);
    dst_vertices[3] =   Point(f_out.cols, f_out.rows);
    Mat R0 = getPerspectiveTransform(src_vertices, dst_vertices);
    cvWarpPerspective(&CvMat(f_img), &CvMat(f_out), &CvMat(R0));
    // imshow("f_img",f_img);
    //imshow("f_out",f_out);
    //waitKey(0);
    return f_out;
}

cv::Mat MatFiltration::ToAirscape(const Mat& f_img, Mat& f_out, bool ff)
{
    f_out = Mat(f_img.rows, f_img.cols, CV_8UC3, Scalar(255, 255, 255));
    //1600x1200
    //Point2f src_vertices[4];
    //src_vertices[0] = Point(691, 421);
    //src_vertices[1] = Point(847,421);
    //src_vertices[2] = Point(509,601);
    //src_vertices[3] = Point(1083,601);
    //Point2f dst_vertices[4];
    //dst_vertices[0] = Point(509, 421);
    //dst_vertices[1] = Point(1083,421);
    //dst_vertices[2] = Point(509,801);
    //dst_vertices[3] = Point(1083,801);
    //800x600
    Point2f src_vertices[4];
    src_vertices[0] = Point(351, 206);
    src_vertices[1] = Point(417, 206);
    src_vertices[2] = Point(219, 328);
    src_vertices[3] = Point(580, 328);
    Point2f dst_vertices[4];
    dst_vertices[0] = Point(219, 206);
    dst_vertices[1] = Point(580, 206);
    dst_vertices[2] = Point(219, 558);
    dst_vertices[3] = Point(580, 558);
    //Point2f src_vertices[4];
    //src_vertices[0] = Point(188, 363);
    //src_vertices[1] = Point(634,363);
    //src_vertices[2] = Point(90,453);
    //src_vertices[3] = Point(745,453);
    //Point2f dst_vertices[4];
    //dst_vertices[0] = Point(90, 363);
    //dst_vertices[1] = Point(745,363);
    //dst_vertices[2] = Point(90,683);
    //dst_vertices[3] = Point(745,683);
    Mat R0;
    if (ff)
        R0 = getPerspectiveTransform(src_vertices, dst_vertices);
    else
        R0 = getPerspectiveTransform(dst_vertices, src_vertices);
    cvWarpPerspective(&CvMat(f_img), &CvMat(f_out), &CvMat(R0));
    return f_out;
}

cv::Mat MatFiltration::convolveDFT(Mat A, Mat B, Mat& C)
{
    // reallocate the output array if needed
    C.create(abs(A.rows - B.rows) + 1, abs(A.cols - B.cols) + 1, A.type());
    Size dftSize;
    // calculate the size of DFT transform
    dftSize.width = getOptimalDFTSize(A.cols + B.cols - 1);
    dftSize.height = getOptimalDFTSize(A.rows + B.rows - 1);
    // allocate temporary buffers and initialize them with 0's
    Mat tempA(dftSize, A.type(), Scalar::all(0));
    Mat tempB(dftSize, B.type(), Scalar::all(0));
    // copy A and B to the top-left corners of tempA and tempB, respectively
    Mat roiA(tempA, Rect(0, 0, A.cols, A.rows));
    A.copyTo(roiA);
    Mat roiB(tempB, Rect(0, 0, B.cols, B.rows));
    B.copyTo(roiB);
    // now transform the padded A & B in-place;
    // use "nonzeroRows" hint for faster processing
    dft(tempA, tempA, 0, A.rows);
    dft(tempB, tempB, 0, B.rows);
    // multiply the spectrums;
    // the function handles packed spectrum representations well
    mulSpectrums(tempA, tempB, tempA, DFT_COMPLEX_OUTPUT);
    //mulSpectrums(tempA, tempB, tempA, DFT_REAL_OUTPUT);
    // transform the product back from the frequency domain.
    // Even though all the result rows will be non-zero,
    // you need only the first C.rows of them, and thus you
    // pass nonzeroRows == C.rows
    dft(tempA, tempA, DFT_INVERSE + DFT_SCALE, C.rows);
    // now copy the result back to C.
    tempA(Rect(0, 0, C.cols, C.rows)).copyTo(C);
    // all the temporary buffers will be deallocated automatically
    return C;
}

void MatFiltration::testDFT(const Mat& In_img)
{
    Mat GRAYimg;
    cvtColor(In_img, GRAYimg, CV_BGR2GRAY);
    Mat kernel = (Mat_<float> (3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
    cout << kernel;
    Mat floatI = Mat_<float> (GRAYimg);   // change image type into float
    Mat filteredI;
    convolveDFT(floatI, kernel, filteredI);
    normalize(filteredI, filteredI, 0, 1, CV_MINMAX);    // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).
    imshow("image", GRAYimg);
    imshow("filtered", filteredI);
    waitKey(0);
}

cv::Mat MatFiltration::ImgMorpho(const Mat& f_img)
{
    //以下用详细例子参考
    Mat eroded, dilated;
    cv::erode(f_img, eroded, cv::Mat());
    cv::dilate(f_img, dilated, cv::Mat());
    imshow("腐蚀图像", eroded);
    imshow("膨胀图像", dilated);
    Mat element(7, 7, CV_8U, cv::Scalar(1));
    cv::erode(f_img, eroded, element);
    cv::imshow("腐蚀图像7x7", eroded);
    erode(f_img, eroded, cv::Mat(), cv::Point(-1, -1), 3);
    imshow("腐蚀图像3次", eroded);
    Mat element5(5, 5, CV_8U, cv::Scalar(1)), opened, closed;
    cv::morphologyEx(f_img, opened, cv::MORPH_OPEN, element5);
    imshow("开运算", opened);
    cv::morphologyEx(f_img, closed, cv::MORPH_CLOSE, element5);
    imshow("闭运算", closed);
    Mat morp = f_img.clone();
    cv::morphologyEx(morp, morp, cv::MORPH_CLOSE, element5);
    cv::morphologyEx(morp, morp, cv::MORPH_OPEN, element5);
    imshow("形态学梯度 先闭后开", morp);
    Mat morp2 = f_img.clone();
    cv::morphologyEx(morp2, morp2, cv::MORPH_OPEN, element5);
    cv::morphologyEx(morp2, morp2, cv::MORPH_CLOSE, element5);
    imshow("形态学梯度 先开后闭", morp2);
    //morphologyEx(morp, morp, MORPH_TOPHAT, element);      //顶帽
    //morphologyEx(morp, morp, MORPH_BLACKHAT, element);    //黑帽
    return morp2;
}

cv::Mat MatFiltration::MorphoEdge(const Mat& f_img, Mat& f_out/*=Mat() */, int threshold/*=40*/)
{
    Mat t_img = f_img;
    //用灰度图处理
    if (t_img.channels() > 1)
        cvtColor(t_img, t_img, CV_BGR2GRAY);
    //形态学滤波变化
    cv::morphologyEx(t_img, f_out, cv::MORPH_GRADIENT, cv::Mat());
    //得到边缘了哦
    if (threshold > 0)
        cv::threshold(f_out, f_out, threshold, 255, cv::THRESH_BINARY_INV);
    return f_out;
}

cv::Mat MatFiltration::GetCorners(const Mat& f_img, Mat& f_out/*=Mat() */)
{
    f_out = f_img.clone();
    Mat t_use;
    cvtColor(f_out, f_out, CV_RGB2GRAY);    //灰度图
    //分别创建各种元素
    Mat cross(5, 5, CV_8U, cv::Scalar(0));
    Mat diamond(5, 5, CV_8U, cv::Scalar(1));
    Mat square(5, 5, CV_8U, cv::Scalar(1));
    Mat x(5, 5, CV_8U, cv::Scalar(0));
    //创建十字形
    for (int i = 0; i < 5; i++)
    {
        cross.at<uchar> (2, i) = 1;
        cross.at<uchar> (i, 2) = 1;
    }
    // 创建菱形元素
    diamond.at<uchar> (0, 0) = 0;
    diamond.at<uchar> (0, 1) = 0;
    diamond.at<uchar> (1, 0) = 0;
    diamond.at<uchar> (4, 4) = 0;
    diamond.at<uchar> (3, 4) = 0;
    diamond.at<uchar> (4, 3) = 0;
    diamond.at<uchar> (4, 0) = 0;
    diamond.at<uchar> (4, 1) = 0;
    diamond.at<uchar> (3, 0) = 0;
    diamond.at<uchar> (0, 4) = 0;
    diamond.at<uchar> (0, 3) = 0;
    diamond.at<uchar> (1, 4) = 0;
    // 创建X形元素
    for (int i = 0; i < 5; i++)
    {
        x.at<uchar> (i, i) = 1;
        x.at<uchar> (4 - i, i) = 1;
    }
    // 十字形膨胀
    cv::dilate(f_out, t_use, cross);
    // 菱形腐蚀
    cv::erode(t_use, t_use, diamond);
    cv::Mat result2;
    // X形膨胀
    cv::dilate(f_out, result2, x);
    // 方形腐蚀
    cv::erode(result2, result2, square);
    // 通过两张图像做差值得到角点图像
    cv::absdiff(result2, t_use, t_use);
    // 阈值化得到二值图像
    //threshold(t_use, t_use, -1, 255, cv::THRESH_BINARY_INV);
    // 形态学变换
    cv::morphologyEx(t_use, t_use, cv::MORPH_TOPHAT, cv::Mat());
    cv::threshold(t_use, t_use, 40, 255, cv::THRESH_BINARY_INV);
    //此时得到的是角点图
    imshow("Corners角点图", t_use);
    //下面在原图上标出角点图
    cv::Mat_<uchar>::const_iterator it = t_use.begin<uchar>();
    cv::Mat_<uchar>::const_iterator itend = t_use.end<uchar>();
    // for each pixel
    for (int i = 0; it != itend; ++it, ++i)
        if (!*it)
            cv::circle(f_out, cv::Point(i % f_out.step, i / f_out.step), 5, cv::Scalar(255, 0, 0));
    imshow("原图上标出角点图", f_out);
    //waitKey(0);
    return f_out;
}

cv::Mat MatHistogram::DrawHist(MatND& hist)
{
    Mat histImg;
    cv::MatND histtmp = hist.clone();
    double maxVal = 0, minVal = 0;
    int histSize[] = { 256 };                       //项的数量
    cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);    //求最大值最小值
    histImg = Mat(histSize[0], histSize[0], CV_8U, cv::Scalar(255));
    int hpt = static_cast<int>(0.9 * histSize[0]);
    for (int h = 0; h < histSize[0]; h++)
    {
        float binVal = hist.at<float> (h);
        int intensity = static_cast<int>(binVal * hpt / maxVal);
        cv::line(histImg, cv::Point(h, histSize[0]),
                 cv::Point(h, histSize[0] - intensity), cv::Scalar::all(0));
    }
    return histImg;
}

cv::MatND MatHistogram::GetGrayHist(Mat& f_img, MatND& hist /*= MatND()*/, Mat& histImg /*= Mat()*/)
{
    Mat gray;
    if (f_img.channels() > 1)
        cvtColor(f_img, gray, CV_RGB2GRAY);        //转灰度
    else
        gray = f_img;
    int channels[] = { 0 };                        //仅用到一个通道
    int histSize[] = { 256 };                      //项的数量
    float range[] = { 0, 256 };                    //像素的最小和最大值
    const float* ranges[] = { range };
    calcHist(&gray,
             1,          // histogram of 1 image only   [计算单张图片的直方图]
             channels,   // the channel used            [通道数量]
             cv::Mat(),  // no mask is used             [不使用图像作为掩码]
             hist,       // the resulting histogram     [返回的直方图]
             1,          // it is a 1D histogram
             histSize,   // number of bins              [项的数量]
             ranges      // pixel value range           [像素值的范围]
            );
    cv::MatND histtmp = hist.clone();
    double maxVal = 0, minVal = 0;
    cv::minMaxLoc(histtmp, &minVal, &maxVal, 0, 0);    //求最大值最小值
    histImg = Mat(histSize[0], histSize[0], CV_8U, cv::Scalar(255));       //类型CV_8U
    int hpt = static_cast<int>(0.9 * histSize[0]);
    for (int h = 0; h < histSize[0]; h++)
    {
        float binVal = histtmp.at<float> (h);
        int intensity = static_cast<int>(binVal * hpt / maxVal);
        cv::line(histImg, cv::Point(h, histSize[0]),
                 cv::Point(h, histSize[0] - intensity), cv::Scalar::all(0));
    }
    //imshow("histImg",histImg);
    //waitKey(0);
    return hist;
}

cv::MatND MatHistogram::GetColorHist(Mat& f_img, MatND& hist /*= MatND()*/, Mat& histImg /*= Mat()*/)
{
    // 设定bin数目
    int histSize[] = { 256, 256, 256 };
    // 设定取值范围 ( R,G,B) )
    float hranges[] = { 0.0, 255.0 };
    const float* ranges[] = { hranges, hranges, hranges };
    int channels[] = { 0, 1, 2 };
    // Compute histogram
    calcHist(&f_img,
             1,          // 计算单张图像的直方图
             channels,   // 通道数量
             cv::Mat(),  // 不使用图像作为掩码
             hist,       // 直方图结果
             3,          // 这是3D直方图
             histSize,   // 项的数量
             ranges      // 像素值的范围
            );
    //画直方图 扩展
    //return hist;
    //*************************为了画出直方图**************************
    vector<Mat> rgbhist;
    vector<Mat> rgb_planes;
    split(f_img, rgb_planes);               // 分割成3个单通道图像 ( R, G 和 B )
    int histSize2 = 255;                     // 设定bin数目
    float range[] = { 0, 255 };             // 设定取值范围 ( R,G,B) )
    const float* histRange = { range };
    bool uniform(true), accumulate(false);
    Mat r_hist, g_hist, b_hist;
    // 计算直方图:
    calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize2, &histRange, uniform, accumulate);
    calcHist(&rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize2, &histRange, uniform, accumulate);
    calcHist(&rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize2, &histRange, uniform, accumulate);
    rgbhist.push_back(r_hist);
    rgbhist.push_back(g_hist);
    rgbhist.push_back(b_hist);
    //这样相加估计不合理
    Mat t_hist = r_hist + g_hist + b_hist;
    // 创建直方图画布
    int hist_w = 256;
    int hist_h = 256;   //400
    int bin_w = cvRound((double) hist_w / histSize2);
    histImg = Mat(hist_w, hist_h, CV_8UC3, Scalar(255, 255, 255));
    // 将直方图归一化到范围 [ 0, histImage.rows ]
    normalize(r_hist, r_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(b_hist, b_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    // 在直方图画布上画出直方图
    for (int i = 1; i < histSize2; i++)
    {
        line(histImg, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float> (i - 1))),
             Point(bin_w * (i), hist_h - cvRound(r_hist.at<float> (i))),
             Scalar(0, 0, 255), 2, 8, 0);
        line(histImg, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float> (i - 1))),
             Point(bin_w * (i), hist_h - cvRound(g_hist.at<float> (i))),
             Scalar(0, 255, 0), 2, 8, 0);
        line(histImg, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float> (i - 1))),
             Point(bin_w * (i), hist_h - cvRound(b_hist.at<float> (i))),
             Scalar(255, 0, 0), 2, 8, 0);
    }
    //imshow("r", DrawHist(r_hist));
    //imshow("g", DrawHist(g_hist));
    //imshow("b", DrawHist(b_hist));
    return hist;
    namedWindow("calcHist Demo", CV_WND_PROP_AUTOSIZE);
    imshow("calcHist Demo", histImg);
    waitKey(0);
}

cv::SparseMat MatHistogram::GetColorHist(Mat& f_img, SparseMat& hist /*= SparseMat()*/, Mat& histImg /*= Mat()*/)
{
    int histSize[] = { 256, 256, 256 };
    float hranges[] = { 0.0, 255.0 };
    const float* ranges[] = { hranges, hranges, hranges };
    int channels[] = { 0, 1, 2 };
    calcHist(&f_img, 1, channels, cv::Mat(), hist, 3, histSize, ranges);
    return hist;
}

cv::MatND MatHistogram::GetHSHist(Mat& f_img, MatND& hist /*= Mat()*/, Mat& histImg /*= Mat()*/)
{
    //绘制H-S二维直方图
    Mat hsv;
    cvtColor(f_img, hsv, CV_BGR2HSV);
    int hbins(256), sbins(180);             // Quantize the hue to 30 levels and the saturation to 32 levels
    int histSize[] = { hbins, sbins };
    float hranges[] = { 0, 180 };           // hue varies from 0 to 179, see cvtColor
    float sranges[] = { 0, 256 };           // saturation varies from 0 (black-gray-white) to 255 (pure spectrum color)
    const float* ranges[] = { hranges, sranges };
    int channels[] = { 0, 1 };              // we compute the histogram from the 0-th and 1-st channels
    calcHist(&hsv, 1, channels, Mat(), /* do not use mask*/hist, 2, histSize, ranges, true, false);
    double maxVal = 0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);
    int scale = 2;
    histImg = Mat::zeros(sbins * scale, hbins * scale, CV_8UC3);
    for (int h = 0; h < hbins; h++)
        for (int s = 0; s < sbins; s++)
        {
            float binVal = hist.at<float> (h, s);
            int intensity = cvRound(binVal * 255 / maxVal);
            rectangle(histImg, Point(h * scale, s * scale),
                      Point((h + 1) *scale - 1, (s + 1) *scale - 1),
                      Scalar::all(intensity),
                      CV_FILLED);
        }
    return hist;
    namedWindow("H-S Histogram", CV_WND_PROP_AUTOSIZE);
    imshow("H-S Histogram", histImg);
    waitKey(0);
}

double MatHistogram::CalcEntropy(Mat& img)
{
    // 将输入的矩阵为图像
    double temp[256];
    // 清零
    for (int i = 0; i < 256; i++)
        temp[i] = 0.0;
    // 计算每个像素的累积值
    for (int m = 0; m < img.rows; m++)
    {
        // 有效访问行列的方式
        const uchar* t = img.ptr<uchar> (m);
        for (int n = 0; n < img.cols; n++)
        {
            int i = t[n];
            temp[i] = temp[i] + 1;
        }
    }
    // 计算每个像素的概率
    for (int i = 0; i < 256; i++)
        temp[i] = temp[i] / (img.rows * img.cols);
    double result = 0;
    // 根据定义计算图像熵
    for (int i = 0; i < 256; i++)
        result = (temp[i] == 0.0 ? result : result - temp[i] * (log(temp[i]) / log(2.0)));
    return result;
    //下面计算图像熵也可以参考
    //float EntropyA::calEntropy( const Mat &image,int threshold )
    //{
    //    float total_P(0), Entropy(0), percent;
    //    Mat hist = getHistogram(image);
    //    const float* hist_p = (float*) hist.ptr<float>(0);
    //    for(int i = 0;i<threshold;i++)
    //        total_P  += hist_p[i];//总和
    //    for(int i = 0;i<threshold;i++)
    //    {
    //        if(hist_p[i]!=0)
    //        {
    //            percent = hist_p[i]/total_P;//每个亮度概率
    //            Entropy += percent * log(1/percent);
    //        } else {
    //            percent = 0;
    //            Entropy = Entropy;
    //        }
    //    }
    //    return Entropy;
    //}
}

cv::Mat MatHistogram::ColorHistEqualize(Mat& In_img, Mat& Out_img /*= Mat()*/)
{
    if (In_img.channels() == 1)         //灰度图
    {
        equalizeHist(In_img, Out_img);
        return Out_img;
    }
    else
    {
        vector<Mat> rgb_planes;         //分割成3个单通道图像 ( R, G 和 B )
        split(In_img, rgb_planes);      //分离通道 分别均衡化
        equalizeHist(rgb_planes[0], rgb_planes[0]);
        equalizeHist(rgb_planes[1], rgb_planes[1]);
        equalizeHist(rgb_planes[2], rgb_planes[2]);
        merge(rgb_planes, Out_img);     //合并通道
        return Out_img;
    }
    return Out_img;
}

cv::Mat MatHistogram::AutoControst(Mat& In_img)
{
    //进行自动对比度校正
    double HistRed[256] = { 0 }, HistGreen[256] = { 0 }, HistBlue[256] = { 0 }, dlowcut = 0.1, dhighcut = 0.1;
    int bluemap[256] = { 0 }, redmap[256] = { 0 }, greenmap[256] = { 0 };
    for (int i = 0; i < In_img.rows; i++)
    {
        for (int j = 0; j < In_img.cols; j++)
        {
            int iblue = In_img.at<Vec3b> (i, j) [0];
            int igreen = In_img.at<Vec3b> (i, j) [1];
            int ired = In_img.at<Vec3b> (i, j) [2];
            HistBlue[iblue]++;
            HistGreen[igreen]++;
            HistRed[ired]++;
        }
    }
    int isum(0), PixelAmount = In_img.rows * In_img.cols;
    // blue
    int iminblue(0), imaxblue(0);
    for (int y = 0; y < 256; y++)   //这两个操作我基本能够了解了
    {
        isum = isum + HistBlue[y];
        if (isum >= PixelAmount * dlowcut * 0.01)
        {
            iminblue = y;
            break;
        }
    }
    isum = 0;
    for (int y = 255; y >= 0; y--)
    {
        isum = isum + HistBlue[y];
        if (isum >= PixelAmount * dhighcut * 0.01)
        {
            imaxblue = y;
            break;
        }
    }
    //red
    isum = 0;
    int iminred = 0;
    int imaxred = 0;
    for (int y = 0; y < 256; y++)   //这两个操作我基本能够了解了
    {
        isum = isum + HistRed[y];
        if (isum >= PixelAmount * dlowcut * 0.01)
        {
            iminred = y;
            break;
        }
    }
    isum = 0;
    for (int y = 255; y >= 0; y--)
    {
        isum = isum + HistRed[y];
        if (isum >= PixelAmount * dhighcut * 0.01)
        {
            imaxred = y;
            break;
        }
    }
    //green
    isum = 0;
    int imingreen = 0;
    int imaxgreen = 0;
    for (int y = 0; y < 256; y++)   //这两个操作我基本能够了解了
    {
        isum = isum + HistGreen[y];
        if (isum >= PixelAmount * dlowcut * 0.01)
        {
            imingreen = y;
            break;
        }
    }
    isum = 0;
    for (int y = 255; y >= 0; y--)
    {
        isum = isum + HistGreen[y];
        if (isum >= PixelAmount * dhighcut * 0.01)
        {
            imaxgreen = y;
            break;
        }
    }
    /////////自动色阶
    //自动对比度
    int imin = 255;
    int imax = 0;
    if (imin > iminblue)  imin = iminblue;
    if (imin > iminred)   imin = iminred;
    if (imin > imingreen) imin = imingreen;
    iminblue = imin;
    imingreen = imin;
    iminred = imin;
    if (imax < imaxblue)   imax = imaxblue;
    if (imax < imaxgreen)  imax = imaxgreen;
    if (imax < imaxred)    imax = imaxred;
    imaxred = imax;
    imaxgreen = imax;
    imaxblue = imax;
    /////////////////
    //blue
    for (int y = 0; y < 256; y++)
    {
        if (y <= iminblue)
            bluemap[y] = 0;
        else
        {
            if (y > imaxblue)
                bluemap[y] = 255;
            else
            {
                //  BlueMap(Y) = (Y - MinBlue) / (MaxBlue - MinBlue) * 255      '线性隐射
                float ftmp = (float)(y - iminblue) / (imaxblue - iminblue);
                bluemap[y] = (int)(ftmp * 255);
            }
        }
    }
    //red
    for (int y = 0; y < 256; y++)
    {
        if (y <= iminred)
            redmap[y] = 0;
        else
        {
            if (y > imaxred)
                redmap[y] = 255;
            else
            {
                //  BlueMap(Y) = (Y - MinBlue) / (MaxBlue - MinBlue) * 255      '线性隐射
                float ftmp = (float)(y - iminred) / (imaxred - iminred);
                redmap[y] = (int)(ftmp * 255);
            }
        }
    }
    //green
    for (int y = 0; y < 256; y++)
    {
        if (y <= imingreen)
            greenmap[y] = 0;
        else
        {
            if (y > imaxgreen)
                greenmap[y] = 255;
            else
            {
                //  BlueMap(Y) = (Y - MinBlue) / (MaxBlue - MinBlue) * 255      '线性隐射
                float ftmp = (float)(y - imingreen) / (imaxgreen - imingreen);
                greenmap[y] = (int)(ftmp * 255);
            }
        }
    }
    //查表
    for (int i = 0; i < In_img.rows; i++)
    {
        for (int j = 0; j < In_img.cols; j++)
        {
            In_img.at<Vec3b> (i, j) [0] = bluemap[In_img.at<Vec3b> (i, j) [0]];
            In_img.at<Vec3b> (i, j) [1] = greenmap[In_img.at<Vec3b> (i, j) [1]];
            In_img.at<Vec3b> (i, j) [2] = redmap[In_img.at<Vec3b> (i, j) [2]];
        }
    }
    imshow("自动对比度", In_img);
    waitKey(0);
    return In_img;
    //////////////////////////////////////////////////////////////////////////
    //Mat src = imread("../DllRes/IMG/road.jpg");
    //imshow("原始图片", src);
    //Mat dst = src.clone();
    //const float thresholdco = 0.05;
    //const int thresholdnum = 100;
    //int histogram[256] = { 0 };
    //for (int i = 0; i < dst.rows; i++)
    //{
    //    for (int j = 0; j < dst.cols; j++)
    //    {
    //        int b = dst.at<Vec3b>(i, j)[0];
    //        int g = dst.at<Vec3b>(i, j)[1];
    //        int r = dst.at<Vec3b>(i, j)[2];
    //        //计算灰度值
    //        int gray = (r * 299 + g * 587 + b * 114) / 1000;
    //        histogram[gray]++;
    //    }
    //}
    //int calnum = 0;
    //int total = dst.rows * dst.cols;
    //int num;
    ////下面的循环得到满足系数thresholdco的临界灰度级
    //for (int i = 0; i < 256; i++)
    //{
    //    if ((float)calnum / total < thresholdco) //得到前5%的高亮像素。
    //    {
    //        calnum += histogram[255 - i];//histogram保存的是某一灰度值的像素个数,calnum是边界灰度之上的像素数
    //        num = i;
    //    } else break;
    //}
    //int averagegray = 0;
    //calnum = 0;
    ////得到满足条件的象素总的灰度值
    //for (int i = 255; i >= 255 - num; i--)
    //{
    //    averagegray += histogram[i] * i; //总的像素的个数*灰度值
    //    calnum += histogram[i]; //总的像素数
    //}
    //averagegray /= calnum;
    ////得到光线补偿的系数
    //float co = 255.0 / (float)averagegray;
    ////for (int i = 0; i < dst.rows; i++)
    ////{
    ////    for (int j = 0; j < dst.cols; j++)
    ////    {
    ////        dst.at<Vec3b>(i,j)[0]= CLAMP0255(co*dst.at<Vec3b>(i,j)[0]+0.5);
    ////        dst.at<Vec3b>(i,j)[1]= CLAMP0255(co*dst.at<Vec3b>(i,j)[1]+0.5);
    ////        dst.at<Vec3b>(i,j)[2]= CLAMP0255(co*dst.at<Vec3b>(i,j)[2]+0.5);
    ////    }
    ////}
    //imshow("Face detection in color images", dst);
    //cv::waitKey();
}



void MatApp::MatLookUp(Mat& f_img, Mat& f_out /*= Mat()*/)
{
    int dim(256);
    Mat lookup(1, &dim, CV_8U);
    for (int i = 0; i < 256; i++)
        lookup.at<uchar> (i) = 255 - i;     //填充查找表
    cv::LUT(f_img, lookup, f_out);          //应用查找表
}

cv::MatND MatApp::HistStretching(Mat& f_img, Mat& f_out /*= Mat()*/, Mat& histImg /*= Mat()*/, int minValue /*= 100*/)
{
    //求出图像的直方图
    cvtColor(f_img, f_img, CV_RGB2GRAY);       //灰度模式
    MatND t_hist = MatHistogram().GetGrayHist(f_img, Mat(), Mat());
    //过滤两边没有的值
    int imin = 0, imax = 255;
    for (; imin < 255; imin++)
        if (t_hist.at<float> (imin) > minValue)
            break;
    for (; imax > 0; imax--)
        if (t_hist.at<float> (imax) > minValue)
            break;
    int dim(256);
    Mat lookup(1, &dim, CV_8U);
    for (int i = 0; i < 256; i++)
    {
        if (i < imin) lookup.at<uchar> (i) = 0;
        else if (i > imax) lookup.at<uchar> (i) = 255;
        else lookup.at<uchar> (i) = static_cast<uchar>(255.0 * (i - imin) / (imax - imin) + 0.5);
    }
    cv::LUT(f_img, lookup, f_out);          //应用查找表
    MatHistogram().GetGrayHist(f_out, Mat(), histImg);
    return histImg;
}

void MatApp::ColorHistStretching(Mat& f_img, Mat& f_out /*= Mat() */)
{
}

void MatApp::HistObjectFinder(const Mat& f_img /*= Mat()*/, Mat& findimg /*= Mat() */)
{
    //Mat t2matshow = f_img.clone();
    //imshow("原图", f_img);          //显示原图
    //rectangle(t2matshow, Rect(360, 55, 40, 50), Scalar(0));   //截取区域画出
    //imshow("截取区域对比显示", t2matshow);
    Mat t_img = f_img.clone();
    cvtColor(t_img, t_img, CV_BGR2GRAY);                        //灰度模式
    //直方图归一化
    MatND hisCurr = MatHistogram().GetGrayHist(findimg, Mat(), Mat());
    cv::normalize(hisCurr, hisCurr, 1.0);    //cv::normalize(shistogram,shistogram,1.0,cv::NORM_L2);
    //反投影直方图
    float hranges[] = { 0.0, 255.0 };
    int channels[] = { 0, 1, 2 };
    const float* ranges[] = { hranges, hranges, hranges };
    Mat result, tmp;
    cv::calcBackProject(&t_img, 1, channels, hisCurr, result, ranges, 255.0);
    //过滤阈值的方法  -0.1f
    //result.convertTo(tmp, CV_8U, -1.0, 255.0);
    //imshow("result", tmp);
    //过滤阈值的方法  0.12f
    cv::threshold(result, result, 255 * 0.12f, 255, cv::THRESH_BINARY);
    imshow("灰度模式找直方图区域", result);
    waitKey(0);
}

void MatApp::ColorHistObjectFinder(const Mat& f_img, Mat& findimg /*= Mat() */)
{
    //彩色图降低一下颜色数
    Mat t_img = f_img.clone();
    MatPixelsTool().ColorReduce(t_img, t_img, 32);
    MatPixelsTool().ColorReduce(findimg, findimg, 32);
    imshow("t_img", t_img);
    imshow("findimg", findimg);
    //计算彩色直方图
    MatND hisCurr = MatHistogram().GetColorHist(findimg, Mat(), Mat());
    //直方图归一化
    cv::normalize(hisCurr, hisCurr, 1.0);    //cv::normalize(shistogram,shistogram,1.0,cv::NORM_L2);
    //反投影直方图
    float hranges[] = { 0.0, 255.0 };
    int channels[] = { 0, 1, 2 };
    const float* ranges[] = { hranges, hranges, hranges };
    Mat result;
    cv::calcBackProject(&t_img, 1, channels, hisCurr, result, ranges, 255.0);
    //过滤阈值的方法  0.05f
    cv::threshold(result, result, 255 * 0.05f, 255, cv::THRESH_BINARY);
    imshow("result", result);
}

void MatApp::HistMeanShift(const Mat& f_img, Mat& findimg /*= Mat() */)
{
    Mat t_img = f_img.clone(),  hsv;
    //样本图片
    findimg = f_img(cv::Rect(110, 260, 35, 40));
    //计算样本彩色色调直方图
    cv::MatND colorhist = MatHistogram().GetHSHist(findimg);
    normalize(colorhist, colorhist, 1.0);
    //转换HSV
    cvtColor(t_img, hsv, CV_BGR2HSV);
    //分割图像
    vector<cv::Mat> v;
    cv::split(hsv, v);
    //消除与低饱和像素
    threshold(v[1], v[1], 65, 255, cv::THRESH_BINARY);    //cv::imshow("Saturation", v[1]);
    //返回投影色调直方图
    float hranges[] = { 0.0f , 180.0f};
    const float* ranges[] = { hranges, hranges, hranges };
    int channels[] = { 0 } ;
    cv::Mat result;
    cv::calcBackProject(&hsv, 1, channels, colorhist, result, ranges, 255.0);
    cv::threshold(result, result, 255 * 0.2f, 255, cv::THRESH_BINARY);    //cv::imshow("Result Hue", result);
    //与v[1]计算得到过滤区
    cv::bitwise_and(result, v[1], result);
    cv::imshow("Result Hue and", result);
    cv::Rect rect(110, 260, 35, 40);
    cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER, 10, 0.01);
    cout << "meanshift= " << cv::meanShift(result, rect, criteria) << endl;
    cv::rectangle(t_img, rect, cv::Scalar(0, 255, 0));
    cv::imshow("Image 2 result", t_img);
    waitKey(0);
}

double MatApp::HistImgComparator(const Mat& f_img, const Mat& f_img2)
{
    Mat A = f_img.clone(),  B = f_img2.clone();
    //1.降低颜色数为32位
    MatPixelsTool().ColorReduce(A, A, 32);
    //2.计算彩色直方图
    MatND hisA = MatHistogram().GetColorHist(A, Mat(), Mat());
    MatND hisB = MatHistogram().GetColorHist(B, Mat(), Mat());
    //3.生成直方图相似对比值 (注意返回的是两图像的共有颜色数)
    return cv::compareHist(hisA, hisB, CV_COMP_INTERSECT);
}

cv::Mat MatApp::WaterCut(const Mat& f_img, Mat& f_out/*=Mat() */)
{
    Mat t_img = f_img.clone();
    //取二值图
    cvtColor(t_img, t_img, CV_RGB2GRAY);
    threshold(t_img, t_img, 60, 255, THRESH_BINARY_INV);
    //通过膨胀和腐蚀 过滤噪点
    Mat fg, bg;
    erode(t_img, fg, cv::Mat(), cv::Point(-1, -1), 6);       //移除噪点和微小物体      imshow("显示前景",fg);
    dilate(t_img, bg, cv::Mat(), cv::Point(-1, -1), 6);       //识别不包含物体的像素
    threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);    //imshow("显示背景",bg);
    //记下标记图像
    cv::Mat markers(t_img.size(), CV_8U, cv::Scalar(0));
    markers = fg + bg;
    imshow("标记图像", markers);
    waitKey(0);
    //-------------------------------------------------------------
    //将标记图像转成整数图像
    markers.convertTo(markers, CV_32S);
    //用分水岭算法
    cv::watershed(f_img, markers);    //注意不要忘记此处使用彩色图
    Mat Segmentation, Watersheds;
    //以图像的形式显示结果
    markers.convertTo(Segmentation, CV_8U);
    imshow("图像的形式显示", Segmentation);
    //以分水岭的形式显示结果
    markers.convertTo(Watersheds, CV_8U, 255, 255);
    imshow("分水岭显示", Watersheds);
    waitKey(0);
    return Watersheds;
    //------------分水岭算法实例2--------------------------------
    Mat image = cv::imread("tower.jpg");
    //标记图像
    Mat imageMask(image.size(), CV_8U, cv::Scalar(0));
    // 设置背景像素(注意 不要看错颜色的标记)
    rectangle(imageMask, Point(5, 5), Point(image.cols - 5, image.rows - 5), Scalar(255), 3);
    // 设置前景像素
    rectangle(imageMask, Point(image.cols / 2 - 10, image.rows / 2 - 10), Point(image.cols / 2 + 10, image.rows / 2 + 10), Scalar(1), 10);
    cv::watershed(image, imageMask);    //注意不要忘记此处使用彩色图
    Mat TwoWatersheds;
    //以分水岭的形式显示结果
    imageMask.convertTo(TwoWatersheds, CV_8U, 255, 255);
    imshow("两种标记 分水岭显示", TwoWatersheds);
    waitKey(0);
}

cv::Mat MatApp::GrabCutMat(const Mat& f_img, Mat& f_out/*=Mat() */)
{
    // 定义前景物体的包围盒
    Rect rectangle(50, 70, f_img.cols - 150, f_img.rows - 180);
    Mat result;                     // 分割(4种可能的值)
    Mat bgModel, fgModel; // 模型(供内部使用)
    // GrabCut 分割
    grabCut(f_img,      // input image
            result,                     // 分割结果
            rectangle,               // 包含前景物体的矩形
            bgModel, fgModel, // 模型
            1,                            // 迭代次数
            GC_INIT_WITH_RECT);  // 使用矩形进行初始化
    // 得到可能为前景的像素
    compare(result, GC_PR_FGD, result, CMP_EQ);
    // 生成输出图像
    Mat foreground(f_img.size(), CV_8UC3, Scalar(255, 255, 255));
    f_img.copyTo(foreground, result);    // 不复制背景像素
    imshow("显示前景扣出来的图 ", foreground);
    waitKey(0);
    return foreground;
    //--------例子2 找水牛---------------------------------------
    Mat image = cv::imread("group.jpg");
    Rect rectangle2(10, 100, 380, 180);
    Mat bkgModel, fgrModel; // the models (internally used)
    Mat result2;
    cv::grabCut(image, result2, rectangle2, bkgModel, fgrModel, 5, cv::GC_INIT_WITH_RECT);
    // 使用按位与 核对第1个位
    result2 = result2 & 1;
    cv::Mat foreground_img(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
    foreground_img.create(image.size(), CV_8UC3);
    foreground_img.setTo(cv::Scalar(255, 255, 255));
    image.copyTo(foreground_img, result2);    // bg pixels not copied
    imshow("显示抠出来的水牛", foreground_img);
    cv::waitKey();
    return foreground_img;
}

cv::Mat MatApp::ImgFloodfill(const Mat& f_img, Mat& f_out /*= Mat() */)
{
    f_out = f_img.clone();
    Rect ccomp;
    floodFill(f_out, Point(50, 300), Scalar(155, 255, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
    return f_out;
}

void MatApp::LoopArrowShow(void)
{
    Mat loopshow = Mat(512, 160, CV_8UC3, Scalar(255, 255, 255)), loop2;
    for (;;)
        for (int j = 0; j <= 50; j += 5)   //50是间距
        {
            loopshow = Mat(512, 160, CV_8UC3, Scalar(255, 255, 255));
            for (int i = 0; i < 12; i++)
                //MatDraw().DrawLoopArrow(loopshow,Point( 200,50*i +j ));
                MatFiltration().InverseProjection(MatDraw().DrawLoopArrow(loopshow, Point(80, 50 * i + j - 50)), loop2);
            imshow("原图", loopshow);
            imshow("逆投影变换图", loop2);
            waitKey(1);
        }
}

void MatApp::TestLoopArrowShow(const char* f_name)
{
    Mat t_img, f_img = imread(f_name);
    Mat loopshow = Mat(595, 800, CV_8UC3, Scalar(0, 0, 0)), loop2;
    for (;;)
        //for (int j=0;j<=50;j+=5)  //50是间距
        for (int j = 50; j >= 0; j -= 5)   //50是间距
        {
            double t = (double) cvGetTickCount();
            t_img = f_img.clone();
            loopshow = Mat(595, 800, CV_8UC3, Scalar(0, 0, 0));
            for (int i = 0; i < 13; i++)       //绘制个数
                MatFiltration().ToAirscape(MatDraw().DrawLoopArrow(loopshow, Point(410, 50 * i + j - 50)), loop2, false);          //绘制坐标
            cv::GaussianBlur(loop2, loop2, Size(3, 3), 1.5);       //滤波处理
            MatPixelsTool().MatAddMat(t_img, loop2, Point(0, 0), 1);
            t = (double) cvGetTickCount() - t;
            printf("detection time = %g ms\n", t / ((double) cvGetTickFrequency() * 1000.));
            //imshow("原图", loopshow);
            //imshow("逆投影变换图", loop2);
            imshow("最后图", t_img);
            waitKey(1);
        }
    //-------------main---------------------------------------------------------------
    //        Mat t_tmp = Mat(600, 800, CV_8UC3, Scalar(0, 0, 0) ) ,tmp2;
    //        vector<Point> pts;
    //        pts.push_back( Point(250,50) );
    //        pts.push_back( Point(290, 140) );
    //        pts.push_back( Point(305, 235) );
    //        pts.push_back( Point(365, 305) );
    //        pts.push_back( Point(375,400) );
    //        pts.push_back( Point(375,401) );    //结尾点虚拟
    //        MatDraw().TrackArrowShow(t_tmp,pts);
    //        MatFiltration().InverseProjection( t_tmp,tmp2 );   //逆投影
    //        //MatPixels().ColorGetROI(tmp2,tmp2);
    //        imwrite("png2.bmp",t_tmp);
    //        imshow("弯曲轨迹鸟瞰图",t_tmp);
    //        imshow("投影弯曲轨迹",tmp2);
    //        waitKey(0);
    //-----------------------------------------------------------------------------------
}
