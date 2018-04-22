//***********************************************************************
//       图像处理基本学习记录代码集
//       rows 是行数 相当于 height
//       cols 是列数 相当于 weight
//
//***********************************************************************
#ifndef MATDISPOSE_H__
#define MATDISPOSE_H__


C_API bool compare_image_cv(const char* inputFile, const char* outputFile, int resizeID);

//像素处理学习
class MatPixelsTool
{
public:
    //创建所有颜色卡及创建MAT的方法
    void TestColorMat(void);
    //像素遍历操作(ff方法种类)请参照cookbook
    Mat ColorMatPixels(int ff = 0);
    //改变图像的对比度和亮度
    Mat AntithesisMat(const Mat& In_img, Mat& Out_img, double f_alpha, int f_beta);
    //过滤一种颜色像素成黑色块 (根据颜色距离ff)
    Mat ColorFindPixels(const Mat& In_img, Mat& Out_img, Scalar f_color, int ff = 10);
    //像素降低颜色方法
    Mat ColorReduce(const Mat& In_img, Mat& Out_img, int div = 64);
    //图像相加融合 [ff=0.5半透明水印效果 -1 时为覆盖 -2为Png覆盖]
    Mat MatAddMat(Mat& InOut_img, const Mat& Inlogo, Point Ptbegin, float ff = 0.5);
    //图像缩放比例
    Mat MatResize(const Mat& In_img, Mat& Out_img, double val = 0.5);
    //图像截取[起点和终点]
    Mat CutRectMat(const Mat& In_img, Mat& Out_img, Point Ptbegin = Point(0, 0), Point Ptend = Point(0, 0));
    //图像截取[区域]
    Mat CutRectMat(const Mat& In_img, Mat& Out_img, Rect f_rect);
    //连通颜色区域像素裁剪提取(例如提前绿色 其他变黑)
    Mat ColorGetROI(const Mat& In_img, Mat& Out_img, Scalar f_color = Scalar(0, 255, 0));
    //生成马赛克
    Mat MosaicPixels(const Mat& In_img, Mat& Out_img, int ff = 10);
    //将Mat保存透明PNG方法
    void WritePng(const Mat& In_img, const char* f_name);
};

//图像变换处理
class MatFiltration
{
public:
    //灰度图(去色)
    Mat GetGrayMat(const Mat& In_img);
    //计算二值化图(ff为因子 60 128)
    Mat Binarization(const Mat& In_img, Mat& Out_img, int ff = 60);
    //图像卷积--滤波器锐化图片
    Mat Sharpen2D(const Mat& f_img, Mat& f_out);
    //各种滤波  模糊 高斯模糊 方向滤波
    void FiltersImg(const Mat& f_img);
    //图像的旋转 (angle旋转角度)
    Mat MatRotate(const Mat& f_img, Mat& f_out, double angle);
    //图像几何变换：旋转，缩放，斜切 仿射变换 ???
    Mat WarpAffineImg(const Mat& f_img, Mat& f_out);
    //逆投影 扭曲 变形 变换
    Mat InverseProjection(const Mat& f_img, Mat& f_out = Mat());
    //投影变换生成鸟瞰图
    Mat ToAirscape(const Mat& f_img, Mat& f_out = Mat(), bool ff = true);
    //傅里叶级数
    Mat convolveDFT(Mat A, Mat B, Mat& C);
    //测试傅里叶级数
    void testDFT(const Mat& In_img);
    //腐蚀 膨胀 形态学梯度 黑帽 顶帽
    Mat ImgMorpho(const Mat& f_img);
    //形态学滤波检测边缘
    Mat MorphoEdge(const Mat& f_img, Mat& f_out = Mat() , int threshold = 40);
    //形态学检测角点的方法
    Mat GetCorners(const Mat& f_img, Mat& f_out = Mat());

    //Laplace：拉普拉斯变换,图像边缘检测
    //cvLogPolar：       对数极坐标变换；
    //cvDFT：            离散傅里叶变换；
    //cvDCT：            离散余弦变换
    //cvDistTransform：图像的距离变换
    //cvRodrigues2：罗德里格斯变换；
    //图像金字塔[具体看代码]
    //cvPyrUp：图像金字塔，将现有的图像在每个维度上都放大两倍；
    //cvPyrSegmentation：利用金字塔实现图像分割；

};

//直方图处理学习
class MatHistogram
{
private:
    //将1维直方图数据绘制统计图单独算法
    Mat DrawHist(MatND& hist);
public:
    //计算灰度直方图[原图,直方图数据,直方图统计图]
    MatND GetGrayHist(Mat& In_img, MatND& hist = MatND(), Mat& histImg = Mat());
    //计算彩色直方图(原图,直方图数据,直方图统计图)
    MatND GetColorHist(Mat& In_img, MatND& hist = MatND(), Mat& histImg = Mat());
    SparseMat GetColorHist(Mat& In_img, SparseMat& hist = SparseMat(), Mat& histImg = Mat());
    //计算H-S二维直方图
    MatND GetHSHist(Mat& In_img, MatND& hist = Mat(), Mat& histImg = Mat());
    //计算图像熵
    double CalcEntropy(Mat& In_img);
public:
    //直方图的均衡化
    Mat ColorHistEqualize(Mat& In_img, Mat& Out_img = Mat());
    //自动对比度 需要修正?????
    Mat AutoControst(Mat& In_img);
};

//高级应用
class MatApp
{
    //查找表改变直方图方法生成负片
    void MatLookUp(Mat& f_img, Mat& f_out = Mat());
    //直方图拉伸方法改进图像对比度Contrast(忽略两端不足minValue的项)
    MatND HistStretching(Mat& f_img, Mat& f_out = Mat(), Mat& histImg = Mat(), int minValue = 100);
    //扩展拉伸彩色直方图改进图像
    void ColorHistStretching(Mat& f_img, Mat& f_out = Mat());
    //反投影直方图方法
    void HistObjectFinder(const Mat& f_img = Mat(), Mat& findimg = Mat());
    //彩色反投影直方图方法
    void ColorHistObjectFinder(const Mat& f_img, Mat& findimg = Mat());
    //均值漂移算法
    void HistMeanShift(const Mat& f_img, Mat& findimg = Mat());
    //直方图检索图片相似度
    double HistImgComparator(const Mat& f_img, const Mat& f_img2);

    //分水岭分割图像
    Mat WaterCut(const Mat& f_img, Mat& f_out = Mat());
    //GrabCut提取前景背景 此算法非常有用*
    Mat GrabCutMat(const Mat& f_img, Mat& f_out = Mat());
    //漫水填充算法（Floodfill）
    Mat ImgFloodfill(const Mat& f_img, Mat& f_out = Mat());

    //箭头绘制播放演示 逆投影变换图
    void LoopArrowShow(void);
    //箭头绘制动画模拟演示
    void TestLoopArrowShow(const char* f_name);
};





#endif