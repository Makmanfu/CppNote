
#include "stdafx.h"
#include "FaceOperate.h"

//imple类的定义
class OperateEngine::imple
{
public:
    imple();
    ~imple();
public:
    Face_Result_E ImpleInitDate();
    Face_Result_E ImpleCheckFace(Check_Parameter_t& Par);
    Face_Result_E ImpleTrainFace(Train_Parameter_t& Par);
    Face_Result_E ImpleLocateFae(Local_Parameter_t& Par);
private:
    boost::shared_ptr<CheckEngine> mCheckEngine;
    boost::shared_ptr<TrainEngine> mTrainEngine;
    boost::shared_ptr<PositionEngine> mPostionEngine;
};

OperateEngine::imple::imple()
{
    mCheckEngine.reset(new CheckEngine());
    mTrainEngine.reset(new TrainEngine());
    mPostionEngine.reset(new PositionEngine());
}

OperateEngine::imple::~imple()
{
}

Face_Result_E OperateEngine::imple::ImpleInitDate()
{
    Face_Result_E _result = RESULT_UNKNOWN;
    return _result;
}

Face_Result_E OperateEngine::imple::ImpleCheckFace(Check_Parameter_t& Par)
{
    if (mCheckEngine->initCheckEngine(Par) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    if (mCheckEngine->checkStart() != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

Face_Result_E OperateEngine::imple::ImpleTrainFace(Train_Parameter_t& Par)
{
    if (mTrainEngine->initTrainEngine(Par) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    if (mTrainEngine->trainStart() != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

Face_Result_E OperateEngine::imple::ImpleLocateFae(Local_Parameter_t& Par)
{
    if (mPostionEngine->initPositionEngine(Par) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    if (mPostionEngine->positionStart() != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

OperateEngine::OperateEngine(void)
    : _this(new imple())
{
}

OperateEngine::~OperateEngine(void)
{
}

Face_Result_E OperateEngine::initEngine()
{
    return _this->ImpleInitDate();
}

Face_Result_E OperateEngine::checkFace(Check_Parameter_t& Par)
{
    return _this->ImpleCheckFace(Par);
}

Face_Result_E OperateEngine::TrainFace(Train_Parameter_t& Par)
{
    return _this->ImpleTrainFace(Par);
}

Face_Result_E OperateEngine::LocateFae(Local_Parameter_t& Par)
{
    return _this->ImpleLocateFae(Par);
}

//FaceOperateClass *********************************************************************

FaceOperateClass::FaceOperateClass()
    : mCheckParameter()
    , mTrainParameter()
    , mLocalParameter()
{
    mEngine = new OperateEngine();
    //mLogger = new Logger();
    //mLogger->SetLogLevel(LogLevelAll);
}

FaceOperateClass::~FaceOperateClass()
{
}

Face_Result_E FaceOperateClass::initFaceOperate()
{
    return mEngine->initEngine();
}

Face_Result_E FaceOperateClass::FaceCheckOpertae(Import_Type_e _type, char* CheckVideoFilePath, char* ParamXmlBuf, int ParamBufLean, char* PicturePath, char* OutPicturePath, char* OutXmlPath, CallBackSchedule cb)
{
    mCheckSchedule = cb;
    //mLogger->TraceInfo(ParamXmlBuf);
    if (checkFullParameter(_type, CheckVideoFilePath, ParamXmlBuf, ParamBufLean, PicturePath, OutPicturePath, OutXmlPath) == RESULT_SUCCESS)
    {
        //mLogger->TraceInfo("CheckFullParameter Success");
        if (mEngine->checkFace(mCheckParameter) == RESULT_SUCCESS)
        {
            //mLogger->TraceInfo("checkFace Success");
            return RESULT_SUCCESS;
        }
        else
        {
            //mLogger->TraceError("checkFace Failure");
            return RESULT_FAILURE;
        }
    }
    else
    {
        //mLogger->TraceError("ChefFullParameter Failure");
        return RESULT_PARAMET_FULL_FAILURE;
    }
}

Face_Result_E FaceOperateClass::TrainFaceOpertae(char* xmlPath, char* outModelNamePaht)
{
    //mLogger->TraceInfo("Train xmlPaht");
    //mLogger->TraceInfo(xmlPath);
    //mLogger->TraceInfo("Train outModelNamepaht");
    //mLogger->TraceInfo(outModelNamePaht);
    if (trainFullParameter(xmlPath, outModelNamePaht) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    if (mEngine->TrainFace(mTrainParameter) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

Face_Result_E FaceOperateClass::LocateFaceOpertae(Import_Type_e _type, char* modlePath, char* videoPath, char* picturePaht, char* ParamXmlBuf, int ParamBufLean, char* outFilePath, CallBackLocate cb, CallBackSchedule cb1)
{
    mLocateSchedule = cb1;
    mLocate = cb;
    if (localFullParameter(_type, modlePath, videoPath, picturePaht, ParamXmlBuf, ParamBufLean, outFilePath) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    if (mEngine->LocateFae(mLocalParameter) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

void FaceOperateClass::checkSchedule(int64_t a, int64_t b)
{
    if (mCheckSchedule != NULL)
        mCheckSchedule(a, b);
}

void FaceOperateClass::localSchedule(int64_t a, int64_t b)
{
    if (mLocateSchedule != NULL)
        mLocateSchedule(a, b);
}

void FaceOperateClass::local(char* name, char* remark, char* modelPath, int label, double confidence)
{
    if (mLocate != NULL)
        mLocate(name, remark, modelPath, label, confidence);
}

Face_Result_E FaceOperateClass::checkFullParameter(Import_Type_e _type, char* CheckVideoFilePath, char* ParamXmlBuf, int ParamBufLean, char* PicturePath, char* OutPicturePath, char* OutXmlPath)
{
    //mLogger->TraceInfo(PicturePath);
    mCheckParameter.cleanData();
    mCheckParameter.cb = boost::bind(&FaceOperateClass::checkSchedule, this, _1, _2);
    mCheckParameter.SequenceInfo.Importmodel = _type;
    if (_type != IMPORT_PICTURE)
    {
        if (strlen(CheckVideoFilePath) == 0)
            return RESULT_PARAMET_FULL_FAILURE;
        mCheckParameter.SequenceInfo.VideoPaht = CheckVideoFilePath;
        mCheckParameter.inputFilePaht.push_back(CheckVideoFilePath);
    }
    else
    {
        if (strlen(PicturePath) == 0)
            return RESULT_PARAMET_FULL_FAILURE;
        vec.clear();
        string pics = PicturePath;
        vec = split(pics, ";");
        for (int i = 0; i < (int) vec.size(); i ++)
        {
            if (!vec[i].empty())
                mCheckParameter.inputFilePaht.push_back(const_cast<char*>(vec[i].c_str()));
        }
    }
    mCheckParameter.SequenceInfo.faceSavePath = OutPicturePath;
    mCheckParameter.SequenceInfo.SequenSaveXmlPaht = OutXmlPath;
    if (!readXmlParameter(ParamXmlBuf, ParamBufLean))
        return RESULT_PARAMET_FULL_FAILURE;
    return RESULT_SUCCESS;
}

bool FaceOperateClass::readXmlParameter(char* xmlBuf, int lean)
{
    mCheckXmlOperate = new XmlOperate();
    if (mCheckXmlOperate->initXmlOperate() != RESULT_SUCCESS)
        return false;
    mCheckXmlOperate->setBindXmlReadElement(boost::bind(&FaceOperateClass::checkXmlText, this, _1, _2));
    if (mCheckXmlOperate->loadXmlFile(xmlBuf, lean) != RESULT_SUCCESS)
        return false;
    return true;
}

std::vector<std::string> FaceOperateClass::split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < (std::string::size_type) size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

void FaceOperateClass::checkXmlText(const char* text, Xml_Element_Type_e _type)
{
    const char* element;
    switch (_type)
    {
        case XML_ELEMENT:
            if (strcmp(text, "Project") == 0)
            {
                element = mCheckXmlOperate->getXMLElementText();
                mCheckParameter.SequenceInfo.projectName = element;
            }
            if (strcmp(text, "CheckMode") == 0)
            {
                element = mCheckXmlOperate->getXMLElementText();
                mCheckParameter.checkParameterInfo.VideoReadMode = std::atoi(element);
            }
            if (strcmp(text, "Width") == 0)
            {
                element = mCheckXmlOperate->getXMLElementText();
                mCheckParameter.checkParameterInfo.faceWidht = std::atoi(element);
            }
            if (strcmp(text, "Height") == 0)
            {
                element = mCheckXmlOperate->getXMLElementText();
                mCheckParameter.checkParameterInfo.faceHeight = std::atoi(element);
            }
            break;
        case XML_ATTRIBUT:
            if (strcmp(text, "Step") == 0)
            {
                element = mCheckXmlOperate->getXMLAttributeText();
                mCheckParameter.checkParameterInfo.VideoReadSetp = std::atoi(element);
            }
            if (strcmp(text, "FaceAndEyes") == 0)
                mCheckParameter.SequenceInfo.CheckModel = MODEL_FACEANDEYES;
            if (strcmp(text, "Face") == 0)
                mCheckParameter.SequenceInfo.CheckModel = MODEL_FACE;
            if (strcmp(text, "FaceAndLeftEye") == 0)
                mCheckParameter.SequenceInfo.CheckModel = MODEL_FACEANDLEFTEYE;
            if (strcmp(text, "FaceAndRightEye") == 0)
                mCheckParameter.SequenceInfo.CheckModel = MODEL_FACEANDRIGHTEYE;
            break;
    }
}

Face_Result_E FaceOperateClass::trainFullParameter(char* xmlPath, char* outModelNamePath)
{
    mTrainXmlOperate = new XmlOperate();
    mTrainParameter.cleanData();
    if (mTrainXmlOperate->initXmlOperate() != RESULT_SUCCESS)
        return RESULT_FAILURE;
    mTrainParameter.saveModelPaht = outModelNamePath;
    //std::vector<std::string> vec;
    vec.clear();
    string pics = xmlPath;
    vec = split(pics, ";");
    for (int i = 0; i < (int) vec.size(); i ++)
    {
        TrainFaceInfo_t _tempModel;
        _tempModel.modelName = vec[i];
        if (!vec[i].empty())
        {
            mTrainParameter.FaceTrainInfo.push_back(_tempModel);
            if (!trainReadXmlParameter(const_cast<char*>(_tempModel.modelName.c_str()), 0))
                return RESULT_FAILURE;
        }
    }
    return RESULT_SUCCESS;
}

bool FaceOperateClass::trainReadXmlParameter(char* xmlPath, int lange)
{
    mTrainXmlOperate->setBindXmlReadElement(boost::bind(&FaceOperateClass::trainXmlText, this, _1, _2));
    if (mTrainXmlOperate->loadXmlFile(xmlPath, lange) != RESULT_SUCCESS)
        return false;
    return true;
}

void FaceOperateClass::trainXmlText(const char* text, Xml_Element_Type_e _type)
{
    std::vector<TrainFaceInfo_t>::reverse_iterator it = mTrainParameter.FaceTrainInfo.rbegin();
    const char* element;
    switch (_type)
    {
        case XML_ELEMENT:
            if (strcmp(text, "FileRead") == 0)
            {
                if (strcmp(mTrainXmlOperate->getXMLElementText(), "false") == 0)
                    (*it).FileRead = false;
                else
                    (*it).FileRead = true;
            }
            if (strcmp(text, "Samplepaht") == 0)
                (*it).Samplepaht = mTrainXmlOperate->getXMLElementText();
            if (strcmp(text, "samplename") == 0)
                (*it).samplename = mTrainXmlOperate->getXMLElementText();
            if (strcmp(text, "name") == 0)
                (*it).name = mTrainXmlOperate->getXMLElementText();
            if (strcmp(text, "label") == 0)
            {
                element = mTrainXmlOperate->getXMLElementText();
                (*it).label = std::atoi(element);
            }
            if (strcmp(text, "remark") == 0)
                (*it).remark = mTrainXmlOperate->getXMLElementText();
            if (strcmp(text, "face") == 0)
                (*it).facepaht.push_back(mTrainXmlOperate->getXMLElementText());
            break;
        case XML_ATTRIBUT:
            break;
    }
}

Face_Result_E FaceOperateClass::localFullParameter(Import_Type_e _type, char* modlePath, char* videoPath, char* picturePaht, char* ParamXmlBuf, int ParamBufLean , char* outFilePath)
{
    //mLogger->TraceInfo("localFullParameter");
    //mLogger->TraceInfo(videoPath);
    mLocalParameter.cleanData();
    mLocalParameter.cb = boost::bind(&FaceOperateClass::localSchedule, this, _1, _2);
    mLocalParameter.cb1 = boost::bind(&FaceOperateClass::local, this, _1, _2, _3, _4, _5);
    mLocalParameter._type = _type;
    std::vector<std::string> vec1;
    vec1.clear();
    string pics1 = videoPath;
    vec1 = split(pics1, ";");
    if (_type == IMPORT_VIDEO)
    {
        std::vector<std::string>::iterator it = vec1.begin();
        for (; it != vec1.end(); it++)
            mLocalParameter.inputFilePath.push_back(*it);
    }
    mLocalParameter.VideoChoose = mLocalParameter.inputFilePath.size();
    string flag("/");
    string strSrc = outFilePath;
    size_t sub_pos1 = 0;
    size_t sub_pos2 = strSrc.size();
    string subStr = "";
    string substr1 = "";
    while ((sub_pos2 = strSrc.find_first_of(flag, sub_pos1)) != string::npos)
    {
        if (!subStr.empty())
        {
            substr1 = substr1.append(subStr);
            substr1 = substr1.append("/");
        }
        subStr =  strSrc.substr(sub_pos1, sub_pos2 - sub_pos1);
        sub_pos1 = sub_pos2 + 1;
    }
    if (subStr.empty())
    {
        string flag("\\");
        while ((sub_pos2 = strSrc.find_first_of(flag, sub_pos1)) != string::npos)
        {
            if (!subStr.empty())
            {
                substr1 = substr1.append(subStr);
                substr1 = substr1.append("/");
            }
            subStr =  strSrc.substr(sub_pos1, sub_pos2 - sub_pos1);
            sub_pos1 = sub_pos2 + 1;
        }
    }
    //mLogger->TraceInfo(substr1.c_str());
    memcpy(mLocalParameter.videoFileName, const_cast<char*>(subStr.c_str()), subStr.size());
    sprintf(mLocalParameter.outFileXmlPaht, "%s/%s.xml", const_cast<char*>(substr1.c_str()), mLocalParameter.videoFileName);
    sprintf(mLocalParameter.outFilePicPaht, "%s", outFilePath);
    sprintf(mLocalParameter.outPicFile, "%s\\", mLocalParameter.videoFileName);
    //mLogger->TraceInfo(mLocalParameter.outFilePicPaht);
    //mLogger->TraceInfo(mLocalParameter.outFileXmlPaht);
    mLocateXmlOperate = new XmlOperate();
    mLocateXmlOperate->initXmlOperate();
    mLocateXmlOperate->setBindXmlReadElement(boost::bind(&FaceOperateClass::localXmlText, this, _1, _2));
    if (!localReadXmlParameter(ParamXmlBuf, ParamBufLean))
        return RESULT_FAILURE;
    //std::vector<std::string> vec;
    vec.clear();
    string pics = modlePath;
    vec = split(pics, ";");
    for (int i = 0; i < (int) vec.size(); i ++)
    {
        if (!vec[i].empty())
        {
            ModelXmlInfo_t _tempXmlInfo;
            mLocalParameter.mdItemInfo.push_back(_tempXmlInfo);
            if (!localReadXmlParameter(const_cast<char*>(vec[i].c_str()), 0))
                return RESULT_FAILURE;
        }
    }
    //  for(int j = 0; j < mLocalParameter.inputFilePath.size(); j++)
    //  {
    //      mLogger->TraceInfo(mLocalParameter.inputFilePath[j].c_str());
    //  }
    return RESULT_SUCCESS;
}

bool FaceOperateClass::localReadXmlParameter(char* xmlPaht, int lange)
{
    if (mLocateXmlOperate->loadXmlFile(xmlPaht, lange) != RESULT_SUCCESS)
        return false;
    return true;
}

void FaceOperateClass::localXmlText(const char* text, Xml_Element_Type_e _type)
{
    std::vector<ModelXmlInfo_t>::reverse_iterator it = mLocalParameter.mdItemInfo.rbegin();
    const char* element;
    switch (_type)
    {
        case XML_ELEMENT:
            if (strcmp(text, "CheckMode") == 0)
            {
                element = mLocateXmlOperate->getXMLElementText();
                mLocalParameter.VideoReadMode = std::atoi(element);
            }
            if (strcmp(text, "ModelPaht") == 0)
                (*it).modelPath = mLocateXmlOperate->getXMLElementText();
            if (strcmp(text, "Model") == 0)
            {
                ModelItemInfo_t _tempItem;
                _tempItem.cleanData();
                (*it)._modelItem.push_back(_tempItem);
            }
            if (strcmp(text, "Name") == 0)
            {
                std::vector<ModelItemInfo_t>::reverse_iterator it1 = (*it)._modelItem.rbegin();
                (*it1).name = mLocateXmlOperate->getXMLElementText();
            }
            if (strcmp(text, "Label") == 0)
            {
                std::vector<ModelItemInfo_t>::reverse_iterator it1 = (*it)._modelItem.rbegin();
                element = mLocateXmlOperate->getXMLElementText();
                (*it1).label = std::atoi(element);
            }
            if (strcmp(text, "Remar") == 0)
            {
                std::vector<ModelItemInfo_t>::reverse_iterator it1 = (*it)._modelItem.rbegin();
                (*it1).remark = mLocateXmlOperate->getXMLElementText();
            }
            break;
        case XML_ATTRIBUT:
            if (strcmp(text, "Step") == 0)
            {
                element = mLocateXmlOperate->getXMLAttributeText();
                mLocalParameter.VideoReadSetp = std::atoi(element);
            }
            if (strcmp(text, "article") == 0)
            {
                element = mLocateXmlOperate->getXMLAttributeText();
                mLocalParameter.VideoChoose = std::atoi(element) <= 0 ? mLocalParameter.inputFilePath.size() : std::atoi(element);
                if (mLocalParameter.VideoChoose <= 0)
                    mLocalParameter.VideoChoose = 1;
            }
            break;
    }
}

