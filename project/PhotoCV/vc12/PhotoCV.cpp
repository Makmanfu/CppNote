
#include "stdafx.h"
#include "CToolClass.hpp"
#include "DemoFun.h"
#include "CVDlg.h"

#include "HudDisplay.h"
#include "MatDispose.h"
#include "JpegCompress.h"


int main(int argc, char** argv)
{
    SetConsoleTitleA("VISION");
    system("color 0A");  //0D
    CMDProc fp = NULL;
    CmdProc<CMDProc> cmds(argv[0]);
    cmds.CreateToolBat();
    cmds.DefaultFunAdd(test_Show);
    cmds.ComandAdd(string("testShow"), test_Show);         //测试opencv
    cmds.ComandAdd(string("testSift"), test_sift);         //sift特征点匹配
    cmds.ComandAdd(string("testLBP"), test_LBP);           //LBP算子效果
    cmds.ComandAdd(string("testface"), test_face);         //检测人脸例子
    cmds.ComandAdd(string("testCam"), test_Cam);           //检测摄像头例子
    cmds.ComandAdd(string("testpep"), test_pep);           //检测行人例子
    cmds.ComandAdd(string("testSpeed"), test_Speed);       //检测限速牌
    cmds.ComandAdd(string("TrainSample"), TrainSample);    //生成样本数据测试训练
    cmds.ComandAdd(string("Gethist"), test_gethist);       //计算图像直方图
    cmds.ComandAdd(string("CheckNum"), test_CheckNum);     //分割图像测试
    cmds.ComandAdd(string("ArrowShow"), TestArrowShow);    //HUD
    cmds.ComandAdd(string("JpegResize"), JpegResize);     //jpeg Compress
    string strcmd = argc > 1 ? argv[1] : "default";
    cmds.GetCmdFunProc(strcmd, &fp);
    if (fp != NULL)
        fp(argc, argv);
    return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    WINProc fp = NULL;
    CWINParam cmds;
    cmds.DefaultFunAdd(&CVDlg::WinMain);
    cmds.ComandAdd(string("CVDEMO"), &CVDlg::WinMain);
    cmds.GetCmdFunProc(string(lpCmdLine), &fp);
    if (fp != NULL)
        fp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return 0;
}

extern "C" __declspec(dllexport) int dmain(int argc, char** argv)
{
    return main(argc, argv);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}


////Gdiplus::Bitmap转 IplImage   pIplImage 需要外部释放 Mosesyuan
//void BitmapToIplImage(Gdiplus::Bitmap* pBitmap, IplImage*& pIplImg)
//{
//    if (!pBitmap)
//        return;
//    if (pIplImg)
//    {
//        cvReleaseImage(&pIplImg);
//        pIplImg = NULL;
//    }
//    BitmapData bmpData;
//    Gdiplus::Rect rect(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
//    pBitmap->LockBits(&rect, ImageLockModeRead, PixelFormat24bppRGB, &bmpData);
//    IplImage* tempImg = cvCreateImage(cvSize(pBitmap->GetWidth(), pBitmap->GetHeight()), IPL_DEPTH_8U, 3);
//    BYTE* temp = (bmpData.Stride > 0) ? ((BYTE*)bmpData.Scan0) : ((BYTE*)bmpData.Scan0 + bmpData.Stride * (bmpData.Height - 1));
//    memcpy(tempImg->imageData, temp, abs(bmpData.Stride)*bmpData.Height);
//    pBitmap->UnlockBits(&bmpData);
//    pIplImg = tempImg;
//    //判断Top-Down or Bottom-Up
//    if (bmpData.Stride < 0)
//        cvFlip(pIplImg, pIplImg);
//}
//
////OpenCV中 IplImage 转 Gdiplus::Bitmap  pBitmap 同样需要外部释放！！
//void IplImageToBitmap(IplImage* pIplImg, Gdiplus::Bitmap*& pBitmap)
//{
//    if (!pIplImg)
//        return;
//    BITMAPINFOHEADER bmih;
//    memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
//    bmih.biSize = sizeof(BITMAPINFOHEADER);
//    bmih.biWidth = pIplImg->width;
//    bmih.biHeight = pIplImg->height;
//    bmih.biPlanes = 1;
//    bmih.biBitCount = pIplImg->depth * pIplImg->nChannels;
//    bmih.biSizeImage = pIplImg->imageSize;
//    BYTE* pData = new BYTE[bmih.biSizeImage];
//    memcpy(pData, pIplImg->imageDataOrigin, pIplImg->imageSize);
//    if (pBitmap)
//    {
//        delete pBitmap;
//        pBitmap = NULL;
//    }
//    pBitmap = Gdiplus::Bitmap::FromBITMAPINFO((BITMAPINFO*)&bmih, pData);
//}
