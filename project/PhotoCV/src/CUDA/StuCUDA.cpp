
#include "stdafx.h"
#include "StuCUDA.h"
#ifdef _OpenCV_CUDA__

#if CV_MAJOR_VERSION<3

#endif

void TestGpuCUDA(void)
{
    int num_devices = getCudaEnabledDeviceCount();
    if (num_devices <= 0)
    {
        std::cerr << "there is no device" << std::endl;
        return ;
    }
    int enable_device_id = -1;
    for (int i = 0; i < num_devices; ++i)
    {
        DeviceInfo dev_info(i);
        if (dev_info.isCompatible())
            enable_device_id = i;
    }
    if (enable_device_id < 0)
    {
        std::cerr << "GPU module isn't built for GPU" << std::endl;
        return ;
    }
    setDevice(enable_device_id);
    cv::Mat src_image = cv::Mat(512, 512, CV_8UC3, Scalar(0, 255, 0));
    cv::Mat dst_image;
    GpuMat d_src_img(src_image);
    GpuMat d_dst_img;
    cvtColor(d_src_img, d_dst_img, CV_BGR2GRAY);
    d_dst_img.download(dst_image);
    cv::imshow("test", dst_image);
    cv::waitKey(0);
    return ;
}

//GPU测试算法
#ifdef ___TEST_CPU_GPU____

double getPSNR(const Mat& I1, const Mat& I2)
{
    Mat s1;
    absdiff(I1, I2, s1);    // |I1 - I2|
    s1.convertTo(s1, CV_32F);    // cannot make a square on 8 bits
    s1 = s1.mul(s1);    // |I1 - I2|^2
    Scalar s = sum(s1);    // sum elements per channel
    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
    if (sse <= 1e-10)   // for small values return zero
        return 0;
    else
    {
        double mse = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}

double getPSNR_GPU(const Mat& I1, const Mat& I2)
{
    GpuMat gI1, gI2, gs, t1, t2;
    gI1.upload(I1);
    gI2.upload(I2);
    gI1.convertTo(t1, CV_32F);
    gI2.convertTo(t2, CV_32F);
    absdiff(t1.reshape(1), t2.reshape(1), gs);
    multiply(gs, gs, gs);
    Scalar s = sum(gs);
    double sse = s.val[0] + s.val[1] + s.val[2];
    if (sse <= 1e-10)   // for small values return zero
        return 0;
    else
    {
        double mse = sse / (double)(gI1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}

double getPSNR_GPU_optimized(const Mat& I1, const Mat& I2, BufferPSNR& b)
{
    b.gI1.upload(I1);
    b.gI2.upload(I2);
    b.gI1.convertTo(b.t1, CV_32F);
    b.gI2.convertTo(b.t2, CV_32F);
    absdiff(b.t1.reshape(1), b.t2.reshape(1), b.gs);
    multiply(b.gs, b.gs, b.gs);
    double sse = sum(b.gs, b.buf) [0];
    if (sse <= 1e-10)       // for small values return zero
        return 0;
    else
    {
        double mse = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}

Scalar getMSSIM(const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d = CV_32F;
    Mat I1, I2;
    i1.convertTo(I1, d);    // cannot calculate on one byte large values
    i2.convertTo(I2, d);
    Mat I2_2 = I2.mul(I2);    // I2^2
    Mat I1_2 = I1.mul(I1);    // I1^2
    Mat I1_I2 = I1.mul(I2);    // I1 * I2
    /*************************** END INITS **********************************/
    Mat mu1, mu2; // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);
    Mat mu1_2 = mu1.mul(mu1);
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1_mu2 = mu1.mul(mu2);
    Mat sigma1_2, sigma2_2, sigma12;
    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;
    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;
    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;
    ///////////////////////////////// FORMULA ////////////////////////////////
    Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);    // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);    // t1 =((mu1 2 + mu2 2 + C1).*(sigma1 2 + sigma2 2 + C2))
    Mat ssim_map;
    divide(t3, t1, ssim_map);    // ssim map = t3./t1;
    Scalar mssim = mean(ssim_map);    // mssim = average of ssim map
    return mssim;
}

Scalar getMSSIM_GPU(const Mat& i1, const Mat& i2)
{
    const float C1 = 6.5025f, C2 = 58.5225f;
    /***************************** INITS **********************************/
    GpuMat gI1, gI2, gs1, tmp1, tmp2;
    gI1.upload(i1);
    gI2.upload(i2);
    gI1.convertTo(tmp1, CV_MAKE_TYPE(CV_32F, gI1.channels()));
    gI2.convertTo(tmp2, CV_MAKE_TYPE(CV_32F, gI2.channels()));
    vector<GpuMat> vI1, vI2;
    split(tmp1, vI1);
    split(tmp2, vI2);
    Scalar mssim;
    for (int i = 0; i < gI1.channels(); ++i)
    {
        GpuMat I2_2, I1_2, I1_I2;
        multiply(vI2[i], vI2[i], I2_2);    // I2^2
        multiply(vI1[i], vI1[i], I1_2);    // I1^2
        multiply(vI1[i], vI2[i], I1_I2);    // I1 * I2
        /*************************** END INITS **********************************/
        GpuMat mu1, mu2; // PRELIMINARY COMPUTING
        GaussianBlur(vI1[i], mu1, Size(11, 11), 1.5);
        GaussianBlur(vI2[i], mu2, Size(11, 11), 1.5);
        GpuMat mu1_2, mu2_2, mu1_mu2;
        multiply(mu1, mu1, mu1_2);
        multiply(mu2, mu2, mu2_2);
        multiply(mu1, mu2, mu1_mu2);
        GpuMat sigma1_2, sigma2_2, sigma12;
        GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
        subtract(sigma1_2, mu1_2, sigma1_2);    // sigma1 2 -= mu1 2;
        GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
        subtract(sigma2_2, mu2_2, sigma2_2);    // sigma2 2 -= mu2 2;
        GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
        subtract(sigma12, mu1_mu2, sigma12);    // sigma12 -= mu1 mu2;
        ///////////////////////////////// FORMULA ////////////////////////////////
        GpuMat t1, t2, t3;
        mu1_mu2.convertTo(t1, -1, 2, C1);    // t1 = 2 * mu1 mu2 + C1;
        sigma12.convertTo(t2, -1, 2, C2);    // t2 = 2 * sigma12 + C2;
        multiply(t1, t2, t3);    // t3 = ((2*mu1 mu2 + C1).*(2*sigma12 + C2))
        addWeighted(mu1_2, 1.0, mu2_2, 1.0, C1, t1);    // t1 = mu1 2 + mu2 2 + C1;
        addWeighted(sigma1_2, 1.0, sigma2_2, 1.0, C2, t2);    // t2 = sigma1 2 + sigma2 2 + C2;
        multiply(t1, t2, t1);    // t1 =((mu1 2 + mu2 2 + C1).*(sigma1 2 + sigma2 2 + C2))
        GpuMat ssim_map;
        divide(t3, t1, ssim_map);    // ssim map = t3./t1;
        Scalar s = sum(ssim_map);
        mssim.val[i] = s.val[0] / (ssim_map.rows * ssim_map.cols);
    }
    return mssim;
}

Scalar getMSSIM_GPU_optimized(const Mat& i1, const Mat& i2, BufferMSSIM& b)
{
    const float C1 = 6.5025f, C2 = 58.5225f;
    /***************************** INITS **********************************/
    b.gI1.upload(i1);
    b.gI2.upload(i2);
    Stream stream;
    stream.enqueueConvert(b.gI1, b.t1, CV_32F);
    stream.enqueueConvert(b.gI2, b.t2, CV_32F);
    split(b.t1, b.vI1, stream);
    split(b.t2, b.vI2, stream);
    Scalar mssim;
    GpuMat buf;
    for (int i = 0; i < b.gI1.channels(); ++i)
    {
        multiply(b.vI2[i], b.vI2[i], b.I2_2, stream);    // I2^2
        multiply(b.vI1[i], b.vI1[i], b.I1_2, stream);    // I1^2
        multiply(b.vI1[i], b.vI2[i], b.I1_I2, stream);    // I1 * I2
        GaussianBlur(b.vI1[i], b.mu1, Size(11, 11), buf, 1.5, 0, BORDER_DEFAULT, -1, stream);
        GaussianBlur(b.vI2[i], b.mu2, Size(11, 11), buf, 1.5, 0, BORDER_DEFAULT, -1, stream);
        multiply(b.mu1, b.mu1, b.mu1_2, stream);
        multiply(b.mu2, b.mu2, b.mu2_2, stream);
        multiply(b.mu1, b.mu2, b.mu1_mu2, stream);
        GaussianBlur(b.I1_2, b.sigma1_2, Size(11, 11), buf, 1.5, 0, BORDER_DEFAULT, -1, stream);
        subtract(b.sigma1_2, b.mu1_2, b.sigma1_2, GpuMat(), -1, stream);
        //b.sigma1 2 -= b.mu1 2; - This would result in an extra data transfer operation
        GaussianBlur(b.I2_2, b.sigma2_2, Size(11, 11), buf, 1.5, 0, BORDER_DEFAULT, -1, stream);
        subtract(b.sigma2_2, b.mu2_2, b.sigma2_2, GpuMat(), -1, stream);
        //b.sigma2 2 -= b.mu2 2;
        GaussianBlur(b.I1_I2, b.sigma12, Size(11, 11), buf, 1.5, 0, BORDER_DEFAULT, -1, stream);
        subtract(b.sigma12, b.mu1_mu2, b.sigma12, GpuMat(), -1, stream);
        //b.sigma12 -= b.mu1 mu2;
        //here too it would be an extra data transfer due to call of operator*(Scalar, Mat)
        multiply(b.mu1_mu2, 2, b.t1, 1, -1, stream);    //b.t1 = 2 * b.mu1 mu2 + C1;
        add(b.t1, C1, b.t1, GpuMat(), -1, stream);
        multiply(b.sigma12, 2, b.t2, 1, -1, stream);    //b.t2 = 2 * b.sigma12 + C2;
        add(b.t2, C2, b.t2, GpuMat(), -12, stream);
        multiply(b.t1, b.t2, b.t3, 1, -1, stream);    // t3 = ((2*mu1 mu2 + C1).*(2*sigma12 + C2))
        add(b.mu1_2, b.mu2_2, b.t1, GpuMat(), -1, stream);
        add(b.t1, C1, b.t1, GpuMat(), -1, stream);
        add(b.sigma1_2, b.sigma2_2, b.t2, GpuMat(), -1, stream);
        add(b.t2, C2, b.t2, GpuMat(), -1, stream);
        multiply(b.t1, b.t2, b.t1, 1, -1, stream);    // t1 =((mu1 2 + mu2 2 + C1).*(sigma1 2 + sigma2 2 + C2))
        divide(b.t3, b.t1, b.ssim_map, 1, -1, stream);    // ssim map = t3./t1;
        stream.waitForCompletion();
        Scalar s = sum(b.ssim_map, b.buf);
        mssim.val[i] = s.val[0] / (b.ssim_map.rows * b.ssim_map.cols);
    }
    return mssim;
}

#endif

void witeGpuMat(string f_name, GpuMat& f_mat)
{
    Mat tmp;
    f_mat.download(tmp);
    imwrite(f_name, tmp);
}

//f_mat>=f_num 得出f_out
void largerthan(GpuMat& f_mat, float f_num, GpuMat& temp)
{
    GpuMat f_out;
    f_num = -f_num;
    multiply(f_mat, cv::Scalar(-1), f_out);
    threshold(f_out, f_out, f_num, 255, CV_THRESH_BINARY_INV);
    f_out.convertTo(temp, CV_8U);
}

//f_mat<f_num 得出f_out
void lessthan(GpuMat& f_mat, float f_num, GpuMat& temp)
{
    GpuMat f_out;
    f_num = -f_num;
    multiply(f_mat, cv::Scalar(-1), f_out);
    threshold(f_out, f_out, f_num, 255, CV_THRESH_BINARY);
    f_out.convertTo(temp, CV_8U);
}

void saveXYZ1(const char* filename, const Mat& mat)
{
    char szLine[20000] = {0};
    char szWord[10000] = {0};
    FILE* fp = fopen(filename, "wt");
    for (int x = 0; x < mat.rows; x++)
    {
        strcpy(szLine, "");
        for (int y = 0; y < mat.cols; y++)
        {
            double point = cvGetReal2D(&CvMat(mat), x, y);
            sprintf(szWord, "%.8f,", point);
            strcat(szLine, szWord);
        }
        fprintf(fp, "%s\n", szLine);
    }
    fclose(fp);
}

void saveXYZ1(const char* filename, const GpuMat& mat)
{
    Mat tmp;
    mat.download(tmp);
    saveXYZ1(filename, tmp);
}





#endif