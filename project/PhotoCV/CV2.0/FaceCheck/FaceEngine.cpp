
#include "stdafx.h"
#include "FaceEngine.h"

//XmlOperate ************************************************************************

XmlOperate::XmlOperate()
    : mXmlDoc()
    , mRootXmlElement(NULL)
    , mPchildXmlNode(NULL)
    , mAttributXmlNode(NULL)
    , mdecl(NULL)
{
}

XmlOperate::~XmlOperate()
{
    ClearXmlData();
}

Face_Result_E XmlOperate::initXmlOperate()
{
    return ClearXmlData();
}

void XmlOperate::setBindXmlReadElement(FunXmlGetElement _fun)
{
    mGetElementCB = _fun;
}

Face_Result_E XmlOperate::loadXmlFile(char* pathOrBuf, int leng)
{
    Face_Result_E _result = RESULT_UNKNOWN;
    if (leng == 0)
    {
        if (!mXmlDoc.LoadFile(pathOrBuf))
            return RESULT_XML_LOAD_FAILURE;
    }
    else
    {
        if (!mXmlDoc.Parse(pathOrBuf))
            return RESULT_XML_LOAD_FAILURE;
    }
    _result = GetDocRoodNode();
    if (_result == RESULT_SUCCESS)
    {
        if (ParaseUpdateXml(mRootXmlElement) == RESULT_SUCCESS)
            return RESULT_SUCCESS;
        else
            return RESULT_XML_RESOLVE_FAILURE;
    }
    else
        return RESULT_XML_RESOLVE_FAILURE;
}

const char* XmlOperate::getXMLElementText()
{
    if (mPchildXmlNode)
    {
        TiXmlNode* _temp = mPchildXmlNode->FirstChild();
        if (_temp)
        {
            if (_temp->Type() == TiXmlNode::TINYXML_TEXT)
                return _temp->Value();
            else
                return "";
        }
        else
            return "";
    }
    else
        return "";
}

const char* XmlOperate::getXMLAttributeText()
{
    return mAttributXmlNode->Value();
}

Face_Result_E XmlOperate::createXmlDocHead(char* Version , char* Encoding , char* Standalone)
{
    Face_Result_E _result = RESULT_UNKNOWN;
    _result = ClearXmlData();
    if (_result == RESULT_FAILURE)
        return RESULT_FAILURE;
    mdecl = new TiXmlDeclaration(Version, Encoding, Standalone);
    mXmlDoc.LinkEndChild(mdecl);
    return RESULT_SUCCESS;
}

//添加节点
Face_Result_E XmlOperate::addNode(char* strNodeName, char* strElementNodeName, char* strElementNodeValue)
{
    if (!InsterElement(strNodeName, strElementNodeName, strElementNodeValue))
        return RESULT_XML_INSTALL_ELEMENT_FALURE;
    return RESULT_SUCCESS;
}

//添加属性
Face_Result_E XmlOperate::addNodeAttribut(char* strNodeName, char* strElementAttributName, char* strElementAttributValue)
{
    if (!InsterAttribut(strNodeName, strElementAttributName, strElementAttributValue))
        return RESULT_XML_INSTALL_ATTRIBUTE_FALURE;
    return RESULT_SUCCESS;
}

Face_Result_E XmlOperate::saveXmlDoc(char* paht)
{
    mXmlDoc.SaveFile(paht);
    return RESULT_SUCCESS;
}

Face_Result_E XmlOperate::ClearXmlData()
{
    mXmlDoc.Clear();
    if (mRootXmlElement != NULL)
        mRootXmlElement = NULL;
    if (mPchildXmlNode != NULL)
        mPchildXmlNode = NULL;
    if (mAttributXmlNode != NULL)
        mAttributXmlNode = NULL;
    if (mdecl != NULL)
        mdecl = NULL;
    return RESULT_SUCCESS;
}

Face_Result_E XmlOperate::GetDocRoodNode()
{
    mRootXmlElement = mXmlDoc.RootElement();
    if (!mRootXmlElement)
        return RESULT_XML_ROOT_FAILURE;
    return RESULT_SUCCESS;
}

Face_Result_E XmlOperate::ParaseUpdateXml(TiXmlNode* pParent)
{
    if (pParent == NULL)
        return RESULT_XML_ROOT_FAILURE;
    TiXmlNode* pchild = pParent->FirstChild();
    while (pchild)
    {
        int t = pchild->Type();
        if (t == TiXmlNode::TINYXML_ELEMENT)
        {
            mPchildXmlNode = pchild;
            mGetElementCB(pchild->Value(), XML_ELEMENT);
            mAttributXmlNode = pchild->ToElement()->FirstAttribute();
            if (mAttributXmlNode)
            {
                TiXmlNode* node = pchild;
                while (node)
                {
                    while (mAttributXmlNode)
                    {
                        mGetElementCB(mAttributXmlNode->Name(), XML_ATTRIBUT);
                        mAttributXmlNode = mAttributXmlNode->Next();
                    }
                    node =  node->NextSiblingElement();
                }
            }
            ParaseUpdateXml(pchild);
        }
        pchild = pchild->NextSibling();
    }
    return RESULT_SUCCESS;
}

//插入节点
bool XmlOperate::InsterElement(char* strNodeName, char* strElementNodeName, char* strElementNodeValue)
{
    if (strNodeName == NULL && strElementNodeValue == NULL)
    {
        TiXmlElement* MainRoot = new TiXmlElement(strElementNodeName);
        mXmlDoc.LinkEndChild(MainRoot);
        return true;
    }
    else
    {
        TiXmlElement* pRootEle = mXmlDoc.RootElement();
        while (pRootEle != NULL)
        {
            if (NULL == pRootEle)
                return false;
            TiXmlElement* pNode = NULL;
            GetNodePointerByName(pRootEle, strNodeName, pNode);
            TiXmlElement* pNode2 = pNode;
            while (pNode2 != NULL)
            {
                pNode2 = pNode2->NextSiblingElement();
                if (pNode2 != NULL)
                    pNode = pNode2;
            }
            if (NULL != pNode)
            {
                TiXmlElement* _temp = new TiXmlElement(strElementNodeName);
                if (strElementNodeValue != NULL)
                    _temp->LinkEndChild(new TiXmlText(strElementNodeValue));
                pNode->LinkEndChild(_temp);
                //mXmlDoc.LinkEndChild(_temp);
                return true;
            }
            pRootEle = pRootEle->NextSiblingElement();
        }
        return false;
    }
}

//插入属性
bool XmlOperate::InsterAttribut(char* strNodeName, char* strElementAttributName, char* strElementAttributValue)
{
    if (strNodeName == NULL && strElementAttributValue == NULL && strElementAttributName == NULL)
        return false;
    else
    {
        TiXmlElement* pRootEle = mXmlDoc.RootElement();
        while (pRootEle != NULL)
        {
            if (NULL == pRootEle)
                return false;
            TiXmlElement* pNode = NULL;
            GetNodePointerByName(pRootEle, strNodeName, pNode);
            TiXmlElement* pNode2 = pNode;
            while (pNode2 != NULL)
            {
                pNode2 = pNode2->NextSiblingElement();
                if (pNode2 != NULL)
                    pNode = pNode2;
            }
            if (NULL != pNode)
            {
                pNode->SetAttribute(strElementAttributName, strElementAttributValue);
                return true;
            }
            pRootEle = pRootEle->NextSiblingElement();
        }
        return false;
    }
}

bool XmlOperate::GetNodePointerByName(TiXmlElement* pRootEle, char* strNodeName, TiXmlElement*& Node)
{
    char* _temp = const_cast<char*>(pRootEle->Value());
    // 假如等于根节点名，就退出
    if (strcmp(strNodeName, _temp) == 0)
    {
        Node = pRootEle;
        return true;
    }
    TiXmlElement* pEle = pRootEle;
    for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
    {
        //递归处理子节点，获取节点指针
        if (GetNodePointerByName(pEle, strNodeName, Node))
            return true;
    }
    return false;
}

//VideoOpenCVOperate **************************************************************

VideoOpenCVOperate::VideoOpenCVOperate()
    : mCapture(NULL)
    , frame_count(0)
{
}

VideoOpenCVOperate::~VideoOpenCVOperate()
{
    ReleaseOperCV();
}

Face_Result_E VideoOpenCVOperate::initVideoOpenCVOperate()
{
    mVideoInfo.height = 0;
    mVideoInfo.width = 0;
    mVideoInfo.VIDuration = 0;
    frame_count = 0;
    if (mCapture)
        cvReleaseCapture(&mCapture);
    return RESULT_SUCCESS;
}

Face_Result_E VideoOpenCVOperate::openVideoOpenCVFile(char* paht)
{
    frame_count = 0;
    std::cout << "Paht =:" << paht << std::endl;
    mCapture = cvCreateFileCapture(paht);    //读取视频
    if (mCapture == NULL)
        return RESULT_VIDEO_OPENG_FALURE;
    double frames = cvGetCaptureProperty(mCapture, CV_CAP_PROP_FRAME_COUNT);    //读取视频中有多少帧
    IplImage* frame;
    frame = cvQueryFrame(mCapture);    //抓取帧
    FillVideoInfo(frames, frame->width, frame->height, frame->nChannels);
    return RESULT_SUCCESS;
}

void VideoOpenCVOperate::getVideoOpenCVFrame(int conut , Mat& f_out)
{
    cvSetCaptureProperty(mCapture, CV_CAP_PROP_POS_FRAMES, conut);    //设置视频帧的读取位置
    IplImage* frame1;
    frame1 = cvQueryFrame(mCapture);    //抓取帧
    //cvSaveImage("d:\\temp\\c\\a.jpg",frame1);
    return ;
}

IplImage* VideoOpenCVOperate::getVideoOpenCVFrame(int conut)
{
    cvSetCaptureProperty(mCapture, CV_CAP_PROP_POS_FRAMES, conut);    //设置视频帧的读取位置
    IplImage* frame1;
    frame1 = cvQueryFrame(mCapture);    //抓取帧
    //cvSaveImage("d:\\temp\\c\\a.jpg",frame1);
    if (frame1 == NULL)
        return NULL;
    else
        return frame1;
}

bool VideoOpenCVOperate::FillVideoInfo(double duration, int width, int height, int Channels)
{
    mVideoInfo.width = width;
    mVideoInfo.height = height;
    mVideoInfo.VIDuration = (int64_t) duration;
    mVideoInfo.Channels = Channels;
    double _fps = cvGetCaptureProperty(mCapture, CV_CAP_PROP_FPS);    //读取视频的帧率
    int _vfps = 1000 / _fps;                                        //计算每帧播放的时间
    mVideoInfo.vfps = _vfps;
    mVideoInfo.fps = _fps;
    return RESULT_SUCCESS;
}

void VideoOpenCVOperate::ReleaseOperCV()
{
    if (mCapture != NULL)
        cvReleaseCapture(&mCapture);
}

//PositionEngine *****************************************************************

PositionEngine::PositionEngine()
{
    mOpenCVDecode.reset(new VideoOpenCVOperate());
    mPosition.reset(new PositionFaceOperate());
    mPositionXmlOperate.reset(new XmlOperate());
    falg = 0;
}

PositionEngine::~PositionEngine()
{
    falg = 0;
}

Face_Result_E PositionEngine::initPositionEngine(Local_Parameter_t& ptr)
{
    falg++;
    mLocalParam = ptr;
    if (mOpenCVDecode->initVideoOpenCVOperate() != RESULT_SUCCESS)
        return RESULT_FAILURE;
    if (falg == 1)
        mPosition->loadModelDate(mLocalParam.mdItemInfo);
    if (mPosition->initPositionFaceOperate() != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

Face_Result_E PositionEngine::positionStart()
{
    int id = 0;
    char DecVideoPaht[512];
    for (int j = 0; j < mLocalParam.VideoChoose; j++)
    {
        id++;
        memset(DecVideoPaht, '\0', sizeof(DecVideoPaht));
        char* DecVideoPaht_1 = const_cast<char*>(mLocalParam.inputFilePath[j].c_str());
        if (strlen(DecVideoPaht_1) <= 0)
            return RESULT_FAILURE;
        memcpy(DecVideoPaht, DecVideoPaht_1, strlen(DecVideoPaht_1));
        if (mOpenCVDecode->openVideoOpenCVFile(DecVideoPaht) != RESULT_SUCCESS)
            return RESULT_FAILURE;
        VideoAndPictrueInfo_t videoInfo;
        videoInfo = mOpenCVDecode->getVideoInfo();
        mPosition->setDate(150, 150, videoInfo.width, videoInfo.height);
        int i = 0;
        int FrameTime = 0;
        while (true)
        {
            i++;
            Mat Face;
            FrameTime = i * videoInfo.vfps;
            if (mLocalParam.VideoReadMode == 2)
            {
                if ((i % mLocalParam.VideoReadSetp) == 0)
                {
                    IplImage* _image = mOpenCVDecode->getVideoOpenCVFrame(i);
                    if (_image == NULL || i >= videoInfo.VIDuration)
                        break;
                    mLocalParam.cb(videoInfo.VIDuration, i);
                    mPosition->getFace(_image, Face);
                    if (!Face.empty())
                    {
                        Mat PrepocessFaceS;
                        int lab = -1;
                        double xin = -1;
                        mPosition->facePretreatmen(Face, PrepocessFaceS);
                        if (!PrepocessFaceS.empty())
                        {
                            mPosition->predictFace(PrepocessFaceS, lab, xin);
                            setCalBack(lab, xin, _image, FrameTime, id);
                        }
                    }
                }
            }
            else
            {
                mLocalParam.cb(videoInfo.VIDuration, i);
                IplImage* _image = mOpenCVDecode->getVideoOpenCVFrame(i);
                if (_image == NULL || i >= videoInfo.VIDuration)
                    break;
                mPosition->getFace(_image, Face);
                if (!Face.empty())
                {
                    Mat PrepocessFaceS;
                    int lab = -1;
                    double xin = -1;
                    mPosition->facePretreatmen(Face, PrepocessFaceS);
                    if (!PrepocessFaceS.empty())
                    {
                        mPosition->predictFace(PrepocessFaceS, lab, xin);
                        setCalBack(lab, xin, _image, FrameTime, id);
                    }
                }
            }
        }
        mOpenCVDecode->ReleaseOperCV();
    }
    if (writePositionResultXml() != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

void PositionEngine::setCalBack(int a , double b , IplImage* src, int time_a, int id)
{
    FaceInfo_t _tempFaceInfo;
    std::vector<ModelXmlInfo_t>::iterator it = mLocalParam.mdItemInfo.begin();
    for (; it != mLocalParam.mdItemInfo.end(); it++)
    {
        ModelXmlInfo_t _tempXmlInfo = *it;
        std::vector<ModelItemInfo_t>::iterator it2 = _tempXmlInfo._modelItem.begin();
        for (; it2 != _tempXmlInfo._modelItem.end(); it2++)
        {
            ModelItemInfo_t _tempItemInfo = *it2;
            if (_tempItemInfo.label == a)
            {
                _tempFaceInfo.id = time_a;
                _tempFaceInfo.sec = (time_a % 1000);
                _tempFaceInfo.s = (time_a / 1000) % 60;
                _tempFaceInfo.m = ((time_a / 1000) / 60) % 60;
                _tempFaceInfo.h = ((time_a / 1000) / (60 * 60)) % 60;
                char t[256];
                sprintf(t, "%d_sub_%d_%d_%d_%d.jpg", id, _tempFaceInfo.h, _tempFaceInfo.m, _tempFaceInfo.s, _tempFaceInfo.sec);
                _tempFaceInfo.framPicName = t;
                char t3[512];
                memset(t3, '\0', sizeof(t3));
                sprintf(t3, "%s%s", mLocalParam.outPicFile, const_cast<char*>(_tempFaceInfo.framPicName.c_str()));
                //sprintf(const_cast<char*>(_tempFaceInfo.XmlPicPaht.c_str()),"%s%s",mLocalParam.outPicFile,const_cast<char*>(_tempFaceInfo.framPicName.c_str()));
                _tempFaceInfo.XmlPicPaht = t3;
                char t1[256];
                sprintf(t1, "%d:%d:%d.%d", _tempFaceInfo.h, _tempFaceInfo.m, _tempFaceInfo.s, _tempFaceInfo.sec);
                _tempFaceInfo.frameTime = t1;
                _tempFaceInfo.name = _tempItemInfo.name;
                _tempFaceInfo.label = a;
                _tempFaceInfo.explain = _tempItemInfo.remark;
                _tempFaceInfo.modelPaht = _tempXmlInfo.modelPath;
                _tempFaceInfo.reliability = b;
                char _tempName[100] = {'\0'};
                sprintf(_tempName, "%s/%s", mLocalParam.outFilePicPaht, const_cast<char*>(_tempFaceInfo.framPicName.c_str()));
                cvSaveImage(_tempName, src);
                _tempFaceInfo.picFullPaht = _tempName;
                mFaceInfo.push_back(_tempFaceInfo);
                _tempItemInfo.remark.append(";");
                _tempItemInfo.remark.append(_tempFaceInfo.frameTime);
                char* _a = const_cast<char*>(_tempItemInfo.name.c_str());
                char* _b = const_cast<char*>(_tempItemInfo.remark.c_str());
                char* _c = const_cast<char*>(_tempXmlInfo.modelPath.c_str());
                mLocalParam.cb1(_a, _b, _c, a, b);
            }
        }
    }
}

Face_Result_E PositionEngine::writePositionResultXml()
{
    Face_Result_E _Result = RESULT_UNKNOWN;
    char temp_char[50];
    _Result = mPositionXmlOperate->createXmlDocHead("1.0", "GB2312", "yes");
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    _Result = mPositionXmlOperate->addNode(NULL, "Media", NULL);
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    for (int i = 0; i < mFaceInfo.size(); i++)
    {
        _Result = mPositionXmlOperate->addNode("Media", "Segment", NULL);
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _Result = mPositionXmlOperate->addNodeAttribut("Segment", "Text", const_cast<char*>(mFaceInfo[i].name.c_str()));
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _Result = mPositionXmlOperate->addNodeAttribut("Segment", "picfile", const_cast<char*>(mFaceInfo[i].XmlPicPaht.c_str()));
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _itoa(mFaceInfo[i].id, temp_char, 10);
        _Result = mPositionXmlOperate->addNodeAttribut("Segment", "StartTime", temp_char);
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _itoa(mFaceInfo[i].reliability, temp_char, 10);
        _Result = mPositionXmlOperate->addNodeAttribut("Segment", "Reliability", temp_char);
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _itoa(mFaceInfo[i].label, temp_char, 10);
        _Result = mPositionXmlOperate->addNodeAttribut("Segment", "Lable", temp_char);
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
    }
    _Result = mPositionXmlOperate->addNode(NULL, "Result", NULL);
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    _Result = mPositionXmlOperate->addNode("Result", "Info", NULL);
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    for (int i = 0; i < mLocalParam.inputFilePath.size(); i++)
    {
        _Result = mPositionXmlOperate->addNode("Info", "VideoFilePath", const_cast<char*>(mLocalParam.inputFilePath[i].c_str()));
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
    }
    _Result = mPositionXmlOperate->addNode("Info", "XmlPath", mLocalParam.outFileXmlPaht);
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    _Result = mPositionXmlOperate->addNode("Info", "PicPath", mLocalParam.outFilePicPaht);
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    _Result = mPositionXmlOperate->addNode("Info", "VideoName", mLocalParam.videoFileName);
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    _Result = mPositionXmlOperate->saveXmlDoc(const_cast<char*>(mLocalParam.outFileXmlPaht));
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

//TrainEngine ********************************************************************

TrainEngine::TrainEngine()
{
    mTrainFaceOperate.reset(new TrainFaceOpertae());
    mXmlOperate.reset(new XmlOperate());
}

TrainEngine::~TrainEngine()
{
}

Face_Result_E TrainEngine::initTrainEngine(Train_Parameter_t& par)
{
    mTrainParameter = par;
    if (mTrainFaceOperate->initTrainFaceOpertae() != RESULT_SUCCESS)
        return RESULT_FAILURE;
    mTrainFaceOperate->setData(150, 150);
    return RESULT_SUCCESS;
}

Face_Result_E TrainEngine::trainStart()
{
    if (!TrainFace())
        return RESULT_FAILURE;
    if (!saveSequenceTrainXml())
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

bool TrainEngine::TrainFace()
{
    Ptr<FaceRecognizer> model_1;
    std::vector<Mat> images;
    std::vector<int> labels;
    std::vector<TrainFaceInfo_t>::iterator it = mTrainParameter.FaceTrainInfo.begin();
    for (; it != mTrainParameter.FaceTrainInfo.end(); it++)
    {
        TrainFaceInfo_t _tempInfo = *it;
        string _path = _tempInfo.Samplepaht + "\\" + _tempInfo.samplename + "\\";
        std::vector<string>::iterator it2 = _tempInfo.facepaht.begin();
        for (; it2 != _tempInfo.facepaht.end(); it2++)
        {
            Mat dst;
            string _pathName = _path;
            _pathName.append(*it2);
            if (mTrainFaceOperate->facePretreatment(const_cast<char*>(_pathName.c_str()), dst) == RESULT_SUCCESS)
            {
                if (!dst.empty())
                {
                    images.push_back(dst);
                    labels.push_back(_tempInfo.label);
                }
            }
            else
                return false;
        }
    }
    model_1 = mTrainFaceOperate->faceTrainlearnCollected(images, labels, "FaceRecognizer.Eigenfaces");
    model_1->save(mTrainParameter.saveModelPaht);
    model_1.release();
    return true;
}

bool TrainEngine::saveSequenceTrainXml()
{
    string moldePaht = mTrainParameter.saveModelPaht;
    int pos = moldePaht.find(".xml", 0);
    string _moldeSeqPaht = moldePaht.substr(0, pos);
    _moldeSeqPaht.append("_seq");
    _moldeSeqPaht.append(".xml");
    char _tempNumber[50] = {'\0'};
    Face_Result_E _Result = RESULT_UNKNOWN;
    _Result = mXmlOperate->createXmlDocHead("1.0", "GB2312", "yes");
    if (_Result != RESULT_SUCCESS)
        return false;
    _Result = mXmlOperate->addNode(NULL, "ModelResult", NULL);
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    std::vector<TrainFaceInfo_t>::iterator it = mTrainParameter.FaceTrainInfo.begin();
    for (; it != mTrainParameter.FaceTrainInfo.end(); it++)
    {
        char _tempName[20] = {'\0'};
        TrainFaceInfo_t _tempInfo = *it;
        _Result = mXmlOperate->addNode("ModelResult", "Model", NULL);
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _Result = mXmlOperate->addNode("Model", "Name", const_cast<char*>(_tempInfo.name.c_str()));
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _itoa(_tempInfo.label, _tempName, 10);
        _Result = mXmlOperate->addNode("Model", "Label", _tempName);
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _Result = mXmlOperate->addNode("Model", "Remar", const_cast<char*>(_tempInfo.remark.c_str()));
        if (_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
    }
    _Result = mXmlOperate->addNode("ModelResult", "ModelPaht", mTrainParameter.saveModelPaht);
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    _Result = mXmlOperate->saveXmlDoc(const_cast<char*>(_moldeSeqPaht.c_str()));
    if (_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return true;
}

//CheckEngine *******************************************************************

CheckEngine::CheckEngine()
{
    mCheckFace.reset(new CheckFaceOperatec());
    /*mDecipherFFmpeg.reset(new VideFFmpegOperate());*/
    mDecipherOpenCV.reset(new VideoOpenCVOperate());
    mXmlOperate.reset((new XmlOperate()));
    Continuous = true;
}

CheckEngine::~CheckEngine()
{
}

Face_Result_E CheckEngine::initCheckEngine(Check_Parameter_t& ptr)
{
    mCheckParam = ptr;
    Continuous = true;
    //  if(mCheckParam.SequenceInfo.Importmodel == IMPORT_VIDEO || mCheckParam.SequenceInfo.Importmodel == IMPORT_FFMPEG_VIDEO)
    //  {
    // //       if(mDecipherFFmpeg->initVideoOperate() != RESULT_SUCCESS)
    // //       {
    // //           return RESULT_FAILURE;
    // //       }
    //  }else
    if (mCheckParam.SequenceInfo.Importmodel == IMPORT_VIDEO || mCheckParam.SequenceInfo.Importmodel == IMPORT_OPENCV_VIDEO)
    {
        if (mDecipherOpenCV->initVideoOpenCVOperate() != RESULT_SUCCESS)
            return RESULT_FAILURE;
    }
    if (mCheckFace->initCheckface() != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

Face_Result_E CheckEngine::checkStart()
{
    switch (mCheckParam.SequenceInfo.Importmodel)
    {
        case IMPORT_OPENCV_VIDEO:
        case IMPORT_VIDEO:
            if (checkVideo() != RESULT_SUCCESS)
                return RESULT_FAILURE;
            break;
        case IMPORT_PICTURE:
            if (checkPicture() != RESULT_SUCCESS)
                return RESULT_FAILURE;
            break;
    }
    return RESULT_SUCCESS;
}

Face_Result_E CheckEngine::checkVideo()
{
    switch (mCheckParam.SequenceInfo.Importmodel)
    {
        case IMPORT_VIDEO:
        //  case IMPORT_FFMPEG_VIDEO:
        //      if(videoFFmpegDecipher() != RESULT_SUCCESS)
        //      {
        //          return RESULT_FAILURE;
        //      }
        //      break;
        case IMPORT_OPENCV_VIDEO:
            if (videoOpencvDecipher() != RESULT_SUCCESS)
                return RESULT_FAILURE;
            break;
    }
    return RESULT_SUCCESS;
}

Face_Result_E CheckEngine::checkPicture()
{
    int j = 0;
    mCheckParam.OriginalSourceInfo.VIDuration = mCheckParam.inputFilePaht.size();
    for (int i = 0; i < mCheckParam.inputFilePaht.size(); i++)
    {
        j++;
        char* _tempPicturePaht = mCheckParam.inputFilePaht[i];
        Mat Face;
        Mat cameraFrame;
        cameraFrame = cvLoadImage(_tempPicturePaht, CV_LOAD_IMAGE_UNCHANGED);
        mCheckFace->checkFace(_tempPicturePaht, Face);
        if (!Face.empty())
            checkAddFaceItem(Face, j, cameraFrame);
    }
    saveCheckResult();
    return RESULT_SUCCESS;
}

Face_Result_E CheckEngine::videoFFmpegDecipher()
{
    //  char* DecVideoPaht = mCheckParam.inputFilePaht[0];
    // //   if(mDecipherFFmpeg->openVideoFile(DecVideoPaht) != RESULT_SUCCESS)
    // //   {
    // //       return RESULT_FAILURE;
    // //   }
    //  VideoAndPictrueInfo_t videoInfo;
    //  //videoInfo = mDecipherFFmpeg->getVideoInfo();
    //  mCheckParam.OriginalSourceInfo.width = videoInfo.width;
    //  mCheckParam.OriginalSourceInfo.height = videoInfo.height;
    //  mCheckParam.OriginalSourceInfo.VIDuration = videoInfo.VIDuration;
    //  if(FaceDetection(true) != RESULT_SUCCESS)
    //  {
    //      return RESULT_FAILURE;
    //  }
    return RESULT_SUCCESS;
}

Face_Result_E CheckEngine::videoOpencvDecipher()
{
    char* DecVideoPaht = mCheckParam.inputFilePaht[0];
    if (mDecipherOpenCV->openVideoOpenCVFile(DecVideoPaht) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    VideoAndPictrueInfo_t videoInfo;
    videoInfo = mDecipherOpenCV->getVideoInfo();
    mCheckParam.OriginalSourceInfo.width = videoInfo.width;
    mCheckParam.OriginalSourceInfo.height = videoInfo.height;
    mCheckParam.OriginalSourceInfo.VIDuration = videoInfo.VIDuration;
    mCheckParam.OriginalSourceInfo.Channels = videoInfo.Channels;
    if (FaceDetection(false) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

Face_Result_E CheckEngine::FaceDetection(bool falgs)
{
    int _FrameChannels = mCheckParam.OriginalSourceInfo.Channels != -1 ? mCheckParam.OriginalSourceInfo.Channels : 3;
    int _buffSize = mCheckParam.OriginalSourceInfo.width * mCheckParam.OriginalSourceInfo.height * _FrameChannels;
    int64_t i = 0;
    uint8_t* buf = (uint8_t*) malloc(_buffSize);
    //mCheckFace->setDate(mCheckParam.OriginalSourceInfo.width,mCheckParam.OriginalSourceInfo.height,mCheckParam.checkParameterInfo.faceWidht,mCheckParam.checkParameterInfo.faceHeight);
    while (true)
    {
        i++;
        Mat Face;
        if (mCheckParam.checkParameterInfo.VideoReadMode == 1)
        {
            if (falgs)
            {
                //              if(mDecipherFFmpeg->getFrame(buf,_buffSize) != RESULT_SUCCESS)
                //              {
                //                  break;
                //              }
                //              mCheckFace->checkFace(buf,_buffSize,Face,mCheckParam.SequenceInfo.CheckModel);
                //              Mat frame;
                //              mCheckFace->CharBufToCVMat(frame,buf,_buffSize);
                //              checkAddFaceItem(Face,i,frame);
            }
            else
            {
                IplImage* _image = mDecipherOpenCV->getVideoOpenCVFrame(i);
                if (_image == NULL || mCheckParam.OriginalSourceInfo.VIDuration <= i)
                    break;
                mCheckFace->checkFace(_image, Face, mCheckParam.SequenceInfo.CheckModel);
                Mat frame(_image);
                checkAddFaceItem(Face, i, frame);
            }
        }
        else if (mCheckParam.checkParameterInfo.VideoReadMode == 2)
        {
            if ((i % mCheckParam.checkParameterInfo.VideoReadSetp) == 0)
            {
                if (falgs)
                {
                    //                  if(mDecipherFFmpeg->getFrameSpeed(buf,_buffSize,mCheckParam.checkParameterInfo.VideoReadSetp) != RESULT_SUCCESS)
                    //                  {
                    //                      return RESULT_FAILURE;
                    //                  }
                    //                  mCheckFace->checkFace(buf,_buffSize,Face,mCheckParam.SequenceInfo.CheckModel);
                    //                  Mat frame;
                    //                  mCheckFace->CharBufToCVMat(frame,buf,_buffSize);
                    //                  checkAddFaceItem(Face,i,frame);
                }
                else
                {
                    IplImage* _image = mDecipherOpenCV->getVideoOpenCVFrame(i);
                    if (_image == NULL || mCheckParam.OriginalSourceInfo.VIDuration <= i)
                        break;
                    mCheckFace->checkFace(_image, Face, mCheckParam.SequenceInfo.CheckModel);
                    Mat frame(_image);
                    checkAddFaceItem(Face, i, frame);
                }
            }
        }
    }
    free(buf);
    saveCheckResult();
    return RESULT_SUCCESS;
}

void CheckEngine::checkAddFaceItem(Mat& Face , int64_t i, Mat& frame)
{
    mCheckParam.cb(mCheckParam.OriginalSourceInfo.VIDuration, i);
    char _tempName[20] = {'\0'};
    sprintf(_tempName, "_%d.jpg", i);
    char _tempName1[20] = {'\0'};
    sprintf(_tempName1, "_%d_frame.jpg", i);
    FaceItemInfo _Item;
    if (!Face.empty())
    {
        if (Continuous)
        {
            _Item.Frames = i;
            _Item.Facename = _tempName;
            _Item.FrameName = _tempName1;
            string _temp_paht = mCheckParam.SequenceInfo.faceSavePath + _Item.Facename;
            imwrite(_temp_paht.c_str(), Face);
            string _temp_paht1 =  mCheckParam.SequenceInfo.faceSavePath + _tempName1;
            imwrite(_temp_paht1.c_str(), frame);
            mCheckParam.SequenceInfo.FaceIetmSeque.push_back(_Item);
            Continuous = false;
        }
        else
            Continuous = true;
    }
}

Face_Result_E CheckEngine::saveCheckResult()
{
    Face_Result_E t_Result = RESULT_UNKNOWN;
    char temp_char[50];
    t_Result = mXmlOperate->createXmlDocHead("1.0", "GB2312", "yes");
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    t_Result = mXmlOperate->addNode(NULL, "RecoginseResult", NULL);
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    t_Result = mXmlOperate->addNode("RecoginseResult", "SystemInfo", NULL);
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    t_Result = mXmlOperate->addNode("SystemInfo", "FileRead", "false");
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    if (mCheckParam.SequenceInfo.Importmodel != IMPORT_PICTURE)
    {
        t_Result = mXmlOperate->addNode("RecoginseResult", "VideoInfo", NULL);
        if (t_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        t_Result = mXmlOperate->addNode("VideoInfo", "VideoPaht", mCheckParam.inputFilePaht[0]);
        if (t_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _itoa(mCheckParam.OriginalSourceInfo.width, temp_char, 10);
        t_Result = mXmlOperate->addNode("VideoInfo", "VideoWidth", temp_char);
        if (t_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _itoa(mCheckParam.OriginalSourceInfo.height, temp_char, 10);
        t_Result = mXmlOperate->addNode("VideoInfo", "VideoHeight", temp_char);
        if (t_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _itoa(mCheckParam.OriginalSourceInfo.VIDuration, temp_char, 10);
        t_Result = mXmlOperate->addNode("VideoInfo", "VideoDuration", temp_char);
        if (t_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _itoa(mCheckParam.checkParameterInfo.VideoReadMode, temp_char, 10);
        t_Result = mXmlOperate->addNode("VideoInfo", "VideoReadModl", temp_char);
        if (t_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        _itoa(mCheckParam.checkParameterInfo.VideoReadSetp, temp_char, 10);
        t_Result = mXmlOperate->addNode("VideoInfo", "VideoReadSetp", temp_char);
        if (t_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
    }
    else
    {
        t_Result = mXmlOperate->addNode("RecoginseResult", "PictureInfo", NULL);
        if (t_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        for (int i = 0; i < mCheckParam.inputFilePaht.size(); i++)
        {
            t_Result = mXmlOperate->addNode("PictureInfo", "PicturePath", mCheckParam.inputFilePaht[i]);
            if (t_Result != RESULT_SUCCESS)
                return RESULT_FAILURE;
        }
    }
    t_Result = mXmlOperate->addNode("RecoginseResult", "FaceInfo", NULL);
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    t_Result = mXmlOperate->addNode("FaceInfo", "FaceSavePaht", const_cast<char*>(mCheckParam.SequenceInfo.faceSavePath.c_str()));
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    t_Result = mXmlOperate->addNode("FaceInfo", "FaceXml", const_cast<char*>(mCheckParam.SequenceInfo.SequenSaveXmlPaht.c_str()));
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    _itoa(mCheckParam.checkParameterInfo.faceWidht, temp_char, 10);
    t_Result = mXmlOperate->addNode("FaceInfo", "FaceWidth", temp_char);
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    _itoa(mCheckParam.checkParameterInfo.faceHeight, temp_char, 10);
    t_Result = mXmlOperate->addNode("FaceInfo", "FaceHeight", temp_char);
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    t_Result = mXmlOperate->addNode("FaceInfo", "Faces", NULL);
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    _itoa((int) mCheckParam.SequenceInfo.FaceIetmSeque.size(), temp_char, 10);
    t_Result = mXmlOperate->addNode("Faces", "FaceCout", temp_char);
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    for (int i = 0; i < mCheckParam.SequenceInfo.FaceIetmSeque.size(); i++)
    {
        FaceItemInfo item = mCheckParam.SequenceInfo.FaceIetmSeque[i];
        t_Result = mXmlOperate->addNode("Faces", "Face", const_cast<char*>(item.Facename.c_str()));
        if (t_Result != RESULT_SUCCESS)
            return RESULT_FAILURE;
        if (mCheckParam.SequenceInfo.Importmodel != IMPORT_PICTURE)
        {
            _itoa(item.Frames, temp_char, 10);
            t_Result = mXmlOperate->addNodeAttribut("Face", "frame", temp_char);
            if (t_Result != RESULT_SUCCESS)
                return RESULT_FAILURE;
            t_Result = mXmlOperate->addNodeAttribut("Face", "frame_pic", const_cast<char*>(item.FrameName.c_str()));
            if (t_Result != RESULT_SUCCESS)
                return RESULT_FAILURE;
        }
    }
    t_Result = mXmlOperate->saveXmlDoc(const_cast<char*>(mCheckParam.SequenceInfo.SequenSaveXmlPaht.c_str()));
    if (t_Result != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}



