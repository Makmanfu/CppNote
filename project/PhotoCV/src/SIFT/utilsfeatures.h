/** @file
    Miscellaneous utility functions.

    Copyright (C) 2006-2012  Rob Hess <rob@iqengines.com>

    @version 1.1.2-20100521
*/

#ifndef UTILSFEATURES_H
#define UTILSFEATURES_H

#include "cxcore.h"
#include <stdlib.h>
#include <stdio.h>
//#include <dirent.h>
#define inline __inline


/*
    ���ļ���������ͼ��Ļ�����������:
    1����ȡĳλ�õ����ص�
    2������ĳλ�õ����ص㣨8λ��32λ��64λ����
    3����������֮��ľ����ƽ��
    4����ͼƬĳһ�㻭һ����X��
    5��������ͼƬ�ϳ�Ϊһ�������Ƕ���֮�ͣ����Ƕ��ߵĽϴ��ߡ�img1 �����Ͻǣ�img2�����½ǡ�
*/

//��x�ľ���ֵ
/* absolute value */
#ifndef ABS
    #define ABS(x) ( ( (x) < 0 )? -(x) : (x) )
#endif

//������������ȡ�����ø�������ͼ�������ֵ
/***************************** Inline Functions ******************************/


/**
    A function to get a pixel value from an 8-bit unsigned image.

    @param img an image
    @param r row
    @param c column
    @return Returns the value of the pixel at (\a r, \a c) in \a img
*/
static inline int pixval8(IplImage* img, int r, int c)
{
    return (int)(((uchar*)(img->imageData + img->widthStep * r)) [c]);
}


/**
    A function to set a pixel value in an 8-bit unsigned image.

    @param img an image
    @param r row
    @param c column
    @param val pixel value
*/
static inline void setpix8(IplImage* img, int r, int c, uchar val)
{
    ((uchar*)(img->imageData + img->widthStep * r)) [c] = val;
}


/*  ��32λ�����͵�ͨ��ͼ���л�ȡָ�����������ֵ����������
    ������
    img������ͼ��
    r��������
    c��������
    ����ֵ������(c,r)��(r��c��)������ֵ
*/
/**
    A function to get a pixel value from a 32-bit floating-point image.

    @param img an image
    @param r row
    @param c column
    @return Returns the value of the pixel at (\a r, \a c) in \a img
*/
static inline float pixval32f(IplImage* img, int r, int c)
{
    return ((float*)(img->imageData + img->widthStep * r)) [c];
}


/**
    A function to set a pixel value in a 32-bit floating-point image.

    @param img an image
    @param r row
    @param c column
    @param val pixel value
*/
static inline void setpix32f(IplImage* img, int r, int c, float val)
{
    ((float*)(img->imageData + img->widthStep * r)) [c] = val;
}


/**
    A function to get a pixel value from a 64-bit floating-point image.

    @param img an image
    @param r row
    @param c column
    @return Returns the value of the pixel at (\a r, \a c) in \a img
*/
static inline double pixval64f(IplImage* img, int r, int c)
{
    return (double)(((double*)(img->imageData + img->widthStep * r)) [c]);
}


/**
    A function to set a pixel value in a 64-bit floating-point image.

    @param img an image
    @param r row
    @param c column
    @param val pixel value
*/
static inline void setpix64f(IplImage* img, int r, int c, double val)
{
    ((double*)(img->imageData + img->widthStep * r)) [c] = val;
}


/**************************** Function Prototypes ****************************/

//������
/**
    Prints an error message and aborts the program.  The error message is
    of the form "Error: ...", where the ... is specified by the \a format
    argument

    @param format an error message format string (as with \c printf(3)).
*/
extern void fatal_error(char* format, ...);


//��ȡһ���ļ�ȫ�������ļ�������չ�����ӵ�һ��
/**
    Replaces a file's extension, which is assumed to be everything after the
    last dot ('.') character.

    @param file the name of a file

    @param extn a new extension for \a file; should not include a dot (i.e.
    \c "jpg", not \c ".jpg") unless the new file extension should contain
    two dots.

    @return Returns a new string formed as described above.  If \a file does
    not have an extension, this function simply adds one.
*/
extern char* replace_extension(const char* file, const char* extn);

//�ļ�����ȥ��·��c:\\traffic.jpg => traffic.jpg
/**
    Prepends a path to a filename.

    @param path a path
    @param file a file name

    @return Returns a new string containing a full path name consisting of
    \a path prepended to \a file.
*/
extern char* prepend_path(const char* path, const char* file);


/**
    A function that removes the path from a filename.  Similar to the Unix
    basename command.

    @param pathname a (full) path name

    @return Returns the basename of \a pathname.
*/
extern char* basename(const char* pathname);

//��ʾ�������
/**
    Displays progress in the console with a spinning pinwheel.  Every time this
    function is called, the state of the pinwheel is incremented.  The pinwheel
    has four states that loop indefinitely: '|', '/', '-', '\'.

    @param done if 0, this function simply increments the state of the pinwheel;
    otherwise it prints "done"
*/
extern void progress(int done);


/**
    Erases a specified number of characters from a stream.

    @param stream the stream from which to erase characters
    @param n the number of characters to erase
*/
extern void erase_from_stream(FILE* stream, int n);

//���鳤�ȼӱ�
/**
    Doubles the size of an array with error checking

    @param array pointer to an array whose size is to be doubled
    @param n number of elements allocated for \a array
    @param size size in bytes of elements in \a array

    @return Returns the new number of elements allocated for \a array.  If no
    memory is available, returns 0 and frees \a array.
*/
extern int array_double(void** array, int n, int size);

//��������ĶԽ��߾���
/**
    Calculates the squared distance between two points.

    @param p1 a point
    @param p2 another point
*/
extern double dist_sq_2D(CvPoint2D64f p1, CvPoint2D64f p2);

//�ڵ�pt�����棬���ʾ������Ǹ��㻭������
/**
    Draws an x on an image.

    @param img an image
    @param pt the center point of the x
    @param r the x's radius
    @param w the x's line weight
    @param color the color of the x
*/
extern void draw_x(IplImage* img, CvPoint pt, int r, int w, CvScalar color);


/*  ������ͼ��ϳ�Ϊһ�ţ���ֱ�ŷ�,���Ƕ���֮�ͣ����Ƕ��ߵĽϴ���
    ������img1��λ���Ϸ���ͼ���ָ�룬img2��λ���·���ͼ���ָ��
    ����ֵ���ϳ�ͼ��
*/
/**
    Combines two images by scacking one on top of the other

    @param img1 top image
    @param img2 bottom image

    @return Returns the image resulting from stacking \a img1 on top if \a img2
*/
extern IplImage* stack_imgs(IplImage* img1, IplImage* img2);

/*  ������ͼ��ϳ�Ϊһ�ţ�ˮƽ�ŷ�
    ������img1��λ����ߵ�ͼ���ָ�룬img2��λ���ұߵ�ͼ���ָ��
    ����ֵ���ϳ�ͼ��
*/
extern IplImage* stack_imgs_horizontal(IplImage* img1, IplImage* img2);


/**
    Displays an image, making sure it fits on screen.  cvWaitKey() must be
    called after this function so the event loop is entered and the
    image is displayed.

    @param img an image, possibly too large to display on-screen
    @param title the title of the window in which \a img is displayed
*/
extern void display_big_img(IplImage* img, char* title);


/**
    Allows user to view an array of images as a video.  Keyboard controls
    are as follows:

    <ul>
    <li>Space - start and pause playback</li>
    <li>Page Up - skip forward 10 frames</li>
    <li>Page Down - jump back 10 frames</li>
    <li>Right Arrow - skip forward 1 frame</li>
    <li>Left Arrow - jump back 1 frame</li>
    <li>Backspace - jump back to beginning</li>
    <li>Esc - exit playback</li>
    <li>Closing the window also exits playback</li>
    </ul>

    @param imgs an array of images
    @param n number of images in \a imgs
    @param win_name name of window in which images are displayed
*/
extern void vid_view(IplImage** imgs, int n, char* win_name);

//�鿴ĳ�������Ƿ��Ѿ��ر�
/**
    Checks if a HighGUI window is still open or not

    @param name the name of the window we're checking

    @return Returns 1 if the window named \a name has been closed or 0 otherwise
*/
extern int win_closed(char* name);


//==================================================
/*
    ���ļ�������С���ȶ���(С����)��ص�һЩ�ṹ�ͺ�������
*/

/******************************* Defs and macros *****************************/

/* initial # of priority queue elements for which to allocate space */
#define MINPQ_INIT_NALLOCD 512  //��ʼ����ռ����

/********************************** Structures *******************************/
/*���ṹ*/
/** an element in a minimizing priority queue */
struct pq_node
{
    void* data;
    int key;
};

/*��С���ȶ��нṹ*/
/** a minimizing priority queue */
struct min_pq
{
    struct pq_node* pq_array;    /* array containing priority queue */ //���ָ��
    int nallocd;                 /* number of elements allocated */ //����Ŀռ����
    int n;                       /**< number of elements in pq */ //Ԫ�ظ���
};


/*************************** Function Prototypes *****************************/
/*  ��ʼ����С���ȼ�����
*/
/**
    Creates a new minimizing priority queue.
*/
extern struct min_pq* minpq_init();

/*  ����Ԫ�ص����ȶ���
    ������
    min_pq�����ȶ���
    data:Ҫ���������
    key:��data�����ļ�ֵ
    ����ֵ��0���ɹ���1��ʧ��
*/
/**
    Inserts an element into a minimizing priority queue.

    @param min_pq a minimizing priority queue
    @param data the data to be inserted
    @param key the key to be associated with \a data

    @return Returns 0 on success or 1 on failure.
*/
extern int minpq_insert(struct min_pq* min_pq, void* data, int key);


/*  �������ȶ����м�ֵ��С��Ԫ�أ�������ɾ����
    ������min_pq�����ȶ���
    ����ֵ����СԪ�ص�ָ��
*/
/**
    Returns the element of a minimizing priority queue with the smallest key
    without removing it from the queue.

    @param min_pq a minimizing priority queue

    @return Returns the element of \a min_pq with the smallest key or NULL
    if \a min_pq is empty
*/
extern void* minpq_get_min(struct min_pq* min_pq);


/*  ���ز��Ƴ�������С��ֵ��Ԫ��
    ������min_pq�����ȼ�����
    ����ֵ����СԪ�ص�ָ��
*/
/**
    Removes and returns the element of a minimizing priority queue with the
    smallest key.

    @param min_pq a minimizing priority queue

    @return Returns the element of \a min_pq with the smallest key of NULL
    if \a min_pq is empty
*/
extern void* minpq_extract_min(struct min_pq* min_pq);

/*  �ͷ����ȶ���
*/
/**
    De-allocates the memory held by a minimizing priorioty queue

    @param min_pq pointer to a minimizing priority queue
*/
extern void minpq_release(struct min_pq** min_pq);










//================================================================
/*
    ���ļ��а����ĺ��������У�
    1��RANSAC�㷨����ƥ��ɸѡ
    2������һЩ��RANSAC�㷨��غ���
*/

/********************************** Structures *******************************/

struct feature;

//RANSAC�㷨���õ��Ľṹ
//��RANSAC�㷨�����У����������ݻᱻ��ֵ��feature�ṹ��feature_data��Ա
/** holds feature data relevant to ransac */
struct ransac_data
{
    void* orig_feat_data; //������������feature_data�����ǰ��ֵ
    int sampled; //��ʶλ��ֵΪ1��ʶ���������Ƿ�ѡΪ����
};

//һЩ�궨��
/******************************* Defs and macros *****************************/

/*  RANSAC�㷨���ݴ��
    ����ƥ����<pt,mpt>���Լ��任����H��
    ���pt��H�任��ĵ��mpt֮��ľ����ƽ��С��RANSAC_ERR_TOL����ɽ�����뵱ǰһ�¼�
*/
/* RANSAC error tolerance in pixels */
#define RANSAC_ERR_TOL 3

//�ڵ���Ŀռ��������Ŀ�İٷֱȵ���Сֵ
/** pessimistic estimate of fraction of inliers for RANSAC */
#define RANSAC_INLIER_FRAC_EST 0.25

//һ��ƥ����֧�ִ���ģ�͵ĸ��ʣ���֪���Ǹ�ʲô�õģ�
/** estimate of the probability that a correspondence supports a bad model */
#define RANSAC_PROB_BAD_SUPP 0.10

//������һ���������ĺ����꣬������ȡ����feat�е�feature_data��Ա��ת��Ϊransac_data��ʽ��ָ��
/* extracts a feature's RANSAC data */
#define feat_ransac_data( feat ) ( (struct ransac_data*) (feat)->feature_data )


/*  ������һ������ָ������ransac_xform_fn���䷵��ֵ��CvMat*���ͣ�����������
    ֮�������ransac_xform_fn�����庯��ָ��
    �����͵ĺ���ָ�뱻����ransac_form()�����Ĳ�����
    �����͵ĺ��������ƥ���Լ��ϼ����һ���任������Ϊ����ֵ
    ������
    pts����ļ���
    mpts����ļ��ϣ�pts[i]��mpts[i]�ǻ����Ӧ��ƥ���
    n��pts��mpts�����е�ĸ�����pts��mpts�е�ĸ���������ͬ
    ����ֵ��һ���任���󣬽�pts�е�ÿһ����ת��Ϊmpts�еĶ�Ӧ�㣬����ֵΪ�ձ�ʾʧ��
*/
/**
    Prototype for transformation functions passed to ransac_xform().  Functions
    of this type should compute a transformation matrix given a set of point
    correspondences.

    @param pts array of points
    @param mpts array of corresponding points; each \a pts[\a i], \a i=0..\a
    n-1, corresponds to \a mpts[\a i]
    @param n number of points in both \a pts and \a mpts

    @return Should return a transformation matrix that transforms each point in
    \a pts to the corresponding point in \a mpts or NULL on failure.
*/
typedef CvMat* (*ransac_xform_fn)(CvPoint2D64f* pts, CvPoint2D64f* mpts, int n);


/*  ������һ������ָ������ransac_err_fn���䷵��ֵ��double���ͣ�����������
    ֮�������ransac_err_fn�����庯��ָ��
    �����͵ĺ���ָ�뱻����ransac_form()�����Ĳ�����
    �����͵ĺ��������ƥ����(pt,mpt)�ͱ任����M�����һ��double���͵Ĵ������ֵ��Ϊ����ֵ
    �˴������ֵ��������"��mpt"��"��pt��M����任��ĵ�"֮���Ƿ���һ��
    ������
    pt��һ����
    mpt����pt�Ķ�Ӧƥ���
    M���任����
    ����ֵ��"��mpt"��"��pt��M����任��ĵ�"֮��Ĵ������ֵ
*/
/**
    Prototype for error functions passed to ransac_xform().  For a given
    point, its correspondence, and a transform, functions of this type should
    compute a measure of error between the correspondence and the point after
    the point has been transformed by the transform.

    @param pt a point
    @param mpt \a pt's correspondence
    @param T a transform

    @return Should return a measure of error between \a mpt and \a pt after
    \a pt has been transformed by the transform \a T.
*/
typedef double (*ransac_err_fn)(CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* T);


/***************************** Function Prototypes ***************************/

/*  ����RANSAC�㷨����������ɸѡ����������ƥ��ı任����
    ������
    features�����������飬ֻ�е�mtype���͵�ƥ������ʱ�ű��������е�Ӧ�Լ���
    n�����������
    mtype������ʹ��ÿ����������ĸ�ƥ������б任����ļ��㣬Ӧ����FEATURE_MDL_MATCH��
    FEATURE_BCK_MATCH��FEATURE_MDL_MATCH�е�һ��������FEATURE_MDL_MATCH��
    ��Ӧ��ƥ����������ÿ���������img_pt�����ƥ����mdl_pt��
    ���򣬶�Ӧ��ƥ������ÿ���������img_pt�����ƥ����img_pt��
    xform_fn������ָ�룬ָ���������ĵ�Խ��б任�������ĺ�����һ�㴫��lsq_homog()����
    m���ں���xform_fn�м���任������Ҫ����С������Ը���
    p_badxform������Ĵ�����ʣ�������RANSAC�㷨������ı任�������ĸ��ʣ���ǰ�������ģ�͵Ĵ������С��p_badxformʱ����ֹͣ
    err_fn������ָ�룬���ڸ����ı任���󣬼����ƶ���ƥ����֮��ı任��һ�㴫��homog_xfer_err()����
    err_tol���ݴ�ȣ����ڸ����ı任�����ڴ˷�Χ�ڵĵ�Ա���Ϊ���ڵ�
    inliers�����������ָ�����飬ָ�����������յ��ڵ㼯�ϣ���Ϊ�գ���ʾû���������Ҫ���һ�¼���
    ��������ڴ潫�ڱ������б����䣬ʹ���������ڵ��ó��ͷţ�free(*inliers)
    n_in��������������ռ�������ڵ����Ŀ
    ����ֵ��RANSAC�㷨������ı任������Ϊ�գ���ʾ������޷�������ɽ��ܾ���
*/
/**
    Calculates a best-fit image transform from image feature correspondences
    using RANSAC.

    For more information refer to:

    Fischler, M. A. and Bolles, R. C.  Random sample consensus: a paradigm for
    model fitting with applications to image analysis and automated cartography.
    <EM>Communications of the ACM, 24</EM>, 6 (1981), pp. 381--395.

    @param features an array of features; only features with a non-NULL match
    of type \a mtype are used in homography computation
    @param n number of features in \a feat
    @param mtype determines which of each feature's match fields to use
    for transform computation; should be one of FEATURE_FWD_MATCH,
    FEATURE_BCK_MATCH, or FEATURE_MDL_MATCH; if this is FEATURE_MDL_MATCH,
    correspondences are assumed to be between a feature's img_pt field
    and its match's mdl_pt field, otherwise correspondences are assumed to
    be between the the feature's img_pt field and its match's img_pt field
    @param xform_fn pointer to the function used to compute the desired
    transformation from feature correspondences
    @param m minimum number of correspondences necessary to instantiate the
    transform computed by \a xform_fn
    @param p_badxform desired probability that the final transformation
    returned by RANSAC is corrupted by outliers (i.e. the probability that
    no samples of all inliers were drawn)
    @param err_fn pointer to the function used to compute a measure of error
    between putative correspondences and for a given transform
    @param err_tol correspondences within this distance of each other are
    considered as inliers for a given transform
    @param inliers if not NULL, output as an array of pointers to the final
    set of inliers; memory for this array is allocated by this function and
    must be freed by the caller using free(*inliers)
    @param n_in if not NULL, output as the final number of inliers

    @return Returns a transformation matrix computed using RANSAC or NULL
    on error or if an acceptable transform could not be computed.
*/
extern CvMat* ransac_xform(struct feature* features, int n, int mtype,
                           ransac_xform_fn xform_fn, int m,
                           double p_badxform, ransac_err_fn err_fn,
                           double err_tol, struct feature** * inliers,
                           int* n_in);


/*  �������Ա任�����е�ƥ�����ƽ�浥Ӧ��
    ������
    pts����ļ���
    mpts����ļ��ϣ�pts[i]��mpts[i]�ǻ����Ӧ��ƥ���
    n��pts��mpts�����е�ĸ�����pts��mpts�е�ĸ���������ͬ�����ҵ�Ը�������Ϊ4
    ����ֵ���任���󣬿ɽ�pts�еĵ�任Ϊmpts�еĵ㣬�����������4�򷵻ؿ�
*/
/**
    Calculates a planar homography from point correspondeces using the direct
    linear transform.  Intended for use as a ransac_xform_fn.

    @param pts array of points
    @param mpts array of corresponding points; each \a pts[\a i], \a i=0..\a
    n-1, corresponds to \a mpts[\a i]
    @param n number of points in both \a pts and \a mpts; must be at least 4

    @return Returns the \f$3 \times 3\f$ planar homography matrix that
    transforms points in \a pts to their corresponding points in \a mpts
    or NULL if fewer than 4 correspondences were provided
*/
extern CvMat* dlt_homog(CvPoint2D64f* pts, CvPoint2D64f* mpts, int n);


/*  ����4������������С����ƽ�浥Ӧ�Ա任����
    ������
    pts����ļ���
    mpts����ļ��ϣ�pts[i]��mpts[i]�ǻ����Ӧ��ƥ���
    n��pts��mpts�����е�ĸ�����pts��mpts�е�ĸ���������ͬ�����ҵ�Ը�������Ϊ4
    ����ֵ���任���󣬿ɽ�pts�еĵ�任Ϊmpts�еĵ㣬�����������4�򷵻ؿ�
*/
/**
    Calculates a least-squares planar homography from point correspondeces.
    Intended for use as a ransac_xform_fn.

    @param pts array of points
    @param mpts array of corresponding points; each \a pts[\a i], \a i=0..\a
    n-1, corresponds to \a mpts[\a i]
    @param n number of points in both \a pts and \a mpts; must be at least 4

    @return Returns the \f$3 \times 3\f$ least-squares planar homography
    matrix that transforms points in \a pts to their corresponding points
    in \a mpts or NULL if fewer than 4 correspondences were provided
*/
extern CvMat* lsq_homog(CvPoint2D64f* pts, CvPoint2D64f* mpts, int n);


/*  ���ڸ����ĵ�Ӧ�Ծ���H�����������pt��H�任��ĵ�����ƥ���mpt֮������
    ���磺������x�����Ӧ��x'����Ӧ�Ծ���H�������x'��Hx֮��ľ����ƽ����d(x', Hx)^2
    ������
    pt��һ����
    mpt��pt�Ķ�Ӧƥ���
    H����Ӧ�Ծ���
    ����ֵ��ת�����
*/
/**
    Calculates the transfer error between a point and its correspondence for
    a given homography, i.e. for a point \f$x\f$, it's correspondence \f$x'\f$,
    and homography \f$H\f$, computes \f$d(x', Hx)^2\f$.  Intended for use as
    a ransac_err_fn.

    @param pt a point
    @param mpt \a pt's correspondence
    @param H a homography matrix

    @return Returns the transfer error between \a pt and \a mpt given \a H
*/
extern double homog_xfer_err(CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* H);


/*  �����pt��͸�ӱ任��ĵ㣬������һ��pt��͸�ӱ任����T������任��ĵ�
    ������(x,y)���任����M������[x',y',w']^T = M * [x,y,1]^T(^T��ʾת��)��
    ��任��ĵ���(u,v) = (x'/w', y'/w')
    ע�⣺����任��͸�ӱ任������
    ������
    pt��һ����ά��
    T��͸�ӱ任����
    ����ֵ��pt��͸�ӱ任��ĵ�
*/
/**
    Performs a perspective transformation on a single point.  That is, for a
    point \f$(x, y)\f$ and a \f$3 \times 3\f$ matrix \f$T\f$ this function
    returns the point \f$(u, v)\f$, where<BR>

    \f$[x' \ y' \ w']^T = T \times [x \ y \ 1]^T\f$,<BR>

    and<BR>

    \f$(u, v) = (x'/w', y'/w')\f$.

    Note that affine transforms are a subset of perspective transforms.

    @param pt a 2D point
    @param T a perspective transformation matrix

    @return Returns the point \f$(u, v)\f$ as above.
*/
extern CvPoint2D64f persp_xform_pt(CvPoint2D64f pt, CvMat* T);





































/*
    ���ļ��ж����˴洢������Ľṹ��feature���Լ���������ԭ�͵�������
    1��������ĵ��뵼��
    2�����������
*/



/*  ����������ͣ�
    FEATURE_OXFD��ʾ��ţ���ѧVGG�ṩ��Դ���е��������ʽ��
    FEATURE_LOWE��ʾ��David.Lowe�ṩ��Դ���е��������ʽ
*/
/** FEATURE_OXFD <BR> FEATURE_LOWE */
enum feature_type
{
    FEATURE_OXFD,
    FEATURE_LOWE,
};

/*  ������ƥ�����ͣ�
    FEATURE_FWD_MATCH������feature�ṹ�е�fwd_match���Ƕ�Ӧ��ƥ���
    FEATURE_BCK_MATCH������feature�ṹ�е�bck_match���Ƕ�Ӧ��ƥ���
    FEATURE_MDL_MATCH������feature�ṹ�е�mdl_match���Ƕ�Ӧ��ƥ���
*/
/** FEATURE_FWD_MATCH <BR> FEATURE_BCK_MATCH <BR> FEATURE_MDL_MATCH */
enum feature_match_type
{
    FEATURE_FWD_MATCH,
    FEATURE_BCK_MATCH,
    FEATURE_MDL_MATCH,
};

/*���������������ɫ*/
/* colors in which to display different feature types */
#define FEATURE_OXFD_COLOR CV_RGB(255,255,0)
#define FEATURE_LOWE_COLOR CV_RGB(255,0,255)

/*������������ӳ��ȣ���Ϊ128*/
/** max feature descriptor length */
#define FEATURE_MAX_D 128

/*  ������ṹ��
    �˽ṹ��ɴ洢2�����͵������㣺
    FEATURE_OXFD��ʾ��ţ���ѧVGG�ṩ��Դ���е��������ʽ��
    FEATURE_LOWE��ʾ��David.Lowe�ṩ��Դ���е��������ʽ��
    �����OXFD���͵������㣬�ṹ���е�a,b,c��Ա��������������Χ�ķ�������(��Բ�Ĳ���)��������
    �����LOWE���͵������㣬�ṹ���е�scl��ori��Ա������������Ĵ�С�ͷ���
    fwd_match��bck_match��mdl_matchһ��ͬʱֻ��һ�������ã�����ָ�����������Ӧ��ƥ���
*/
/**
    Structure to represent an affine invariant image feature.  The fields
    x, y, a, b, c represent the affine region around the feature:

    a(x-u)(x-u) + 2b(x-u)(y-v) + c(y-v)(y-v) = 1
*/
struct feature
{
    double x;                      /**< x coord */ //�������x����
    double y;                      /**< y coord */ //�������y����
    double a;                      /**< Oxford-type affine region parameter */ //OXFD����������Բ�Ĳ���
    double b;                      /**< Oxford-type affine region parameter */ //OXFD����������Բ�Ĳ���
    double c;                      /**< Oxford-type affine region parameter */ //OXFD����������Բ�Ĳ���
    double scl;                    /**< scale of a Lowe-style feature *///LOWE������ĳ߶�
    double ori;                    /**< orientation of a Lowe-style feature */ //LOWE������ķ���
    int d;                         /**< descriptor length */ //���������ӵĳ��ȣ���ά����һ����128
    double descr[FEATURE_MAX_D];   /**< descriptor */ //128ά�����������ӣ���һ��double����
    int type;                      /**< feature type, OXFD or LOWE */ //����������
    int category;                  /**< all-purpose feature category */
    struct feature* fwd_match;     /**< matching feature from forward image */   //ָ�����������Ӧ��ƥ���
    struct feature* bck_match;     /**< matching feature from backmward image */ //ָ�����������Ӧ��ƥ���
    struct feature* mdl_match;     /**< matching feature from model */           //ָ�����������Ӧ��ƥ���
    CvPoint2D64f img_pt;           /**< location in image */ //�����������,����(x,y)
    CvPoint2D64f mdl_pt;           /**< location in model */ //��ƥ��������mdl_matchʱ�õ�
    void* feature_data;            /**< user-definable data */ //�û����������:
    //��SIFT��ֵ�����У���detection_data�ṹ��ָ��
    //��k-d�������У���bbf_data�ṹ��ָ��
    //��RANSAC�㷨�У���ransac_data�ṹ��ָ��
};


/*  ���ļ��ж���ͼ������
    �ļ��е��������ʽ������FEATURE_OXFD��FEATURE_LOWE��ʽ
    ������
    filename���ļ���
    type������������
    feat�������洢�������feature�����ָ��
    ����ֵ����������������
*/
/**
    Reads image features from file.  The file should be formatted either as
    from the code provided by the Visual Geometry Group at Oxford or from
    the code provided by David Lowe.


    @param filename location of a file containing image features
    @param type determines how features are input.  If \a type is FEATURE_OXFD,
    the input file is treated as if it is from the code provided by the VGG
    at Oxford: http://www.robots.ox.ac.uk:5000/~vgg/research/affine/index.html
    <BR><BR>
    If \a type is FEATURE_LOWE, the input file is treated as if it is from
    David Lowe's SIFT code: http://www.cs.ubc.ca/~lowe/keypoints
    @param feat pointer to an array in which to store imported features; memory
    for this array is allocated by this function and must be released by
    the caller using free(*feat)

    @return Returns the number of features imported from filename or -1 on error
*/
extern int import_features(char* filename, int type, struct feature** feat);


/*  ����feature���鵽�ļ�
    ������
    filename���ļ���
    feat����������
    n�����������
    ����ֵ��0���ɹ���1��ʧ��
*/
/**
    Exports a feature set to a file formatted depending on the type of
    features, as specified in the feature struct's type field.

    @param filename name of file to which to export features
    @param feat feature array
    @param n number of features

    @return Returns 0 on success or 1 on error
*/
extern int export_features(char* filename, struct feature* feat, int n);


/*  ��ͼƬ�ϻ���������
    ������
    img��ͼ��
    feat������������
    n�����������
*/
/**
    Displays a set of features on an image

    @param img image on which to display features
    @param feat array of Oxford-type features
    @param n number of features
*/
extern void draw_features(IplImage* img, struct feature* feat, int n);


/*  �����������������Ӽ��ŷ�Ͼ����ƽ��
    ������
    f1:��һ��������
    f2:�ڶ���������
    ����ֵ��ŷ�Ͼ����ƽ��
*/
/**
    Calculates the squared Euclidian distance between two feature descriptors.

    @param f1 first feature
    @param f2 second feature

    @return Returns the squared Euclidian distance between the descriptors of
    \a f1 and \a f2.
*/
extern double descr_dist_sq(struct feature* f1, struct feature* f2);





















/*
    ���ļ��а���K-D���Ľ�������������������
*/
/********************************* Structures ********************************/

struct feature;

/*K-D���еĽ��ṹ*/
/** a node in a k-d tree */
struct kd_node
{
    int ki;                      /**< partition key index */ //�ָ�λ��(����)��ά������(��һά�Ƿָ�λ��)��ȡֵΪ1-128
    double kv;                   /**< partition key value */  //�����ֵ(����������������������ά���ϵķ�������ֵ)
    int leaf;                    /**< 1 if node is a leaf, 0 otherwise */ //�Ƿ�Ҷ�ӽ��ı�־
    struct feature* features;    /**< features at this node */  //�˽���Ӧ�������㼯��(����)
    int n;                       /**< number of features */ //������ĸ���
    struct kd_node* kd_left;     /**< left child */  //������
    struct kd_node* kd_right;    /**< right child */  //������
};


/*************************** Function Prototypes *****************************/
/*  ���ݸ����������㼯�Ͻ���k-d��
    ������
    features�����������飬ע�⣺�˺�������ı�features������Ԫ�ص�����˳��
    n�����������
    ����ֵ�������õ�k-d��������ָ��
*/
/**
    A function to build a k-d tree database from keypoints in an array.

    @param features an array of features; <EM>this function rearranges the order
    of the features in this array, so you should take appropriate measures if
    you are relying on the order of the features (e.g. call this function
    before order is important)</EM>
    @param n the number of features in \a features

    @return Returns the root of a kd tree built from \a features.
*/
extern struct kd_node* kdtree_build(struct feature* features, int n);


/*  ��BBF�㷨��k-d���в���ָ���������k�������������
    ������
    kd_root��ͼ��������k-d��������
    feat��Ŀ��������
    k�����ڸ���
    nbrs��k�������������ָ�����飬����Ŀ��������ľ�����������
    ��������ڴ潫�ڱ������б����䣬ʹ���������ڵ��ó��ͷţ�free(*nbrs)
    max_nn_chks����������������������ֵ��������
    ����ֵ���洢��nbrs�еĽ��ڸ���������-1��ʾʧ��
*/
/**
    Finds an image feature's approximate k nearest neighbors in a kd tree using
    Best Bin First search.

    @param kd_root root of an image feature kd tree
    @param feat image feature for whose neighbors to search
    @param k number of neighbors to find
    @param nbrs pointer to an array in which to store pointers to neighbors
    in order of increasing descriptor distance; memory for this array is
    allocated by this function and must be freed by the caller using
    free(*nbrs)
    @param max_nn_chks search is cut off after examining this many tree entries

    @return Returns the number of neighbors found and stored in \a nbrs, or
    -1 on error.
*/
extern int kdtree_bbf_knn(struct kd_node* kd_root, struct feature* feat,
                          int k, struct feature** * nbrs, int max_nn_chks);


/**
    Finds an image feature's approximate k nearest neighbors within a specified
    spatial region in a kd tree using Best Bin First search.

    @param kd_root root of an image feature kd tree
    @param feat image feature for whose neighbors to search
    @param k number of neighbors to find
    @param nbrs pointer to an array in which to store pointers to neighbors
    in order of increasing descriptor distance; memory for this array is
    allocated by this function and must be freed by the caller using
    free(*nbrs)
    @param max_nn_chks search is cut off after examining this many tree entries
    @param rect rectangular region in which to search for neighbors
    @param model if true, spatial search is based on kdtree features' model
    locations; otherwise it is based on their image locations

    @return Returns the number of neighbors found and stored in \a nbrs
    (in case \a k neighbors could not be found before examining
    \a max_nn_checks keypoint entries).
*/
extern int kdtree_bbf_spatial_knn(struct kd_node* kd_root,
                                  struct feature* feat, int k,
                                  struct feature** * nbrs, int max_nn_chks,
                                  CvRect rect, int model);


/*  �ͷ�k-d��ռ�õĴ洢�ռ�
*/
/**
    De-allocates memory held by a kd tree

    @param kd_root pointer to the root of a kd tree
*/
extern void kdtree_release(struct kd_node* kd_root);





//=================================================
#define GETOPT_H_

#ifdef __cplusplus
extern "C" {
#endif

extern char* optarg;
extern int optind;
extern int opterr;
extern int optopt;

int getopt(int argc, char** argv, char* optstr);

#ifdef __cplusplus
}
#endif

#endif
