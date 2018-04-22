
#include "stdafx.h"
#include "TestCarNum.h"
#include <iostream>
#include <vector>
#include "DetectRegions.h"

using namespace std;
using namespace cv;

const int numFilesChars[] = {35, 40, 42, 41, 42, 33, 30, 31, 49, 44, 30, 24, 21, 20, 34, 9, 10, 3, 11, 3, 15, 4, 9, 12, 10, 21, 18, 8, 15, 7};

OCR ocr;    //全局

int generateRandom(int n, int min, int max, vector<int>* samples)
{
    int range = max - min;
    int r = rand() % range + min;
    if (samples->at(r) == 0)
    {
        samples->at(r) = 1;
        n++;
    }
    if (n < 100)
        generateRandom(n, min, max, samples);
    return 0;
}

float test(Mat samples, Mat classes)
{
    float errors = 0;
    for (int i = 0; i < samples.rows; i++)
    {
        int result = ocr.classify(samples.row(i));
        if (result != classes.at<int> (i))
            errors++;
    }
    return errors / samples.rows;
}

//获得测试文件的文件名
string getFilename(string s)
{
    char sep = '/';
    char sepExt = '.';
#ifdef _WIN32
    sep = '\\';
#endif
    size_t i = s.rfind(sep, s.length());
    if (i != string::npos)
    {
        string fn = (s.substr(i + 1, s.length() - i));
        size_t j = fn.rfind(sepExt, fn.length());
        if (i != string::npos)
            return fn.substr(0, j);
        else
            return fn;
    }
    else
        return s.substr(0, s.length() - 4);
}

int trainSVM(int argc, char** argv)
{
    cout << "OpenCV Training SVM Automatic Number Plate Recognition\n";
    cout << "\n";
    char* path_Plates;
    char* path_NoPlates;
    int numPlates;
    int numNoPlates;
    int imageWidth = 144;
    int imageHeight = 33;
    //Check if user specify image to process
    if (argc >= 5)
    {
        numPlates = atoi(argv[1]);
        numNoPlates = atoi(argv[2]);
        path_Plates = argv[3];
        path_NoPlates = argv[4];
    }
    else
    {
        cout << "Usage:\n" << argv[0] << " <num Plate Files> <num Non Plate Files> <path to plate folder files> <path to non plate files> \n";
        return 0;
    }
    Mat classes;//(numPlates+numNoPlates, 1, CV_32FC1);
    Mat trainingData;//(numPlates+numNoPlates, imageWidth*imageHeight, CV_32FC1 );
    Mat trainingImages;
    vector<int> trainingLabels;
    for (int i = 0; i < numPlates; i++)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << path_Plates << i << ".jpg";
        Mat img = imread(ss.str(), 0);
        img = img.reshape(1, 1);
        trainingImages.push_back(img);
        trainingLabels.push_back(1);
    }
    for (int i = 0; i < numNoPlates; i++)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << path_NoPlates << i << ".jpg";
        Mat img = imread(ss.str(), 0);
        img = img.reshape(1, 1);
        trainingImages.push_back(img);
        trainingLabels.push_back(0);
    }
    Mat(trainingImages).copyTo(trainingData);
    //trainingData = trainingData.reshape(1,trainingData.rows);
    trainingData.convertTo(trainingData, CV_32FC1);
    Mat(trainingLabels).copyTo(classes);
    FileStorage fs("SVM.xml", FileStorage::WRITE);
    fs << "TrainingData" << trainingData;
    fs << "classes" << classes;
    fs.release();
    return 0;
}

int trainOCR(int argc, char** argv)
{
    cout << "OpenCV Training OCR Automatic Number Plate Recognition\n";
    cout << "\n";
    char* path;
    //Check if user specify image to process
    if (argc >= 1)
        path = argv[1];
    else
    {
        cout << "Usage:\n" << argv[0] << " <path to chars folders files> \n";
        return 0;
    }
    Mat classes;
    Mat trainingDataf5;
    Mat trainingDataf10;
    Mat trainingDataf15;
    Mat trainingDataf20;
    vector<int> trainingLabels;
    OCR ocr;
    for (int i = 0; i < OCR::numCharacters; i++)
    {
        int numFiles = numFilesChars[i];
        for (int j = 0; j < numFiles; j++)
        {
            cout << "Character " << OCR::strCharacters[i] << " file: " << j << "\n";
            stringstream ss(stringstream::in | stringstream::out);
            ss << path << OCR::strCharacters[i] << "/" << j << ".jpg";
            Mat img = imread(ss.str(), 0);
            Mat f5 = ocr.features(img, 5);
            Mat f10 = ocr.features(img, 10);
            Mat f15 = ocr.features(img, 15);
            Mat f20 = ocr.features(img, 20);
            trainingDataf5.push_back(f5);
            trainingDataf10.push_back(f10);
            trainingDataf15.push_back(f15);
            trainingDataf20.push_back(f20);
            trainingLabels.push_back(i);
        }
    }
    trainingDataf5.convertTo(trainingDataf5, CV_32FC1);
    trainingDataf10.convertTo(trainingDataf10, CV_32FC1);
    trainingDataf15.convertTo(trainingDataf15, CV_32FC1);
    trainingDataf20.convertTo(trainingDataf20, CV_32FC1);
    Mat(trainingLabels).copyTo(classes);
    FileStorage fs("OCR.xml", FileStorage::WRITE);
    fs << "TrainingDataF5" << trainingDataf5;
    fs << "TrainingDataF10" << trainingDataf10;
    fs << "TrainingDataF15" << trainingDataf15;
    fs << "TrainingDataF20" << trainingDataf20;
    fs << "classes" << classes;
    fs.release();
    return 0;
}

int evalOCR(int argc, char** argv)
{
    int nh;
    char* data;
    //Check params
    if (argc >= 2)
    {
        nh = atoi(argv[1]);
        data = argv[2];
    }
    else
    {
        cout << "Usage:\n" << argv[0] << " <num hidden nodes> <data to load from xml>\n";
        return 0;
    }
    Mat classes;
    Mat trainingData;
    //Read file storage.
    FileStorage fs;
    fs.open("OCR.xml", FileStorage::READ);
    fs[data] >> trainingData;
    fs["classes"] >> classes;
    float result;
    //init random generator
    srand(time(NULL));
    //Create 100 random pos for samples
    std::vector<int> isSample(trainingData.rows, 0);
    generateRandom(0, 0, trainingData.rows - 1, &isSample);
    //Create sample data
    Mat train, trainClasses;
    Mat samples, samplesClasses;
    for (int i = 0; i < trainingData.rows; i++)
    {
        if (isSample[i] == 1)
        {
            samples.push_back(trainingData.row(i));
            samplesClasses.push_back(classes.row(i));
        }
        else
        {
            train.push_back(trainingData.row(i));
            trainClasses.push_back(classes.row(i));
        }
    }
    result = 0;
    ocr.train(train, trainClasses, nh);
    result = test(samples, samplesClasses);
    //cout << nh <<  "\t" << data << "\t"<< result <<"\n";
    cout << result ;
    return 0;
}

int testCar(int argc, char** argv)
{
    cout << "OpenCV Automatic Number Plate Recognition\n";
    char* filename;
    Mat input_image;
    ////Check if user specify image to process
    //if(argc >= 2 )
    //{
    //    filename= argv[1];
    //    //load image  in gray level
    //    input_image=imread(filename,1);
    //}else{
    //    printf("Use:\n\t%s image\n",argv[0]);
    //    return 0;
    //}
    filename = "9588DWV.jpg";
    //filename= "F:\\go\\CppNote\\exe\\testcar.jpg";
    input_image = imread(filename, 1);
    string filename_whithoutExt = getFilename(filename);
    cout << "working with file: " << filename_whithoutExt << "\n";
    //Detect posibles plate regions
    DetectRegions detectRegions;
    detectRegions.setFilename(filename_whithoutExt);
    detectRegions.saveRegions = false;
    detectRegions.showSteps = false;
    vector<Plate> posible_regions = detectRegions.run(input_image);
    //SVM for each plate region to get valid car plates
    //Read file storage.
    FileStorage fs;
    fs.open("SVM.xml", FileStorage::READ);
    Mat SVM_TrainingData;
    Mat SVM_Classes;
    fs["TrainingData"] >> SVM_TrainingData;
    fs["classes"] >> SVM_Classes;
    //Set SVM params
    CvSVMParams SVM_params;
    SVM_params.svm_type = CvSVM::C_SVC;
    SVM_params.kernel_type = CvSVM::LINEAR; //CvSVM::LINEAR;
    SVM_params.degree = 0;
    SVM_params.gamma = 1;
    SVM_params.coef0 = 0;
    SVM_params.C = 1;
    SVM_params.nu = 0;
    SVM_params.p = 0;
    SVM_params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01);
    //Train SVM
    CvSVM svmClassifier(SVM_TrainingData, SVM_Classes, Mat(), Mat(), SVM_params);
    //For each possible plate, classify with svm if it's a plate or no
    vector<Plate> plates;
    for (int i = 0; i < posible_regions.size(); i++)
    {
        Mat img = posible_regions[i].plateImg;
        Mat p = img.reshape(1, 1);
        p.convertTo(p, CV_32FC1);
        int response = (int) svmClassifier.predict(p);
        if (response == 1)
            plates.push_back(posible_regions[i]);
    }
    cout << "Num plates detected: " << plates.size() << "\n";
    //For each plate detected, recognize it with OCR
    OCR ocr("OCR.xml");
    ocr.saveSegments = true;
    ocr.DEBUG = false;
    ocr.filename = filename_whithoutExt;
    for (int i = 0; i < plates.size(); i++)
    {
        Plate plate = plates[i];
        string plateNumber = ocr.run(&plate);
        string licensePlate = plate.str();
        cout << "================================================\n";
        cout << "License plate number: " << licensePlate << "\n";
        cout << "================================================\n";
        rectangle(input_image, plate.position, Scalar(0, 0, 200));
        putText(input_image, licensePlate, Point(plate.position.x, plate.position.y),
                CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 200), 2);
        if (false)
        {
            imshow("Plate Detected seg", plate.plateImg);
            cvWaitKey(0);
        }
    }
    imshow("Plate Detected", input_image);
    waitKey(0);
    //for(;;)
    //{
    //    int c;
    //    c = cvWaitKey(10);
    //    if( (char) c == 27)
    //        break;
    //}
    return 0;
}


