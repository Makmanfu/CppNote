

#include "stdafx.h"
#include <windows.h>
#include <winbase.h>
#include <io.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include "StuOpDetect.h"

StuSVM::StuSVM(void)
{
    SetHogWH();
}

StuSVM::StuSVM(int hogw, int hogh)
{
    SetHogWH(hogw, hogh);
}

StuSVM::~StuSVM(void)
{
}

void StuSVM::DemoShow(void)
{
    //-----训练限速牌的识别-----
    //StuSVM mSvm;
    //设置尺寸
    //mSvm.SetHogWH(40, 40);
    //训练xml
    //mSvm.TrainPathXml("SVM_SPEED.yml",
    //    "D:/Work/CPP/PRO/CppNote/CV_Img/exe/POS/",
    //    "D:/Work/CPP/PRO/CppNote/CV_Img/exe/NEG/");
    //定位分类
    //cout << mSvm.DataPredict("SVM_SPEED.yml", Mat) << endl;
    SetHogWH(40, 40);
    //训练xml
    TrainPathXml("SVM_SPEED.yml",
                 "../cfg/POS/",
                 "../cfg/NEG/");
}

void StuSVM::TrainPathXml(const char* OutXmlFile, const char* InPosPath, const char* InNegPath)
{
    CreateSimpleFile(InPosPath, m_vecPos);
    CreateSimpleFile(InNegPath, m_vecNeg);
    TrainXml(OutXmlFile, m_vecPos, m_vecNeg);
}

int StuSVM::PredictTarget(const char* OutXmlFile, Mat& f_checked) const
{
    //return 0;
#if CV_MAJOR_VERSION<3
    Ptr<CvSVM> svmtest(new CvSVM);
    svmtest->load(OutXmlFile);
    //CvSVM svmtest;
    //svmtest.load(OutXmlFile);
#else
    Ptr<ml::SVM> svmtest = ml::StatModel::load<ml::SVM> (OutXmlFile);
#endif
    Mat trainImg;
    resize(f_checked, trainImg, CurSize, 0, 0, INTER_CUBIC);    //要搞成同样的大小才可以检测到
    HOGDescriptor* hog = new HOGDescriptor(CurSize, cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);             //具体意思见参考文章1,2
    vector<float>descriptors;//结果数组
    hog->compute(trainImg, descriptors, Size(1, 1), Size(0, 0));          //调用计算函数开始计算
    cout << "The Detection Result:" << endl;
    cout << "HOG dims: " << descriptors.size() << endl;
    Mat SVMtrainMat = Mat::zeros(1, (int) descriptors.size(), CV_32FC1);
    unsigned long n = 0;
    for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
        SVMtrainMat.at<float> (0, n++) = *iter;
    float ret = svmtest->predict(SVMtrainMat);
    //ret就是分类结果
    return (int) ret;
}

void StuSVM::SetHogWH(int hogw /*=64*/, int hogh/*=128*/)
{
    hogWidht = hogw;
    hogHeight = hogh;
    CurSize = Size(hogw, hogh);
}

void StuSVM::CreateSimpleFile(const char* InPath, vector<string>& InOutData)
{
    InOutData.clear();
    struct _finddata_t filefind;
    string curr = string(InPath) + "*.*";       //搜索文件
    int pResultNum(0);                          //搜索结果数计数
    intptr_t done = 0, handle;
    if ((handle = _findfirst(curr.c_str(), &filefind)) == -1)
        return ;                                //什么都没找到
    while (!(done = _findnext(handle, &filefind)))
    {
        if (!strcmp(filefind.name, ".."))
            continue;                           //此级没有文件
        if ((filefind.attrib & _A_SUBDIR))      //文件是否为文件夹
            continue;
        else
        {
            ++pResultNum;
            InOutData.push_back(string(InPath) + filefind.name);
        }
    }
    _findclose(handle);
    //return pResultNum;
    return ;
}

void StuSVM::TrainXml(const char* OutXmlFile, vector<string>& vecPos, vector<string>& vecNeg)
{
    //以下支持opencv2/3 请不要修改
    unsigned long n;
    vector<string> img_data;                //正负样本合集
    vector<int> img_flag;                   //正负样本分类标记
    for (vector<string>::const_iterator it = vecPos.begin(); it != vecPos.end(); ++it)
    {
        img_data.push_back(*it);
        img_flag.push_back(1);              //正样本为1
    }
    for (vector<string>::const_iterator it = vecNeg.begin(); it != vecNeg.end(); ++it)
    {
        img_data.push_back(*it);
        img_flag.push_back(0);              //负样本为0
    }
    int nImgNum = (int) img_data.size();    //样本总数量
    Mat data_mat, res_mat = Mat::zeros(nImgNum, 1, CV_32S);    //类型矩阵,存储每个样本的类型标志
    Mat src, trainImg = Mat::zeros(hogHeight, hogWidht, CV_8UC3);    //需要分析的图片
    for (int i = 0; i != img_data.size() - 1; i++)
    {
        src = imread(img_data[i].c_str(), 1);
        resize(src, trainImg, CurSize, 0, 0, INTER_CUBIC);
        //具体意思见参考文章1,2
        HOGDescriptor* hog = new HOGDescriptor(CurSize, Size(16, 16), Size(8, 8), Size(8, 8), 9);
        //结果数组
        vector<float>descriptors;
        //调用计算函数开始计算
        hog->compute(trainImg, descriptors, Size(1, 1), Size(0, 0));
        if (i == 0)
            data_mat = Mat::zeros(nImgNum, (int) descriptors.size(), CV_32FC1);      //根据输入图片大小进行分配空间
        n = 0;
        for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
        {
            data_mat.at<float> (i, n) = *iter;
            n++;
        }
        res_mat.at<int> (i, 0) = img_flag[i];
    }
#if CV_MAJOR_VERSION>2  //opencv3版本
    Ptr<ml::SVM> svm = ml::SVM::create();
    svm->setType(ml::SVM::C_SVC);    // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
    svm->setKernel(ml::SVM::LINEAR);
    svm->setDegree(10.0);
    svm->setGamma(0.09);
    svm->setCoef0(1.0);
    svm->setC(10.0);    // From paper, soft classifier
    svm->setNu(0.5);
    svm->setP(1.0);    // for EPSILON_SVR, epsilon in loss function?
    svm->setTermCriteria(TermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON));
    svm->train(data_mat, ml::ROW_SAMPLE, res_mat);
    svm->save(OutXmlFile);
#else
    CvSVM svm;
    CvSVMParams param;
    CvTermCriteria criteria;
    criteria = cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
    param = CvSVMParams(CvSVM::C_SVC, CvSVM::LINEAR, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria);
    svm.train(data_mat, res_mat, Mat(), Mat(), param);
    //☆☆利用训练数据和确定的学习参数,进行SVM学习☆☆☆☆
    svm.save(OutXmlFile);
#endif
}

#if CV_MAJOR_VERSION<3
OpPepDetection::OpPepDetection(void) : StuSVM(64, 128)
{
}

OpPepDetection::OpPepDetection(int hogw, int hogh) : StuSVM(hogw, hogh)
{
}

OpPepDetection::~OpPepDetection()
{
}

void OpPepDetection::TrainPathXml(const char* OutXmlFile, const char* InPosPath, const char* InNegPath)
{
    CreateSimpleFile(InPosPath, m_vecPos);
    CreateSimpleFile(InNegPath, m_vecNeg);
    TrainXml(OutXmlFile, m_vecPos, m_vecNeg);
}

void OpPepDetection::TrainXml(const char* OutXmlFile, vector<string>& vecPos, vector<string>& vecNeg)
{
    int HardExampleNO = 0;  //4435;
    Mat src;
    int DescriptorDim;      //HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定
    Mat sampleFeatureMat;   //所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数
    Mat sampleLabelMat;     //训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，-1表示无人
    //检测窗口(64,128),块尺寸(16,16),块步长(8,8),cell尺寸(8,8),直方图bin个数9
    HOGDescriptor hog(Size(hogWidht, hogHeight), Size(16, 16), Size(8, 8), Size(8, 8), 9);                //HOG检测器，用来计算HOG描述子的
    //-----------------------------加载样本时使用参数----------------------------------------
    int posNum = 0, negNum = 0;
    vector<string> pos_path, neg_path;      //正负样本
    for (vector<string>::const_iterator it = vecPos.begin(); it != vecPos.end(); ++it)
        pos_path.push_back(*it);
    for (vector<string>::const_iterator it = vecNeg.begin(); it != vecNeg.end(); ++it)
        neg_path.push_back(*it);
    //------------------------------------------------------------------------------------
    /**************************处理正样本************************************/
    for (vector<string>::size_type num = 0; num != pos_path.size(); num++)
    {
        src = imread(pos_path[num].c_str());
        //这里做缩放或裁剪处理
        src = src(Rect(16, 16, 64, 128));       //将96*160的INRIA正样本图片剪裁为64*128，即剪去上下左右各16个像素
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src, descriptors, Size(8, 8));       //计算HOG描述子，检测窗口移动步长(8,8)
        //处理第一个样本时初始化特征向量矩阵和类别矩阵，因为只有知道了特征向量的维数才能初始化特征向量矩阵
        if (0 == num)
        {
            DescriptorDim = descriptors.size();//HOG描述子的维数
            //初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat
            sampleFeatureMat = Mat::zeros(posNum + negNum + HardExampleNO, DescriptorDim, CV_32FC1);
            //初始化训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，0表示无人
            sampleLabelMat = Mat::zeros(posNum + negNum + HardExampleNO, 1, CV_32FC1);
        }
        //将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
        for (int i = 0; i < DescriptorDim; i++)
            sampleFeatureMat.at<float> (num, i) = descriptors[i];   //第num个样本的特征向量中的第i个元素
        sampleLabelMat.at<float> (num, 0) = 1;   //正样本类别为1，有人
    }
    /*******************依次读取负样本图片，生成HOG描述子*******************************/
    for (vector<string>::size_type num = 0; num != pos_path.size(); num++)
    {
        src = imread(pos_path[num].c_str());
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src, descriptors, Size(8, 8));       //计算HOG描述子，检测窗口移动步长(8,8)
        //将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
        for (int i = 0; i < DescriptorDim; i++)
            sampleFeatureMat.at<float> (num + posNum, i) = descriptors[i];   //第posNum+num个样本的特征向量中的第i个元素
        sampleLabelMat.at<float> (num + posNum, 0) = -1;   //负样本类别为-1，无人
    }
    /*****************************训练SVM分类器**************************************/
    //迭代终止条件，当迭代满1000次或误差小于FLT_EPSILON时停止迭代
    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
    //SVM参数：SVM类型为C_SVC；线性核函数；松弛因子C=0.01
    CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);
    mSvm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);    //训练分类器
    mSvm.save(OutXmlFile);    //将训练好的SVM模型保存为xml文件           cout<<"训练完成"<<endl;
}

int OpPepDetection::PredictTarget(const char* OutXmlFile, Mat& f_checked)
{
    //加载xml文件
    mSvm.load(OutXmlFile);
    //HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定
    int DescriptorDim = mSvm.get_var_count();
    //支持向量的个数
    int supportVectorNum = mSvm.get_support_vector_count();
    //cout<<"支持向量个数："<<supportVectorNum<<endl;
    //alpha向量，长度等于支持向量个数
    Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);
    //支持向量矩阵
    Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);
    //alpha向量乘以支持向量矩阵的结果
    Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);
    //将支持向量的数据复制到supportVectorMat矩阵中
    for (int i = 0; i < supportVectorNum; i++)
    {
        const float* pSVData = mSvm.get_support_vector(i);    //返回第i个支持向量的数据指针
        for (int j = 0; j < DescriptorDim; j++)
            supportVectorMat.at<float> (i, j) = pSVData[j];
    }
    //将alpha向量的数据复制到alphaMat中
    double* pAlphaData = mSvm.get_alpha_vector(); //返回SVM的决策函数中的alpha向量
    for (int i = 0; i < supportVectorNum; i++)
        alphaMat.at<float> (0, i) = pAlphaData[i];
    //计算-(alphaMat * supportVectorMat),结果放到resultMat中gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//不知道为什么加负号？
    resultMat = -1 * alphaMat * supportVectorMat;
    //得到最终的setSVMDetector(const vector<float>& detector)参数中可用的检测子
    vector<float> myDetector;
    //将resultMat中的数据复制到数组myDetector中
    for (int i = 0; i < DescriptorDim; i++)
        myDetector.push_back(resultMat.at<float> (0, i));
    //最后添加偏移量rho，得到检测子
    myDetector.push_back(mSvm.get_rho());
    cout << "检测子维数：" << myDetector.size() << endl;
    //保存检测子参数到文件测试看数据
    //std::ofstream fout("HOGDetectorForOpenCV.txt");
    //for(int i=0; i<myDetector.size(); i++)
    //    fout<<myDetector[i]<<endl;
    //fout.close();
    //设置检测数据
    vector<float> aaa =  HOGDescriptor::getDefaultPeopleDetector();
    people_dectect.setSVMDetector(myDetector);
    //矩形框数组
    vector<Rect> found, found_filtered;
    people_dectect.detectMultiScale(f_checked, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
    cout << "矩形个数：" << found.size() << endl;
    size_t i, j;
    for (i = 0; i < found.size(); i++)
    {
        Rect r = found[i];
        for (j = 0; j < found.size(); j++)
            if (j != i && (r & found[j]) == r)
                break;
        if (j == found.size())
            found_filtered.push_back(r);
    }
    int outnum = 0;
    outnum = found_filtered.size();
    cout << "过滤后矩形的个数：" << outnum << endl;
    //画矩形框，因为hog检测出的矩形框比实际人体框要稍微大些,所以这里需要做一些调整
    for (i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];
        r.x += cvRound(r.width * 0.1);
        r.width = cvRound(r.width * 0.8);
        r.y += cvRound(r.height * 0.07);
        r.height = cvRound(r.height * 0.8);
        rectangle(f_checked, r.tl(), r.br(), Scalar(0, 0, 255), 2);
    }
    return outnum;
}

int OpPepDetection::PredictTarget(Mat f_img) const
{
    int outnum = 0;
    HOGDescriptor people_dectect_hog;
    people_dectect_hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    vector<Rect> found, found_filtered;
    people_dectect_hog.detectMultiScale(f_img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
    size_t i, j;
    for (i = 0; i < found.size(); i++)
    {
        Rect r = found[i];
        for (j = 0; j < found.size(); j++)
            if (j != i && (r & found[j]) == r)
                break;
        if (j == found.size())
            found_filtered.push_back(r);
    }
    outnum = found_filtered.size();
    cout << "过滤后矩形的个数：" << outnum << endl;
    for (i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];
        r.x += cvRound(r.width * 0.1);
        r.width = cvRound(r.width * 0.8);
        r.y += cvRound(r.height * 0.07);
        r.height = cvRound(r.height * 0.8);
        rectangle(f_img, r.tl(), r.br(), Scalar(0, 255, 0), 2);
    }
    return outnum;
}
#endif


StuDetect::StuDetect(void)
{
}

StuDetect::~StuDetect(void)
{
}

void StuDetect::FindImgFaceEyes(Mat& f_img, CascadeClassifier& f_cascade,
                                CascadeClassifier& f_nestedCascade, double f_scale, bool f_tryflip) const
{
    int i = 0;  /*颜色随机 和 找到人脸数*/
    double t = 0;
    //结果区域
    vector<Rect> faces, faces2;
    const static Scalar colors[] = {CV_RGB(0, 0, 255),
                                    CV_RGB(0, 128, 255),
                                    CV_RGB(0, 255, 255),
                                    CV_RGB(0, 255, 0),
                                    CV_RGB(255, 128, 0),
                                    CV_RGB(255, 255, 0),
                                    CV_RGB(255, 0, 0),
                                    CV_RGB(255, 0, 255)
                                   };
    Mat gray, smallImg(cvRound(f_img.rows / f_scale), cvRound(f_img.cols / f_scale), CV_8UC1);
    //灰度化
    cvtColor(f_img, gray, COLOR_BGR2GRAY);
    //缩放
    resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
    //灰度图象直方图均衡化
    equalizeHist(smallImg, smallImg);
    t = (double) cvGetTickCount();
    f_cascade.detectMultiScale(smallImg, faces,
                               1.1, 2, 0
                               //|CASCADE_FIND_BIGGEST_OBJECT
                               //|CASCADE_DO_ROUGH_SEARCH
                               | CASCADE_SCALE_IMAGE,
                               Size(30, 30));
    if (f_tryflip)   //翻转
    {
        flip(smallImg, smallImg, 1);
        f_cascade.detectMultiScale(smallImg, faces2,
                                   1.1, 2, 0
                                   //|CASCADE_FIND_BIGGEST_OBJECT
                                   //|CASCADE_DO_ROUGH_SEARCH
                                   | CASCADE_SCALE_IMAGE,
                                   Size(30, 30));
        for (vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++)
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
    }
    //t = (double)cvGetTickCount() - t;
    //printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));
    for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i % 8];
        int radius;
        double aspect_ratio = (double) r->width / r->height;
        if (0.75 < aspect_ratio && aspect_ratio < 1.3)
        {
            center.x = cvRound((r->x + r->width * 0.5) * f_scale);
            center.y = cvRound((r->y + r->height * 0.5) * f_scale);
            radius = cvRound((r->width + r->height) * 0.25 * f_scale);
            circle(f_img, center, radius, color, 3, 8, 0);
        }
        else
            rectangle(f_img, cvPoint(cvRound(r->x * f_scale), cvRound(r->y * f_scale)),
                      cvPoint(cvRound((r->x + r->width - 1) *f_scale), cvRound((r->y + r->height
                              - 1) *f_scale)), color, 3, 8, 0);
        if (f_nestedCascade.empty())
            continue;
        smallImgROI = smallImg(*r);
        f_nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
                                         1.1, 2, 0
                                         //|CASCADE_FIND_BIGGEST_OBJECT
                                         //|CASCADE_DO_ROUGH_SEARCH
                                         //|CASCADE_DO_CANNY_PRUNING
                                         | CASCADE_SCALE_IMAGE,
                                         Size(30, 30));
        //在结果上画圆
        for (vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++)
        {
            center.x = cvRound((r->x + nr->x + nr->width * 0.5) * f_scale);
            center.y = cvRound((r->y + nr->y + nr->height * 0.5) * f_scale);
            radius = cvRound((nr->width + nr->height) * 0.25 * f_scale);
            circle(f_img, center, radius, color, 3, 8, 0);
        }
    }
    //cv::imshow("face_eye", f_img);
}

int StuDetect::FindMatFace(Mat& f_img, CascadeClassifier& f_cascade, double f_scale) const
{
    int i = 0; /*颜色随机 和 找到人脸数*/
    //结果区域
    vector<Rect> faces;
    //标记颜色
    const static Scalar colors[] = {CV_RGB(0, 255, 0),
                                    CV_RGB(0, 128, 255),
                                    CV_RGB(0, 255, 255),
                                    CV_RGB(0, 0, 255),
                                    CV_RGB(255, 128, 0),
                                    CV_RGB(255, 255, 0),
                                    CV_RGB(255, 0, 0),
                                    CV_RGB(255, 0, 255)
                                   };
    Mat gray, smallImg(cvRound(f_img.rows / f_scale), cvRound(f_img.cols / f_scale), CV_8UC1);
    //转为灰度图
    cvtColor(f_img, gray, CV_BGR2GRAY);    //imshow("gray",gray);
    resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
    //灰度图象直方图均衡化
    equalizeHist(smallImg, smallImg);    //imshow("smallImg",smallImg);
    //时间计算
    double t = (double) cvGetTickCount();
    //查找
    f_cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
    cout << "detection time = " <<
         ((double) cvGetTickCount() - t) / ((double) cvGetTickFrequency() * 1000.)
         << " ms.   num=" << faces.size() << endl;
    for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
    {
        Point center;
        Scalar color = colors[i % 8];
        int radius;  //半径
        //检测的区域w/h比值
        double aspect_ratio = (double) r->width / r->height;
        //下面画区域
        if (0.75 < aspect_ratio && aspect_ratio < 1.3)
        {
            //确定圆心位置,就可以定位置了
            center.x = cvRound((r->x + r->width * 0.5) * f_scale);
            center.y = cvRound((r->y + r->height * 0.5) * f_scale);
            radius = cvRound((r->width + r->height) * 0.25 * f_scale);
            //circle( f_img, center, radius, color, 3, 8, 0 );
            f_img = DrawCircle(f_img, center, radius, color);
        }
        else //画矩形
            rectangle(f_img, Point(cvRound(r->x * f_scale), cvRound(r->y * f_scale)),
                      Point(cvRound((r->x + r->width - 1) *f_scale), cvRound((r->y + r->height - 1) *f_scale)), color, 3, 8, 0);
        //下面是截取区域的方法
        //Mat f_faceimg=f_img( Rect(r->x*f_scale,r->y*f_scale,(r->x + r->width-1)*f_scale,(r->y + r->height-1)*f_scale) );
    }
    return i;
}

int StuDetect::FindMatHogPeople(Mat& f_img) const
{
    //double t = (double)cvGetTickCount();
    int outnum = 0;
    //HOG特征检测器
    HOGDescriptor people_dectect_hog;
    //采用默认的   已经训练好了的svm系数作为此次检测的模型
    people_dectect_hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    //矩形框数组
    vector<Rect> found, found_filtered;
    //img为输入待检测的图片；found为检测到目标区域列表；参数3为程序内部计算为行人目标的阈值，也就是检测到的特征到SVM分类超平面的距离;
    //参数4为滑动窗口每次移动的距离。它必须是块移动的整数倍；参数5为图像扩充的大小；参数6为比例系数，即测试图片每次尺寸缩放增加的比例；
    //参数7为组阈值，即校正系数，当一个目标被多个窗口检测出来时，该参数此时就起了调节作用，为0时表示不起调节作用。
    people_dectect_hog.detectMultiScale(f_img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
    //cout<<"矩形个数："<<found.size()<<endl;
    //找出所有没有嵌套的矩形框r,并放入found_filtered中,如果有嵌套的话,则取外面最大的那个矩形框放入found_filtered中
    //从源码中可以看出:
    //#define __SIZE_TYPE__ long unsigned int
    //typedef __SIZE_TYPE__ size_t;
    //因此,size_t是一个long unsigned int类型
    size_t i, j;
    for (i = 0; i < found.size(); i++)
    {
        Rect r = found[i];
        //下面的这个for语句是找出所有没有嵌套的矩形框r,并放入found_filtered中,如果有嵌套的
        //话,则取外面最大的那个矩形框放入found_filtered中
        for (j = 0; j < found.size(); j++)
            if (j != i && (r & found[j]) == r)
                break;
        if (j == found.size())
            found_filtered.push_back(r);
    }
    outnum = (int) found_filtered.size();
    //cout << "Pep_time =" << ((double)cvGetTickCount() - t) / ((double)cvGetTickFrequency()*1000.) << "ms ";
    cout << "过滤后矩形的个数：" << outnum << endl;
    //画矩形框，因为hog检测出的矩形框比实际人体框要稍微大些,所以这里需要做一些调整
    for (i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];
        r.x += cvRound(r.width * 0.1);
        r.width = cvRound(r.width * 0.8);
        r.y += cvRound(r.height * 0.07);
        r.height = cvRound(r.height * 0.8);
        rectangle(f_img, r.tl(), r.br(), Scalar(0, 255, 0), 2);
    }
    return outnum;
}

bool StuDetect::CheckYmlFile(const char* ypath)
{
    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind = FindFirstFileA(ypath, &FindFileData);
    bool rValue = (hFind != INVALID_HANDLE_VALUE) ? true : false;
    FindClose(hFind);
    return rValue;
}

int StuDetect::SpeedSignCheck(Mat& fsrc, Check_Info& CFGinfo) const
{
    double pi = 3.1415926;
    int ImgHeight(40), ImgWidht(40);
    cv::Mat trainImg;// = cv::Mat::zeros(ImgHeight, ImgWidht, CV_8UC3);//需要分析的图片
    const char* ymlfile = "../Resource/SVM_SPEED.yml";
    if (!CheckYmlFile(ymlfile))
        if (CheckYmlFile("SVM_SPEED.yml"))
            ymlfile = "SVM_SPEED.yml";
        else
            return -1;
#if CV_MAJOR_VERSION<3
    Ptr<CvSVM> svmtest(new CvSVM);
    svmtest->load(ymlfile);
#else
    Ptr<ml::SVM> svmtest = ml::StatModel::load<ml::SVM> (ymlfile);
#endif
    unsigned long n = 0;
    //设置源图像ROI
    Mat img = fsrc(Rect(CFGinfo.InRegion.bx, CFGinfo.InRegion.by, CFGinfo.InRegion.ex -
                        CFGinfo.InRegion.bx, CFGinfo.InRegion.ey - CFGinfo.InRegion.by));
    //转到HSV
    cv::Mat  hsv_base;
    cv::cvtColor(img, hsv_base, CV_BGR2HSV);      //16ms
    //颜色分割
    cv::Mat colorCut = cv::Mat(img.rows, img.cols, CV_32F), how_red = cv::Mat(img.rows, img.cols, CV_32F);
    //优化????????????
    for (int x = 0; x < hsv_base.rows; x++)
    {
        for (int y = 0; y < hsv_base.cols; y++)
        {
            double h_src = hsv_base.data[x * 3 * hsv_base.cols + 3 * y + 0] / 255.0 * 360;
            double s_src = hsv_base.data[x * 3 * hsv_base.cols + 3 * y + 1] / 255.0;
            //double v_src = hsv_base.data[x * 3 * hsv_base.cols + 3 * y + 2];
            if (h_src >= 300 && h_src < 360)
                ((float*) how_red.data) [x * hsv_base.cols + y] = float (s_src * sin((h_src - 300.0)
                        / 180.0 * pi) / sin(60.0 / 180 * pi));
            else if (h_src >= 0 && h_src <= 60)
                ((float*) how_red.data) [x * hsv_base.cols + y] = float (s_src * sin((60.0 - h_src)
                        / 180.0 * pi) / sin(60.0 / 180 * pi));
            else
                ((float*) how_red.data) [x * hsv_base.cols + y] = 0;
            ((float*) colorCut.data) [x * hsv_base.cols + y] = 0;
            /* if ((h_src>330||(h_src<=30&&s_src>=0.2)) &&v_src>0.1 )*/
            /* if (h_src>330||(h_src<=30&&s_src>=0.2))*/
            if (h_src > 300 || (h_src <= 60/*&&s_src>=0.2*/))
                ((float*) colorCut.data) [x * hsv_base.cols + y] = 255.0;
        }
    }
    colorCut /= 255.0;
    double  maxVal(0), minVal(0);
    //cv::minMaxLoc(how_red, &minVal, &maxVal);
    //红色位图均值滤波//
    //红色程度图滤波//
    cv::Mat blueredImg = cv::Mat(colorCut.rows, colorCut.cols, CV_32F), blueredHowRed = cv::Mat(colorCut.rows, colorCut.cols, CV_32F);
    cv::blur(colorCut, blueredImg, Size(7, 7));
    cv::blur(how_red, blueredHowRed, Size(7, 7));
    //////////////////////////////////////////////////////////////////////////
    //得到最终细化后要提取区域的图像
    cv::Mat RefinedImg(colorCut.rows, colorCut.cols, CV_8UC1), refine(colorCut.rows, colorCut.cols, CV_8UC1);
    //优化
    for (int x = 0; x < img.rows; x++)
    {
        for (int y = 0; y < img.cols; y++)
        {
            double f = ((float*) blueredHowRed.data) [x * img.cols + y];
            double r = ((float*) blueredImg.data) [x * img.cols + y];
            double ff = ((float*) how_red.data) [x * img.cols + y];
            if (r >= 0.8)
                RefinedImg.data[x * img.cols + y] = refine.data[x * img.cols + y] = 255;
            else if (ff >= f && (r < 0.8 && r >= 0.2))
                RefinedImg.data[x * img.cols + y] = refine.data[x * img.cols + y] = 255;
            else
                RefinedImg.data[x * img.cols + y] = refine.data[x * img.cols + y] = 0;
        }
    }
    //开闭运算
    cv::morphologyEx(RefinedImg, RefinedImg, cv::MORPH_OPEN, cv::Mat());
    //????????????????????????????????????????
    //提取区域寻找红孔
    vector<vector<cv::Point> > contours;
    cv::findContours(RefinedImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);    // retrieve all pixels of each contours
    //筛选区域，太大的太小的去除//
    cv::Mat resultt(colorCut.rows, colorCut.cols, CV_8U, Scalar(0));
    //对筛选后的区域求外接多凸边形//
    vector<Point> hull;
    for (vector<vector<Point> >::const_iterator itc1 = contours.begin(); itc1 != contours.end(); ++itc1)
    {
        cv::convexHull(Mat(*itc1), hull);
        for (vector<Point>::const_iterator it = hull.begin(); it != (hull.end() - 1); ++it)
            cv::line(resultt, *it, * (it + 1), Scalar(255), 3);
        cv::line(resultt, * (hull.begin()), * (hull.end() - 1), Scalar(255), 3);
    }
    vector<vector<cv::Point> > contours2;
    cv::findContours(resultt, contours2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //填充区域。
    cv::drawContours(resultt, contours2, -1, Scalar(255), CV_FILLED);
    cv::minMaxLoc(resultt, &minVal, &maxVal);
    //显示红区域
    cv::Mat Hole(img.rows, img.cols, CV_8UC1);
    for (int x = 0; x < img.rows; x++)
        for (int y = 0; y < img.cols; y++)
            Hole.data[x * img.cols + y] = resultt.data[x * img.cols + y] > refine.data[x * img.cols + y] ? 255 : 0;
    cv::erode(Hole, Hole, cv::Mat());
    // 开闭运算
    cv::morphologyEx(Hole, Hole, cv::MORPH_OPEN, cv::Mat());
    // 提取轮廓
    vector<vector<cv::Point> > contours_tmp;
    cv::findContours(Hole, contours_tmp, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    double maxarea(800), minarea(190);
    for (vector<vector<cv::Point> >::const_iterator itContours = contours_tmp.begin(); itContours != contours_tmp.end(); ++itContours)
    {
        //计算面积
        double tmparea = fabs(cv::contourArea(*itContours));
        //计算矩形区域
        Rect aRect = cv::boundingRect(*itContours);
        //计算纵横比
        float rate = (float) aRect.height / aRect.width;
        //计算圆
        int Ri = (aRect.height + aRect.width) / 4;
        float rater = float ((pi * Ri * Ri) / tmparea);
        if (!(tmparea < minarea ||                          //删除面积小于设定值的轮廓
              aRect.width > 100 || aRect.height > 100 ||      //删除宽高大于阈值的轮廓
              aRect.height < 10 || aRect.width < 10 ||        //删除宽高小于10的轮廓
              rate < 0.8 || rate > 1.5 ||                     //删除纵横比不在一定范围内的轮廓
              rater < 0.8 || rater > 1.2))                    //删除非圆的轮廓
        {
            int xmax(0), ymax(0), xmin(2000), ymin(2000);             //矩形框无穷大
            //剪出矩形待检测图片
            for (vector<cv::Point>::const_iterator itcont = (*itContours).begin(); itcont != (*itContours).end(); ++itcont)
            {
                if ((itcont->x) > xmax)
                    xmax = itcont->x;
                if ((itcont->y) > ymax)
                    ymax = itcont->y;
                if ((itcont->x) < xmin)
                    xmin = itcont->x;
                if ((itcont->y) < ymin)
                    ymin = itcont->y;
            }
            if ((xmin < xmax) && (ymin < ymax))
            {
                Mat test = fsrc(Rect(xmin, ymin, xmax - xmin, ymax - ymin));
                //要搞成同样的大小才可以检测到
                resize(test, trainImg, Size(ImgWidht, ImgHeight), 0, 0, INTER_CUBIC);
                //结果数组
                vector<float>descriptors;
                Ptr<HOGDescriptor> hog(new cv::HOGDescriptor(Size(ImgWidht, ImgHeight), Size(16, 16), Size(8, 8), Size(8, 8), 9));
                //cv::HOGDescriptor *hog = new cv::HOGDescriptor(Size(ImgWidht, ImgHeight), Size(16, 16), Size(8, 8), Size(8, 8), 9);
                //调用计算函数开始计算
                hog->compute(trainImg, descriptors, Size(1, 1), Size(0, 0));
                //delete[] hog;
                Mat SVMtrainMat = Mat::zeros(1, (int) descriptors.size(), CV_32FC1);
                n = 0;
                for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
                    SVMtrainMat.at<float> (0, n++) = *iter;
                if (1 == svmtest->predict(SVMtrainMat))      //分类结果为1
                    CFGinfo.OutRegions.push_back(Sign_Rect(CFGinfo.InRegion.bx + xmin, CFGinfo.InRegion.by + ymin
                                                           , CFGinfo.InRegion.bx + xmax, CFGinfo.InRegion.by + ymax));
            }
        }
    }
    return (int) CFGinfo.OutRegions.size();
}

int StuDetect::SpeedSignVec(Mat& f_src, vector<Rect>& f_faces) const
{
    //double time1 = (double)cvGetTickCount();
    double pi = 3.1415926;
    int ImgHeight(40), ImgWidht(40);
    //需要分析的图片
    cv::Mat trainImg = cv::Mat::zeros(ImgHeight, ImgWidht, CV_8UC3);
    //设置源图像ROI
    Mat img, hsv_base;
    f_src.copyTo(img);
    //转到HSV
    cv::cvtColor(img, hsv_base, CV_BGR2HSV);      //16ms
    //颜色分割
    cv::Mat colorCut = cv::Mat(img.rows, img.cols, CV_32F), how_red = cv::Mat(img.rows, img.cols, CV_32F);
    //优化HSV
    for (int x = 0; x < hsv_base.rows; x++)
    {
        for (int y = 0; y < hsv_base.cols; y++)
        {
            double h_src = hsv_base.data[x * 3 * hsv_base.cols + 3 * y + 0] / 255.0 * 360;
            double s_src = hsv_base.data[x * 3 * hsv_base.cols + 3 * y + 1] / 255.0;
            double v_src = hsv_base.data[x * 3 * hsv_base.cols + 3 * y + 2];
            if (h_src >= 300 && h_src < 360)
                ((float*) how_red.data) [x * hsv_base.cols + y] = float (s_src * sin((h_src - 300.0)
                        / 180.0 * pi) / sin(60.0 / 180 * pi));
            else if (h_src >= 0 && h_src <= 60)
                ((float*) how_red.data) [x * hsv_base.cols + y] = float (s_src * sin((60.0 - h_src)
                        / 180.0 * pi) / sin(60.0 / 180 * pi));
            else
                ((float*) how_red.data) [x * hsv_base.cols + y] = 0.0;
            ((float*) colorCut.data) [x * hsv_base.cols + y] = float ((h_src > 300 || h_src <= 60) ? 255.0 : 0.0);
        }
    }
    double*  max_value, *min_value, maxVal(0), minVal(0);
    max_value = &maxVal;
    min_value = &minVal;
    cv::minMaxLoc(how_red, min_value, max_value);
    //红色位图均值滤波
    //红色程度图滤波
    cv::Mat blueredImg = cv::Mat(colorCut.rows, colorCut.cols, CV_32F), blueredHowRed = cv::Mat(colorCut.rows, colorCut.cols, CV_32F);
    colorCut /= 255.;
    cv::blur(colorCut, blueredImg, Size(7, 7));
    cv::minMaxLoc(blueredImg, min_value, max_value);
    cv::blur(how_red, blueredHowRed, Size(7, 7));
    //////////////////////////////////////////////////////////////////////////
    //得到最终细化后要提取区域的图像
    cv::Mat RefinedImg(colorCut.rows, colorCut.cols, CV_8UC1), refine(colorCut.rows, colorCut.cols, CV_8UC1);
    //优化
    for (int x = 0; x < img.rows; x++)
    {
        for (int y = 0; y < img.cols; y++)
        {
            double f = ((float*) blueredHowRed.data) [x * img.cols + y];
            double r = ((float*) blueredImg.data) [x * img.cols + y];
            double ff = ((float*) how_red.data) [x * img.cols + y];
            if (r >= 0.8)
            {
                RefinedImg.data[x * img.cols + y] = 255;
                refine.data[x * img.cols + y] = 255;
            }
            else if (ff >= f && (r < 0.8 && r >= 0.2))
            {
                RefinedImg.data[x * img.cols + y] = 255;
                refine.data[x * img.cols + y] = 255;
            }
            else
            {
                RefinedImg.data[x * img.cols + y] = 0;
                refine.data[x * img.cols + y] = 0;
            }
        }
    }
    //开闭运算
    cv::dilate(RefinedImg, RefinedImg, cv::Mat());
    cv::erode(RefinedImg, RefinedImg, cv::Mat());
    //提取区域寻找红孔
    vector<vector<cv::Point>> contours;
    cv::findContours(RefinedImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);    // retrieve all pixels of each contours
    //筛选区域，太大的太小的去除
    cv::Mat resultt(colorCut.rows, colorCut.cols, CV_8U, Scalar(0));
    //对筛选后的区域求外接多凸边形
    vector<cv::Point> hull;
    for (vector<vector<Point> >::const_iterator itc1 = contours.begin(); itc1 != contours.end(); ++itc1)
    {
        cv::convexHull(Mat(*itc1), hull);
        for (vector<Point>::const_iterator it = hull.begin(); it != (hull.end() - 1); ++it)
            cv::line(resultt, *it, * (it + 1), Scalar(255), 3);
        cv::line(resultt, * (hull.begin()), * (hull.end() - 1), Scalar(255), 3);
    }
    vector<vector<cv::Point>> contours2;
    cv::findContours(resultt, contours2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //填充区域。
    cv::drawContours(resultt, contours2, -1, Scalar(255), CV_FILLED);
    //求凸多边形
    cv::minMaxLoc(resultt, min_value, max_value);
    //显示红孔
    cv::Mat Hole(img.rows, img.cols, CV_8UC1);
    for (int x = 0; x < img.rows; x++)
        for (int y = 0; y < img.cols; y++)
            Hole.data[x * img.cols + y] = resultt.data[x * img.cols + y] > refine.data[x * img.cols + y] ? 255 : 0;
    cv::erode(Hole, Hole, cv::Mat());
    //开闭运算//
    cv::morphologyEx(Hole, Hole, cv::MORPH_OPEN, cv::Mat());
    // 提取轮廓
    vector<vector<cv::Point> > contours_tmp;
    cv::findContours(Hole, contours_tmp, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    double maxarea(800), minarea(190);
    for (vector<vector<cv::Point> >::const_iterator itContours = contours_tmp.begin(); itContours != contours_tmp.end(); ++itContours)
    {
        //计算面积
        double tmparea = fabs(cv::contourArea(*itContours));
        //计算矩形区域
        Rect aRect = cv::boundingRect(*itContours);
        //计算纵横比
        float rate = (float) aRect.height / aRect.width;
        //计算圆
        int Ri = (aRect.height + aRect.width) / 4;
        float rater = float ((pi * Ri * Ri) / tmparea);
        if (!(tmparea < minarea ||                          //删除面积小于设定值的轮廓
              aRect.width > 100 || aRect.height > 100 ||      //删除宽高大于阈值的轮廓
              aRect.height < 10 || aRect.width < 10 ||
              rate < 0.8 || rate > 1.5 ||                     //删除纵横比不在一定范围内的轮廓
              rater < 0.8 || rater > 1.2))                    //删除非圆的轮廓
        {
            int xmax(0), ymax(0), xmin(2000), ymin(2000);
            for (vector<cv::Point>::const_iterator itcont = (*itContours).begin(); itcont != (*itContours).end(); ++itcont)
            {
                if ((itcont->x) > xmax)
                    xmax = itcont->x;
                if ((itcont->y) > ymax)
                    ymax = itcont->y;
                if ((itcont->x) < xmin)
                    xmin = itcont->x;
                if ((itcont->y) < ymin)
                    ymin = itcont->y;
            }
            if ((xmin < xmax) && (ymin < ymax))
            {
                //Mat test = f_src(Rect(xmin, ymin, xmax - xmin, ymax - ymin));
                //imshow("待分类",test);
                //waitKey(0);
                f_faces.push_back(Rect(xmin, ymin, xmax - xmin, ymax - ymin));
            }
        }
    }
    //cout << "SpeedCheck time = " << ((double)cvGetTickCount() - time1) / ((double)cvGetTickFrequency()*1000.) << " ms.   num=" << f_faces.size() << endl;
    return (int) f_faces.size();
}

Mat StuDetect::DrawCircle(Mat& f_img, Point pt, int r, Scalar color) const
{
    if (r < 10) r = 10;
    Mat f_out = f_img.clone();
    int thickness = 2, r_l = 3 * r / 2;
    circle(f_out, pt, r, color, thickness);
    line(f_out, Point(pt.x - r_l, pt.y), Point(pt.x - r, pt.y), color, thickness);
    line(f_out, Point(pt.x + r_l, pt.y), Point(pt.x + r, pt.y), color, thickness);
    line(f_out, Point(pt.x, pt.y - r_l), Point(pt.x, pt.y - r), color, thickness);
    line(f_out, Point(pt.x, pt.y + r_l), Point(pt.x, pt.y + r), color, thickness);
    char buf[255] = { 0 };
    sprintf(buf, "(%d,%d)", pt.x, pt.y);            //    sprintf(buf,"%s : %d",f_name,f_num);
    cv::putText(f_out, buf, Point(pt.x + r, pt.y - r), CV_FONT_HERSHEY_DUPLEX, 0.5, color, 1);
    return f_out;
}

StuOpDetect::StuOpDetect(void)
{
}

StuOpDetect::~StuOpDetect(void)
{
}

void StuOpDetect::DemoShow(void)
{
    //=========正样本数据生成===========================
    //StuOpDetect().man_GetPngPosImg("../DllRes/img/lanbuda.png",1000,15);
    //=========负样本数据生成===========================
    //StuOpDetect().man_GetNegImg("../DllRes/img/group.jpg",1000,156);
}

void StuOpDetect::man_GetPngPosImg(char* f_png, int f_num, int f_lengh) const
{
    CreateDirectoryA("Pos", NULL);
    Mat f_logo_n = cv::imread(f_png), mask_n = cv::imread(f_png, 0);
    Mat f_logo = f_logo_n.clone(), mask = mask_n.clone();
    Mat f_NaMat = Mat(f_logo.rows + f_lengh, f_logo.cols + f_lengh, CV_8UC4);
    char saveFileName[100] = { '\0' };
    /*采用自己设置的参数来保存图片*/
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);    //png格式下，默认的参数为3.
    float du = 0.0;     //旋转度数随机数
    for (int k = 0; k < f_num; k++)
    {
        f_NaMat = Mat(f_logo.rows + f_lengh, f_logo.cols + f_lengh, CV_8UC3, Scalar(255, 255, 255));
        int i = rand() % f_lengh;
        int j = rand() % f_lengh;
        //下面是图像旋转方法
        du = float (rand() * float (-15.0));     //du = -15.0(度);
        f_logo = f_logo_n.clone();
        mask = mask_n.clone();
        corecv_rotateImage(f_logo, du);
        corecv_rotateImage(mask, du);
        Mat imageROI = f_NaMat(cv::Rect(j, i, f_logo.cols, f_logo.rows));
        f_logo.copyTo(imageROI, mask);
        sprintf(saveFileName, "pos/%d.png", k + 1);
        //imshow(saveFileName,f_NaMat);
        //waitKey(0);
        imwrite(saveFileName, f_NaMat, compression_params);
    }
    cout << "正样本数据生成完成!" << endl;
}

void StuOpDetect::man_GetNegImg(char* f_file, int f_num, int f_lengh) const
{
    //生成负样本
    CreateDirectoryA("Neg", NULL);
    Mat f_img = imread(f_file), t_out;
    int t_w(f_img.rows - f_lengh), t_h(f_img.cols - f_lengh);
    char saveFileName[100] = { '\0' };
    /*采用自己设置的参数来保存图片*/
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    for (int k = 0; k < f_num; k++)
    {
        //随机点的坐标计算
        int i = rand() % t_h;
        int j = rand() % t_w;
        t_out = f_img(Rect(Point(i, j), Point(i + f_lengh, j + f_lengh)));
        sprintf(saveFileName, "Neg/%d.jpg", k + 1);
        //imshow("ss",t_out);
        //waitKey(0);
        imwrite(saveFileName, t_out, compression_params);
    }
    cout << "负样本数据生成完成!" << endl;
}

void StuOpDetect::FaceCheck(Mat& f_img, vector<Rect>& f_faces) const
{
    f_faces.clear();
    CascadeClassifier f_cascade;
    if (!f_cascade.load("SVM_face.xml"))
        return;
    double f_scale = 1;
    Mat gray, smallImg(cvRound(f_img.rows / f_scale), cvRound(f_img.cols / f_scale), CV_8UC1);
    cvtColor(f_img, gray, CV_BGR2GRAY);
    resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
    equalizeHist(smallImg, smallImg);
    //时间计算
    double tface = (double) cvGetTickCount();
    f_cascade.detectMultiScale(smallImg, f_faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
    cout << "FaceCheck time = " << ((double) cvGetTickCount() - tface) / ((double) cvGetTickFrequency() * 1000.) << " ms" << endl;
    return;
}

void StuOpDetect::SpeedCheck(Mat& f_img, vector<Rect>& f_faces)
{
    f_faces.clear();
    Ptr<Check_Info> Sign_Cfg(new Check_Info);
    Sign_Cfg->InRegion = Sign_Rect(0, 0, f_img.cols, f_img.rows);
    //double time1 = (double)cvGetTickCount();
    int num = SpeedSignCheck(f_img, *Sign_Cfg);
    //cout << "SpeedCheck time = " << ((double)cvGetTickCount() - time1) / ((double)cvGetTickFrequency()*1000.) << " ms.   num=" << num << endl;
    if (num > 0)
        for (vector<Sign_Rect>::const_iterator it = Sign_Cfg->OutRegions.begin(); it != Sign_Cfg->OutRegions.end(); ++it)
            f_faces.push_back(Rect(it->bx, it->by, it->ex - it->bx, it->ey - it->by));
}

void StuOpDetect::corecv_rotateImage(Mat& f_img, double angle) const
{
    // 设置目标图像的大小和类型与源图像一致
    Mat t_dstImg_warp = Mat::zeros(f_img.rows, f_img.cols, f_img.type());
    //【6】对图像进行缩放后再旋转
    // 计算绕图像中点顺时针旋转50度缩放因子为0.6的旋转矩阵
    Point center = Point(t_dstImg_warp.cols / 2, t_dstImg_warp.rows / 2);
    //double angle = -30.0;   //旋转度数
    double scale = 1.0;   //缩放比例
    // 通过上面的旋转细节信息求得旋转矩阵
    Mat rotMat(30, 30, CV_32FC1);
    rotMat = getRotationMatrix2D(center, angle, scale);
    // 旋转已缩放后的图像
    warpAffine(f_img, f_img, rotMat, t_dstImg_warp.size());
    //imshow( "test", f_img );
    //waitKey(0);
}



CmdDetect::CmdDetect(int argc, char** argv) : argcl(argc), argvl(argv)
{
}

CmdDetect::~CmdDetect()
{
}

void CmdDetect::DemoShow(void)
{
    //--------------------------------------------------------------------------
    //CmdDetect(argc, argv).FrameCheckMat(&CmdDetect::FaceCheckShow);
    //void TestFace(void)
    //{
    //  int argc = 2;
    //  char* tmp = "testchardata";
    //  char** argv = &tmp;// NULL;
    //  argv[1] = "demovideo.avi";
    //  //cout<<argv[1]<<endl;
    //  CmdDetect(argc, argv).FrameCheckMat(&CmdDetect::FaceCheckShow);
    //}
}

int CmdDetect::FrameCheckMat(func fp)
{
    char* t_filename = "";
    if (argcl > 1)
        t_filename = argvl[1];
    else
    {
        (this->*fp)("");
        t_filename = "lena.jpg";
    }
    cout << t_filename << endl;
    (this->*fp)(t_filename);
    return 0;
}

void CmdDetect::FaceCheckShow(const char* f_file)
{
    VideoCapture vcap;
    Mat image;
    //判断摄像头和视频文件
    bool t_boolCam = strlen(f_file) < 1;
    bool t_boolImg = false;
    if (t_boolCam)
    {
        vcap = VideoCapture(0);
        if (!vcap.isOpened())   return;
    }
    else
    {
        image = imread(f_file);
        if (image.empty())     //判断视频文件
        {
            vcap.open(f_file);
            if (!vcap.isOpened()) return;
        }
        else
            t_boolImg = true;
    }
    //-----------------------------------SVM
    //UPackDllRes("DllRes.dll", "SVM_face.xml", 110, "CFG");
    double scale = 1;
    bool tryflip = false;
    CascadeClassifier cascade;
    if (!cascade.load("../Resource/SVM_face.xml"))
        if (!cascade.load("SVM_face.xml")) return;
    //-----------------------------------
    StuDetect fdFace;       //解决多次创建对象
    //图片显示
    if (t_boolImg)
    {
        fdFace.FindMatFace(image, cascade, scale);
        namedWindow("图片检测", CV_WINDOW_AUTOSIZE);    //CV_WND_PROP_FULLSCREEN    CV_WINDOW_AUTOSIZE
        imshow("图片检测", image);
        waitKey(0);
        return;
    }
    Mat t_outshow;
    string str_title;
    if (t_boolCam)
    {
        //摄像头
        str_title = "摄像头检测";
    }
    else    //视频播放      //方便测试 开始为1帧
    {
        str_title = "视频文件检测";
        vcap.set(CV_CAP_PROP_POS_FRAMES, 1);
    }
    while (true)
    {
        vcap >> t_outshow;
        if (t_outshow.empty()) return;
        fdFace.FindMatFace(t_outshow, cascade, scale);
        resize(t_outshow, t_outshow, Size(800, 600));
        namedWindow("视频文件检测", CV_WINDOW_AUTOSIZE);
        imshow("视频文件检测", t_outshow);
        waitKey(1);
    }
}

void CmdDetect::PedestrianCheck(const char* f_file /*= ""*/)
{
    VideoCapture vcap;
    Mat image;
    //判断摄像头和视频文件
    bool t_boolCam = strlen(f_file) < 1;
    bool t_boolImg = false;
    if (t_boolCam)
    {
        vcap = VideoCapture(0);
        if (!vcap.isOpened()) return;
    }
    else
    {
        image = imread(f_file);
        if (image.empty())
        {
            vcap.open(f_file);
            if (!vcap.isOpened()) return;
        }
        else
            t_boolImg = true;
    }
    StuDetect fdPep;
    //图片显示
    if (t_boolImg)
    {
        fdPep.FindMatHogPeople(image);
        imshow("图片", image);
        waitKey(0);
        return;
    }
    Mat t_outshow;
    string str_title;
    if (t_boolCam)
    {
        //摄像头
        str_title = "摄像头";
    }
    else    //视频播放      //方便测试 开始为1帧
    {
        str_title = "视频";
        vcap.set(CV_CAP_PROP_POS_FRAMES, 1);
    }
    while (true)
    {
        vcap >> t_outshow;
        if (t_outshow.empty())  return;
        fdPep.FindMatHogPeople(t_outshow);
        resize(t_outshow, t_outshow, Size(800, 600));
        namedWindow(str_title, CV_WINDOW_AUTOSIZE);
        imshow(str_title, t_outshow);
        waitKey(1);
    }
}

void CmdDetect::SpeedSignCheck(const char* f_file /*= ""*/)
{
    VideoCapture vcap;
    Mat image;
    //判断摄像头和视频文件
    bool t_boolCam = strlen(f_file) < 1;
    bool t_boolImg = false;
    if (t_boolCam)
    {
        vcap = VideoCapture(0);
        if (!vcap.isOpened())   return;
    }
    else
    {
        image = imread(f_file);
        if (image.empty())     //判断视频文件
        {
            vcap.open(f_file);
            if (!vcap.isOpened())   return;
        }
        else
            t_boolImg = true;
    }
    //参数
    vector<Rect> Sign_info;
    StuOpDetect fdSpeed;
    //图片显示
    if (t_boolImg)
    {
        fdSpeed.SpeedCheck(image, Sign_info);
        if (!Sign_info.empty())
            for (vector<Rect>::const_iterator r = Sign_info.begin(); r != Sign_info.end(); r++)
                cv::rectangle(image, *r, Scalar(0, 255, 0), 3);
        namedWindow("图片检测", CV_WINDOW_AUTOSIZE);    //CV_WND_PROP_FULLSCREEN        CV_WINDOW_AUTOSIZE
        Mat tmpshow(image);
        resize(tmpshow, tmpshow, Size(800, 600));
        imshow("图片检测", tmpshow);
        waitKey(0);
        return;
    }
    Mat t_outshow;
    string str_title;
    if (t_boolCam)
    {
        //摄像头
        str_title = "摄像头检测";
    }
    else    //视频播放      //方便测试 开始为1帧
    {
        str_title = "视频文件检测";
        vcap.set(CV_CAP_PROP_POS_FRAMES, 1);
    }
    while (true)
    {
        vcap >> t_outshow;
        if (t_outshow.empty())  return;
        fdSpeed.SpeedCheck(t_outshow, Sign_info);         //StuDetect().SpeedSignVec(t_outshow, Sign_info);
        if (!Sign_info.empty())
            for (vector<Rect>::const_iterator r = Sign_info.begin(); r != Sign_info.end(); r++)
                DrawCircle(t_outshow, *r, 0);
        Mat tmpshow(t_outshow);
        resize(tmpshow, tmpshow, Size(800, 600));
        namedWindow(str_title, CV_WINDOW_AUTOSIZE);
        imshow(str_title, tmpshow);
        waitKey(1);
    }
}

string CmdDetect::GetThisNowTime(void)
{
    time_t t = time(0);
    char buffer[9] = { 0 };
    strftime(buffer, 9, "%H:%M:%S", localtime(&t));
    //strftime(buffer, 9, "%H:%M", localtime(&t));
    return std::string(buffer);
}

cv::Mat CmdDetect::DrawCircle(Mat& f_img, Rect rt, float num/*=0*/)
{
    int r = rt.width / 2;
    Point pt(rt.width / 2 + rt.x, rt.height / 2 + rt.y);
    if (r < 10) r = 10;
    int thickness = 2, r_l = 3 * r / 2;
    circle(f_img, pt, r, Scalar(0, 255, 0), thickness);
    line(f_img, Point(pt.x - r_l, pt.y), Point(pt.x - r, pt.y), Scalar(0, 255, 0), thickness);
    line(f_img, Point(pt.x + r_l, pt.y), Point(pt.x + r, pt.y), Scalar(0, 255, 0), thickness);
    line(f_img, Point(pt.x, pt.y - r_l), Point(pt.x, pt.y - r), Scalar(0, 255, 0), thickness);
    line(f_img, Point(pt.x, pt.y + r_l), Point(pt.x, pt.y + r), Scalar(0, 255, 0), thickness);
    char buf[255] = { 0 };
    if (0 == num)
    {
        sprintf(buf, "(%d,%d)", pt.x, pt.y);    //    sprintf(buf,"%s : %d",f_name,f_num);
        cv::putText(f_img, buf, Point(pt.x + r, pt.y - r), CV_FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 255, 0), 2);
    }
    else
    {
        sprintf(buf, "%.1fm", num);           //    sprintf(buf,"%s : %d",f_name,f_num);
        cv::putText(f_img, buf, Point(pt.x + r, pt.y - r), CV_FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 255, 0), 2);
    }
    return f_img;
}

