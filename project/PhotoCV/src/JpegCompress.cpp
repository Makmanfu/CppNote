
#include "stdafx.h"
#include "JpegCompress.h"

//读取Jpeg图片的数据并返回，如果出错，返回NULL
unsigned char* JpegRead(const char* path, int& width, int& height)
{
    FILE* file = fopen(path, "rb");
    if (file == NULL)
        return NULL;
    struct jpeg_decompress_struct info; //for our jpeg info
    //  struct jpeg_error_mgr err; //the error handler
    //  info.err = jpeg_std_error(&err);
    struct my_error_mgr my_err;
    info.err = jpeg_std_error(&my_err.pub);
    my_err.pub.error_exit = my_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(my_err.setjmp_buffer))
    {
        /*  If we get here, the JPEG code has signaled an error.
        We need to clean up the JPEG object, close the input file, and return.*/
        printf("Error occured\n");
        jpeg_destroy_decompress(&info);
        fclose(file);
        return NULL;
    }
    jpeg_create_decompress(&info); //fills info structure
    jpeg_stdio_src(&info, file);        //void
    int ret_Read_Head = jpeg_read_header(&info, 1); //int
    if (ret_Read_Head != JPEG_HEADER_OK)
    {
        printf("jpeg_read_header failed\n");
        fclose(file);
        jpeg_destroy_decompress(&info);
        return NULL;
    }
    bool bStart = jpeg_start_decompress(&info);
    if (!bStart)
    {
        printf("jpeg_start_decompress failed\n");
        fclose(file);
        jpeg_destroy_decompress(&info);
        return NULL;
    }
    int w = width = info.output_width;
    int h = height = info.output_height;
    int numChannels = info.num_components; // 3 = RGB, 4 = RGBA
    unsigned long dataSize = w * h * numChannels;
    // read RGB(A) scanlines one at a time into jdata[]
    unsigned char* data = (unsigned char*)malloc(dataSize);
    if (!data)
        return NULL;
    unsigned char* rowptr;
    while (info.output_scanline < h)
    {
        rowptr = data + info.output_scanline * w * numChannels;
        jpeg_read_scanlines(&info, &rowptr, 1);
    }
    jpeg_finish_decompress(&info);
    fclose(file);
    return data;
}

//压缩尺寸方法
unsigned char* JpegReSize(int w_Dest, int h_Dest, int bit_depth, unsigned char* src, int w_Src, int h_Src)
{
    /*  参数为：
    返回图片的宽度(w_Dest),
    返回图片的高度(h_Dest),
    返回图片的位深(bit_depth),
    源图片的RGB数据(src),
    源图片的宽度(w_Src),
    源图片的高度(h_Src)
    */
    int sw = w_Src - 1, sh = h_Src - 1, dw = w_Dest - 1, dh = h_Dest - 1;
    int B, N, x, y;
    int nPixelSize = bit_depth / 8;
    unsigned char* pLinePrev, *pLineNext;

    //T<char> pDest(w_Dest * h_Dest * bit_depth / 8);
    unsigned char* pDest = new unsigned char[w_Dest * h_Dest * bit_depth / 8];  //stack full
    unsigned char* tmp;
    unsigned char* pA, *pB, *pC, *pD;
    for (int i = 0; i <= dh; ++i)
    {
        tmp = pDest + i * w_Dest * nPixelSize;
        y = i * sh / dh;
        N = dh - i * sh % dh;
        pLinePrev = src + (y++) * w_Src * nPixelSize;
        //pLinePrev =(unsigned char *)aSrc->m_bitBuf+((y++)*aSrc->m_width*nPixelSize);
        pLineNext = (N == dh) ? pLinePrev : src + y * w_Src * nPixelSize;
        //pLineNext = ( N == dh ) ? pLinePrev : (unsigned char *)aSrc->m_bitBuf+(y*aSrc->m_width*nPixelSize);
        for (int j = 0; j <= dw; ++j)
        {
            x = j * sw / dw * nPixelSize;
            B = dw - j * sw % dw;
            pA = pLinePrev + x;
            pB = pA + nPixelSize;
            pC = pLineNext + x;
            pD = pC + nPixelSize;
            if (B == dw)
            {
                pB = pA;
                pD = pC;
            }
            for (int k = 0; k < nPixelSize; ++k)
            {
                *tmp++ = (unsigned char)(int)((B * N * (*pA++ - *pB - *pC + *pD) + dw * N * *pB++
                    + dh * B * *pC++ + (dw * dh - dh * B - dw * N) * *pD++
                    + dw * dh / 2) / (dw * dh));
            }
        }
    }
    return pDest;
}

//写文件
bool JpegWrite(const char* filename, unsigned char* image_buffer, int quality, int image_height, int image_width)
{
    if (filename == NULL || image_buffer == NULL) return false;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* outfile;                                  /* target file */
    JSAMPROW row_pointer[1];                        /* pointer to JSAMPLE row[s] */
    int row_stride;                                 /* physical row width in image buffer */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);
    if ((outfile = fopen(filename, "wb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", filename);
        return false;
    }
    jpeg_stdio_dest(&cinfo, outfile);
    //jpeg_mem_dest
    cinfo.image_width = image_width;                /* image width and height, in pixels    */
    cinfo.image_height = image_height;
    cinfo.input_components = 3;                     /* # of color components per pixel      */
    cinfo.in_color_space = JCS_RGB;                 /* colorspace of input image            */
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);        /* limit to baseline-JPEG values        */
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = image_width * 3;                   /* JSAMPLEs per row in image_buffer     */
    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    return true;
}

//生成缩小图片
bool compare_image(const char* inputFile, const char* outputFile)
{
    if (inputFile == NULL || outputFile == NULL) return false;
    //读取jpeg图片像素数组
    int w = 0, h = 0;
    unsigned char* buff = JpegRead(inputFile, w, h);
    if (buff == NULL)
    {
        printf("ReadJpeg Failed\n");
        return false;
    }
    //缩放图片，缩放后的大小为(tb_w,tb_h)
    //int tb_w = 500, tb_h = 500;
    int tb_w = w , tb_h = h ;
    unsigned char* img_buf = JpegReSize(tb_w, tb_h, 24, buff, w, h);
    free(buff);
    //将缩放后的像素数组保存到jpeg文件
    bool bRetWrite = JpegWrite(outputFile, img_buf, 90, tb_h, tb_w);
    delete[] img_buf;
    if (bRetWrite)
        return true;
    else
    {
        printf("GenerateImageThumbnail: write failed\n");
        return true;
    }
}

int JpegResize(int argc, char** argv)
{
    compare_image("1.jpg", "2.jpg");
    return 1;
}





