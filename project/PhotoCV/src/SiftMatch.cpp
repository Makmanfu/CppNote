
#include "stdafx.h"
#include "SiftMatch.h"




SiftMatch::SiftMatch()
{
}

SiftMatch::~SiftMatch()
{
}

void SiftMatch::loadIMG(const char* imglname, const char* imgrname)
{
    imgL = imread(imglname);
    imgR = imread(imgrname);
}

