//***********************************************************************
//
//       ͼ�������������ܽ�
//
//***********************************************************************

#ifndef CHESFEATURES_H__
#define CHESFEATURES_H__


//ԭʼ��LBP�㷨
//ʹ��ģ�����
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


//�������ѧϰ
class MatDetection
{
private:
    double PI;
public:
    MatDetection() : PI(3.1415926) {}
public:
    //cvAcc�����Խ�8λ��������ͼ���ۼ�Ϊ����ͼ��
    //cvAdaptiveThreshold��ͼ������Ӧ��ֵ

    //Sobel���� Canny����
    Mat CannyMat(const Mat& f_img, Mat& contoursInv);
    //������ֱ��
    Mat CheckLines(const Mat& f_img, Mat& f_out);
    //���ʻ�����������ֱ��
    Mat GetOneLine(const Mat& f_img, Mat& f_out);
    //����任��⳵����
    Mat CheckRoadEdge(const Mat& f_img, Mat& f_out);
    //������Բ
    Mat CheckCircle(const Mat& f_img, Mat& f_out);
    //��չ��������Բ��3����

    //��ͨ�������� ������˲���
    //cvDrawContours������������
    //cvApproxPoly��ʹ�ö���αƽ�һ��������
    //cvContourPerimeter���������ȣ�
    //cvContoursMoments�����������أ�

    //��ȡ��ͨ������
    void GetConnectivity(const Mat& f_img);
    //������ͨ�������״(��������)
    void RectConnectivity(void);
};

//�����������ѧϰ
class CharacteristicPt
{
public:
    CharacteristicPt();
    virtual ~CharacteristicPt();
public:
    //sift����
    void SiftFeature(Mat& In_img) const;
    //siftƥ��Ա�
    void SiftFeature(Mat& In_img, Mat& In_img2) const;
    //SGBMͼ�����Ա�
    void compareMat(Mat& In_img, Mat& In_img2) const;

};




#endif
