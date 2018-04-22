//***********************************************************************
//
//       图像算子特征点总结
//
//***********************************************************************

#ifndef CHESFEATURES_H__
#define CHESFEATURES_H__


//原始的LBP算法
//使用模板参数
template <typename _Tp> static
void oLBP(InputArray _src, OutputArray _dst)
{
    // get matrices
    Mat src = _src.getMat();
    // allocate memory for result
    _dst.create(src.rows - 2, src.cols - 2, CV_8UC1);
    Mat dst = _dst.getMat();
    // zero the result matrix
    dst.setTo(0);
    // calculate patterns
    for (int i = 1; i < src.rows - 1; i++)
    {
        for (int j = 1; j < src.cols - 1; j++)
        {
            _Tp center = src.at<_Tp> (i, j);
            unsigned char code = 0;
            code |= (src.at<_Tp> (i - 1, j - 1) >= center) << 7;
            code |= (src.at<_Tp> (i - 1, j) >= center) << 6;
            code |= (src.at<_Tp> (i - 1, j + 1) >= center) << 5;
            code |= (src.at<_Tp> (i, j + 1) >= center) << 4;
            code |= (src.at<_Tp> (i + 1, j + 1) >= center) << 3;
            code |= (src.at<_Tp> (i + 1, j) >= center) << 2;
            code |= (src.at<_Tp> (i + 1, j - 1) >= center) << 1;
            code |= (src.at<_Tp> (i, j - 1) >= center) << 0;
            dst.at<unsigned char> (i - 1, j - 1) = code;
        }
    }
}


//基础检测学习
class MatDetection
{
private:
    double PI;
public:
    MatDetection() : PI(3.1415926) {}
public:
    //cvAcc：可以将8位整数类型图像累加为浮点图像；
    //cvAdaptiveThreshold：图像自适应阈值

    //Sobel算子 Canny算子
    Mat CannyMat(const Mat& f_img, Mat& contoursInv);
    //霍夫检测直线
    Mat CheckLines(const Mat& f_img, Mat& f_out);
    //概率霍夫检测检测最优直线
    Mat GetOneLine(const Mat& f_img, Mat& f_out);
    //霍夫变换检测车道线
    Mat CheckRoadEdge(const Mat& f_img, Mat& f_out);
    //霍夫检测圆
    Mat CheckCircle(const Mat& f_img, Mat& f_out);
    //扩展霍夫检测椭圆和3角形

    //连通域计算面积 长宽过滤操作
    //cvDrawContours：绘制轮廓；
    //cvApproxPoly：使用多边形逼近一个轮廓；
    //cvContourPerimeter：轮廓长度；
    //cvContoursMoments：计算轮廓矩；

    //提取连通域轮廓
    void GetConnectivity(const Mat& f_img);
    //计算联通区域的形状(补充上面)
    void RectConnectivity(void);
};

//特征点和算子学习
class CharacteristicPt
{
public:
    CharacteristicPt();
    virtual ~CharacteristicPt();
public:
    //sift特征
    void SiftFeature(Mat& In_img) const;
    //sift匹配对比
    void SiftFeature(Mat& In_img, Mat& In_img2) const;
    //SGBM图像差异对比
    void compareMat(Mat& In_img, Mat& In_img2) const;

};




#endif
