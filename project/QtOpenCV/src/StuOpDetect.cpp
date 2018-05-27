

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
    //-----ѵ�������Ƶ�ʶ��-----
    //StuSVM mSvm;
    //���óߴ�
    //mSvm.SetHogWH(40, 40);
    //ѵ��xml
    //mSvm.TrainPathXml("SVM_SPEED.yml",
    //    "D:/Work/CPP/PRO/CppNote/CV_Img/exe/POS/",
    //    "D:/Work/CPP/PRO/CppNote/CV_Img/exe/NEG/");
    //��λ����
    //cout << mSvm.DataPredict("SVM_SPEED.yml", Mat) << endl;
    SetHogWH(40, 40);
    //ѵ��xml
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
    resize(f_checked, trainImg, CurSize, 0, 0, INTER_CUBIC);    //Ҫ���ͬ���Ĵ�С�ſ��Լ�⵽
    HOGDescriptor* hog = new HOGDescriptor(CurSize, cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);             //������˼���ο�����1,2
    vector<float>descriptors;//�������
    hog->compute(trainImg, descriptors, Size(1, 1), Size(0, 0));          //���ü��㺯����ʼ����
    cout << "The Detection Result:" << endl;
    cout << "HOG dims: " << descriptors.size() << endl;
    Mat SVMtrainMat = Mat::zeros(1, (int) descriptors.size(), CV_32FC1);
    unsigned long n = 0;
    for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
        SVMtrainMat.at<float> (0, n++) = *iter;
    float ret = svmtest->predict(SVMtrainMat);
    //ret���Ƿ�����
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
    string curr = string(InPath) + "*.*";       //�����ļ�
    int pResultNum(0);                          //�������������
    intptr_t done = 0, handle;
    if ((handle = _findfirst(curr.c_str(), &filefind)) == -1)
        return ;                                //ʲô��û�ҵ�
    while (!(done = _findnext(handle, &filefind)))
    {
        if (!strcmp(filefind.name, ".."))
            continue;                           //�˼�û���ļ�
        if ((filefind.attrib & _A_SUBDIR))      //�ļ��Ƿ�Ϊ�ļ���
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
    //����֧��opencv2/3 �벻Ҫ�޸�
    unsigned long n;
    vector<string> img_data;                //���������ϼ�
    vector<int> img_flag;                   //��������������
    for (vector<string>::const_iterator it = vecPos.begin(); it != vecPos.end(); ++it)
    {
        img_data.push_back(*it);
        img_flag.push_back(1);              //������Ϊ1
    }
    for (vector<string>::const_iterator it = vecNeg.begin(); it != vecNeg.end(); ++it)
    {
        img_data.push_back(*it);
        img_flag.push_back(0);              //������Ϊ0
    }
    int nImgNum = (int) img_data.size();    //����������
    Mat data_mat, res_mat = Mat::zeros(nImgNum, 1, CV_32S);    //���;���,�洢ÿ�����������ͱ�־
    Mat src, trainImg = Mat::zeros(hogHeight, hogWidht, CV_8UC3);    //��Ҫ������ͼƬ
    for (int i = 0; i != img_data.size() - 1; i++)
    {
        src = imread(img_data[i].c_str(), 1);
        resize(src, trainImg, CurSize, 0, 0, INTER_CUBIC);
        //������˼���ο�����1,2
        HOGDescriptor* hog = new HOGDescriptor(CurSize, Size(16, 16), Size(8, 8), Size(8, 8), 9);
        //�������
        vector<float>descriptors;
        //���ü��㺯����ʼ����
        hog->compute(trainImg, descriptors, Size(1, 1), Size(0, 0));
        if (i == 0)
            data_mat = Mat::zeros(nImgNum, (int) descriptors.size(), CV_32FC1);      //��������ͼƬ��С���з���ռ�
        n = 0;
        for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
        {
            data_mat.at<float> (i, n) = *iter;
            n++;
        }
        res_mat.at<int> (i, 0) = img_flag[i];
    }
#if CV_MAJOR_VERSION>2  //opencv3�汾
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
    //�������ѵ�����ݺ�ȷ����ѧϰ����,����SVMѧϰ�����
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
    int DescriptorDim;      //HOG�����ӵ�ά������ͼƬ��С����ⴰ�ڴ�С�����С��ϸ����Ԫ��ֱ��ͼbin��������
    Mat sampleFeatureMat;   //����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��
    Mat sampleLabelMat;     //ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�-1��ʾ����
    //��ⴰ��(64,128),��ߴ�(16,16),�鲽��(8,8),cell�ߴ�(8,8),ֱ��ͼbin����9
    HOGDescriptor hog(Size(hogWidht, hogHeight), Size(16, 16), Size(8, 8), Size(8, 8), 9);                //HOG���������������HOG�����ӵ�
    //-----------------------------��������ʱʹ�ò���----------------------------------------
    int posNum = 0, negNum = 0;
    vector<string> pos_path, neg_path;      //��������
    for (vector<string>::const_iterator it = vecPos.begin(); it != vecPos.end(); ++it)
        pos_path.push_back(*it);
    for (vector<string>::const_iterator it = vecNeg.begin(); it != vecNeg.end(); ++it)
        neg_path.push_back(*it);
    //------------------------------------------------------------------------------------
    /**************************����������************************************/
    for (vector<string>::size_type num = 0; num != pos_path.size(); num++)
    {
        src = imread(pos_path[num].c_str());
        //���������Ż�ü�����
        src = src(Rect(16, 16, 64, 128));       //��96*160��INRIA������ͼƬ����Ϊ64*128������ȥ�������Ҹ�16������
        vector<float> descriptors;//HOG����������
        hog.compute(src, descriptors, Size(8, 8));       //����HOG�����ӣ���ⴰ���ƶ�����(8,8)
        //�����һ������ʱ��ʼ�����������������������Ϊֻ��֪��������������ά�����ܳ�ʼ��������������
        if (0 == num)
        {
            DescriptorDim = descriptors.size();//HOG�����ӵ�ά��
            //��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat
            sampleFeatureMat = Mat::zeros(posNum + negNum + HardExampleNO, DescriptorDim, CV_32FC1);
            //��ʼ��ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�0��ʾ����
            sampleLabelMat = Mat::zeros(posNum + negNum + HardExampleNO, 1, CV_32FC1);
        }
        //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
        for (int i = 0; i < DescriptorDim; i++)
            sampleFeatureMat.at<float> (num, i) = descriptors[i];   //��num�����������������еĵ�i��Ԫ��
        sampleLabelMat.at<float> (num, 0) = 1;   //���������Ϊ1������
    }
    /*******************���ζ�ȡ������ͼƬ������HOG������*******************************/
    for (vector<string>::size_type num = 0; num != pos_path.size(); num++)
    {
        src = imread(pos_path[num].c_str());
        vector<float> descriptors;//HOG����������
        hog.compute(src, descriptors, Size(8, 8));       //����HOG�����ӣ���ⴰ���ƶ�����(8,8)
        //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
        for (int i = 0; i < DescriptorDim; i++)
            sampleFeatureMat.at<float> (num + posNum, i) = descriptors[i];   //��posNum+num�����������������еĵ�i��Ԫ��
        sampleLabelMat.at<float> (num + posNum, 0) = -1;   //���������Ϊ-1������
    }
    /*****************************ѵ��SVM������**************************************/
    //������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����
    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
    //SVM������SVM����ΪC_SVC�����Ժ˺������ɳ�����C=0.01
    CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);
    mSvm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);    //ѵ��������
    mSvm.save(OutXmlFile);    //��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�           cout<<"ѵ�����"<<endl;
}

int OpPepDetection::PredictTarget(const char* OutXmlFile, Mat& f_checked)
{
    //����xml�ļ�
    mSvm.load(OutXmlFile);
    //HOG�����ӵ�ά������ͼƬ��С����ⴰ�ڴ�С�����С��ϸ����Ԫ��ֱ��ͼbin��������
    int DescriptorDim = mSvm.get_var_count();
    //֧�������ĸ���
    int supportVectorNum = mSvm.get_support_vector_count();
    //cout<<"֧������������"<<supportVectorNum<<endl;
    //alpha���������ȵ���֧����������
    Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);
    //֧����������
    Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);
    //alpha��������֧����������Ľ��
    Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);
    //��֧�����������ݸ��Ƶ�supportVectorMat������
    for (int i = 0; i < supportVectorNum; i++)
    {
        const float* pSVData = mSvm.get_support_vector(i);    //���ص�i��֧������������ָ��
        for (int j = 0; j < DescriptorDim; j++)
            supportVectorMat.at<float> (i, j) = pSVData[j];
    }
    //��alpha���������ݸ��Ƶ�alphaMat��
    double* pAlphaData = mSvm.get_alpha_vector(); //����SVM�ľ��ߺ����е�alpha����
    for (int i = 0; i < supportVectorNum; i++)
        alphaMat.at<float> (0, i) = pAlphaData[i];
    //����-(alphaMat * supportVectorMat),����ŵ�resultMat��gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//��֪��Ϊʲô�Ӹ��ţ�
    resultMat = -1 * alphaMat * supportVectorMat;
    //�õ����յ�setSVMDetector(const vector<float>& detector)�����п��õļ����
    vector<float> myDetector;
    //��resultMat�е����ݸ��Ƶ�����myDetector��
    for (int i = 0; i < DescriptorDim; i++)
        myDetector.push_back(resultMat.at<float> (0, i));
    //������ƫ����rho���õ������
    myDetector.push_back(mSvm.get_rho());
    cout << "�����ά����" << myDetector.size() << endl;
    //�������Ӳ������ļ����Կ�����
    //std::ofstream fout("HOGDetectorForOpenCV.txt");
    //for(int i=0; i<myDetector.size(); i++)
    //    fout<<myDetector[i]<<endl;
    //fout.close();
    //���ü������
    vector<float> aaa =  HOGDescriptor::getDefaultPeopleDetector();
    people_dectect.setSVMDetector(myDetector);
    //���ο�����
    vector<Rect> found, found_filtered;
    people_dectect.detectMultiScale(f_checked, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
    cout << "���θ�����" << found.size() << endl;
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
    cout << "���˺���εĸ�����" << outnum << endl;
    //�����ο���Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ,����������Ҫ��һЩ����
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
    cout << "���˺���εĸ�����" << outnum << endl;
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
    int i = 0;  /*��ɫ��� �� �ҵ�������*/
    double t = 0;
    //�������
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
    //�ҶȻ�
    cvtColor(f_img, gray, COLOR_BGR2GRAY);
    //����
    resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
    //�Ҷ�ͼ��ֱ��ͼ���⻯
    equalizeHist(smallImg, smallImg);
    t = (double) cvGetTickCount();
    f_cascade.detectMultiScale(smallImg, faces,
                               1.1, 2, 0
                               //|CASCADE_FIND_BIGGEST_OBJECT
                               //|CASCADE_DO_ROUGH_SEARCH
                               | CASCADE_SCALE_IMAGE,
                               Size(30, 30));
    if (f_tryflip)   //��ת
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
        //�ڽ���ϻ�Բ
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
    int i = 0; /*��ɫ��� �� �ҵ�������*/
    //�������
    vector<Rect> faces;
    //�����ɫ
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
    //תΪ�Ҷ�ͼ
    cvtColor(f_img, gray, CV_BGR2GRAY);    //imshow("gray",gray);
    resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
    //�Ҷ�ͼ��ֱ��ͼ���⻯
    equalizeHist(smallImg, smallImg);    //imshow("smallImg",smallImg);
    //ʱ�����
    double t = (double) cvGetTickCount();
    //����
    f_cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
    cout << "detection time = " <<
         ((double) cvGetTickCount() - t) / ((double) cvGetTickFrequency() * 1000.)
         << " ms.   num=" << faces.size() << endl;
    for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
    {
        Point center;
        Scalar color = colors[i % 8];
        int radius;  //�뾶
        //��������w/h��ֵ
        double aspect_ratio = (double) r->width / r->height;
        //���滭����
        if (0.75 < aspect_ratio && aspect_ratio < 1.3)
        {
            //ȷ��Բ��λ��,�Ϳ��Զ�λ����
            center.x = cvRound((r->x + r->width * 0.5) * f_scale);
            center.y = cvRound((r->y + r->height * 0.5) * f_scale);
            radius = cvRound((r->width + r->height) * 0.25 * f_scale);
            //circle( f_img, center, radius, color, 3, 8, 0 );
            f_img = DrawCircle(f_img, center, radius, color);
        }
        else //������
            rectangle(f_img, Point(cvRound(r->x * f_scale), cvRound(r->y * f_scale)),
                      Point(cvRound((r->x + r->width - 1) *f_scale), cvRound((r->y + r->height - 1) *f_scale)), color, 3, 8, 0);
        //�����ǽ�ȡ����ķ���
        //Mat f_faceimg=f_img( Rect(r->x*f_scale,r->y*f_scale,(r->x + r->width-1)*f_scale,(r->y + r->height-1)*f_scale) );
    }
    return i;
}

int StuDetect::FindMatHogPeople(Mat& f_img) const
{
    //double t = (double)cvGetTickCount();
    int outnum = 0;
    //HOG���������
    HOGDescriptor people_dectect_hog;
    //����Ĭ�ϵ�   �Ѿ�ѵ�����˵�svmϵ����Ϊ�˴μ���ģ��
    people_dectect_hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    //���ο�����
    vector<Rect> found, found_filtered;
    //imgΪ���������ͼƬ��foundΪ��⵽Ŀ�������б�����3Ϊ�����ڲ�����Ϊ����Ŀ�����ֵ��Ҳ���Ǽ�⵽��������SVM���೬ƽ��ľ���;
    //����4Ϊ��������ÿ���ƶ��ľ��롣�������ǿ��ƶ���������������5Ϊͼ������Ĵ�С������6Ϊ����ϵ����������ͼƬÿ�γߴ��������ӵı�����
    //����7Ϊ����ֵ����У��ϵ������һ��Ŀ�걻������ڼ�����ʱ���ò�����ʱ�����˵������ã�Ϊ0ʱ��ʾ����������á�
    people_dectect_hog.detectMultiScale(f_img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
    //cout<<"���θ�����"<<found.size()<<endl;
    //�ҳ�����û��Ƕ�׵ľ��ο�r,������found_filtered��,�����Ƕ�׵Ļ�,��ȡ���������Ǹ����ο����found_filtered��
    //��Դ���п��Կ���:
    //#define __SIZE_TYPE__ long unsigned int
    //typedef __SIZE_TYPE__ size_t;
    //���,size_t��һ��long unsigned int����
    size_t i, j;
    for (i = 0; i < found.size(); i++)
    {
        Rect r = found[i];
        //��������for������ҳ�����û��Ƕ�׵ľ��ο�r,������found_filtered��,�����Ƕ�׵�
        //��,��ȡ���������Ǹ����ο����found_filtered��
        for (j = 0; j < found.size(); j++)
            if (j != i && (r & found[j]) == r)
                break;
        if (j == found.size())
            found_filtered.push_back(r);
    }
    outnum = (int) found_filtered.size();
    //cout << "Pep_time =" << ((double)cvGetTickCount() - t) / ((double)cvGetTickFrequency()*1000.) << "ms ";
    cout << "���˺���εĸ�����" << outnum << endl;
    //�����ο���Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ,����������Ҫ��һЩ����
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
    cv::Mat trainImg;// = cv::Mat::zeros(ImgHeight, ImgWidht, CV_8UC3);//��Ҫ������ͼƬ
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
    //����Դͼ��ROI
    Mat img = fsrc(Rect(CFGinfo.InRegion.bx, CFGinfo.InRegion.by, CFGinfo.InRegion.ex -
                        CFGinfo.InRegion.bx, CFGinfo.InRegion.ey - CFGinfo.InRegion.by));
    //ת��HSV
    cv::Mat  hsv_base;
    cv::cvtColor(img, hsv_base, CV_BGR2HSV);      //16ms
    //��ɫ�ָ�
    cv::Mat colorCut = cv::Mat(img.rows, img.cols, CV_32F), how_red = cv::Mat(img.rows, img.cols, CV_32F);
    //�Ż�????????????
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
    //��ɫλͼ��ֵ�˲�//
    //��ɫ�̶�ͼ�˲�//
    cv::Mat blueredImg = cv::Mat(colorCut.rows, colorCut.cols, CV_32F), blueredHowRed = cv::Mat(colorCut.rows, colorCut.cols, CV_32F);
    cv::blur(colorCut, blueredImg, Size(7, 7));
    cv::blur(how_red, blueredHowRed, Size(7, 7));
    //////////////////////////////////////////////////////////////////////////
    //�õ�����ϸ����Ҫ��ȡ�����ͼ��
    cv::Mat RefinedImg(colorCut.rows, colorCut.cols, CV_8UC1), refine(colorCut.rows, colorCut.cols, CV_8UC1);
    //�Ż�
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
    //��������
    cv::morphologyEx(RefinedImg, RefinedImg, cv::MORPH_OPEN, cv::Mat());
    //????????????????????????????????????????
    //��ȡ����Ѱ�Һ��
    vector<vector<cv::Point> > contours;
    cv::findContours(RefinedImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);    // retrieve all pixels of each contours
    //ɸѡ����̫���̫С��ȥ��//
    cv::Mat resultt(colorCut.rows, colorCut.cols, CV_8U, Scalar(0));
    //��ɸѡ�����������Ӷ�͹����//
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
    //�������
    cv::drawContours(resultt, contours2, -1, Scalar(255), CV_FILLED);
    cv::minMaxLoc(resultt, &minVal, &maxVal);
    //��ʾ������
    cv::Mat Hole(img.rows, img.cols, CV_8UC1);
    for (int x = 0; x < img.rows; x++)
        for (int y = 0; y < img.cols; y++)
            Hole.data[x * img.cols + y] = resultt.data[x * img.cols + y] > refine.data[x * img.cols + y] ? 255 : 0;
    cv::erode(Hole, Hole, cv::Mat());
    // ��������
    cv::morphologyEx(Hole, Hole, cv::MORPH_OPEN, cv::Mat());
    // ��ȡ����
    vector<vector<cv::Point> > contours_tmp;
    cv::findContours(Hole, contours_tmp, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    double maxarea(800), minarea(190);
    for (vector<vector<cv::Point> >::const_iterator itContours = contours_tmp.begin(); itContours != contours_tmp.end(); ++itContours)
    {
        //�������
        double tmparea = fabs(cv::contourArea(*itContours));
        //�����������
        Rect aRect = cv::boundingRect(*itContours);
        //�����ݺ��
        float rate = (float) aRect.height / aRect.width;
        //����Բ
        int Ri = (aRect.height + aRect.width) / 4;
        float rater = float ((pi * Ri * Ri) / tmparea);
        if (!(tmparea < minarea ||                          //ɾ�����С���趨ֵ������
              aRect.width > 100 || aRect.height > 100 ||      //ɾ����ߴ�����ֵ������
              aRect.height < 10 || aRect.width < 10 ||        //ɾ�����С��10������
              rate < 0.8 || rate > 1.5 ||                     //ɾ���ݺ�Ȳ���һ����Χ�ڵ�����
              rater < 0.8 || rater > 1.2))                    //ɾ����Բ������
        {
            int xmax(0), ymax(0), xmin(2000), ymin(2000);             //���ο������
            //�������δ����ͼƬ
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
                //Ҫ���ͬ���Ĵ�С�ſ��Լ�⵽
                resize(test, trainImg, Size(ImgWidht, ImgHeight), 0, 0, INTER_CUBIC);
                //�������
                vector<float>descriptors;
                Ptr<HOGDescriptor> hog(new cv::HOGDescriptor(Size(ImgWidht, ImgHeight), Size(16, 16), Size(8, 8), Size(8, 8), 9));
                //cv::HOGDescriptor *hog = new cv::HOGDescriptor(Size(ImgWidht, ImgHeight), Size(16, 16), Size(8, 8), Size(8, 8), 9);
                //���ü��㺯����ʼ����
                hog->compute(trainImg, descriptors, Size(1, 1), Size(0, 0));
                //delete[] hog;
                Mat SVMtrainMat = Mat::zeros(1, (int) descriptors.size(), CV_32FC1);
                n = 0;
                for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
                    SVMtrainMat.at<float> (0, n++) = *iter;
                if (1 == svmtest->predict(SVMtrainMat))      //������Ϊ1
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
    //��Ҫ������ͼƬ
    cv::Mat trainImg = cv::Mat::zeros(ImgHeight, ImgWidht, CV_8UC3);
    //����Դͼ��ROI
    Mat img, hsv_base;
    f_src.copyTo(img);
    //ת��HSV
    cv::cvtColor(img, hsv_base, CV_BGR2HSV);      //16ms
    //��ɫ�ָ�
    cv::Mat colorCut = cv::Mat(img.rows, img.cols, CV_32F), how_red = cv::Mat(img.rows, img.cols, CV_32F);
    //�Ż�HSV
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
    //��ɫλͼ��ֵ�˲�
    //��ɫ�̶�ͼ�˲�
    cv::Mat blueredImg = cv::Mat(colorCut.rows, colorCut.cols, CV_32F), blueredHowRed = cv::Mat(colorCut.rows, colorCut.cols, CV_32F);
    colorCut /= 255.;
    cv::blur(colorCut, blueredImg, Size(7, 7));
    cv::minMaxLoc(blueredImg, min_value, max_value);
    cv::blur(how_red, blueredHowRed, Size(7, 7));
    //////////////////////////////////////////////////////////////////////////
    //�õ�����ϸ����Ҫ��ȡ�����ͼ��
    cv::Mat RefinedImg(colorCut.rows, colorCut.cols, CV_8UC1), refine(colorCut.rows, colorCut.cols, CV_8UC1);
    //�Ż�
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
    //��������
    cv::dilate(RefinedImg, RefinedImg, cv::Mat());
    cv::erode(RefinedImg, RefinedImg, cv::Mat());
    //��ȡ����Ѱ�Һ��
    vector<vector<cv::Point>> contours;
    cv::findContours(RefinedImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);    // retrieve all pixels of each contours
    //ɸѡ����̫���̫С��ȥ��
    cv::Mat resultt(colorCut.rows, colorCut.cols, CV_8U, Scalar(0));
    //��ɸѡ�����������Ӷ�͹����
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
    //�������
    cv::drawContours(resultt, contours2, -1, Scalar(255), CV_FILLED);
    //��͹�����
    cv::minMaxLoc(resultt, min_value, max_value);
    //��ʾ���
    cv::Mat Hole(img.rows, img.cols, CV_8UC1);
    for (int x = 0; x < img.rows; x++)
        for (int y = 0; y < img.cols; y++)
            Hole.data[x * img.cols + y] = resultt.data[x * img.cols + y] > refine.data[x * img.cols + y] ? 255 : 0;
    cv::erode(Hole, Hole, cv::Mat());
    //��������//
    cv::morphologyEx(Hole, Hole, cv::MORPH_OPEN, cv::Mat());
    // ��ȡ����
    vector<vector<cv::Point> > contours_tmp;
    cv::findContours(Hole, contours_tmp, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    double maxarea(800), minarea(190);
    for (vector<vector<cv::Point> >::const_iterator itContours = contours_tmp.begin(); itContours != contours_tmp.end(); ++itContours)
    {
        //�������
        double tmparea = fabs(cv::contourArea(*itContours));
        //�����������
        Rect aRect = cv::boundingRect(*itContours);
        //�����ݺ��
        float rate = (float) aRect.height / aRect.width;
        //����Բ
        int Ri = (aRect.height + aRect.width) / 4;
        float rater = float ((pi * Ri * Ri) / tmparea);
        if (!(tmparea < minarea ||                          //ɾ�����С���趨ֵ������
              aRect.width > 100 || aRect.height > 100 ||      //ɾ����ߴ�����ֵ������
              aRect.height < 10 || aRect.width < 10 ||
              rate < 0.8 || rate > 1.5 ||                     //ɾ���ݺ�Ȳ���һ����Χ�ڵ�����
              rater < 0.8 || rater > 1.2))                    //ɾ����Բ������
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
                //imshow("������",test);
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
    //=========��������������===========================
    //StuOpDetect().man_GetPngPosImg("../DllRes/img/lanbuda.png",1000,15);
    //=========��������������===========================
    //StuOpDetect().man_GetNegImg("../DllRes/img/group.jpg",1000,156);
}

void StuOpDetect::man_GetPngPosImg(char* f_png, int f_num, int f_lengh) const
{
    CreateDirectoryA("Pos", NULL);
    Mat f_logo_n = cv::imread(f_png), mask_n = cv::imread(f_png, 0);
    Mat f_logo = f_logo_n.clone(), mask = mask_n.clone();
    Mat f_NaMat = Mat(f_logo.rows + f_lengh, f_logo.cols + f_lengh, CV_8UC4);
    char saveFileName[100] = { '\0' };
    /*�����Լ����õĲ���������ͼƬ*/
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);    //png��ʽ�£�Ĭ�ϵĲ���Ϊ3.
    float du = 0.0;     //��ת���������
    for (int k = 0; k < f_num; k++)
    {
        f_NaMat = Mat(f_logo.rows + f_lengh, f_logo.cols + f_lengh, CV_8UC3, Scalar(255, 255, 255));
        int i = rand() % f_lengh;
        int j = rand() % f_lengh;
        //������ͼ����ת����
        du = float (rand() * float (-15.0));     //du = -15.0(��);
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
    cout << "�����������������!" << endl;
}

void StuOpDetect::man_GetNegImg(char* f_file, int f_num, int f_lengh) const
{
    //���ɸ�����
    CreateDirectoryA("Neg", NULL);
    Mat f_img = imread(f_file), t_out;
    int t_w(f_img.rows - f_lengh), t_h(f_img.cols - f_lengh);
    char saveFileName[100] = { '\0' };
    /*�����Լ����õĲ���������ͼƬ*/
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    for (int k = 0; k < f_num; k++)
    {
        //�������������
        int i = rand() % t_h;
        int j = rand() % t_w;
        t_out = f_img(Rect(Point(i, j), Point(i + f_lengh, j + f_lengh)));
        sprintf(saveFileName, "Neg/%d.jpg", k + 1);
        //imshow("ss",t_out);
        //waitKey(0);
        imwrite(saveFileName, t_out, compression_params);
    }
    cout << "�����������������!" << endl;
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
    //ʱ�����
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
    // ����Ŀ��ͼ��Ĵ�С��������Դͼ��һ��
    Mat t_dstImg_warp = Mat::zeros(f_img.rows, f_img.cols, f_img.type());
    //��6����ͼ��������ź�����ת
    // ������ͼ���е�˳ʱ����ת50����������Ϊ0.6����ת����
    Point center = Point(t_dstImg_warp.cols / 2, t_dstImg_warp.rows / 2);
    //double angle = -30.0;   //��ת����
    double scale = 1.0;   //���ű���
    // ͨ���������תϸ����Ϣ�����ת����
    Mat rotMat(30, 30, CV_32FC1);
    rotMat = getRotationMatrix2D(center, angle, scale);
    // ��ת�����ź��ͼ��
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
    //�ж�����ͷ����Ƶ�ļ�
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
        if (image.empty())     //�ж���Ƶ�ļ�
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
    StuDetect fdFace;       //�����δ�������
    //ͼƬ��ʾ
    if (t_boolImg)
    {
        fdFace.FindMatFace(image, cascade, scale);
        namedWindow("ͼƬ���", CV_WINDOW_AUTOSIZE);    //CV_WND_PROP_FULLSCREEN    CV_WINDOW_AUTOSIZE
        imshow("ͼƬ���", image);
        waitKey(0);
        return;
    }
    Mat t_outshow;
    string str_title;
    if (t_boolCam)
    {
        //����ͷ
        str_title = "����ͷ���";
    }
    else    //��Ƶ����      //������� ��ʼΪ1֡
    {
        str_title = "��Ƶ�ļ����";
        vcap.set(CV_CAP_PROP_POS_FRAMES, 1);
    }
    while (true)
    {
        vcap >> t_outshow;
        if (t_outshow.empty()) return;
        fdFace.FindMatFace(t_outshow, cascade, scale);
        resize(t_outshow, t_outshow, Size(800, 600));
        namedWindow("��Ƶ�ļ����", CV_WINDOW_AUTOSIZE);
        imshow("��Ƶ�ļ����", t_outshow);
        waitKey(1);
    }
}

void CmdDetect::PedestrianCheck(const char* f_file /*= ""*/)
{
    VideoCapture vcap;
    Mat image;
    //�ж�����ͷ����Ƶ�ļ�
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
    //ͼƬ��ʾ
    if (t_boolImg)
    {
        fdPep.FindMatHogPeople(image);
        imshow("ͼƬ", image);
        waitKey(0);
        return;
    }
    Mat t_outshow;
    string str_title;
    if (t_boolCam)
    {
        //����ͷ
        str_title = "����ͷ";
    }
    else    //��Ƶ����      //������� ��ʼΪ1֡
    {
        str_title = "��Ƶ";
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
    //�ж�����ͷ����Ƶ�ļ�
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
        if (image.empty())     //�ж���Ƶ�ļ�
        {
            vcap.open(f_file);
            if (!vcap.isOpened())   return;
        }
        else
            t_boolImg = true;
    }
    //����
    vector<Rect> Sign_info;
    StuOpDetect fdSpeed;
    //ͼƬ��ʾ
    if (t_boolImg)
    {
        fdSpeed.SpeedCheck(image, Sign_info);
        if (!Sign_info.empty())
            for (vector<Rect>::const_iterator r = Sign_info.begin(); r != Sign_info.end(); r++)
                cv::rectangle(image, *r, Scalar(0, 255, 0), 3);
        namedWindow("ͼƬ���", CV_WINDOW_AUTOSIZE);    //CV_WND_PROP_FULLSCREEN        CV_WINDOW_AUTOSIZE
        Mat tmpshow(image);
        resize(tmpshow, tmpshow, Size(800, 600));
        imshow("ͼƬ���", tmpshow);
        waitKey(0);
        return;
    }
    Mat t_outshow;
    string str_title;
    if (t_boolCam)
    {
        //����ͷ
        str_title = "����ͷ���";
    }
    else    //��Ƶ����      //������� ��ʼΪ1֡
    {
        str_title = "��Ƶ�ļ����";
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

