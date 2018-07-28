//*************************************************************************
//
//                     JPG图片缩放
//
//
//
//*************************************************************************

#ifndef JPEGCOMPRESS_H
#define JPEGCOMPRESS_H

#include <stdio.h>
//#include "jpeglib.h"
#include <setjmp.h>

struct my_error_mgr
{
    struct jpeg_error_mgr pub;  /* "public" fields */
    jmp_buf setjmp_buffer;  /* for return to caller */
};

typedef struct my_error_mgr* my_error_ptr;

//Here's the routine that will replace the standard error_exit method:
METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_ptr myerr = (my_error_ptr)cinfo->err;
    /* Always display the message. */
    /* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message)(cinfo);
    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}

//读取Jpeg图片的数据并返回，如果出错，返回NULL
C_API unsigned char* JpegRead(const char* path, int& width, int& height);

//压缩尺寸方法
C_API unsigned char* JpegReSize(int w_Dest, int h_Dest, int bit_depth, unsigned char* src, int w_Src, int h_Src);

//写文件
C_API bool JpegWrite(const char* filename, unsigned char* image_buffer, int quality, int image_height, int image_width);

//生成缩小图片
C_API bool compare_image(const char* inputFile, const char* outputFile);

int JpegResize(int argc, char** argv);



#endif


