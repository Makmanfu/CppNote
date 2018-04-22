

#ifndef DetectRegions_h
#define DetectRegions_h

#include <string.h>
#include <vector>
#include <time.h>

class Plate
{
public:
    Plate();
    Plate(Mat img, Rect pos);
    string str();
public:
    Rect position;
    Mat plateImg;
    vector<char> chars;
    vector<Rect> charsPos;
};

class DetectRegions
{
public:
    string filename;
    bool saveRegions;
    bool showSteps;
public:
    DetectRegions();
    void setFilename(string f);
    vector<Plate> run(Mat input);
private:
    vector<Plate> segment(Mat input);
    bool verifySizes(RotatedRect mr);
    Mat histeq(Mat in);
};

class CharSegment
{
public:
    CharSegment();
    CharSegment(Mat i, Rect p);
public:
    Mat img;
    Rect pos;
};

class OCR
{
#define HORIZONTAL      1
#define VERTICAL        0
public:
    bool DEBUG;
    bool saveSegments;
    string filename;
    static const int numCharacters;
    int charSize;
public:
    static const char strCharacters[];
    OCR(string trainFile);
    OCR();
    string run(Plate* input);
    Mat preprocessChar(Mat in);
    int classify(Mat f);
    void train(Mat trainData, Mat trainClasses, int nlayers);
    int classifyKnn(Mat f);
    void trainKnn(Mat trainSamples, Mat trainClasses, int k);
    Mat features(Mat input, int size);
private:
    bool trained;
    vector<CharSegment> segment(Plate input);
    Mat Preprocess(Mat in, int newSize);
    Mat getVisualHistogram(Mat* hist, int type);
    void drawVisualFeatures(Mat character, Mat hhist, Mat vhist, Mat lowData);
    Mat ProjectedHistogram(Mat img, int t);
    bool verifySizes(Mat r);
    CvANN_MLP  ann;
    CvKNearest knnClassifier;
    int K;
};

#endif
