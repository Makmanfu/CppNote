/*
    Miscellaneous utility functions.

    Copyright (C) 2006-2012  Rob Hess <rob@iqengines.com>

    @version 1.1.2-20100521
    /*
    此文件中实现了图像的基本操作函数:
    1、获取某位置的像素点
    2、设置某位置的像素点（8位，32位和64位），
    3、计算两点之间的距离的平方
    4、在图片某一点画一个“X”
    5、将两张图片合成为一个，高是二者之和，宽是二者的较大者。img1 在左上角，img2在右下角。
*/
#include "stdafx.h"
#include "utilsfeatures.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

//#include <gdk/gdk.h>
//#include <gtk/gtk.h>

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include <math.h>
#include <time.h>
/*************************** Function Definitions ****************************/


/*
    Prints an error message and aborts the program.  The error message is
    of the form "Error: ...", where the ... is specified by the \a format
    argument

    @param format an error message format string (as with \c printf(3)).
*/
void fatal_error(char* format, ...)
{
    va_list ap;
    fprintf(stderr, "Error: ");
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    abort();
}



/*
    Replaces a file's extension, which is assumed to be everything after the
    last dot ('.') character.

    @param file the name of a file

    @param extn a new extension for \a file; should not include a dot (i.e.
    \c "jpg", not \c ".jpg") unless the new file extension should contain
    two dots.

    @return Returns a new string formed as described above.  If \a file does
    not have an extension, this function simply adds one.
*/
char* replace_extension(const char* file, const char* extn)
{
    char* new_file, *lastdot;
    new_file = (char*) calloc(strlen(file) + strlen(extn) + 2, sizeof(char));
    strcpy(new_file, file);
    lastdot = strrchr(new_file, '.');
    if (lastdot)
        * (lastdot + 1) = '\0';
    else
        strcat(new_file, ".");
    strcat(new_file, extn);
    return new_file;
}



/*
    Prepends a path to a filename.

    @param path a path
    @param file a file name

    @return Returns a new string containing a full path name consisting of
    \a path prepended to \a file.
*/
char* prepend_path(const char* path, const char* file)
{
    int n = strlen(path) + strlen(file) + 2;
    char* pathname = (char*) calloc(n, sizeof(char));
    //windows _snprintf
    _snprintf(pathname, n, "%s/%s", path, file);
    return pathname;
}



/*
    A function that removes the path from a filename.  Similar to the Unix
    basename command.

    @param pathname a (full) path name

    @return Returns the basename of \a pathname.
*/
char* basename(const char* pathname)
{
    char* base, *last_slash;
    last_slash = (char*)(strrchr(pathname, '/'));          //const_cast<char*>
    if (!last_slash)
    {
        base = (char*) calloc(strlen(pathname) + 1, sizeof(char));
        strcpy(base, pathname);
    }
    else
    {
        base = (char*) calloc(strlen(last_slash++), sizeof(char));
        strcpy(base, last_slash);
    }
    return base;
}



/*
    Displays progress in the console with a spinning pinwheel.  Every time this
    function is called, the state of the pinwheel is incremented.  The pinwheel
    has four states that loop indefinitely: '|', '/', '-', '\'.

    @param done if 0, this function simply increments the state of the pinwheel;
    otherwise it prints "done"
*/
void progress(int done)
{
    char state[4] = { '|', '/', '-', '\\' };
    static int cur = -1;
    if (cur == -1)
        fprintf(stderr, "  ");
    if (done)
    {
        fprintf(stderr, "\b\bdone\n");
        cur = -1;
    }
    else
    {
        cur = (cur + 1) % 4;
        fprintf(stdout, "\b\b%c ", state[cur]);
        fflush(stderr);
    }
}



/*
    Erases a specified number of characters from a stream.

    @param stream the stream from which to erase characters
    @param n the number of characters to erase
*/
void erase_from_stream(FILE* stream, int n)
{
    int j;
    for (j = 0; j < n; j++)
        fprintf(stream, "\b");
    for (j = 0; j < n; j++)
        fprintf(stream, " ");
    for (j = 0; j < n; j++)
        fprintf(stream, "\b");
}



/*
    Doubles the size of an array with error checking

    @param array pointer to an array whose size is to be doubled
    @param n number of elements allocated for \a array
    @param size size in bytes of elements in \a array

    @return Returns the new number of elements allocated for \a array.  If no
    memory is available, returns 0.
*/
int array_double(void** array, int n, int size)
{
    void* tmp;
    tmp = realloc(*array, 2 * n * size);
    if (!tmp)
    {
        fprintf(stderr, "Warning: unable to allocate memory in array_double(),"
                " %s line %d\n", __FILE__, __LINE__);
        if (*array)
            free(*array);
        *array = NULL;
        return 0;
    }
    *array = tmp;
    return n * 2;
}



/*
    Calculates the squared distance between two points.

    @param p1 a point
    @param p2 another point
*/
double dist_sq_2D(CvPoint2D64f p1, CvPoint2D64f p2)
{
    double x_diff = p1.x - p2.x;
    double y_diff = p1.y - p2.y;
    return x_diff * x_diff + y_diff * y_diff;
}



/*
    Draws an x on an image.

    @param img an image
    @param pt the center point of the x
    @param r the x's radius
    @param w the x's line weight
    @param color the color of the x
*/
void draw_x(IplImage* img, CvPoint pt, int r, int w, CvScalar color)
{
    cvLine(img, pt, cvPoint(pt.x + r, pt.y + r), color, w, 8, 0);
    cvLine(img, pt, cvPoint(pt.x - r, pt.y + r), color, w, 8, 0);
    cvLine(img, pt, cvPoint(pt.x + r, pt.y - r), color, w, 8, 0);
    cvLine(img, pt, cvPoint(pt.x - r, pt.y - r), color, w, 8, 0);
}


/*  将两张图像合成为一张，垂直排放
    参数：img1：位于上方的图像的指针，img2：位于下方的图像的指针
    返回值：合成图像
*/
/*
    Combines two images by scacking one on top of the other

    @param img1 top image
    @param img2 bottom image

    @return Returns the image resulting from stacking \a img1 on top if \a img2
*/
extern IplImage* stack_imgs(IplImage* img1, IplImage* img2)
{
    //生成合成图像
    //IplImage* stacked = cvCreateImage(cvSize(MAX(img1->width, img2->width),
    //    img1->height + img2->height),
    //    IPL_DEPTH_8U, 3);
    //cvZero(stacked);
    //cvSetImageROI(stacked, cvRect(0, 0, img1->width, img1->height));
    //cvAdd(img1, stacked, stacked, NULL);
    ////竖着
    //cvSetImageROI(stacked, cvRect(0, img1->height, img2->width, img2->height));
    //cvAdd(img2, stacked, stacked, NULL);
    //cvResetImageROI(stacked);
    //return stacked;
    //横着
    IplImage* stacked = cvCreateImage(cvSize(img1->width + img2->width,
                                      MAX(img1->height, img2->height)),
                                      IPL_DEPTH_8U, 3);
    cvZero(stacked);
    cvSetImageROI(stacked, cvRect(0, 0, img1->width, img1->height));
    cvAdd(img1, stacked, stacked, NULL);
    //横着
    cvSetImageROI(stacked, cvRect(img1->width, 0, img2->width, img2->height));
    cvAdd(img2, stacked, stacked, NULL);
    cvResetImageROI(stacked);
    return stacked;
}

/*  (自己写的函数)
    将两张图像合成为一张，水平排放
    参数：img1：位于左边的图像的指针，img2：位于右边的图像的指针
    返回值：合成图像
*/
extern IplImage* stack_imgs_horizontal(IplImage* img1, IplImage* img2)
{
    //生成合成图像
    IplImage* stacked = cvCreateImage(cvSize(img1->width + img2->width, MAX(img1->height, img2->height)),
                                      IPL_DEPTH_8U, 3);
    cvZero(stacked);    //清零
    cvSetImageROI(stacked, cvRect(0, 0, img1->width, img1->height));
    cvAdd(img1, stacked, stacked, NULL);    //叠加第一张图像
    cvSetImageROI(stacked, cvRect(img1->width, 0, img2->width, img2->height));
    cvAdd(img2, stacked, stacked, NULL);    //叠加第二张图像
    cvResetImageROI(stacked);
    return stacked;
}


/*
    Displays an image, making sure it fits on screen.  cvWaitKey() must be
    called after this function so the event loop is entered and the
    image is displayed.

    @param img an image, possibly too large to display on-screen
    @param title the title of the window in which \a img is displayed
*/
void display_big_img(IplImage* img, char* title)
{
    //?????????????
    IplImage* smallipl;
    int scr_width, scr_height;
    double img_aspect, scr_aspect, scale;
    scr_width = GetSystemMetrics(SM_CXSCREEN);
    scr_height = GetSystemMetrics(SM_CYSCREEN);
    if (img->width >= 0.90 * scr_width || img->height >= 0.90 * scr_height)
    {
        img_aspect = (double)(img->width) / img->height;
        scr_aspect = (double)(scr_width) / scr_height;
        if (img_aspect > scr_aspect)
            scale = 0.90 * scr_width / img->width;
        else
            scale = 0.90 * scr_height / img->height;
        smallipl = cvCreateImage(cvSize(img->width * scale, img->height * scale),
                                 img->depth, img->nChannels);
        cvResize(img, smallipl, CV_INTER_AREA);
    }
    else
        smallipl = cvCloneImage(img);
    cvNamedWindow(title, 1);
    cvShowImage(title, smallipl);
    cvReleaseImage(&smallipl);
}


void display_big_img2(IplImage* img, char* title)
{
    cvNamedWindow(title, 1);
    cvShowImage(title, img);
}


/*
    Allows user to view an array of images as a video.  Keyboard controls
    are as follows:

    <ul>
    <li>Space - start and pause playback</li>
    <li>Page Down - skip forward 10 frames</li>
    <li>Page Up - jump back 10 frames</li>
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
void vid_view(IplImage** imgs, int n, char* win_name)
{
    int k, i = 0, playing = 0;
    display_big_img(imgs[i], win_name);
    while (!win_closed(win_name))
    {
        /* if already playing, advance frame and check for pause */
        if (playing)
        {
            i = MIN(i + 1, n - 1);
            display_big_img(imgs[i], win_name);
            k = cvWaitKey(33);
            if (k == ' ' || i == n - 1)
                playing = 0;
        }
        else
        {
            k = cvWaitKey(0);
            switch (k)
            {
                /* space */
                case ' ':
                    playing = 1;
                    break;
                /* esc */
                case 27:
                case 1048603:
                    cvDestroyWindow(win_name);
                    break;
                /* backspace */
                case '\b':
                    i = 0;
                    display_big_img(imgs[i], win_name);
                    break;
                /* left arrow */
                case 65288:
                case 1113937:
                    i = MAX(i - 1, 0);
                    display_big_img(imgs[i], win_name);
                    break;
                /* right arrow */
                case 65363:
                case 1113939:
                    i = MIN(i + 1, n - 1);
                    display_big_img(imgs[i], win_name);
                    break;
                /* page up */
                case 65365:
                case 1113941:
                    i = MAX(i - 10, 0);
                    display_big_img(imgs[i], win_name);
                    break;
                /* page down */
                case 65366:
                case 1113942:
                    i = MIN(i + 10, n - 1);
                    display_big_img(imgs[i], win_name);
                    break;
            }
        }
    }
}



/*
    Checks if a HighGUI window is still open or not

    @param name the name of the window we're checking

    @return Returns 1 if the window named \a name has been closed or 0 otherwise
*/
int win_closed(char* win_name)
{
    if (!cvGetWindowHandle(win_name))
        return 1;
    return 0;
}



//==================================================================
/*
    此文件中包括最小优先队列的实现
*/

//本地函数
/************************* Local Function Prototypes *************************/

static void restore_minpq_order(struct pq_node*, int, int);
static void decrease_pq_node_key(struct pq_node*, int, int);


//内联函数
/************************** Local Inline Functions ***************************/

//返回i节点的父节点的下标
/* returns the array index of element i's parent */
static inline int parent(int i)
{
    return (i - 1) / 2;
}

//返回i节点的右孩子的下标
/* returns the array index of element i's right child */
static inline int right(int i)
{
    return 2 * i + 2;
}

//返回i节点的左孩子的下标
/* returns the array index of element i's left child */
static inline int left(int i)
{
    return 2 * i + 1;
}


/********************** Functions prototyped in minpq.h **********************/

/*  初始化最小优先级队列，分配默认大小的空间
*/
/*
    Creates a new minimizing priority queue.
*/
struct min_pq* minpq_init()
{
    struct min_pq* min_pq;
    min_pq = (struct min_pq*) malloc(sizeof(struct min_pq));
    //默认分配MINPQ_INIT_NALLOCD个空间
    min_pq->pq_array = (struct pq_node*) calloc(MINPQ_INIT_NALLOCD, sizeof(struct pq_node));
    min_pq->nallocd = MINPQ_INIT_NALLOCD;
    min_pq->n = 0;
    return min_pq;
}



/**
    Inserts an element into a minimizing priority queue.

    @param min_pq a minimizing priority queue
    @param data the data to be inserted
    @param key the key to be associated with \a data

    @return Returns 0 on success or 1 on failure.
*/
int minpq_insert(struct min_pq* min_pq, void* data, int key)
{
    int n = min_pq->n;
    /* double array allocation if necessary */
    if (min_pq->nallocd == n)
    {
        min_pq->nallocd = array_double((void**) &min_pq->pq_array, min_pq->nallocd,
                                       sizeof(struct pq_node));
        if (!min_pq->nallocd)
        {
            fprintf(stderr, "Warning: unable to allocate memory, %s, line %d\n",
                    __FILE__, __LINE__);
            return 1;
        }
    }
    min_pq->pq_array[n].data = data;
    min_pq->pq_array[n].key = INT_MAX;
    decrease_pq_node_key(min_pq->pq_array, min_pq->n, key);
    min_pq->n++;
    return 0;
}



/*
    Returns the element of a minimizing priority queue with the smallest key
    without removing it from the queue.

    @param min_pq a minimizing priority queue

    @return Returns the element of \a min_pq with the smallest key or NULL
    if \a min_pq is empty
*/
void* minpq_get_min(struct min_pq* min_pq)
{
    if (min_pq->n < 1)
    {
        fprintf(stderr, "Warning: PQ empty, %s line %d\n", __FILE__, __LINE__);
        return NULL;
    }
    return min_pq->pq_array[0].data;
}



/*
    Removes and returns the element of a minimizing priority queue with the
    smallest key.

    @param min_pq a minimizing priority queue

    @return Returns the element of \a min_pq with the smallest key of NULL
    if \a min_pq is empty
*/
void* minpq_extract_min(struct min_pq* min_pq)
{
    void* data;
    if (min_pq->n < 1)
    {
        fprintf(stderr, "Warning: PQ empty, %s line %d\n", __FILE__, __LINE__);
        return NULL;
    }
    data = min_pq->pq_array[0].data;
    min_pq->n--;
    min_pq->pq_array[0] = min_pq->pq_array[min_pq->n];
    restore_minpq_order(min_pq->pq_array, 0, min_pq->n);
    return data;
}


/*
    De-allocates the memory held by a minimizing priorioty queue

    @param min_pq pointer to a minimizing priority queue
*/
void minpq_release(struct min_pq** min_pq)
{
    if (!min_pq)
    {
        fprintf(stderr, "Warning: NULL pointer error, %s line %d\n", __FILE__,
                __LINE__);
        return;
    }
    if (*min_pq && (*min_pq)->pq_array)
    {
        free((*min_pq)->pq_array);
        free(*min_pq);
        *min_pq = NULL;
    }
}


/************************ Functions prototyped here **************************/

/*
    Decrease a minimizing pq element's key, rearranging the pq if necessary

    @param pq_array minimizing priority queue array
    @param i index of the element whose key is to be decreased
    @param key new value of element <EM>i</EM>'s key; if greater than current
    key, no action is taken
*/
static void decrease_pq_node_key(struct pq_node* pq_array, int i, int key)
{
    struct pq_node tmp;
    if (key > pq_array[i].key)
        return;
    pq_array[i].key = key;
    while (i > 0 && pq_array[i].key < pq_array[parent(i)].key)
    {
        tmp = pq_array[parent(i)];
        pq_array[parent(i)] = pq_array[i];
        pq_array[i] = tmp;
        i = parent(i);
    }
}



/*
    Recursively restores correct priority queue order to a minimizing pq array

    @param pq_array a minimizing priority queue array
    @param i index at which to start reordering
    @param n number of elements in \a pq_array
*/
static void restore_minpq_order(struct pq_node* pq_array, int i, int n)
{
    struct pq_node tmp;
    int l, r, min = i;
    l = left(i);
    r = right(i);
    if (l < n)
        if (pq_array[l].key < pq_array[i].key)
            min = l;
    if (r < n)
        if (pq_array[r].key < pq_array[min].key)
            min = r;
    if (min != i)
    {
        tmp = pq_array[min];
        pq_array[min] = pq_array[i];
        pq_array[i] = tmp;
        restore_minpq_order(pq_array, min, n);
    }
}















//=========================================================
/*
    此文件中包括的函数实现有：
    1、RANSAC算法特征匹配筛选
    2、其他一些与RANSAC算法相关函数
*/

/************************ 未暴露接口的一些本地函数的声明 **************************/
/************************* Local Function Prototypes *************************/

//根据给定的匹配类型，返回输入特征点feat的匹配点
static inline struct feature* get_match(struct feature*, int);
//找到所有具有mtype类型匹配点的特征点，将他们的指针存储在matched数组中
static int get_matched_features(struct feature*, int, int, struct feature***);
//计算保证RANSAC最终计算出的转换矩阵错误的概率小于p_badxform所需的最小内点数目
static int calc_min_inliers(int, int, double, double);
//计算n的阶乘的自然对数
static inline double log_factorial(int);
//从给定的特征点集中随机抽选一个RANSAC样本(即一个4个特征点的数组)
static struct feature** draw_ransac_sample(struct feature**, int, int);
//从特征点数组中获取特征点和其对应匹配点的二维坐标，分别放到输出参数pts和mpts中
static void extract_corresp_pts(struct feature**, int, int, CvPoint2D64f**, CvPoint2D64f**);
//对于给定的模型和错误度量函数，从特征点集和中找出一致集
static int find_consensus(struct feature**, int, int, CvMat*, ransac_err_fn, double, struct feature***);
//释放输入参数的存储空间
static inline void release_mem(CvPoint2D64f*, CvPoint2D64f*, struct feature**);

/********************** Functions prototyped in model.h **********************/


/*  利用RANSAC算法进行特征点筛选，计算出最佳匹配的变换矩阵
    参数：
    features：特征点数组，只有当mtype类型的匹配点存在时才被用来进行单应性计算
    n：特征点个数
    mtype：决定使用每个特征点的哪个匹配域进行变换矩阵的计算，应该是FEATURE_MDL_MATCH，
    FEATURE_BCK_MATCH，FEATURE_MDL_MATCH中的一个。若是FEATURE_MDL_MATCH，
    对应的匹配点对坐标是每个特征点的img_pt域和其匹配点的mdl_pt域，
    否则，对应的匹配点对是每个特征点的img_pt域和其匹配点的img_pt域。
    xform_fn：函数指针，指向根据输入的点对进行变换矩阵计算的函数，一般传入lsq_homog()函数
    m：在函数xform_fn中计算变换矩阵需要的最小特征点对个数
    p_badxform：允许的错误概率，即允许RANSAC算法计算出的变换矩阵错误的概率，当前计算出的模型的错误概率小于p_badxform时迭代停止
    err_fn：函数指针，对于给定的变换矩阵，计算推定的匹配点对之间的变换误差，一般传入homog_xfer_err()函数
    err_tol：容错度，对于给定的变换矩阵，在此范围内的点对被认为是内点
    inliers：输出参数，指针数组，指向计算出的最终的内点集合，若为空，表示没计算出符合要求的一致集
    此数组的内存将在本函数中被分配，使用完后必须在调用出释放：free(*inliers)
    n_in：输出参数，最终计算出的内点的数目
    返回值：RANSAC算法计算出的变换矩阵，若为空，表示出错或无法计算出可接受矩阵
*/
/*
    Calculates a best-fit image transform from image feature correspondences
    using RANSAC.

    For more information refer to:

    Fischler, M. A. and Bolles, R. C.  Random sample consensus: a paradigm for
    model fitting with applications to image analysis and automated cartography.
    <EM>Communications of the ACM, 24</EM>, 6 (1981), pp. 381--395.

    @param features an array of features; only features with a non-NULL match
    of type mtype are used in homography computation
    @param n number of features in feat
    @param mtype determines which of each feature's match fields to use
    for model computation; should be one of FEATURE_FWD_MATCH,
    FEATURE_BCK_MATCH, or FEATURE_MDL_MATCH; if this is FEATURE_MDL_MATCH,
    correspondences are assumed to be between a feature's img_pt field
    and its match's mdl_pt field, otherwise correspondences are assumed to
    be between the the feature's img_pt field and its match's img_pt field
    @param xform_fn pointer to the function used to compute the desired
    transformation from feature correspondences
    @param m minimum number of correspondences necessary to instantiate the
    model computed by xform_fn
    @param p_badxform desired probability that the final transformation
    returned by RANSAC is corrupted by outliers (i.e. the probability that
    no samples of all inliers were drawn)
    @param err_fn pointer to the function used to compute a measure of error
    between putative correspondences and a computed model
    @param err_tol correspondences within this distance of a computed model are
    considered as inliers
    @param inliers if not NULL, output as an array of pointers to the final
    set of inliers
    @param n_in if not NULL and \a inliers is not NULL, output as the final
    number of inliers

    @return Returns a transformation matrix computed using RANSAC or NULL
    on error or if an acceptable transform could not be computed.
*/
CvMat* ransac_xform(struct feature* features, int n, int mtype,
                    ransac_xform_fn xform_fn, int m, double p_badxform,
                    ransac_err_fn err_fn, double err_tol,
                    struct feature** * inliers, int* n_in)
{
    //matched：所有具有mtype类型匹配点的特征点的数组，也就是样本集
    //sample：单个样本，即4个特征点的数组
    //consensus：当前一致集；
    //consensus_max：当前最大一致集(即当前的最好结果的一致集)
    struct feature** matched, ** sample, ** consensus, ** consensus_max = NULL;
    struct ransac_data* rdata;//每个特征点的feature_data域的ransac数据指针
    CvPoint2D64f* pts, *mpts;//每个样本对应的两个坐标数组：特征点坐标数组pts和匹配点坐标数组mpts
    CvMat* M = NULL;//当前变换矩阵
    //p：当前计算出的模型的错误概率，当p小于p_badxform时迭代停止
    //in_frac：内点数目占样本总数目的百分比
    double p, in_frac = RANSAC_INLIER_FRAC_EST;
    //nm：输入的特征点数组中具有mtype类型匹配点的特征点个数
    //in：当前一致集中元素个数
    //in_min：一致集中元素个数允许的最小值，保证RANSAC最终计算出的转换矩阵错误的概率小于p_badxform所需的最小内点数目
    //in_max：当前最优一致集(最大一致集)中元素的个数
    //k：迭代次数，与计算当前模型的错误概率有关
    int i, nm, in, in_min, in_max = 0, k = 0;
    //找到特征点数组features中所有具有mtype类型匹配点的特征点，放到matched数组(样本集)中，返回值nm是matched数组的元素个数
    nm = get_matched_features(features, n, mtype, &matched);
    //若找到的具有匹配点的特征点个数小于计算变换矩阵需要的最小特征点对个数，出错
    if (nm < m)
    {
        //出错处理，特征点对个数不足
        fprintf(stderr, "Warning: not enough matches to compute xform, %s" \
                " line %d\n", __FILE__, __LINE__);
        goto end;
    }
    /* initialize random number generator */
    srand(time(NULL));       //初始化随机数生成器  linux srandom
    //计算保证RANSAC最终计算出的转换矩阵错误的概率小于p_badxform所需的最小内点数目
    in_min = calc_min_inliers(nm, m, RANSAC_PROB_BAD_SUPP, p_badxform);
    //当前计算出的模型的错误概率,内点所占比例in_frac越大，错误概率越小；迭代次数k越大，错误概率越小
    p = pow(1.0 - pow(in_frac, m), k);
    i = 0;
    //当前错误概率大于输入的允许错误概率p_badxform，继续迭代
    while (p > p_badxform)
    {
        //从样本集matched中随机抽选一个RANSAC样本(即一个4个特征点的数组)，放到样本变量sample中
        sample = draw_ransac_sample(matched, nm, m);
        //从样本中获取特征点和其对应匹配点的二维坐标，分别放到输出参数pts和mpts中
        extract_corresp_pts(sample, m, mtype, &pts, &mpts);
        //调用参数中传入的函数xform_fn，计算将m个点的数组pts变换为mpts的矩阵，返回变换矩阵给M
        M = xform_fn(pts, mpts, m);    //一般传入lsq_homog()函数
        if (!M)   //出错判断
            goto iteration_end;
        //给定特征点集，变换矩阵，误差函数，计算出当前一致集consensus，返回一致集中元素个数给in
        in = find_consensus(matched, nm, mtype, M, err_fn, err_tol, &consensus);
        //若当前一致集大于历史最优一致集，即当前一致集为最优，则更新最优一致集consensus_max
        if (in > in_max)
        {
            if (consensus_max)   //若之前有最优值，释放其空间
                free(consensus_max);
            consensus_max = consensus;//更新最优一致集
            in_max = in;//更新最优一致集中元素个数
            in_frac = (double) in_max / nm;   //最优一致集中元素个数占样本总个数的百分比
        }
        else//若当前一致集小于历史最优一致集，释放当前一致集
            free(consensus);
        cvReleaseMat(&M);
iteration_end:
        release_mem(pts, mpts, sample);
        p = pow(1.0 - pow(in_frac, m), ++k);       //更新当前错误概率
    }
    //根据最优一致集计算最终的变换矩阵
    /* calculate final transform based on best consensus set */
    //若最优一致集中元素个数大于最低标准，即符合要求
    if (in_max >= in_min)
    {
        //从最优一致集中获取特征点和其对应匹配点的二维坐标，分别放到输出参数pts和mpts中
        extract_corresp_pts(consensus_max, in_max, mtype, &pts, &mpts);
        //调用参数中传入的函数xform_fn，计算将in_max个点的数组pts变换为mpts的矩阵，返回变换矩阵给M
        M = xform_fn(pts, mpts, in_max);
        /***********下面会再进行一次迭代**********/
        //根据变换矩阵M从样本集matched中计算出一致集consensus，返回一致集中元素个数给in
        in = find_consensus(matched, nm, mtype, M, err_fn, err_tol, &consensus);
        cvReleaseMat(&M);
        release_mem(pts, mpts, consensus_max);
        //从一致集中获取特征点和其对应匹配点的二维坐标，分别放到输出参数pts和mpts中
        extract_corresp_pts(consensus, in, mtype, &pts, &mpts);
        //调用参数中传入的函数xform_fn，计算将in个点的数组pts变换为mpts的矩阵，返回变换矩阵给M
        M = xform_fn(pts, mpts, in);
        if (inliers)
        {
            *inliers = consensus;//将最优一致集赋值给输出参数：inliers，即内点集合
            consensus = NULL;
        }
        if (n_in)
            *n_in = in;//将最优一致集中元素个数赋值给输出参数：n_in，即内点个数
        release_mem(pts, mpts, consensus);
    }
    else if (consensus_max)
    {
        //没有计算出符合要求的一致集
        if (inliers)
            *inliers = NULL;
        if (n_in)
            *n_in = 0;
        free(consensus_max);
    }
    //RANSAC算法结束：恢复特征点中被更改的数据域feature_data，并返回变换矩阵M
end:
    for (i = 0; i < nm; i++)
    {
        //利用宏feat_ransac_data来提取matched[i]中的feature_data成员并转换为ransac_data格式的指针
        rdata = feat_ransac_data(matched[i]);
        //恢复feature_data成员的以前的值
        matched[i]->feature_data = rdata->orig_feat_data;
        free(rdata);    //释放内存
    }
    free(matched);
    return M;//返回求出的变换矩阵M
}



/*
    Calculates a planar homography from point correspondeces using the direct
    linear transform.  Intended for use as a ransac_xform_fn.

    @param pts array of points
    @param mpts array of corresponding points; each pts[i], i=0..n-1,
    corresponds to mpts[i]
    @param n number of points in both pts and mpts; must be at least 4

    @return Returns the 3x3 planar homography matrix that transforms points
    in pts to their corresponding points in mpts or NULL if fewer than 4
    correspondences were provided
*/
CvMat* dlt_homog(CvPoint2D64f* pts, CvPoint2D64f* mpts, int n)
{
    CvMat* H, *A, *VT, *D, h, v9;
    double _h[9];
    int i;
    if (n < 4)
        return NULL;
    /* set up matrices so we can unstack homography into h; Ah = 0 */
    A = cvCreateMat(2 * n, 9, CV_64FC1);
    cvZero(A);
    for (i = 0; i < n; i++)
    {
        cvmSet(A, 2 * i, 3, -pts[i].x);
        cvmSet(A, 2 * i, 4, -pts[i].y);
        cvmSet(A, 2 * i, 5, -1.0);
        cvmSet(A, 2 * i, 6, mpts[i].y * pts[i].x);
        cvmSet(A, 2 * i, 7, mpts[i].y * pts[i].y);
        cvmSet(A, 2 * i, 8, mpts[i].y);
        cvmSet(A, 2 * i + 1, 0, pts[i].x);
        cvmSet(A, 2 * i + 1, 1, pts[i].y);
        cvmSet(A, 2 * i + 1, 2, 1.0);
        cvmSet(A, 2 * i + 1, 6, -mpts[i].x * pts[i].x);
        cvmSet(A, 2 * i + 1, 7, -mpts[i].x * pts[i].y);
        cvmSet(A, 2 * i + 1, 8, -mpts[i].x);
    }
    D = cvCreateMat(9, 9, CV_64FC1);
    VT = cvCreateMat(9, 9, CV_64FC1);
    cvSVD(A, D, NULL, VT, CV_SVD_MODIFY_A + CV_SVD_V_T);
    v9 = cvMat(1, 9, CV_64FC1, NULL);
    cvGetRow(VT, &v9, 8);
    h = cvMat(1, 9, CV_64FC1, _h);
    cvCopy(&v9, &h, NULL);
    h = cvMat(3, 3, CV_64FC1, _h);
    H = cvCreateMat(3, 3, CV_64FC1);
    cvConvert(&h, H);
    cvReleaseMat(&A);
    cvReleaseMat(&D);
    cvReleaseMat(&VT);
    return H;
}


/*  根据4对坐标点计算最小二乘平面单应性变换矩阵
    参数：
    pts：坐标点数组
    mpts：对应点数组，pts[i]与mpts[i]一一对应
    n：pts和mpts数组中点的个数，pts和mpts中点的个数必须相同，一般是4
    返回值：一个3*3的变换矩阵，将pts中的每一个点转换为mpts中的对应点，返回值为空表示失败
*/
/*
    Calculates a least-squares planar homography from point correspondeces.

    @param pts array of points
    @param mpts array of corresponding points; each pts[i], i=1..n, corresponds
    to mpts[i]
    @param n number of points in both pts and mpts; must be at least 4

    @return Returns the 3 x 3 least-squares planar homography matrix that
    transforms points in pts to their corresponding points in mpts or NULL if
    fewer than 4 correspondences were provided
*/
CvMat* lsq_homog(CvPoint2D64f* pts, CvPoint2D64f* mpts, int n)
{
    CvMat* H, *A, *B, X;
    double x[9];//数组x中的元素就是变换矩阵H中的值
    int i;
    //输入点对个数不够4
    if (n < 4)
    {
        fprintf(stderr, "Warning: too few points in lsq_homog(), %s line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }
    //将变换矩阵H展开到一个8维列向量X中，使得AX=B，这样只需一次解线性方程组即可求出X，然后再根据X恢复H
    /* set up matrices so we can unstack homography into X; AX = B */
    A = cvCreateMat(2 * n, 8, CV_64FC1);    //创建2n*8的矩阵，一般是8*8
    B = cvCreateMat(2 * n, 1, CV_64FC1);    //创建2n*1的矩阵，一般是8*1
    X = cvMat(8, 1, CV_64FC1, x);    //创建8*1的矩阵，指定数据为x
    H = cvCreateMat(3, 3, CV_64FC1);    //创建3*3的矩阵
    cvZero(A);    //将A清零
    //由于是展开计算，需要根据原来的矩阵计算法则重新分配矩阵A和B的值的排列
    for (i = 0; i < n; i++)
    {
        cvmSet(A, i, 0, pts[i].x);    //设置矩阵A的i行0列的值为pts[i].x
        cvmSet(A, i + n, 3, pts[i].x);
        cvmSet(A, i, 1, pts[i].y);
        cvmSet(A, i + n, 4, pts[i].y);
        cvmSet(A, i, 2, 1.0);
        cvmSet(A, i + n, 5, 1.0);
        cvmSet(A, i, 6, -pts[i].x * mpts[i].x);
        cvmSet(A, i, 7, -pts[i].y * mpts[i].x);
        cvmSet(A, i + n, 6, -pts[i].x * mpts[i].y);
        cvmSet(A, i + n, 7, -pts[i].y * mpts[i].y);
        cvmSet(B, i, 0, mpts[i].x);
        cvmSet(B, i + n, 0, mpts[i].y);
    }
    //调用OpenCV函数，解线性方程组
    cvSolve(A, B, &X, CV_SVD);    //求X，使得AX=B
    x[8] = 1.0;//变换矩阵的[3][3]位置的值为固定值1
    X = cvMat(3, 3, CV_64FC1, x);
    cvConvert(&X, H);    //将数组转换为矩阵
    cvReleaseMat(&A);
    cvReleaseMat(&B);
    return H;
}


/*  对于给定的单应性矩阵H，计算输入点pt精H变换后的点与其匹配点mpt之间的误差
    例如：给定点x，其对应点x'，单应性矩阵H，则计算x'与Hx之间的距离的平方，d(x', Hx)^2
    参数：
    pt：一个点
    mpt：pt的对应匹配点
    H：单应性矩阵
    返回值：转换误差
*/
/*
    Calculates the transfer error between a point and its correspondence for
    a given homography, i.e. for a point x, it's correspondence x', and
    homography H, computes d(x', Hx)^2.

    @param pt a point
    @param mpt pt's correspondence
    @param H a homography matrix

    @return Returns the transfer error between pt and mpt given H
*/
double homog_xfer_err(CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* H)
{
    CvPoint2D64f xpt = persp_xform_pt(pt, H);    //pt经变换矩阵H变换后的点xpt,即H乘以x对应的向量
    return sqrt(dist_sq_2D(xpt, mpt));
}


/*  计算点pt经透视变换后的点，即给定一点pt和透视变换矩阵T，计算变换后的点
    给定点(x,y)，变换矩阵M，计算[x',y',w']^T = M * [x,y,1]^T(^T表示转置)，
    则变换后的点是(u,v) = (x'/w', y'/w')
    注意：仿射变换是透视变换的特例
    参数：
    pt：一个二维点
    T：透视变换矩阵
    返回值：pt经透视变换后的点
*/
/*
    Performs a perspective transformation on a single point.  That is, for a
    point (x, y) and a 3 x 3 matrix T this function returns the point
    (u, v), where

    [x' y' w']^T = T * [x y 1]^T,

    and

    (u, v) = (x'/w', y'/w').

    Note that affine transforms are a subset of perspective transforms.

    @param pt a 2D point
    @param T a perspective transformation matrix

    @return Returns the point (u, v) as above.
*/
CvPoint2D64f persp_xform_pt(CvPoint2D64f pt, CvMat* T)
{
    //XY：点pt对应的3*1列向量，UV：pt变换后的点对应的3*1列向量
    CvMat XY, UV;
    double xy[3] = { pt.x, pt.y, 1.0 }, uv[3] = { 0 };//对应的数据
    CvPoint2D64f rslt;//结果
    //初始化矩阵头
    cvInitMatHeader(&XY, 3, 1, CV_64FC1, xy, CV_AUTOSTEP);
    cvInitMatHeader(&UV, 3, 1, CV_64FC1, uv, CV_AUTOSTEP);
    cvMatMul(T, &XY, &UV);    //计算矩阵乘法，T*XY,结果放在UV中
    rslt = cvPoint2D64f(uv[0] / uv[2], uv[1] / uv[2]);    //得到转换后的点
    return rslt;
}


/************************ 本地函数的实现**************************************/
/************************ Local funciton definitions *************************/

/*  根据给定的匹配类型，返回输入特征点feat的匹配点
    参数：
    feat：输入特征点
    mtype：匹配类型，是FEATURE_FWD_MATCH，FEATURE_BCK_MATCH，FEATURE_MDL_MATCH之一
    返回值：feat的匹配点的指针，若为空表示mtype参数有误
*/
/*
    Returns a feature's match according to a specified match type

    @param feat feature
    @param mtype match type, one of FEATURE_FWD_MATCH, FEATURE_BCK_MATCH, or
    FEATURE_MDL_MATCH

    @return Returns feat's match corresponding to mtype or NULL for bad mtype
*/
static inline struct feature* get_match(struct feature* feat, int mtype)
{
    //FEATURE_MDL_MATCH：表明feature结构中的mdl_match域是对应的匹配点
    if (mtype == FEATURE_MDL_MATCH)
        return feat->mdl_match;
    //FEATURE_BCK_MATCH：表明feature结构中的bck_match域是对应的匹配点
    if (mtype == FEATURE_BCK_MATCH)
        return feat->bck_match;
    //FEATURE_FWD_MATCH：表明feature结构中的fwd_match域是对应的匹配点
    if (mtype == FEATURE_FWD_MATCH)
        return feat->fwd_match;
    return NULL;
}


/*  找到所有具有mtype类型匹配点的特征点，将他们的指针存储在matched数组中，
    并初始化matched数组中每个特征点的feature_data域为ransac_data类型的数据指针
    参数：
    features：特征点数组
    n：特征点个数
    mtype：匹配类型
    matched：输出参数，含有mtype类型匹配点的特征点的指针数组
    返回值：matched数组中特征点的个数
*/
/*
    Finds all features with a match of a specified type and stores pointers
    to them in an array.  Additionally initializes each matched feature's
    feature_data field with a ransac_data structure.

    @param features array of features
    @param n number of features in features
    @param mtype match type, one of FEATURE_{FWD,BCK,MDL}_MATCH
    @param matched output as an array of pointers to features with a match of
    the specified type

    @return Returns the number of features output in matched.
*/
static int get_matched_features(struct feature* features, int n, int mtype, struct feature** * matched)
{
    struct feature** _matched;//输出数组，具有mtype类型匹配点的特征点指针数组
    struct ransac_data* rdata;//ransac_data类型数据指针
    int i, m = 0;
    _matched = (struct feature**) calloc(n, sizeof(struct feature*));
    //遍历输入的特征点数组
    for (i = 0; i < n; i++)
    {
        //找第i个特征点的mtype类型匹配点，若能正确找到表明此特征点有mtype类型的匹配点，则将其放入_matched数组
        if (get_match(features + i, mtype))
        {
            rdata = (struct ransac_data*) malloc(sizeof(struct ransac_data));          //为ransac_data结构分配空间
            memset(rdata, 0, sizeof(struct ransac_data));       //清零
            rdata->orig_feat_data = features[i].feature_data;//保存第i个特征点的feature_data域之前的值
            _matched[m] = features + i;//放到_matched数组
            _matched[m]->feature_data = rdata;//其feature_data域赋值为ransac_data类型数据的指针
            m++;//_matched数组中元素个数
        }
    }
    *matched = _matched;//输出参数赋值
    return m;//返回值，元素个数
}


/*  计算保证RANSAC最终计算出的转换矩阵错误的概率小于p_badxform所需的最小内点数目
    参数：
    n：推定的匹配点对的个数
    m：计算模型所需的最小点对个数
    p_badsupp：概率，错误模型被一个匹配点对支持的概率
    p_badxform：概率，最终计算出的转换矩阵是错误的的概率
    返回值：保证RANSAC最终计算出的转换矩阵错误的概率小于p_badxform所需的最小内点数目
*/
/*
    Calculates the minimum number of inliers as a function of the number of
    putative correspondences.  Based on equation (7) in

    Chum, O. and Matas, J.  Matching with PROSAC -- Progressive Sample Consensus.
    In <EM>Conference on Computer Vision and Pattern Recognition (CVPR)</EM>,
    (2005), pp. 220--226.

    @param n number of putative correspondences
    @param m min number of correspondences to compute the model in question
    @param p_badsupp prob. that a bad model is supported by a correspondence
    @param p_badxform desired prob. that the final transformation returned is bad

    @return Returns the minimum number of inliers required to guarantee, based
    on p_badsupp, that the probability that the final transformation returned
    by RANSAC is less than p_badxform
*/
static int calc_min_inliers(int n, int m, double p_badsupp, double p_badxform)
{
    //根据论文：Chum, O. and Matas, J.  Matching with PROSAC -- Progressive Sample Consensus
    //中的一个公式计算，看不懂
    double pi, sum;
    int i, j;
    for (j = m + 1; j <= n; j++)
    {
        sum = 0;
        for (i = j; i <= n; i++)
        {
            pi = (i - m) * log(p_badsupp) + (n - i + m) * log(1.0 - p_badsupp) +
                 log_factorial(n - m) - log_factorial(i - m) -
                 log_factorial(n - i);
            /*
                Last three terms above are equivalent to log( n-m choose i-m )
            */
            sum += exp(pi);
        }
        if (sum < p_badxform)
            break;
    }
    return j;
}


//计算n的阶乘的自然对数
/*
    Calculates the natural log of the factorial of a number

    @param n number

    @return Returns log( n! )
*/
static inline double log_factorial(int n)
{
    double f = 0;
    int i;
    for (i = 1; i <= n; i++)
        f += log((float) i);
    return f;
}


/*  从给定的特征点集中随机抽选一个RANSAC样本(即一个4个特征点的数组)
    参数：
    features：作为样本集的特征点数组
    n：features中元素个数
    m：单个样本的尺寸，这里是4(至少需要4个点来计算变换矩阵)
    返回值：一个指针数组，其元素指向被选为样本的特征点，被选为样本的特征点的feature_data域的sampled被设为1
*/
/*
    Draws a RANSAC sample from a set of features.

    @param features array of pointers to features from which to sample
    @param n number of features in features
    @param m size of the sample

    @return Returns an array of pointers to the sampled features; the sampled
    field of each sampled feature's ransac_data is set to 1
*/
static struct feature** draw_ransac_sample(struct feature** features, int n,
        int m)
{
    struct feature** sample, *feat;//sample：被选为样本的点的数组
    struct ransac_data* rdata;
    int i, x;
    //将所有特征点的feature_data域的sampled值都初始化为0，即未被选为样本点
    for (i = 0; i < n; i++)
    {
        //利用宏feat_ransac_data来提取参数中的feature_data成员并转换为ransac_data格式的指针
        rdata = feat_ransac_data(features[i]);
        rdata->sampled = 0;
    }
    sample = (struct feature**) calloc(m, sizeof(struct feature*));           //为样本数组分配空间
    //随机抽取m个特征点作为一个样本，将其指针保存在sample数组中
    for (i = 0; i < m; i++)
    {
        do
        {
            //linux  x = random() % n;
            x = rand() % n; //随机下标
            feat = features[x];
            rdata = feat_ransac_data(feat);    //获得feature_data成员并转换为ransac_data格式的指针
        }
        while (rdata->sampled);  //若抽取的特征点的sampled值为1，继续选取；否则停止，将其作为样本中的一个点
        sample[i] = feat;//放入sample数组
        rdata->sampled = 1;//该点的feature_data成员的sampled域值设为1
    }
    return sample;//返回随机选取的样本
}


/*  从特征点数组中获取特征点和其对应匹配点的二维坐标，分别放到输出参数pts和mpts中
    参数：
    features：特征点数组，将从其中抽取坐标点和其匹配点，此数组中所有特征点都应具有mtype类型的匹配点
    n：feantures中特征点个数
    mtype：匹配类型，若是FEATURE_MDL_MATCH，对应的匹配点对坐标是每个特征点的img_pt域和其匹配点的mdl_pt域，
    否则，对应的匹配点对是每个特征点的img_pt域和其匹配点的img_pt域。
    pts：输出参数，从特征点数组features中获取的二维坐标数组
    mpts：输出参数，从特征点数组features的对应匹配点中获取的二维坐标数组
*/
/*
    Extrancs raw point correspondence locations from a set of features

    @param features array of features from which to extract points and match
    points; each of these is assumed to have a match of type mtype
    @param n number of features
    @param mtype match type; if FEATURE_MDL_MATCH correspondences are assumed
    to be between each feature's img_pt field and it's match's mdl_pt field,
    otherwise, correspondences are assumed to be between img_pt and img_pt
    @param pts output as an array of raw point locations from features
    @param mpts output as an array of raw point locations from features' matches
*/
static void extract_corresp_pts(struct feature** features, int n, int mtype,
                                CvPoint2D64f** pts, CvPoint2D64f** mpts)
{
    struct feature* match;//每个特征点对应的匹配点
    CvPoint2D64f* _pts, *_mpts;
    int i;
    _pts = (CvPoint2D64f*) calloc(n, sizeof(CvPoint2D64f));          //特征点的坐标数组
    _mpts = (CvPoint2D64f*) calloc(n, sizeof(CvPoint2D64f));          //对应匹配点的坐标数组
    //匹配类型是FEATURE_MDL_MATCH，匹配点的坐标是mdl_pt域
    if (mtype == FEATURE_MDL_MATCH)
        for (i = 0; i < n; i++)
        {
            //根据给定的匹配类型，返回输入特征点的匹配点
            match = get_match(features[i], mtype);
            if (!match)
                fatal_error("feature does not have match of type %d, %s line %d",
                            mtype, __FILE__, __LINE__);
            _pts[i] = features[i]->img_pt;//特征点的坐标
            _mpts[i] = match->mdl_pt;//对应匹配点的坐标
        }
    //匹配类型不是FEATURE_MDL_MATCH，匹配点的坐标是img_pt域
    else
        for (i = 0; i < n; i++)
        {
            //根据给定的匹配类型，返回输入特征点的匹配点
            match = get_match(features[i], mtype);
            if (!match)
                fatal_error("feature does not have match of type %d, %s line %d",
                            mtype, __FILE__, __LINE__);
            _pts[i] = features[i]->img_pt;//特征点的坐标
            _mpts[i] = match->img_pt;//对应匹配点的坐标
        }
    *pts = _pts;
    *mpts = _mpts;
}



/*  对于给定的模型和错误度量函数，从特征点集和中找出一致集
    参数：
    features：特征点集合，其中的特征点都具有mtype类型的匹配点
    n：特征点的个数
    mtype：匹配类型，若是FEATURE_MDL_MATCH，对应的匹配点对坐标是每个特征点的img_pt域和其匹配点的mdl_pt域，
    否则，对应的匹配点对是每个特征点的img_pt域和其匹配点的img_pt域。
    M：给定的模型，即一个变换矩阵
    err_fn：错误度量函数，对于给定的变换矩阵，计算推定的匹配点对之间的变换误差
    err_tol：容错度，用来衡量err_fn的返回值，小于err_tol的被加入一致集
    consensus：输出参数，一致集，即一致集中特征点构成的数组
    返回值：一致集中特征点的个数
*/
/*
    For a given model and error function, finds a consensus from a set of
    feature correspondences.

    @param features set of pointers to features; every feature is assumed to
    have a match of type mtype
    @param n number of features in features
    @param mtype determines the match field of each feature against which to
    measure error; if this is FEATURE_MDL_MATCH, correspondences are assumed
    to be between the feature's img_pt field and the match's mdl_pt field;
    otherwise matches are assumed to be between img_pt and img_pt
    @param M model for which a consensus set is being found
    @param err_fn error function used to measure distance from M
    @param err_tol correspondences within this distance of M are added to the
    consensus set
    @param consensus output as an array of pointers to features in the
    consensus set

    @return Returns the number of points in the consensus set
*/
static int find_consensus(struct feature** features, int n, int mtype,
                          CvMat* M, ransac_err_fn err_fn, double err_tol,
                          struct feature** * consensus)
{
    struct feature** _consensus;//一致集
    struct feature* match;//每个特征点对应的匹配点
    CvPoint2D64f pt, mpt;//pt:当前特征点的坐标，mpt：当前特征点的匹配点的坐标
    double err;//变换误差
    int i, in = 0;
    _consensus = (struct feature**) calloc(n, sizeof(struct feature*));           //给一致集分配空间
    //匹配类型是FEATURE_MDL_MATCH，匹配点的坐标是mdl_pt域
    if (mtype == FEATURE_MDL_MATCH)
        for (i = 0; i < n; i++)
        {
            //根据给定的匹配类型，返回输入特征点的匹配点
            match = get_match(features[i], mtype);
            if (!match)
                fatal_error("feature does not have match of type %d, %s line %d",
                            mtype, __FILE__, __LINE__);
            pt = features[i]->img_pt;//特征点的坐标
            mpt = match->mdl_pt;//对应匹配点的坐标
            err = err_fn(pt, mpt, M);    //计算"pt经M变换后的点"和mpt之间的距离的平方，即变换误差
            if (err <= err_tol)   //若变换误差小于容错度，将其加入一致集
                _consensus[in++] = features[i];
        }
    //匹配类型不是FEATURE_MDL_MATCH，匹配点的坐标是img_pt域
    else
        for (i = 0; i < n; i++)
        {
            //根据给定的匹配类型，返回输入特征点的匹配点
            match = get_match(features[i], mtype);
            if (!match)
                fatal_error("feature does not have match of type %d, %s line %d",
                            mtype, __FILE__, __LINE__);
            pt = features[i]->img_pt;//特征点的坐标
            mpt = match->img_pt;//对应匹配点的坐标
            err = err_fn(pt, mpt, M);    //计算"pt经M变换后的点"和mpt之间的距离的平方，即变换误差
            if (err <= err_tol)   //若变换误差小于容错度，将其加入一致集
                _consensus[in++] = features[i];
        }
    *consensus = _consensus;
    return in;//返回一致集中元素个数
}


/*  释放输入参数的存储空间
*/
/*
    Releases memory and reduces code size above

    @param pts1 an array of points
    @param pts2 an array of points
    @param features an array of pointers to features; can be NULL
*/
static inline void release_mem(CvPoint2D64f* pts1, CvPoint2D64f* pts2,
                               struct feature** features)
{
    free(pts1);
    free(pts2);
    if (features)
        free(features);
}



















//==========================================================
/*
    此文件中有几个函数的实现：特征点的导入导出，特征点的绘制
*/

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif

#define CV_RGB( r, g, b )  cvScalar( (b), (g), (r), 0 )

/************************ 未暴露接口的一些本地函数的声明 **************************/
static int import_oxfd_features(char*, struct feature**);      //导入OXFD格式特征点
static int export_oxfd_features(char*, struct feature*, int);      //导出OXFD格式特征点
static void draw_oxfd_features(IplImage*, struct feature*, int);      //画OXFD格式特征点
static void draw_oxfd_feature(IplImage*, struct feature*, CvScalar);      //画单个点

static int import_lowe_features(char*, struct feature**);      //导入LOWE格式特征点
static int export_lowe_features(char*, struct feature*, int);      //导出LOWE格式特征点
static void draw_lowe_features(IplImage*, struct feature*, int);      //画LOWE格式特征点
static void draw_lowe_feature(IplImage*, struct feature*, CvScalar);      //画单个点


/*  从文件中读入图像特征
    文件中的特征点格式必须是FEATURE_OXFD或FEATURE_LOWE格式
    参数：
    filename：文件名
    type：特征点类型
    feat：用来存储特征点的feature数组的指针
    返回值：导入的特征点个数
*/
/*
    Reads image features from file.  The file should be formatted as from
    the code provided by the Visual Geometry Group at Oxford:


    @param filename location of a file containing image features
    @param type determines how features are input.  If \a type is FEATURE_OXFD,
    the input file is treated as if it is from the code provided by the VGG
    at Oxford:

    http://www.robots.ox.ac.uk:5000/~vgg/research/affine/index.html

    If \a type is FEATURE_LOWE, the input file is treated as if it is from
    David Lowe's SIFT code:

    http://www.cs.ubc.ca/~lowe/keypoints
    @param features pointer to an array in which to store features

    @return Returns the number of features imported from filename or -1 on error
*/
int import_features(char* filename, int type, struct feature** feat)
{
    int n;
    //根据特征点类型，调用不同的函数完成导入功能
    switch (type)
    {
        case FEATURE_OXFD:
            n = import_oxfd_features(filename, feat);    //调用函数，导入OXFD格式特征点
            break;
        case FEATURE_LOWE:
            n = import_lowe_features(filename, feat);    //调用函数，导入LOWE格式特征点
            break;
        default: //特征点格式无法识别
            fprintf(stderr, "Warning: import_features(): unrecognized feature" \
                    "type, %s, line %d\n", __FILE__, __LINE__);
            return -1;
    }
    //导入失败
    if (n == -1)
        fprintf(stderr, "Warning: unable to import features from %s,"   \
                " %s, line %d\n", filename, __FILE__, __LINE__);
    return n;
}


/*  导出feature数组到文件
    参数：
    filename：文件名
    feat：特征数组
    n：特征点个数
    返回值：0：成功；1：失败
*/
/*
    Exports a feature set to a file formatted depending on the type of
    features, as specified in the feature struct's type field.

    @param filename name of file to which to export features
    @param feat feature array
    @param n number of features

    @return Returns 0 on success or 1 on error
*/
int export_features(char* filename, struct feature* feat, int n)
{
    int r, type;
    //参数合法性检查
    if (n <= 0 || !feat)
    {
        fprintf(stderr, "Warning: no features to export, %s line %d\n",
                __FILE__, __LINE__);
        return 1;
    }
    type = feat[0].type;//特征点的类型、
    //根据特征点类型，调用不同的函数完成导出功能
    switch (type)
    {
        case FEATURE_OXFD:
            r = export_oxfd_features(filename, feat, n);    //调用函数，导出OXFD格式特征点
            break;
        case FEATURE_LOWE:
            r = export_lowe_features(filename, feat, n);    //调用函数，导出LOWE格式特征点
            break;
        default:
            fprintf(stderr, "Warning: export_features(): unrecognized feature" \
                    "type, %s, line %d\n", __FILE__, __LINE__);
            return -1;
    }
    if (r)   //导出函数返回值非0，表示导出失败
        fprintf(stderr, "Warning: unable to export features to %s," \
                " %s, line %d\n", filename, __FILE__, __LINE__);
    return r;
}


/*  在图片上画出特征点
    参数：
    img：图像
    feat：特征点数组
    n：特征点个数
*/
/*
    Draws a set of features on an image

    @param img image on which to draw features
    @param feat array of Oxford-type features
    @param n number of features
*/
void draw_features(IplImage* img, struct feature* feat, int n)
{
    int type;
    //参数合法性检查
    if (n <= 0 || !feat)
    {
        fprintf(stderr, "Warning: no features to draw, %s line %d\n",
                __FILE__, __LINE__);
        return;
    }
    type = feat[0].type;//特征点的类型
    //根据特征点类型，调用不同的函数完成绘图功能
    switch (type)
    {
        case FEATURE_OXFD:
            draw_oxfd_features(img, feat, n);    //调用函数，在图像上画OXFD格式特征点
            break;
        case FEATURE_LOWE:
            draw_lowe_features(img, feat, n);    //调用函数，在图像上画LOWE格式特征点
            break;
        default:
            fprintf(stderr, "Warning: draw_features(): unrecognized feature" \
                    " type, %s, line %d\n", __FILE__, __LINE__);
            break;
    }
}


/*  计算两个特征描述子间的欧氏距离的平方
    参数：
    f1:第一个特征点
    f2:第二个特征点
    返回值：欧氏距离的平方
*/
/*
    Calculates the squared Euclidian distance between two feature descriptors.

    @param f1 first feature
    @param f2 second feature

    @return Returns the squared Euclidian distance between the descriptors of
    f1 and f2.
*/
double descr_dist_sq(struct feature* f1, struct feature* f2)
{
    double diff, dsq = 0;
    double* descr1, *descr2;
    int i, d;
    d = f1->d;//f1的特征描述子的长度
    if (f2->d != d)   //若f1和f2的特征描述子长度不同，返回
        return DBL_MAX;
    descr1 = f1->descr;//f1的特征描述子，一个double数组
    descr2 = f2->descr;//f2的特征描述子，一个double数组
    //计算欧氏距离的平方，即对应元素的差的平方和
    for (i = 0; i < d; i++)
    {
        diff = descr1[i] - descr2[i];
        dsq += diff * diff;
    }
    return dsq;
}


/***************************** 一些未暴露接口的内部函数 *******************************/
/***************************** Local Functions *******************************/


/*  从文件中读入OXFD格式的图像特征
    参数：
    filename：文件名
    features：用来存储特征点的feature数组的指针
    返回值：导入的特征点个数
*/
/*
    Reads image features from file.  The file should be formatted as from
    the code provided by the Visual Geometry Group at Oxford:

    http://www.robots.ox.ac.uk:5000/~vgg/research/affine/index.html

    @param filename location of a file containing image features
    @param features pointer to an array in which to store features

    @return Returns the number of features imported from filename or -1 on error
*/
static int import_oxfd_features(char* filename, struct feature** features)
{
    struct feature* f;//第一个特征点的指针
    int i, j, n, d;
    double x, y, a, b, c, dv;
    FILE* file;//文件指针
    if (!features)
        fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);
    //打开文件
    if (!(file = fopen(filename, "r")))
    {
        fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
                filename, __FILE__, __LINE__);
        return -1;
    }
    //读入特征描述子维数和特征点个数
    /* read dimension and number of features */
    if (fscanf(file, " %d %d ", &d, &n) != 2)
    {
        fprintf(stderr, "Warning: file read error, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    //特征描述子维数大于定义的最大维数，出错
    if (d > FEATURE_MAX_D)
    {
        fprintf(stderr, "Warning: descriptor too long, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    //分配内存，n个feature结构大小，返回首地址给f
    f = (struct feature*) calloc(n, sizeof(struct feature));
    //遍历文件中的n个特征点
    for (i = 0; i < n; i++)
    {
        //读入仿射区域参数
        /* read affine region parameters */
        if (fscanf(file, " %lf %lf %lf %lf %lf ", &x, &y, &a, &b, &c) != 5)
        {
            fprintf(stderr, "Warning: error reading feature #%d, %s, line %d\n",
                    i + 1, __FILE__, __LINE__);
            free(f);    //发生错误后释放内存
            return -1;
        }
        //给第i个特征点赋值
        f[i].img_pt.x = f[i].x = x;//特征点的x坐标
        f[i].img_pt.y = f[i].y = y;//特征点的y坐标
        f[i].a = a;
        f[i].b = b;
        f[i].c = c;
        f[i].d = d;
        f[i].type = FEATURE_OXFD;//特征点类型
        //读入特征描述子
        /* read descriptor */
        for (j = 0; j < d; j++)
        {
            if (!fscanf(file, " %lf ", &dv))
            {
                fprintf(stderr, "Warning: error reading feature descriptor" \
                        " #%d, %s, line %d\n", i + 1, __FILE__, __LINE__);
                free(f);    //发生错误后释放内存
                return -1;
            }
            f[i].descr[j] = dv;//赋给第i个特征点的第j个特征描述符
        }
        //其他一些没什么用的参数
        f[i].scl = f[i].ori = 0;//OXFD特征点无此参数
        f[i].category = 0;
        f[i].fwd_match = f[i].bck_match = f[i].mdl_match = NULL;
        f[i].mdl_pt.x = f[i].mdl_pt.y = -1;
        f[i].feature_data = NULL;
    }
    //关闭文件
    if (fclose(file))
    {
        fprintf(stderr, "Warning: file close error, %s, line %d\n",
                __FILE__, __LINE__);
        free(f);    //发生错误后释放内存
        return -1;
    }
    *features = f;//将第一个特征点的指针赋给*feature
    return n;//返回读入的特征点个数
}


/*  导出OXFD格式的特征点集到文件
    参数：
    filename：文件名
    feat：特征数组
    n：特征点个数
    返回值：0：成功；1：失败
*/
/*
    Exports a feature set to a file formatted as one from the code provided
    by the Visual Geometry Group at Oxford:

    http://www.robots.ox.ac.uk:5000/~vgg/research/affine/index.html

    @param filename name of file to which to export features
    @param feat feature array
    @param n number of features

    @return Returns 0 on success or 1 on error
*/
static int export_oxfd_features(char* filename, struct feature* feat, int n)
{
    FILE* file;
    int i, j, d;
    if (n <= 0)
    {
        fprintf(stderr, "Warning: feature count %d, %s, line %d\n",
                n, __FILE__, __LINE__);
        return 1;
    }
    //打开文件
    if (!(file = fopen(filename, "w")))
    {
        fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
                filename, __FILE__, __LINE__);
        return 1;
    }
    d = feat[0].d;//特征描述子的维数
    fprintf(file, "%d\n%d\n", d, n);    //首先写入特征描述子的维数和特征点个数
    //依次写入每个特征点的信息
    for (i = 0; i < n; i++)
    {
        //写入仿射区域参数
        fprintf(file, "%f %f %f %f %f", feat[i].x, feat[i].y, feat[i].a,
                feat[i].b, feat[i].c);
        //写入d个特征描述子的元素
        for (j = 0; j < d; j++)
            fprintf(file, " %f", feat[i].descr[j]);
        fprintf(file, "\n");    //换行
    }
    //关闭文件
    if (fclose(file))
    {
        fprintf(stderr, "Warning: file close error, %s, line %d\n",
                __FILE__, __LINE__);
        return 1;
    }
    return 0;
}


/*  在图像上画出OXFD类型的特征点
    参数：
    img：图像指针
    feat：特征数组
    n：特征个数
*/
/*
    Draws Oxford-type affine features

    @param img image on which to draw features
    @param feat array of Oxford-type features
    @param n number of features
*/
static void draw_oxfd_features(IplImage* img, struct feature* feat, int n)
{
    CvScalar color = CV_RGB(255, 255, 255);    //颜色
    int i;
    if (img->nChannels > 1)
        color = FEATURE_OXFD_COLOR;
    //调用函数，依次画出每个特征点
    for (i = 0; i < n; i++)
        draw_oxfd_feature(img, feat + i, color);
}


/*  在图像上画单个OXFD特征点
    参数：
    img：图像指针
    feat：要画的特征点
    color：颜色
*/
/*
    Draws a single Oxford-type feature

    @param img image on which to draw
    @param feat feature to be drawn
    @param color color in which to draw
*/
static void draw_oxfd_feature(IplImage* img, struct feature* feat, CvScalar color)
{
    double m[4] = { feat->a, feat->b, feat->b, feat->c };
    double v[4] = { 0 };//特征向量的数据
    double e[2] = { 0 };//特征值的数据
    CvMat M, V, E;
    double alpha, l1, l2;
    //计算椭圆的轴线和方向
    /* compute axes and orientation of ellipse surrounding affine region */
    cvInitMatHeader(&M, 2, 2, CV_64FC1, m, CV_AUTOSTEP);    //矩阵
    cvInitMatHeader(&V, 2, 2, CV_64FC1, v, CV_AUTOSTEP);    //2个2*1的特征向量组成的矩阵
    cvInitMatHeader(&E, 2, 1, CV_64FC1, e, CV_AUTOSTEP);    //特征值
    cvEigenVV(&M, &V, &E, DBL_EPSILON, 0, 0);    //计算特征值和特征向量
    l1 = 1 / sqrt(e[1]);
    l2 = 1 / sqrt(e[0]);
    alpha = -atan2(v[1], v[0]);
    alpha *= 180 / M_PI;    //CV_PI
    //画椭圆和十字星
    cvEllipse(img, cvPoint(feat->x, feat->y), cvSize(l2, l1), alpha,
              0, 360, CV_RGB(0, 0, 0), 3, 8, 0);
    cvEllipse(img, cvPoint(feat->x, feat->y), cvSize(l2, l1), alpha,
              0, 360, color, 1, 8, 0);
    cvLine(img, cvPoint(feat->x + 2, feat->y), cvPoint(feat->x - 2, feat->y),
           color, 1, 8, 0);
    cvLine(img, cvPoint(feat->x, feat->y + 2), cvPoint(feat->x, feat->y - 2),
           color, 1, 8, 0);
}


/*  从文件中读入LOWE特征点
    参数：
    filename:文件名
    features：存放特征点的特征数组的指针
    返回值：读入的特征点个数
*/
/*
    Reads image features from file.  The file should be formatted as from
    the code provided by David Lowe:

    http://www.cs.ubc.ca/~lowe/keypoints/

    @param filename location of a file containing image features
    @param features pointer to an array in which to store features

    @return Returns the number of features imported from filename or -1 on error
*/
static int import_lowe_features(char* filename, struct feature** features)
{
    struct feature* f;//第一个特征点的指针
    int i, j, n, d;
    double x, y, s, o, dv;
    FILE* file;
    if (!features)
        fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);
    //打开文件
    if (!(file = fopen(filename, "r")))
    {
        fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
                filename, __FILE__, __LINE__);
        return -1;
    }
    //首先读入特征点个数和特征描述子维数
    /* read number of features and dimension */
    if (fscanf(file, " %d %d ", &n, &d) != 2)
    {
        fprintf(stderr, "Warning: file read error, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    //特征描述子维数大于定义的最大维数，出错
    if (d > FEATURE_MAX_D)
    {
        fprintf(stderr, "Warning: descriptor too long, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    //分配内存，n个feature结构大小，返回首地址给f
    f = (struct feature*) calloc(n, sizeof(struct feature));
    //依次读入n个特征点
    for (i = 0; i < n; i++)
    {
        //读入特征点的坐标(注意x,y顺序)，尺度和方向
        /* read affine region parameters */
        if (fscanf(file, " %lf %lf %lf %lf ", &y, &x, &s, &o) != 4)
        {
            fprintf(stderr, "Warning: error reading feature #%d, %s, line %d\n",
                    i + 1, __FILE__, __LINE__);
            free(f);    //出错后释放内存
            return -1;
        }
        //给第i个特征点赋值
        f[i].img_pt.x = f[i].x = x;//特征点的x坐标
        f[i].img_pt.y = f[i].y = y;//特征点的y坐标
        f[i].scl = s;//特征点的大小，即其主方向的梯度的模值
        f[i].ori = o;//特征点的方向，即其主方向
        f[i].d = d;//特征描述子的维数
        f[i].type = FEATURE_LOWE;//类型
        //读入特征描述子
        /* read descriptor */
        for (j = 0; j < d; j++)
        {
            if (!fscanf(file, " %lf ", &dv))
            {
                fprintf(stderr, "Warning: error reading feature descriptor" \
                        " #%d, %s, line %d\n", i + 1, __FILE__, __LINE__);
                free(f);    //出错后释放内存
                return -1;
            }
            f[i].descr[j] = dv;
        }
        //其他一些没什么用的参数
        f[i].a = f[i].b = f[i].c = 0;
        f[i].category = 0;
        f[i].fwd_match = f[i].bck_match = f[i].mdl_match = NULL;
        f[i].mdl_pt.x = f[i].mdl_pt.y = -1;
        f[i].feature_data = NULL;
    }
    //关闭文件
    if (fclose(file))
    {
        fprintf(stderr, "Warning: file close error, %s, line %d\n",
                __FILE__, __LINE__);
        free(f);    //出错后释放内存
        return -1;
    }
    *features = f;//首地址赋给*features
    return n;//返回读入的特征点个数
}


/*  导出LOWE格式特征点集合到文件
    参数：
    filename：文件名
    feat：特征点数组
    n：特征点个数
    返回值：0：成功；1：失败
*/
/*
    Exports a feature set to a file formatted as one from the code provided
    by David Lowe:

    http://www.cs.ubc.ca/~lowe/keypoints/

    @param filename name of file to which to export features
    @param feat feature array
    @param n number of features

    @return Returns 0 on success or 1 on error
*/
static int export_lowe_features(char* filename, struct feature* feat, int n)
{
    FILE* file;
    int i, j, d;
    if (n <= 0)
    {
        fprintf(stderr, "Warning: feature count %d, %s, line %d\n",
                n, __FILE__, __LINE__);
        return 1;
    }
    //打开文件
    if (!(file = fopen(filename, "w")))
    {
        fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
                filename, __FILE__, __LINE__);
        return 1;
    }
    d = feat[0].d;//特征描述子维数
    fprintf(file, "%d %d\n", n, d);    //首先写入特征点个数和特征描述子维数
    //依次写入每个特征点的信息
    for (i = 0; i < n; i++)
    {
        //写入特征点坐标(注意x,y顺序),尺度，方向
        fprintf(file, "%f %f %f %f", feat[i].y, feat[i].x,
                feat[i].scl, feat[i].ori);
        //写入特征描述子
        for (j = 0; j < d; j++)
        {
            //每行20个元素
            /* write 20 descriptor values per line */
            if (j % 20 == 0)
                fprintf(file, "\n");
            fprintf(file, " %d", (int)(feat[i].descr[j]));
        }
        fprintf(file, "\n");
    }
    //关闭文件
    if (fclose(file))
    {
        fprintf(stderr, "Warning: file close error, %s, line %d\n",
                __FILE__, __LINE__);
        return 1;
    }
    return 0;
}


/*  在图像上画LOWE特征点
    参数：
    img：图像指针
    feat：特征点数组
    n：特征点个数
*/
/*
    Draws Lowe-type features

    @param img image on which to draw features
    @param feat array of Oxford-type features
    @param n number of features
*/
static void draw_lowe_features(IplImage* img, struct feature* feat, int n)
{
    CvScalar color = CV_RGB(255, 255, 255);    //颜色
    int i;
    if (img->nChannels > 1)
        color = FEATURE_LOWE_COLOR;
    //调用函数，依次画n个特征点
    for (i = 0; i < n; i++)
        draw_lowe_feature(img, feat + i, color);
}


/*  画单个LOWE特征点
    参数：
    img：图像指针
    feat：要画的特征点
    color：颜色
*/
/*
    Draws a single Lowe-type feature

    @param img image on which to draw
    @param feat feature to be drawn
    @param color color in which to draw
*/
static void draw_lowe_feature(IplImage* img, struct feature* feat, CvScalar color)
{
    int len, hlen, blen, start_x, start_y, end_x, end_y, h1_x, h1_y, h2_x, h2_y;
    double scl, ori;
    double scale = 5.0;
    double hscale = 0.75;
    CvPoint start, end, h1, h2;
    /* compute points for an arrow scaled and rotated by feat's scl and ori */
    //箭头杆的起点的坐标
    start_x = cvRound(feat->x);
    start_y = cvRound(feat->y);
    scl = feat->scl;//特征点的大小
    ori = feat->ori;//特征点的方向，弧度
    len = cvRound(scl * scale);    //箭头杆的长度
    hlen = cvRound(scl * hscale);    //箭头分叉的长度
    blen = len - hlen;
    //箭头杆的终点的坐标
    end_x = cvRound(len *  cos(ori)) + start_x;
    end_y = cvRound(len * -sin(ori)) + start_y;
    //箭头的右分叉的起点的坐标
    h1_x = cvRound(blen *  cos(ori + CV_PI / 18.0)) + start_x;
    h1_y = cvRound(blen * -sin(ori + CV_PI / 18.0)) + start_y;
    //箭头的左分叉的起点的坐标
    h2_x = cvRound(blen *  cos(ori - CV_PI / 18.0)) + start_x;
    h2_y = cvRound(blen * -sin(ori - CV_PI / 18.0)) + start_y;
    start = cvPoint(start_x, start_y);    //箭头杆的起点
    end = cvPoint(end_x, end_y);    //箭头杆的终点
    h1 = cvPoint(h1_x, h1_y);    //箭头的右分叉的起点
    h2 = cvPoint(h2_x, h2_y);    //箭头的左分叉的起点
    cvLine(img, start, end, color, 1, 8, 0);    //画箭头杆
    cvLine(img, end, h1, color, 1, 8, 0);    //画右分叉
    cvLine(img, end, h2, color, 1, 8, 0);    //画左分叉
}




















//============================================================
/*
    此文件中有k-d树的建立和BBF查找函数的实现
*/

//BBF中用到的结构，可存储当前点到目标点的距离
//在kd树搜索过程中，此类型数据会被赋值给feature结构的feature_data成员
struct bbf_data
{
    double d;  //此特征点到目标点的欧式距离值
    void* old_data; //保存此特征点的feature_data域的以前的值
};

/************************ 未暴露接口的一些本地函数的声明 **************************/
/************************* Local Function Prototypes *************************/

//用给定的特征点集初始化k-d树节点
static struct kd_node* kd_node_init(struct feature*, int);
//扩展指定的k-d树节点及其左右孩子
static void expand_kd_node_subtree(struct kd_node*);
//确定输入节点的枢轴索引和枢轴值
static void assign_part_key(struct kd_node*);
//找到输入数组的中值
static double median_select(double*, int);
//找到输入数组中第r小的数
static double rank_select(double*, int, int);
//用插入法对输入数组进行升序排序
static void insertion_sort(double*, int);
//根据给定的枢轴值分割数组，使数组前部分小于pivot，后部分大于pivot
static int partition_array(double*, int, double);
//在指定的k-d树节点上划分特征点集
static void partition_features(struct kd_node*);
//从给定结点搜索k-d树直到叶节点，搜索过程中将未搜索的节点根据优先级放入队列
static struct kd_node* explore_to_leaf(struct kd_node*, struct feature*, struct min_pq*);
//插入一个特征点到最近邻数组，使数组中的点按到目标点的距离升序排列
static int insert_into_nbr_array(struct feature*, struct feature**, int, int);
//判断给定点是否在某矩形中
static int within_rect(CvPoint2D64f, CvRect);


/******************** 已在kdtree.h中声明的函数 **********************/
/******************** Functions prototyped in kdtree.h **********************/

/*  根据给定的特征点集合建立k-d树
    参数：
    features：特征点数组，注意：此函数将会改变features数组中元素的排列顺序
    n：特征点个数
    返回值：建立好的k-d树的树根指针
*/
/*
    A function to build a k-d tree database from keypoints in an array.

    @param features an array of features
    @param n the number of features in features

    @return Returns the root of a kd tree built from features or NULL on
    error.
*/
struct kd_node* kdtree_build(struct feature* features, int n)
{
    struct kd_node* kd_root;
    //输入参数检查
    if (!features || n <= 0)
    {
        fprintf(stderr, "Warning: kdtree_build(): no features, %s, line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }
    //调用函数，用给定的特征点集初始化k-d树节点，返回值作为树根
    kd_root = kd_node_init(features, n);
    //调用函数，扩展根节点kd_root及其左右孩子
    expand_kd_node_subtree(kd_root);
    return kd_root;
}


/*  用BBF算法在k-d树中查找指定特征点的k个最近邻特征点
    参数：
    kd_root：图像特征的k-d树的树根
    feat：目标特征点
    k：近邻个数
    nbrs：k个近邻特征点的指针数组，按到目标特征点的距离升序排列
    此数组的内存将在本函数中被分配，使用完后必须在调用出释放：free(*nbrs)
    max_nn_chks：搜索的最大次数，超过此值不再搜索
    返回值：存储在nbrs中的近邻个数，返回-1表示失败
*/
/*
    Finds an image feature's approximate k nearest neighbors in a kd tree using
    Best Bin First search.

    @param kd_root root of an image feature kd tree
    @param feat image feature for whose neighbors to search
    @param k number of neighbors to find
    @param nbrs pointer to an array in which to store pointers to neighbors
    in order of increasing descriptor distance
    @param max_nn_chks search is cut off after examining this many tree entries

    @return Returns the number of neighbors found and stored in nbrs, or
    -1 on error.
*/
int kdtree_bbf_knn(struct kd_node* kd_root, struct feature* feat, int k,
                   struct feature** * nbrs, int max_nn_chks)
{
    struct kd_node* expl; //expl是当前搜索节点
    struct min_pq* min_pq; //优先级队列
    struct feature* tree_feat, ** _nbrs; //tree_feat是单个SIFT特征，_nbrs中存放着查找出来的近邻特征节点
    struct bbf_data* bbf_data; //bbf_data是一个用来存放临时特征数据和特征间距离的缓存结构
    int i, t = 0, n = 0; //t是搜索的最大次数，n是当前最近邻数组中的元素个数
    //输入参数检查
    if (!nbrs || !feat || !kd_root)
    {
        fprintf(stderr, "Warning: NULL pointer error, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    _nbrs = (struct feature**) calloc(k, sizeof(struct feature*));           //给查找结果分配相应大小的内存
    min_pq = minpq_init(); //min_pq队列初始化，分配默认大小的空间
    minpq_insert(min_pq, kd_root, 0);    //将根节点先插入到min_pq优先级队列中
    //min_pq队列没有回溯完且未达到搜索最大次数
    while (min_pq->n > 0 && t < max_nn_chks)
    {
        //从min_pq中提取(并移除)优先级最高的节点，赋值给当前节点expl
        expl = (struct kd_node*) minpq_extract_min(min_pq);
        if (!expl)
        {
            //出错处理
            fprintf(stderr, "Warning: PQ unexpectedly empty, %s line %d\n", __FILE__, __LINE__);
            goto fail;
        }
        //从当前搜索节点expl一直搜索到叶子节点，搜索过程中将未搜索的节点根据优先级放入队列，返回值为叶子节点
        expl = explore_to_leaf(expl, feat, min_pq);
        if (!expl)
        {
            //出错处理
            fprintf(stderr, "Warning: PQ unexpectedly empty, %s line %d\n", __FILE__, __LINE__);
            goto fail;
        }
        //比较查找最近邻
        for (i = 0; i < expl->n; i++)
        {
            tree_feat = &expl->features[i];//第i个特征点的指针
            bbf_data = (struct bbf_data*) malloc(sizeof(struct bbf_data));          //新建bbf结构
            if (!bbf_data)
            {
                //出错处理
                fprintf(stderr, "Warning: unable to allocate memory," " %s line %d\n", __FILE__, __LINE__);
                goto fail;
            }
            bbf_data->old_data = tree_feat->feature_data;//保存第i个特征点的feature_data域以前的值
            bbf_data->d = descr_dist_sq(feat, tree_feat);    //当前搜索点和目标点之间的欧氏距离
            tree_feat->feature_data = bbf_data;//将bbf结构赋给此特征点的feature_data域
            //判断并插入符合条件的特征点到最近邻数组_nbrs中,插入成功返回1
            //当最近邻数组中元素个数已达到k时，继续插入元素个数不会增加，但会更新元素的值
            n += insert_into_nbr_array(tree_feat, _nbrs, n, k);
        }
        t++;//搜索次数
    }
    minpq_release(&min_pq);    //释放优先队列
    //对于最近邻数组中的特征点，恢复其feature_data域的值
    for (i = 0; i < n; i++)
    {
        bbf_data = (struct bbf_data*)(_nbrs[i]->feature_data);
        _nbrs[i]->feature_data = bbf_data->old_data;//将之前的数据赋值给feature_data域
        free(bbf_data);
    }
    *nbrs = _nbrs;
    return n;
    //失败处理
fail:
    minpq_release(&min_pq);
    //对于最近邻数组中的特征点，恢复其feature_data域的值
    for (i = 0; i < n; i++)
    {
        bbf_data = (struct bbf_data*)(_nbrs[i]->feature_data);
        _nbrs[i]->feature_data = bbf_data->old_data;
        free(bbf_data);
    }
    free(_nbrs);
    *nbrs = NULL;
    return -1;
}



/*
    Finds an image feature's approximate k nearest neighbors within a specified
    spatial region in a kd tree using Best Bin First search.

    @param kd_root root of an image feature kd tree
    @param feat image feature for whose neighbors to search
    @param k number of neighbors to find
    @param nbrs pointer to an array in which to store pointers to neighbors
    in order of increasing descriptor distance
    @param max_nn_chks search is cut off after examining this many tree entries
    @param rect rectangular region in which to search for neighbors
    @param model if true, spatial search is based on kdtree features' model
    locations; otherwise it is based on their image locations

    @return Returns the number of neighbors found and stored in \a nbrs
    (in case \a k neighbors could not be found before examining
    \a max_nn_checks keypoint entries).
*/
int kdtree_bbf_spatial_knn(struct kd_node* kd_root, struct feature* feat,
                           int k, struct feature** * nbrs, int max_nn_chks,
                           CvRect rect, int model)
{
    struct feature** all_nbrs, ** sp_nbrs;
    CvPoint2D64f pt;
    int i, n, t = 0;
    n = kdtree_bbf_knn(kd_root, feat, max_nn_chks, &all_nbrs, max_nn_chks);
    sp_nbrs = (struct feature**) calloc(k, sizeof(struct feature*));
    for (i = 0; i < n; i++)
    {
        if (model)
            pt = all_nbrs[i]->mdl_pt;
        else
            pt = all_nbrs[i]->img_pt;
        if (within_rect(pt, rect))
        {
            sp_nbrs[t++] = all_nbrs[i];
            if (t == k)
                goto end;
        }
    }
end:
    free(all_nbrs);
    *nbrs = sp_nbrs;
    return t;
}


/*  释放k-d树占用的存储空间
*/
/*
    De-allocates memory held by a kd tree

    @param kd_root pointer to the root of a kd tree
*/
void kdtree_release(struct kd_node* kd_root)
{
    if (!kd_root)
        return;
    kdtree_release(kd_root->kd_left);
    kdtree_release(kd_root->kd_right);
    free(kd_root);
}


/************************ 未暴露接口的一些本地函数 **************************/
/************************ Functions prototyped here **************************/

/*  用给定的特征点集初始化k-d树节点
    参数：
    features：特征点集
    n：特征点个数
    返回值：k-d树节点指针
*/
/*
    Initializes a kd tree node with a set of features.  The node is not
    expanded, and no ordering is imposed on the features.

    @param features an array of image features
    @param n number of features

    @return Returns an unexpanded kd-tree node.
*/
static struct kd_node* kd_node_init(struct feature* features, int n)
{
    struct kd_node* kd_node;
    kd_node = (struct kd_node*) malloc(sizeof(struct kd_node));          //分配内存
    memset(kd_node, 0, sizeof(struct kd_node));
    kd_node->ki = -1;//枢轴索引
    kd_node->features = features;//节点对应的特征点集
    kd_node->n = n;//特征点的个数
    return kd_node;
}


/*  递归的扩展指定的k-d树节点及其左右孩子
*/
/*
    Recursively expands a specified kd tree node into a tree whose leaves
    contain one entry each.

    @param kd_node an unexpanded node in a kd tree
*/
static void expand_kd_node_subtree(struct kd_node* kd_node)
{
    //基本情况：叶子节点
    /* base case: leaf node */
    if (kd_node->n == 1 || kd_node->n == 0)
    {
        kd_node->leaf = 1;//叶节点标志位设为1
        return;
    }
    //调用函数，确定节点的枢轴索引和枢轴值
    assign_part_key(kd_node);
    //在指定k-d树节点上划分特征点集(即根据指定节点的ki和kv值来划分特征点集)
    partition_features(kd_node);
    //继续扩展左右孩子
    if (kd_node->kd_left)
        expand_kd_node_subtree(kd_node->kd_left);
    if (kd_node->kd_right)
        expand_kd_node_subtree(kd_node->kd_right);
}


/*  确定输入节点的枢轴索引和枢轴值
    参数：kd_node：输入的k-d树节点
    函数执行完后将给kd_node的ki和kv成员复制
*/
/*
    Determines the descriptor index at which and the value with which to
    partition a kd tree node's features.

    @param kd_node a kd tree node
*/
static void assign_part_key(struct kd_node* kd_node)
{
    struct feature* features;
    //枢轴的值kv，均值mean，方差var，方差最大值var_max
    double kv, x, mean, var, var_max = 0;
    double* tmp;
    int d, n, i, j, ki = 0; //枢轴索引ki
    features = kd_node->features;
    n = kd_node->n;//结点个数
    d = features[0].d;//特征向量的维数
    //枢轴的索引值就是方差最大的那一维的维数,即n个128维的特征向量中，若第k维的方差最大，则k就是枢轴(分割位置)
    /* partition key index is that along which descriptors have most variance */
    for (j = 0; j < d; j++)
    {
        mean = var = 0;
        //求第j维的均值
        for (i = 0; i < n; i++)
            mean += features[i].descr[j];
        mean /= n;
        //求第j维的方差
        for (i = 0; i < n; i++)
        {
            x = features[i].descr[j] - mean;
            var += x * x;
        }
        var /= n;
        //找到最大方差的维数
        if (var > var_max)
        {
            ki = j;//最大方差的维数就是枢轴
            var_max = var;
        }
    }
    //枢轴的值就是所有特征向量的ki维的中值(按ki维排序后中间的那个值)
    /* partition key value is median of descriptor values at ki */
    tmp = (double*) calloc(n, sizeof(double));
    for (i = 0; i < n; i++)
        tmp[i] = features[i].descr[ki];
    //调用函数，找tmp数组的中值
    kv = median_select(tmp, n);
    free(tmp);
    kd_node->ki = ki;//枢轴的维数索引
    kd_node->kv = kv;//枢轴的值
}


/*  找到输入数组的中值
    参数：
    array：输入数组，元素顺序将会被改动
    n：元素个数
    返回值：中值
*/
/*
    Finds the median value of an array.  The array's elements are re-ordered
    by this function.

    @param array an array; the order of its elelemts is reordered
    @param n number of elements in array

    @return Returns the median value of array.
*/
static double median_select(double* array, int n)
{
    //调用函数，找array数组中的第(n-1)/2小的数，即中值
    return rank_select(array, n, (n - 1) / 2);
}


/*  找到输入数组中第r小的数
    参数：
    array：输入数组，元素顺序将会被改动
    n：元素个数
    r：找第r小元素
    返回值：第r小的元素值
*/
/*
    Finds the element of a specified rank in an array using the linear time
    median-of-medians algorithm by Blum, Floyd, Pratt, Rivest, and Tarjan.
    The elements of the array are re-ordered by this function.

    @param array an array; the order of its elelemts is reordered
    @param n number of elements in array
    @param r the zero-based rank of the element to be selected

    @return Returns the element from array with zero-based rank r.
*/
static double rank_select(double* array, int n, int r)
{
    double* tmp, med;
    int gr_5, gr_tot, rem_elts, i, j;
    /* base case */
    if (n == 1)
        return array[0];
    //将数组分成5个一组，共gr_tot组
    /* divide array into groups of 5 and sort them */
    gr_5 = n / 5; //组的个数-1，n/5向下取整
    gr_tot = cvCeil(n / 5.0);    //组的个数，n/5向上取整
    rem_elts = n % 5;//最后一组中的元素个数
    tmp = array;
    //对每组进行插入排序
    for (i = 0; i < gr_5; i++)
    {
        insertion_sort(tmp, 5);
        tmp += 5;
    }
    //最后一组
    insertion_sort(tmp, rem_elts);
    //找中值的中值
    /* recursively find the median of the medians of the groups of 5 */
    tmp = (double*) calloc(gr_tot, sizeof(double));
    //将每个5元组中的中值(即下标为2,2+5,...的元素)复制到temp数组
    for (i = 0, j = 2; i < gr_5; i++, j += 5)
        tmp[i] = array[j];
    //最后一组的中值
    if (rem_elts)
        tmp[i++] = array[n - 1 - rem_elts / 2];
    //找temp中的中值med，即中值的中值
    med = rank_select(tmp, i, (i - 1) / 2);
    free(tmp);
    //利用中值的中值划分数组，看划分结果是否是第r小的数，若不是则递归调用rank_select重新选择
    /* partition around median of medians and recursively select if necessary */
    j = partition_array(array, n, med);    //划分数组，返回med在新数组中的索引
    if (r == j)   //结果是第r小的数
        return med;
    else if (r < j)   //第r小的数在前半部分
        return rank_select(array, j, r);
    else//第r小的数在后半部分
    {
        array += j + 1;
        return rank_select(array, (n - j - 1), (r - j - 1));
    }
}


/*  用插入法对输入数组进行升序排序
    参数：
    array：输入数组
    n：元素个数
*/
/*
    Sorts an array in place into increasing order using insertion sort.

    @param array an array
    @param n number of elements
*/
static void insertion_sort(double* array, int n)
{
    double k;
    int i, j;
    for (i = 1; i < n; i++)
    {
        k = array[i];
        j = i - 1;
        while (j >= 0 && array[j] > k)
        {
            array[j + 1] = array[j];
            j -= 1;
        }
        array[j + 1] = k;
    }
}


/*  根据给定的枢轴值分割数组，使数组前部分小于pivot，后部分大于pivot
    参数：
    array：输入数组
    n：数组的元素个数
    pivot：枢轴值
    返回值：分割后枢轴的下标
*/
/*
    Partitions an array around a specified value.

    @param array an array
    @param n number of elements
    @param pivot value around which to partition

    @return Returns index of the pivot after partitioning
*/
static int partition_array(double* array, int n, double pivot)
{
    double tmp;
    int p, i, j;
    i = -1;
    for (j = 0; j < n; j++)
        if (array[j] <= pivot)
        {
            tmp = array[++i];
            array[i] = array[j];
            array[j] = tmp;
            if (array[i] == pivot)
                p = i;//p保存枢轴的下标
        }
    //将枢轴和最后一个小于枢轴的数对换
    array[p] = array[i];
    array[i] = pivot;
    return i;
}


/*  在指定的k-d树节点上划分特征点集
    使得特征点集的前半部分是第ki维小于枢轴的点，后半部分是第ki维大于枢轴的点
*/
/*
    Partitions the features at a specified kd tree node to create its two
    children.

    @param kd_node a kd tree node whose partition key is set
*/
static void partition_features(struct kd_node* kd_node)
{
    struct feature* features, tmp;
    double kv;
    int n, ki, p, i, j = -1;
    features = kd_node->features;//特征点数组
    n = kd_node->n;//特征点个数
    //printf("%d\n",n);
    ki = kd_node->ki;//枢轴的维数索引(哪一维是枢轴)
    kv = kd_node->kv;//枢轴的值
    for (i = 0; i < n; i++)
    {
        //若第i个特征点的特征向量的第ki维的值小于kv
        if (features[i].descr[ki] <= kv)
        {
            tmp = features[++j];
            features[j] = features[i];
            features[i] = tmp;
            if (features[j].descr[ki] == kv)
                p = j;//p保存枢轴所在的位置
        }
    }
    //将枢轴features[p]和最后一个小于枢轴的点features[j]对换
    tmp = features[p];
    features[p] = features[j];
    features[j] = tmp;
    //此后，枢轴的位置下标为j
    //若所有特征点落在同一侧，则此节点成为叶节点
    /* if all records fall on same side of partition, make node a leaf */
    if (j == n - 1)
    {
        kd_node->leaf = 1;
        return;
    }
    //初始化左孩子的根节点，左孩子共j+1个特征点
    kd_node->kd_left = kd_node_init(features, j + 1);
    //初始化右孩子的根节点，右孩子共n-j-1个特征点
    kd_node->kd_right = kd_node_init(features + (j + 1), (n - j - 1));
}


/*  从给定结点搜索k-d树直到叶节点，搜索过程中将未搜索的节点根据优先级放入队列
    优先级队列和搜索路径是同时生成的，这也是BBF算法的精髓所在：在二叉搜索的时
    候将搜索路径另一侧的分支加入到优先级队列中，供回溯时查找。而优先级队列的排
    序就是根据目标特征与分割超平面的距离ABS(kv - feat->descr[ki])
    参数：
    kd_node：要搜索的子树的树根
    feat：目标特征点
    min_pq：优先级队列
    返回值：叶子节点的指针
*/
/*
    Explores a kd tree from a given node to a leaf.  Branching decisions are
    made at each node based on the descriptor of a given feature.  Each node
    examined but not explored is put into a priority queue to be explored
    later, keyed based on the distance from its partition key value to the
    given feature's desctiptor.

    @param kd_node root of the subtree to be explored
    @param feat feature upon which branching decisions are based
    @param min_pq a minimizing priority queue into which tree nodes are placed
    as described above

    @return Returns a pointer to the leaf node at which exploration ends or
    NULL on error.
*/
static struct kd_node* explore_to_leaf(struct kd_node* kd_node,
                                       struct feature* feat,
                                       struct min_pq* min_pq)
{
    //unexpl中存放着优先级队列的候选节点(还未搜索的节点)，expl为当前搜索节点
    struct kd_node* unexpl, *expl = kd_node;
    double kv;//分割枢轴的值
    int ki;//分割枢轴的维数索引
    //一直搜索到叶子节点，搜索过程中将未搜索的节点根据优先级放入队列
    while (expl  &&  !expl->leaf)
    {
        ki = expl->ki;
        kv = expl->kv;
        //枢轴的维数索引大于特征点的维数，出错
        if (ki >= feat->d)
        {
            fprintf(stderr, "Warning: comparing imcompatible descriptors, %s" " line %d\n", __FILE__, __LINE__);
            return NULL;
        }
        //目标特征点ki维的数据小于等于kv，进入左子树搜索
        if (feat->descr[ki] <= kv)
        {
            unexpl = expl->kd_right;//候选搜索节点是expl的右子树的根
            expl = expl->kd_left;//当前搜索节点是expl的左子树的根
        }
        else//目标特征点ki维的数据大于kv，进入右子树搜索
        {
            unexpl = expl->kd_left;//候选搜索节点是expl的左子树
            expl = expl->kd_right;//当前搜索节点是expl的右子树
        }
        //将候选节点unexpl根据目标特征点ki维与其父节点的距离插入到优先队列中，距离越小，优先级越大
        if (minpq_insert(min_pq, unexpl, ABS(kv - feat->descr[ki])))
        {
            fprintf(stderr, "Warning: unable to insert into PQ, %s, line %d\n", __FILE__, __LINE__);
            return NULL;
        }
    }
    return expl;//返回叶子节点的指针
}


/*  插入一个特征点到最近邻数组，使数组中的点按到目标点的距离升序排列
    参数：
    feat：要插入的特征点，其feature_data域应是指向bbf_data结构的指针，其中的d值时feat和目标点的距离的平方
    nbrs：最近邻数组
    n：已在最近邻数组中的元素个数
    k：最近邻数组元素个数的最大值
    返回值：若feat成功插入，返回1，否则返回0
*/
/*
    Inserts a feature into the nearest-neighbor array so that the array remains
    in order of increasing descriptor distance from the search feature.

    @param feat feature to be inderted into the array; it's feature_data field
    should be a pointer to a bbf_data with d equal to the squared descriptor
    distance between feat and the search feature
    @param nbrs array of nearest neighbors neighbors
    @param n number of elements already in nbrs and
    @param k maximum number of elements in nbrs

    @return If feat was successfully inserted into nbrs, returns 1; otherwise
    returns 0.
*/
static int insert_into_nbr_array(struct feature* feat, struct feature** nbrs,
                                 int n, int k)
{
    struct bbf_data* fdata, *ndata;//fdata是要插入的点的bbf结构，ndata是最近邻数组中的点的bbf结构
    double dn, df; //dn是最近邻数组中特征点的bbf结构中的距离值，df是要插入的特征点的bbf结构中的距离值
    int i, ret = 0;
    //原最近邻数组为空
    if (n == 0)
    {
        nbrs[0] = feat;
        return 1;//插入成功，返回1
    }
    /* check at end of array */
    fdata = (struct bbf_data*) feat->feature_data;    //要插入点的bbf结构
    df = fdata->d;//要插入的特征点的bbf结构中的距离值
    ndata = (struct bbf_data*) nbrs[n - 1]->feature_data;    //最近邻数组中的点的bbf结构
    dn = ndata->d;//最近邻数组中最后一个特征点的bbf结构中的距离值
    //df>=dn，说明要插入在最近邻数组的末尾
    if (df >= dn)
    {
        //最近邻数组中元素个数已达到最大值，无法插入
        if (n == k)
        {
            feat->feature_data = fdata->old_data;//不明白这里是干什么？
            free(fdata);
            return 0;//插入失败，返回0
        }
        nbrs[n] = feat;//插入到末尾
        return 1;//插入成功，返回1
    }
    //运行到此处说明插入位置不在数组末尾
    /* find the right place in the array */
    if (n < k)   //最近邻数组中元素个数小于最大值，可插入
    {
        nbrs[n] = nbrs[n - 1];//原数组最后一个点后移
        ret = 1;//插入结果设为1
    }
    else//最近邻数组中元素个数大于或等于最大值，无法插入，插入结果ret还是0
    {
        //其实也不是无法插入，而是最近邻数组中元素个数不变，但值会更新
        nbrs[n - 1]->feature_data = ndata->old_data;
        free(ndata);
    }
    i = n - 2;
    //在最近邻数组中查找要插入的位置
    while (i >= 0)
    {
        ndata = (struct bbf_data*) nbrs[i]->feature_data;
        dn = ndata->d;
        if (dn <= df)   //找到插入点
            break;
        nbrs[i + 1] = nbrs[i];//一次后移
        i--;
    }
    i++;
    nbrs[i] = feat;//插入
    return ret;//返回结果
}


/*  判断给定点是否在某矩形中
*/
/*
    Determines whether a given point lies within a specified rectangular region

    @param pt point
    @param rect rectangular region

    @return Returns 1 if pt is inside rect or 0 otherwise
*/
static int within_rect(CvPoint2D64f pt, CvRect rect)
{
    if (pt.x < rect.x || pt.y < rect.y)
        return 0;
    if (pt.x > rect.x + rect.width || pt.y > rect.y + rect.height)
        return 0;
    return 1;
}












//===============================================
static const char* ID = "$Id: getopt.c,v 1.2 2003/10/26 03:10:20 vindaci Exp $";

char* optarg = NULL;
int optind = 0;
int opterr = 1;
int optopt = '?';

static char** prev_argv = NULL;
static int prev_argc = 0;
static int argv_index = 0;
static int argv_index2 = 0;
static int opt_offset = 0;
static int dashdash = 0;
static int nonopt = 0;

static void increment_index()
{
    if (argv_index < argv_index2)
    {
        while (prev_argv[++argv_index] && prev_argv[argv_index][0] != '-'
               && argv_index < argv_index2 + 1);
    }
    else argv_index++;
    opt_offset = 1;
}

static int permute_argv_once()
{
    if (argv_index + nonopt >= prev_argc) return 1;
    else
    {
        char* tmp = prev_argv[argv_index];
        memmove(&prev_argv[argv_index], &prev_argv[argv_index + 1],
                sizeof(char**) * (prev_argc - argv_index - 1));
        prev_argv[prev_argc - 1] = tmp;
        nonopt++;
        return 0;
    }
}

int getopt(int argc, char** argv, char* optstr)
{
    int c = 0;
    if (prev_argv != argv || prev_argc != argc)
    {
        prev_argv = argv;
        prev_argc = argc;
        argv_index = 1;
        argv_index2 = 1;
        opt_offset = 1;
        dashdash = 0;
        nonopt = 0;
    }
getopt_top:
    optarg = NULL;
    if (argv[argv_index] && !strcmp(argv[argv_index], "--"))
    {
        dashdash = 1;
        increment_index();
    }
    if (argv[argv_index] == NULL)
        c = -1;
    else if (dashdash || argv[argv_index][0] != '-' || !strcmp(argv[argv_index], "-"))
    {
        if (optstr[0] == '-')
        {
            c = 1;
            optarg = argv[argv_index];
            increment_index();
        }
        else if (optstr[0] == '+' || getenv("POSIXLY_CORRECT"))
        {
            c = -1;
            nonopt = argc - argv_index;
        }
        else
        {
            if (!permute_argv_once()) goto getopt_top;
            else c = -1;
        }
    }
    else
    {
        char* opt_ptr = NULL;
        c = argv[argv_index][opt_offset++];
        if (optstr[0] == '-') opt_ptr = strchr(optstr + 1, c);
        else opt_ptr = strchr(optstr, c);
        if (!opt_ptr)
        {
            if (opterr)
                fprintf(stderr, "%s: invalid option -- %c\n", argv[0], c);
            optopt = c;
            c = '?';
            increment_index();
        }
        else if (opt_ptr[1] == ':')
        {
            if (argv[argv_index][opt_offset] != '\0')
            {
                optarg = &argv[argv_index][opt_offset];
                increment_index();
            }
            else if (opt_ptr[2] != ':')
            {
                if (argv_index2 < argv_index) argv_index2 = argv_index;
                while (argv[++argv_index2] && argv[argv_index2][0] == '-');
                optarg = argv[argv_index2];
                if (argv_index2 + nonopt >= prev_argc) optarg = NULL;
                increment_index();
            }
            else
                increment_index();
            if (optarg == NULL && opt_ptr[2] != ':')
            {
                optopt = c;
                c = '?';
                if (opterr)
                {
                    fprintf(stderr, "%s: option requires an argument -- %c\n",
                            argv[0], optopt);
                }
            }
        }
        else
        {
            if (argv[argv_index][opt_offset] == '\0')
                increment_index();
        }
    }
    if (c == -1)
        optind = argc - nonopt;
    else
        optind = argv_index;
    return c;
}
