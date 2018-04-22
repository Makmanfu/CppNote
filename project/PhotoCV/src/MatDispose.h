//***********************************************************************
//       ͼ�������ѧϰ��¼���뼯
//       rows ������ �൱�� height
//       cols ������ �൱�� weight
//
//***********************************************************************
#ifndef MATDISPOSE_H__
#define MATDISPOSE_H__


C_API bool compare_image_cv(const char* inputFile, const char* outputFile, int resizeID);

//���ش���ѧϰ
class MatPixelsTool
{
public:
    //����������ɫ��������MAT�ķ���
    void TestColorMat(void);
    //���ر�������(ff��������)�����cookbook
    Mat ColorMatPixels(int ff = 0);
    //�ı�ͼ��ĶԱȶȺ�����
    Mat AntithesisMat(const Mat& In_img, Mat& Out_img, double f_alpha, int f_beta);
    //����һ����ɫ���سɺ�ɫ�� (������ɫ����ff)
    Mat ColorFindPixels(const Mat& In_img, Mat& Out_img, Scalar f_color, int ff = 10);
    //���ؽ�����ɫ����
    Mat ColorReduce(const Mat& In_img, Mat& Out_img, int div = 64);
    //ͼ������ں� [ff=0.5��͸��ˮӡЧ�� -1 ʱΪ���� -2ΪPng����]
    Mat MatAddMat(Mat& InOut_img, const Mat& Inlogo, Point Ptbegin, float ff = 0.5);
    //ͼ�����ű���
    Mat MatResize(const Mat& In_img, Mat& Out_img, double val = 0.5);
    //ͼ���ȡ[�����յ�]
    Mat CutRectMat(const Mat& In_img, Mat& Out_img, Point Ptbegin = Point(0, 0), Point Ptend = Point(0, 0));
    //ͼ���ȡ[����]
    Mat CutRectMat(const Mat& In_img, Mat& Out_img, Rect f_rect);
    //��ͨ��ɫ�������زü���ȡ(������ǰ��ɫ �������)
    Mat ColorGetROI(const Mat& In_img, Mat& Out_img, Scalar f_color = Scalar(0, 255, 0));
    //����������
    Mat MosaicPixels(const Mat& In_img, Mat& Out_img, int ff = 10);
    //��Mat����͸��PNG����
    void WritePng(const Mat& In_img, const char* f_name);
};

//ͼ��任����
class MatFiltration
{
public:
    //�Ҷ�ͼ(ȥɫ)
    Mat GetGrayMat(const Mat& In_img);
    //�����ֵ��ͼ(ffΪ���� 60 128)
    Mat Binarization(const Mat& In_img, Mat& Out_img, int ff = 60);
    //ͼ����--�˲�����ͼƬ
    Mat Sharpen2D(const Mat& f_img, Mat& f_out);
    //�����˲�  ģ�� ��˹ģ�� �����˲�
    void FiltersImg(const Mat& f_img);
    //ͼ�����ת (angle��ת�Ƕ�)
    Mat MatRotate(const Mat& f_img, Mat& f_out, double angle);
    //ͼ�񼸺α任����ת�����ţ�б�� ����任 ???
    Mat WarpAffineImg(const Mat& f_img, Mat& f_out);
    //��ͶӰ Ť�� ���� �任
    Mat InverseProjection(const Mat& f_img, Mat& f_out = Mat());
    //ͶӰ�任�������ͼ
    Mat ToAirscape(const Mat& f_img, Mat& f_out = Mat(), bool ff = true);
    //����Ҷ����
    Mat convolveDFT(Mat A, Mat B, Mat& C);
    //���Ը���Ҷ����
    void testDFT(const Mat& In_img);
    //��ʴ ���� ��̬ѧ�ݶ� ��ñ ��ñ
    Mat ImgMorpho(const Mat& f_img);
    //��̬ѧ�˲�����Ե
    Mat MorphoEdge(const Mat& f_img, Mat& f_out = Mat() , int threshold = 40);
    //��̬ѧ���ǵ�ķ���
    Mat GetCorners(const Mat& f_img, Mat& f_out = Mat());

    //Laplace��������˹�任,ͼ���Ե���
    //cvLogPolar��       ����������任��
    //cvDFT��            ��ɢ����Ҷ�任��
    //cvDCT��            ��ɢ���ұ任
    //cvDistTransform��ͼ��ľ���任
    //cvRodrigues2���޵����˹�任��
    //ͼ�������[���忴����]
    //cvPyrUp��ͼ��������������е�ͼ����ÿ��ά���϶��Ŵ�������
    //cvPyrSegmentation�����ý�����ʵ��ͼ��ָ

};

//ֱ��ͼ����ѧϰ
class MatHistogram
{
private:
    //��1άֱ��ͼ���ݻ���ͳ��ͼ�����㷨
    Mat DrawHist(MatND& hist);
public:
    //����Ҷ�ֱ��ͼ[ԭͼ,ֱ��ͼ����,ֱ��ͼͳ��ͼ]
    MatND GetGrayHist(Mat& In_img, MatND& hist = MatND(), Mat& histImg = Mat());
    //�����ɫֱ��ͼ(ԭͼ,ֱ��ͼ����,ֱ��ͼͳ��ͼ)
    MatND GetColorHist(Mat& In_img, MatND& hist = MatND(), Mat& histImg = Mat());
    SparseMat GetColorHist(Mat& In_img, SparseMat& hist = SparseMat(), Mat& histImg = Mat());
    //����H-S��άֱ��ͼ
    MatND GetHSHist(Mat& In_img, MatND& hist = Mat(), Mat& histImg = Mat());
    //����ͼ����
    double CalcEntropy(Mat& In_img);
public:
    //ֱ��ͼ�ľ��⻯
    Mat ColorHistEqualize(Mat& In_img, Mat& Out_img = Mat());
    //�Զ��Աȶ� ��Ҫ����?????
    Mat AutoControst(Mat& In_img);
};

//�߼�Ӧ��
class MatApp
{
    //���ұ�ı�ֱ��ͼ�������ɸ�Ƭ
    void MatLookUp(Mat& f_img, Mat& f_out = Mat());
    //ֱ��ͼ���췽���Ľ�ͼ��Աȶ�Contrast(�������˲���minValue����)
    MatND HistStretching(Mat& f_img, Mat& f_out = Mat(), Mat& histImg = Mat(), int minValue = 100);
    //��չ�����ɫֱ��ͼ�Ľ�ͼ��
    void ColorHistStretching(Mat& f_img, Mat& f_out = Mat());
    //��ͶӰֱ��ͼ����
    void HistObjectFinder(const Mat& f_img = Mat(), Mat& findimg = Mat());
    //��ɫ��ͶӰֱ��ͼ����
    void ColorHistObjectFinder(const Mat& f_img, Mat& findimg = Mat());
    //��ֵƯ���㷨
    void HistMeanShift(const Mat& f_img, Mat& findimg = Mat());
    //ֱ��ͼ����ͼƬ���ƶ�
    double HistImgComparator(const Mat& f_img, const Mat& f_img2);

    //��ˮ��ָ�ͼ��
    Mat WaterCut(const Mat& f_img, Mat& f_out = Mat());
    //GrabCut��ȡǰ������ ���㷨�ǳ�����*
    Mat GrabCutMat(const Mat& f_img, Mat& f_out = Mat());
    //��ˮ����㷨��Floodfill��
    Mat ImgFloodfill(const Mat& f_img, Mat& f_out = Mat());

    //��ͷ���Ʋ�����ʾ ��ͶӰ�任ͼ
    void LoopArrowShow(void);
    //��ͷ���ƶ���ģ����ʾ
    void TestLoopArrowShow(const char* f_name);
};





#endif