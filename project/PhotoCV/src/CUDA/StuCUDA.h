//**************************************************************************************
//*                 GPU 并行开发
//**************************************************************************************

#pragma once
#ifdef _OpenCV_CUDA__

#include <opencv2/opencv.hpp>
#if CV_MAJOR_VERSION<3
    #include <opencv2/gpu/gpu.hpp>
    using namespace cv::gpu;            //注意为防止cv名字空间冲突 不要cv空间
#else
    #include "opencv2/opencv.hpp"
    #include "opencv2/cudaimgproc.hpp"
    #include "opencv2/cudaarithm.hpp"
    #include "opencv2/cudabgsegm.hpp"
    #include "opencv2/cudacodec.hpp"
    #include "opencv2/cudafeatures2d.hpp"
    #include "opencv2/cudafilters.hpp"
    #include "opencv2/cudaimgproc.hpp"
    #include "opencv2/cudalegacy.hpp"
    #include "opencv2/cudaobjdetect.hpp"
    #include "opencv2/cudaoptflow.hpp"
    #include "opencv2/cudastereo.hpp"
    #include "opencv2/cudawarping.hpp"
    using namespace cv::cuda;
#endif
#include <iostream>
#include <vector>
using namespace std;
using std::vector;

#if CV_MAJOR_VERSION<0
    void TestGpu(void);

    //cv::cuda::GpuMat d_mask;
    //cv::cuda::connectivityMask(cv::cuda::GpuMat(image), d_mask, cv::Scalar::all(0), cv::Scalar::all(2));
    //cv::cuda::GpuMat components;
    //TEST_CYCLE() cv::cuda::labelComponents(d_mask, components);

#endif

//测试opencvCUDA库
void TestGpuCUDA(void);

//GPU测试算法
#ifdef ___TEST_CPU_GPU____

// Optimized GPU versions
struct BufferPSNR
{
    // Data allocations are very expensive on GPU. Use a buffer to solve: allocate once reuse later.
    GpuMat gI1, gI2, gs, t1, t2;
    GpuMat buf;
};

// Optimized GPU versions
struct BufferMSSIM
{
    // Data allocations are very expensive on GPU. Use a buffer to solve: allocate once reuse later.
    GpuMat gI1, gI2, gs, t1, t2;
    GpuMat I1_2, I2_2, I1_I2;
    vector<GpuMat> vI1, vI2;
    GpuMat mu1, mu2;
    GpuMat mu1_2, mu2_2, mu1_mu2;
    GpuMat sigma1_2, sigma2_2, sigma12;
    GpuMat t3;
    GpuMat ssim_map;
    GpuMat buf;
};

double getPSNR(const Mat& I1, const Mat& I2);
double getPSNR_GPU(const Mat& I1, const Mat& I2);
double getPSNR_GPU_optimized(const Mat& I1, const Mat& I2, BufferPSNR& b);
Scalar getMSSIM(const Mat& i1, const Mat& i2);
Scalar getMSSIM_GPU(const Mat& i1, const Mat& i2);
Scalar getMSSIM_GPU_optimized(const Mat& i1, const Mat& i2, BufferMSSIM& b);

#endif

void witeGpuMat(string f_name, GpuMat& f_mat);

//f_mat>=f_num 得出f_out
void largerthan(GpuMat& f_mat, float f_num, GpuMat& temp);

//f_mat<f_num 得出f_out
void lessthan(GpuMat& f_mat, float f_num, GpuMat& temp);

//测试mat单通道数据
void saveXYZ(const char* filename, const Mat& mat);

void saveXYZ(const char* filename, const GpuMat& mat);



#endif