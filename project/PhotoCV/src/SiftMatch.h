//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SQ_C_API
    #define SQ_C_API extern "C" __declspec(dllexport)
#endif



class SiftMatch
{
public:
    SiftMatch();
    virtual ~SiftMatch();
private:
    Mat imgL, imgR;         //����ͼƬ
public:
    void loadIMG(const char* imglname, const char* imgrname);
};












