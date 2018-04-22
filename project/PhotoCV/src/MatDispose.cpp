
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
    //����һ����ɫ����ɫͼ
    Mat mMat = Mat(512, 512, CV_8UC3, Scalar(250, 250, 250));
    for (int i = 0; i < mMat.rows; i++)     //����
        for (int j = 0; j < mMat.cols; j++)
        {
            mMat.at<Vec3b> (i, j) [0] = rand() % 255;      //��ɫ
            mMat.at<Vec3b> (i, j) [1] = rand() % 255;      //��ɫ
            mMat.at<Vec3b> (i, j) [2] = rand() % 255;      //��ɫ
        }
    imshow("mMat", mMat);
    waitKey(0);
    //����PNGͼ
    //mMat.isContinuous();     //�������ж�ͼ�������Ƿ�Ϊ������
    //�����Ҷ�ͼ
    //mMat = imread("1.jpg",0);
    //mMat = Mat(512, 512, CV_8U, Scalar(250, 250, 250) );
    Mat result;
    result.create(mMat.rows, mMat.cols, mMat.type());
    //���лҶ�ֵ
    int datavalue = 0;
    Mat gray_img = Mat(16, 16, CV_8U, Scalar(0));
    for (int i = 0; i < gray_img.rows; i++)
        for (int j = 0; j < gray_img.cols; j++)
            gray_img.at<uchar> (i, j) = datavalue++;
    imshow("gray_img", gray_img);
    imwrite("grayimg.jpg", gray_img);
    resize(gray_img, gray_img, cv::Size(640, 640));
    imshow("�Ҷ���������0-255", gray_img);
    //���в�ɫ����
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
    imshow("��ɫ��������0-16777216", Color_img);
    imwrite("Colorimg.jpg", Color_img);
    waitKey(0);
}

Mat MatPixelsTool::AntithesisMat(const Mat& In_img, Mat& Out_img, double f_alpha, int f_beta)
{
    if (!In_img.data)
        return Mat(512, 512, CV_8U, Scalar(0));       //���ذװ�
    Mat tmp = In_img.clone();
    Out_img = Mat::zeros(tmp.size(), tmp.type());
    for (int i = 0; i < tmp.rows; i++)
        for (int j = 0; j < tmp.cols; j++)
            for (int c = 0; c < 3; c++)
                //�������������ܲ���������������Ҫ��ʽת��
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
        if (abs(((*it) [0] - f_color[0]) + ((*it) [1] - f_color[1]) + ((*it) [2] - f_color[2])) < ff)                  //��ɫ����� ��ֵ
            //if (static_cast<int>(cv::norm<int, 3>(cv::Vec3i(f_color[0] - (*it)[0], f_color[1] - (*it)[1], f_color[2] - (*it)[2]))) < ff)
            *itout = 255;   //��ɫ
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

Mat MatPixelsTool::MatResize(const Mat& In_img, Mat& Out_img, double val)
{
    //f_val ���ű���
    Size2d dsize(In_img.cols * val, In_img.rows * val);    // ������ͼƬ�Ĵ�С
    //f_outimg(dsize,CV_32S);// ����һ���µ�Mat��opencv�ľ����������ͣ�
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
    ////����ж�
    //if (f_rect.x > fx || f_rect.x <= 0) f_rect.x = 0;
    //if (f_rect.y > fy || f_rect.y <= 0) f_rect.y = 0;
    ////�յ��ж�
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
                //ע�����㷨�ɸ���ŷ������Ľ� ��ֵ������
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
    //���ǳߴ�����
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
        //��ͨ���Ĵ�����
        //===========================================
        for (int i = 0; i < f_img.rows; i++)     //����
            for (int j = 0; j < f_img.cols; j++)
                f_img.at<uchar> (i, j) = 0;
        //=========����ָ���ٶȻ����=======================
        for (int i = 0; i < f_img.rows; i++)
        {
            uchar* data = f_img.ptr<uchar> (i);   //ÿ�е�ָ��
            for (int j = 0; j < f_img.cols; j++)
                data[j] = 0;
        }
    }
    else    //��ɫ��ͨ��ͼ�Ĵ���
    {
        //===========================================
        for (int i = 0; i < f_img.rows; i++)     //����
            for (int j = 0; j < f_img.cols; j++)
            {
                f_img.at<Vec3b> (i, j) [0] = 0;        //��ɫ
                f_img.at<Vec3b> (i, j) [1] = 255;      //��ɫ
                f_img.at<Vec3b> (i, j) [2] = 0;        //��ɫ
            }
        //===========================================
        t = (double) cvGetTickCount() - t;
        printf("time = %g ms\n", t / ((double) cvGetTickFrequency() * 1000.));
        //���ж��ַ�������
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
                rgba[3] = 0;    //͸��
        }
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);    //png��ʽ�£�Ĭ�ϵĲ���Ϊ3.
    imshow("��ʾ����", tmp);
    imwrite(f_name, tmp, compression_params);
}


cv::Mat MatFiltration::GetGrayMat(const Mat& In_img)
{
    Mat gray;
    //ͼ��ռ�ת��
    cvtColor(In_img, gray, CV_RGB2GRAY);    //�Ҷ�ģʽ
    return gray;
}

cv::Mat MatFiltration::Binarization(const Mat& In_img, Mat& Out_img, int ff /*= 60*/)
{
    //��ֵͼ
    //cv::threshold(GetGrayMat(In_img), Out_img, ff, 255, THRESH_BINARY_INV);
    //��ֵͼ��ȡ��
    cv::threshold(GetGrayMat(In_img), Out_img, ff, 255, THRESH_BINARY);
    return Out_img;
}

cv::Mat MatFiltration::Sharpen2D(const Mat& f_img, Mat& f_out)
{
    /*  ����һ
        //תΪ�Ҷ�ͼ
        Mat img;
        cvtColor(f_img,img,CV_RGB2GRAY);
        f_outimg.create(img.rows,img.cols,img.type());
        //ʹ��3*3�˲��������Ա����������в��ܰ���ͼ������Χ��һȦ
        for(int i = 1;i < img.rows-1;i++)
        {   //ǰһ�С���ǰ�С���һ�е�ָ��
        uchar* previous = img.ptr< uchar>(i-1);
        uchar* current  = img.ptr< uchar>(i);
        uchar* next     = img.ptr< uchar>(i+1);
        //������ͼ�����ָ��
        uchar* output = f_outimg.ptr<uchar>(i);
        for(int j = 1;j < img.cols - 1;j++)
        {
            //���ͼ��ı���ָ���뵱ǰ�е�ָ��ͬ������, ��ÿ�е�ÿһ�����ص��ÿһ��ͨ��ֵΪһ��������, ��ΪҪ���ǵ�ͼ���ͨ����
             output++= cv::saturate_cast<uchar>(5*current[j]-current[j-1]-current[j+1]-previous[j]-next[j]);
            //saturate_cast<uchar>�ὫС��0�����㣬����255�ĸ�Ϊ255
        }
        }
        f_outimg.row(0).setTo(cv::Scalar(0));
        f_outimg.row(f_outimg.rows-1).setTo(cv::Scalar(0));
        f_outimg.col(0).setTo(cv::Scalar(0));
        f_outimg.col(f_outimg.cols-1).setTo(cv::Scalar(0));
    */
    /*  ����2�����˲����������ͼ�����*/
    cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));       // Construct kernel (all entries initialized to 0)
    // assigns kernel values
    kernel.at<float> (1, 1) = 5.0;      // ����������
    kernel.at<float> (0, 1) = -1.0;
    kernel.at<float> (2, 1) = -1.0;
    kernel.at<float> (1, 0) = -1.0;
    kernel.at<float> (1, 2) = -1.0;
    f_out = f_img.clone();
    cv::filter2D(f_img, f_out, f_out.depth(), kernel);    //�����˲�����
    return f_out;
    //--------------��д
    //Mat sharpenedLenakernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    //filter2D(f_img,f_out,f_out.depth(),sharpenedLenakernel);  //�����˲�����
}

void MatFiltration::FiltersImg(const Mat& f_img)
{
    // ��˹�˲�
    Mat result;
    GaussianBlur(f_img, result, cv::Size(5, 5), 1.5);
    imshow("��˹�˲�", result);
    blur(f_img, result, cv::Size(5, 5));
    imshow("��ͨ�˲�", result);
    medianBlur(f_img, result, 5);
    imshow("��ֵ�˲�", result);
    waitKey(0);
}

cv::Mat MatFiltration::MatRotate(const Mat& f_img, Mat& f_out, double angle)
{
    // ����Ŀ��ͼ��Ĵ�С��������Դͼ��һ��
    Mat t_dstImg_warp = Mat::zeros(f_img.rows, f_img.cols, f_img.type());
    //��6����ͼ��������ź�����ת
    // ������ͼ���е�˳ʱ����ת50����������Ϊ0.6����ת����
    Point center = Point(t_dstImg_warp.cols / 2, t_dstImg_warp.rows / 2);
    //double angle = -30.0;     //-30.0����ת30�� 30.0����ת
    double scale = 1.0;         //���ű���
    // ͨ���������תϸ����Ϣ�����ת����
    Mat rotMat(30, 30, CV_32FC1);
    rotMat = getRotationMatrix2D(center, angle, scale);
    // ��ת�����ź��ͼ��
    warpAffine(f_img, f_out, rotMat, t_dstImg_warp.size());
    return f_out;
}

cv::Mat MatFiltration::WarpAffineImg(const Mat& f_img, Mat& f_out)
{
    return f_out;
}

cv::Mat MatFiltration::InverseProjection(const Mat& f_img, Mat& f_out)
{
    long nnn = 60;            //ע��ȡֵ��Χ
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
    //��������ϸ���Ӳο�
    Mat eroded, dilated;
    cv::erode(f_img, eroded, cv::Mat());
    cv::dilate(f_img, dilated, cv::Mat());
    imshow("��ʴͼ��", eroded);
    imshow("����ͼ��", dilated);
    Mat element(7, 7, CV_8U, cv::Scalar(1));
    cv::erode(f_img, eroded, element);
    cv::imshow("��ʴͼ��7x7", eroded);
    erode(f_img, eroded, cv::Mat(), cv::Point(-1, -1), 3);
    imshow("��ʴͼ��3��", eroded);
    Mat element5(5, 5, CV_8U, cv::Scalar(1)), opened, closed;
    cv::morphologyEx(f_img, opened, cv::MORPH_OPEN, element5);
    imshow("������", opened);
    cv::morphologyEx(f_img, closed, cv::MORPH_CLOSE, element5);
    imshow("������", closed);
    Mat morp = f_img.clone();
    cv::morphologyEx(morp, morp, cv::MORPH_CLOSE, element5);
    cv::morphologyEx(morp, morp, cv::MORPH_OPEN, element5);
    imshow("��̬ѧ�ݶ� �ȱպ�", morp);
    Mat morp2 = f_img.clone();
    cv::morphologyEx(morp2, morp2, cv::MORPH_OPEN, element5);
    cv::morphologyEx(morp2, morp2, cv::MORPH_CLOSE, element5);
    imshow("��̬ѧ�ݶ� �ȿ����", morp2);
    //morphologyEx(morp, morp, MORPH_TOPHAT, element);      //��ñ
    //morphologyEx(morp, morp, MORPH_BLACKHAT, element);    //��ñ
    return morp2;
}

cv::Mat MatFiltration::MorphoEdge(const Mat& f_img, Mat& f_out/*=Mat() */, int threshold/*=40*/)
{
    Mat t_img = f_img;
    //�ûҶ�ͼ����
    if (t_img.channels() > 1)
        cvtColor(t_img, t_img, CV_BGR2GRAY);
    //��̬ѧ�˲��仯
    cv::morphologyEx(t_img, f_out, cv::MORPH_GRADIENT, cv::Mat());
    //�õ���Ե��Ŷ
    if (threshold > 0)
        cv::threshold(f_out, f_out, threshold, 255, cv::THRESH_BINARY_INV);
    return f_out;
}

cv::Mat MatFiltration::GetCorners(const Mat& f_img, Mat& f_out/*=Mat() */)
{
    f_out = f_img.clone();
    Mat t_use;
    cvtColor(f_out, f_out, CV_RGB2GRAY);    //�Ҷ�ͼ
    //�ֱ𴴽�����Ԫ��
    Mat cross(5, 5, CV_8U, cv::Scalar(0));
    Mat diamond(5, 5, CV_8U, cv::Scalar(1));
    Mat square(5, 5, CV_8U, cv::Scalar(1));
    Mat x(5, 5, CV_8U, cv::Scalar(0));
    //����ʮ����
    for (int i = 0; i < 5; i++)
    {
        cross.at<uchar> (2, i) = 1;
        cross.at<uchar> (i, 2) = 1;
    }
    // ��������Ԫ��
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
    // ����X��Ԫ��
    for (int i = 0; i < 5; i++)
    {
        x.at<uchar> (i, i) = 1;
        x.at<uchar> (4 - i, i) = 1;
    }
    // ʮ��������
    cv::dilate(f_out, t_use, cross);
    // ���θ�ʴ
    cv::erode(t_use, t_use, diamond);
    cv::Mat result2;
    // X������
    cv::dilate(f_out, result2, x);
    // ���θ�ʴ
    cv::erode(result2, result2, square);
    // ͨ������ͼ������ֵ�õ��ǵ�ͼ��
    cv::absdiff(result2, t_use, t_use);
    // ��ֵ���õ���ֵͼ��
    //threshold(t_use, t_use, -1, 255, cv::THRESH_BINARY_INV);
    // ��̬ѧ�任
    cv::morphologyEx(t_use, t_use, cv::MORPH_TOPHAT, cv::Mat());
    cv::threshold(t_use, t_use, 40, 255, cv::THRESH_BINARY_INV);
    //��ʱ�õ����ǽǵ�ͼ
    imshow("Corners�ǵ�ͼ", t_use);
    //������ԭͼ�ϱ���ǵ�ͼ
    cv::Mat_<uchar>::const_iterator it = t_use.begin<uchar>();
    cv::Mat_<uchar>::const_iterator itend = t_use.end<uchar>();
    // for each pixel
    for (int i = 0; it != itend; ++it, ++i)
        if (!*it)
            cv::circle(f_out, cv::Point(i % f_out.step, i / f_out.step), 5, cv::Scalar(255, 0, 0));
    imshow("ԭͼ�ϱ���ǵ�ͼ", f_out);
    //waitKey(0);
    return f_out;
}

cv::Mat MatHistogram::DrawHist(MatND& hist)
{
    Mat histImg;
    cv::MatND histtmp = hist.clone();
    double maxVal = 0, minVal = 0;
    int histSize[] = { 256 };                       //�������
    cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);    //�����ֵ��Сֵ
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
        cvtColor(f_img, gray, CV_RGB2GRAY);        //ת�Ҷ�
    else
        gray = f_img;
    int channels[] = { 0 };                        //���õ�һ��ͨ��
    int histSize[] = { 256 };                      //�������
    float range[] = { 0, 256 };                    //���ص���С�����ֵ
    const float* ranges[] = { range };
    calcHist(&gray,
             1,          // histogram of 1 image only   [���㵥��ͼƬ��ֱ��ͼ]
             channels,   // the channel used            [ͨ������]
             cv::Mat(),  // no mask is used             [��ʹ��ͼ����Ϊ����]
             hist,       // the resulting histogram     [���ص�ֱ��ͼ]
             1,          // it is a 1D histogram
             histSize,   // number of bins              [�������]
             ranges      // pixel value range           [����ֵ�ķ�Χ]
            );
    cv::MatND histtmp = hist.clone();
    double maxVal = 0, minVal = 0;
    cv::minMaxLoc(histtmp, &minVal, &maxVal, 0, 0);    //�����ֵ��Сֵ
    histImg = Mat(histSize[0], histSize[0], CV_8U, cv::Scalar(255));       //����CV_8U
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
    // �趨bin��Ŀ
    int histSize[] = { 256, 256, 256 };
    // �趨ȡֵ��Χ ( R,G,B) )
    float hranges[] = { 0.0, 255.0 };
    const float* ranges[] = { hranges, hranges, hranges };
    int channels[] = { 0, 1, 2 };
    // Compute histogram
    calcHist(&f_img,
             1,          // ���㵥��ͼ���ֱ��ͼ
             channels,   // ͨ������
             cv::Mat(),  // ��ʹ��ͼ����Ϊ����
             hist,       // ֱ��ͼ���
             3,          // ����3Dֱ��ͼ
             histSize,   // �������
             ranges      // ����ֵ�ķ�Χ
            );
    //��ֱ��ͼ ��չ
    //return hist;
    //*************************Ϊ�˻���ֱ��ͼ**************************
    vector<Mat> rgbhist;
    vector<Mat> rgb_planes;
    split(f_img, rgb_planes);               // �ָ��3����ͨ��ͼ�� ( R, G �� B )
    int histSize2 = 255;                     // �趨bin��Ŀ
    float range[] = { 0, 255 };             // �趨ȡֵ��Χ ( R,G,B) )
    const float* histRange = { range };
    bool uniform(true), accumulate(false);
    Mat r_hist, g_hist, b_hist;
    // ����ֱ��ͼ:
    calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize2, &histRange, uniform, accumulate);
    calcHist(&rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize2, &histRange, uniform, accumulate);
    calcHist(&rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize2, &histRange, uniform, accumulate);
    rgbhist.push_back(r_hist);
    rgbhist.push_back(g_hist);
    rgbhist.push_back(b_hist);
    //������ӹ��Ʋ�����
    Mat t_hist = r_hist + g_hist + b_hist;
    // ����ֱ��ͼ����
    int hist_w = 256;
    int hist_h = 256;   //400
    int bin_w = cvRound((double) hist_w / histSize2);
    histImg = Mat(hist_w, hist_h, CV_8UC3, Scalar(255, 255, 255));
    // ��ֱ��ͼ��һ������Χ [ 0, histImage.rows ]
    normalize(r_hist, r_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(b_hist, b_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    // ��ֱ��ͼ�����ϻ���ֱ��ͼ
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
    //����H-S��άֱ��ͼ
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
    // ������ľ���Ϊͼ��
    double temp[256];
    // ����
    for (int i = 0; i < 256; i++)
        temp[i] = 0.0;
    // ����ÿ�����ص��ۻ�ֵ
    for (int m = 0; m < img.rows; m++)
    {
        // ��Ч�������еķ�ʽ
        const uchar* t = img.ptr<uchar> (m);
        for (int n = 0; n < img.cols; n++)
        {
            int i = t[n];
            temp[i] = temp[i] + 1;
        }
    }
    // ����ÿ�����صĸ���
    for (int i = 0; i < 256; i++)
        temp[i] = temp[i] / (img.rows * img.cols);
    double result = 0;
    // ���ݶ������ͼ����
    for (int i = 0; i < 256; i++)
        result = (temp[i] == 0.0 ? result : result - temp[i] * (log(temp[i]) / log(2.0)));
    return result;
    //�������ͼ����Ҳ���Բο�
    //float EntropyA::calEntropy( const Mat &image,int threshold )
    //{
    //    float total_P(0), Entropy(0), percent;
    //    Mat hist = getHistogram(image);
    //    const float* hist_p = (float*) hist.ptr<float>(0);
    //    for(int i = 0;i<threshold;i++)
    //        total_P  += hist_p[i];//�ܺ�
    //    for(int i = 0;i<threshold;i++)
    //    {
    //        if(hist_p[i]!=0)
    //        {
    //            percent = hist_p[i]/total_P;//ÿ�����ȸ���
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
    if (In_img.channels() == 1)         //�Ҷ�ͼ
    {
        equalizeHist(In_img, Out_img);
        return Out_img;
    }
    else
    {
        vector<Mat> rgb_planes;         //�ָ��3����ͨ��ͼ�� ( R, G �� B )
        split(In_img, rgb_planes);      //����ͨ�� �ֱ���⻯
        equalizeHist(rgb_planes[0], rgb_planes[0]);
        equalizeHist(rgb_planes[1], rgb_planes[1]);
        equalizeHist(rgb_planes[2], rgb_planes[2]);
        merge(rgb_planes, Out_img);     //�ϲ�ͨ��
        return Out_img;
    }
    return Out_img;
}

cv::Mat MatHistogram::AutoControst(Mat& In_img)
{
    //�����Զ��Աȶ�У��
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
    for (int y = 0; y < 256; y++)   //�����������һ����ܹ��˽���
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
    for (int y = 0; y < 256; y++)   //�����������һ����ܹ��˽���
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
    for (int y = 0; y < 256; y++)   //�����������һ����ܹ��˽���
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
    /////////�Զ�ɫ��
    //�Զ��Աȶ�
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
                //  BlueMap(Y) = (Y - MinBlue) / (MaxBlue - MinBlue) * 255      '��������
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
                //  BlueMap(Y) = (Y - MinBlue) / (MaxBlue - MinBlue) * 255      '��������
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
                //  BlueMap(Y) = (Y - MinBlue) / (MaxBlue - MinBlue) * 255      '��������
                float ftmp = (float)(y - imingreen) / (imaxgreen - imingreen);
                greenmap[y] = (int)(ftmp * 255);
            }
        }
    }
    //���
    for (int i = 0; i < In_img.rows; i++)
    {
        for (int j = 0; j < In_img.cols; j++)
        {
            In_img.at<Vec3b> (i, j) [0] = bluemap[In_img.at<Vec3b> (i, j) [0]];
            In_img.at<Vec3b> (i, j) [1] = greenmap[In_img.at<Vec3b> (i, j) [1]];
            In_img.at<Vec3b> (i, j) [2] = redmap[In_img.at<Vec3b> (i, j) [2]];
        }
    }
    imshow("�Զ��Աȶ�", In_img);
    waitKey(0);
    return In_img;
    //////////////////////////////////////////////////////////////////////////
    //Mat src = imread("../DllRes/IMG/road.jpg");
    //imshow("ԭʼͼƬ", src);
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
    //        //����Ҷ�ֵ
    //        int gray = (r * 299 + g * 587 + b * 114) / 1000;
    //        histogram[gray]++;
    //    }
    //}
    //int calnum = 0;
    //int total = dst.rows * dst.cols;
    //int num;
    ////�����ѭ���õ�����ϵ��thresholdco���ٽ�Ҷȼ�
    //for (int i = 0; i < 256; i++)
    //{
    //    if ((float)calnum / total < thresholdco) //�õ�ǰ5%�ĸ������ء�
    //    {
    //        calnum += histogram[255 - i];//histogram�������ĳһ�Ҷ�ֵ�����ظ���,calnum�Ǳ߽�Ҷ�֮�ϵ�������
    //        num = i;
    //    } else break;
    //}
    //int averagegray = 0;
    //calnum = 0;
    ////�õ����������������ܵĻҶ�ֵ
    //for (int i = 255; i >= 255 - num; i--)
    //{
    //    averagegray += histogram[i] * i; //�ܵ����صĸ���*�Ҷ�ֵ
    //    calnum += histogram[i]; //�ܵ�������
    //}
    //averagegray /= calnum;
    ////�õ����߲�����ϵ��
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
        lookup.at<uchar> (i) = 255 - i;     //�����ұ�
    cv::LUT(f_img, lookup, f_out);          //Ӧ�ò��ұ�
}

cv::MatND MatApp::HistStretching(Mat& f_img, Mat& f_out /*= Mat()*/, Mat& histImg /*= Mat()*/, int minValue /*= 100*/)
{
    //���ͼ���ֱ��ͼ
    cvtColor(f_img, f_img, CV_RGB2GRAY);       //�Ҷ�ģʽ
    MatND t_hist = MatHistogram().GetGrayHist(f_img, Mat(), Mat());
    //��������û�е�ֵ
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
    cv::LUT(f_img, lookup, f_out);          //Ӧ�ò��ұ�
    MatHistogram().GetGrayHist(f_out, Mat(), histImg);
    return histImg;
}

void MatApp::ColorHistStretching(Mat& f_img, Mat& f_out /*= Mat() */)
{
}

void MatApp::HistObjectFinder(const Mat& f_img /*= Mat()*/, Mat& findimg /*= Mat() */)
{
    //Mat t2matshow = f_img.clone();
    //imshow("ԭͼ", f_img);          //��ʾԭͼ
    //rectangle(t2matshow, Rect(360, 55, 40, 50), Scalar(0));   //��ȡ���򻭳�
    //imshow("��ȡ����Ա���ʾ", t2matshow);
    Mat t_img = f_img.clone();
    cvtColor(t_img, t_img, CV_BGR2GRAY);                        //�Ҷ�ģʽ
    //ֱ��ͼ��һ��
    MatND hisCurr = MatHistogram().GetGrayHist(findimg, Mat(), Mat());
    cv::normalize(hisCurr, hisCurr, 1.0);    //cv::normalize(shistogram,shistogram,1.0,cv::NORM_L2);
    //��ͶӰֱ��ͼ
    float hranges[] = { 0.0, 255.0 };
    int channels[] = { 0, 1, 2 };
    const float* ranges[] = { hranges, hranges, hranges };
    Mat result, tmp;
    cv::calcBackProject(&t_img, 1, channels, hisCurr, result, ranges, 255.0);
    //������ֵ�ķ���  -0.1f
    //result.convertTo(tmp, CV_8U, -1.0, 255.0);
    //imshow("result", tmp);
    //������ֵ�ķ���  0.12f
    cv::threshold(result, result, 255 * 0.12f, 255, cv::THRESH_BINARY);
    imshow("�Ҷ�ģʽ��ֱ��ͼ����", result);
    waitKey(0);
}

void MatApp::ColorHistObjectFinder(const Mat& f_img, Mat& findimg /*= Mat() */)
{
    //��ɫͼ����һ����ɫ��
    Mat t_img = f_img.clone();
    MatPixelsTool().ColorReduce(t_img, t_img, 32);
    MatPixelsTool().ColorReduce(findimg, findimg, 32);
    imshow("t_img", t_img);
    imshow("findimg", findimg);
    //�����ɫֱ��ͼ
    MatND hisCurr = MatHistogram().GetColorHist(findimg, Mat(), Mat());
    //ֱ��ͼ��һ��
    cv::normalize(hisCurr, hisCurr, 1.0);    //cv::normalize(shistogram,shistogram,1.0,cv::NORM_L2);
    //��ͶӰֱ��ͼ
    float hranges[] = { 0.0, 255.0 };
    int channels[] = { 0, 1, 2 };
    const float* ranges[] = { hranges, hranges, hranges };
    Mat result;
    cv::calcBackProject(&t_img, 1, channels, hisCurr, result, ranges, 255.0);
    //������ֵ�ķ���  0.05f
    cv::threshold(result, result, 255 * 0.05f, 255, cv::THRESH_BINARY);
    imshow("result", result);
}

void MatApp::HistMeanShift(const Mat& f_img, Mat& findimg /*= Mat() */)
{
    Mat t_img = f_img.clone(),  hsv;
    //����ͼƬ
    findimg = f_img(cv::Rect(110, 260, 35, 40));
    //����������ɫɫ��ֱ��ͼ
    cv::MatND colorhist = MatHistogram().GetHSHist(findimg);
    normalize(colorhist, colorhist, 1.0);
    //ת��HSV
    cvtColor(t_img, hsv, CV_BGR2HSV);
    //�ָ�ͼ��
    vector<cv::Mat> v;
    cv::split(hsv, v);
    //������ͱ�������
    threshold(v[1], v[1], 65, 255, cv::THRESH_BINARY);    //cv::imshow("Saturation", v[1]);
    //����ͶӰɫ��ֱ��ͼ
    float hranges[] = { 0.0f , 180.0f};
    const float* ranges[] = { hranges, hranges, hranges };
    int channels[] = { 0 } ;
    cv::Mat result;
    cv::calcBackProject(&hsv, 1, channels, colorhist, result, ranges, 255.0);
    cv::threshold(result, result, 255 * 0.2f, 255, cv::THRESH_BINARY);    //cv::imshow("Result Hue", result);
    //��v[1]����õ�������
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
    //1.������ɫ��Ϊ32λ
    MatPixelsTool().ColorReduce(A, A, 32);
    //2.�����ɫֱ��ͼ
    MatND hisA = MatHistogram().GetColorHist(A, Mat(), Mat());
    MatND hisB = MatHistogram().GetColorHist(B, Mat(), Mat());
    //3.����ֱ��ͼ���ƶԱ�ֵ (ע�ⷵ�ص�����ͼ��Ĺ�����ɫ��)
    return cv::compareHist(hisA, hisB, CV_COMP_INTERSECT);
}

cv::Mat MatApp::WaterCut(const Mat& f_img, Mat& f_out/*=Mat() */)
{
    Mat t_img = f_img.clone();
    //ȡ��ֵͼ
    cvtColor(t_img, t_img, CV_RGB2GRAY);
    threshold(t_img, t_img, 60, 255, THRESH_BINARY_INV);
    //ͨ�����ͺ͸�ʴ �������
    Mat fg, bg;
    erode(t_img, fg, cv::Mat(), cv::Point(-1, -1), 6);       //�Ƴ�����΢С����      imshow("��ʾǰ��",fg);
    dilate(t_img, bg, cv::Mat(), cv::Point(-1, -1), 6);       //ʶ�𲻰������������
    threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);    //imshow("��ʾ����",bg);
    //���±��ͼ��
    cv::Mat markers(t_img.size(), CV_8U, cv::Scalar(0));
    markers = fg + bg;
    imshow("���ͼ��", markers);
    waitKey(0);
    //-------------------------------------------------------------
    //�����ͼ��ת������ͼ��
    markers.convertTo(markers, CV_32S);
    //�÷�ˮ���㷨
    cv::watershed(f_img, markers);    //ע�ⲻҪ���Ǵ˴�ʹ�ò�ɫͼ
    Mat Segmentation, Watersheds;
    //��ͼ�����ʽ��ʾ���
    markers.convertTo(Segmentation, CV_8U);
    imshow("ͼ�����ʽ��ʾ", Segmentation);
    //�Է�ˮ�����ʽ��ʾ���
    markers.convertTo(Watersheds, CV_8U, 255, 255);
    imshow("��ˮ����ʾ", Watersheds);
    waitKey(0);
    return Watersheds;
    //------------��ˮ���㷨ʵ��2--------------------------------
    Mat image = cv::imread("tower.jpg");
    //���ͼ��
    Mat imageMask(image.size(), CV_8U, cv::Scalar(0));
    // ���ñ�������(ע�� ��Ҫ������ɫ�ı��)
    rectangle(imageMask, Point(5, 5), Point(image.cols - 5, image.rows - 5), Scalar(255), 3);
    // ����ǰ������
    rectangle(imageMask, Point(image.cols / 2 - 10, image.rows / 2 - 10), Point(image.cols / 2 + 10, image.rows / 2 + 10), Scalar(1), 10);
    cv::watershed(image, imageMask);    //ע�ⲻҪ���Ǵ˴�ʹ�ò�ɫͼ
    Mat TwoWatersheds;
    //�Է�ˮ�����ʽ��ʾ���
    imageMask.convertTo(TwoWatersheds, CV_8U, 255, 255);
    imshow("���ֱ�� ��ˮ����ʾ", TwoWatersheds);
    waitKey(0);
}

cv::Mat MatApp::GrabCutMat(const Mat& f_img, Mat& f_out/*=Mat() */)
{
    // ����ǰ������İ�Χ��
    Rect rectangle(50, 70, f_img.cols - 150, f_img.rows - 180);
    Mat result;                     // �ָ�(4�ֿ��ܵ�ֵ)
    Mat bgModel, fgModel; // ģ��(���ڲ�ʹ��)
    // GrabCut �ָ�
    grabCut(f_img,      // input image
            result,                     // �ָ���
            rectangle,               // ����ǰ������ľ���
            bgModel, fgModel, // ģ��
            1,                            // ��������
            GC_INIT_WITH_RECT);  // ʹ�þ��ν��г�ʼ��
    // �õ�����Ϊǰ��������
    compare(result, GC_PR_FGD, result, CMP_EQ);
    // �������ͼ��
    Mat foreground(f_img.size(), CV_8UC3, Scalar(255, 255, 255));
    f_img.copyTo(foreground, result);    // �����Ʊ�������
    imshow("��ʾǰ���۳�����ͼ ", foreground);
    waitKey(0);
    return foreground;
    //--------����2 ��ˮţ---------------------------------------
    Mat image = cv::imread("group.jpg");
    Rect rectangle2(10, 100, 380, 180);
    Mat bkgModel, fgrModel; // the models (internally used)
    Mat result2;
    cv::grabCut(image, result2, rectangle2, bkgModel, fgrModel, 5, cv::GC_INIT_WITH_RECT);
    // ʹ�ð�λ�� �˶Ե�1��λ
    result2 = result2 & 1;
    cv::Mat foreground_img(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
    foreground_img.create(image.size(), CV_8UC3);
    foreground_img.setTo(cv::Scalar(255, 255, 255));
    image.copyTo(foreground_img, result2);    // bg pixels not copied
    imshow("��ʾ�ٳ�����ˮţ", foreground_img);
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
        for (int j = 0; j <= 50; j += 5)   //50�Ǽ��
        {
            loopshow = Mat(512, 160, CV_8UC3, Scalar(255, 255, 255));
            for (int i = 0; i < 12; i++)
                //MatDraw().DrawLoopArrow(loopshow,Point( 200,50*i +j ));
                MatFiltration().InverseProjection(MatDraw().DrawLoopArrow(loopshow, Point(80, 50 * i + j - 50)), loop2);
            imshow("ԭͼ", loopshow);
            imshow("��ͶӰ�任ͼ", loop2);
            waitKey(1);
        }
}

void MatApp::TestLoopArrowShow(const char* f_name)
{
    Mat t_img, f_img = imread(f_name);
    Mat loopshow = Mat(595, 800, CV_8UC3, Scalar(0, 0, 0)), loop2;
    for (;;)
        //for (int j=0;j<=50;j+=5)  //50�Ǽ��
        for (int j = 50; j >= 0; j -= 5)   //50�Ǽ��
        {
            double t = (double) cvGetTickCount();
            t_img = f_img.clone();
            loopshow = Mat(595, 800, CV_8UC3, Scalar(0, 0, 0));
            for (int i = 0; i < 13; i++)       //���Ƹ���
                MatFiltration().ToAirscape(MatDraw().DrawLoopArrow(loopshow, Point(410, 50 * i + j - 50)), loop2, false);          //��������
            cv::GaussianBlur(loop2, loop2, Size(3, 3), 1.5);       //�˲�����
            MatPixelsTool().MatAddMat(t_img, loop2, Point(0, 0), 1);
            t = (double) cvGetTickCount() - t;
            printf("detection time = %g ms\n", t / ((double) cvGetTickFrequency() * 1000.));
            //imshow("ԭͼ", loopshow);
            //imshow("��ͶӰ�任ͼ", loop2);
            imshow("���ͼ", t_img);
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
    //        pts.push_back( Point(375,401) );    //��β������
    //        MatDraw().TrackArrowShow(t_tmp,pts);
    //        MatFiltration().InverseProjection( t_tmp,tmp2 );   //��ͶӰ
    //        //MatPixels().ColorGetROI(tmp2,tmp2);
    //        imwrite("png2.bmp",t_tmp);
    //        imshow("�����켣���ͼ",t_tmp);
    //        imshow("ͶӰ�����켣",tmp2);
    //        waitKey(0);
    //-----------------------------------------------------------------------------------
}
