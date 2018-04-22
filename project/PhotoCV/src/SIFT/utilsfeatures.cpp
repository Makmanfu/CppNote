/*
    Miscellaneous utility functions.

    Copyright (C) 2006-2012  Rob Hess <rob@iqengines.com>

    @version 1.1.2-20100521
    /*
    ���ļ���ʵ����ͼ��Ļ�����������:
    1����ȡĳλ�õ����ص�
    2������ĳλ�õ����ص㣨8λ��32λ��64λ����
    3����������֮��ľ����ƽ��
    4����ͼƬĳһ�㻭һ����X��
    5��������ͼƬ�ϳ�Ϊһ�������Ƕ���֮�ͣ����Ƕ��ߵĽϴ��ߡ�img1 �����Ͻǣ�img2�����½ǡ�
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


/*  ������ͼ��ϳ�Ϊһ�ţ���ֱ�ŷ�
    ������img1��λ���Ϸ���ͼ���ָ�룬img2��λ���·���ͼ���ָ��
    ����ֵ���ϳ�ͼ��
*/
/*
    Combines two images by scacking one on top of the other

    @param img1 top image
    @param img2 bottom image

    @return Returns the image resulting from stacking \a img1 on top if \a img2
*/
extern IplImage* stack_imgs(IplImage* img1, IplImage* img2)
{
    //���ɺϳ�ͼ��
    //IplImage* stacked = cvCreateImage(cvSize(MAX(img1->width, img2->width),
    //    img1->height + img2->height),
    //    IPL_DEPTH_8U, 3);
    //cvZero(stacked);
    //cvSetImageROI(stacked, cvRect(0, 0, img1->width, img1->height));
    //cvAdd(img1, stacked, stacked, NULL);
    ////����
    //cvSetImageROI(stacked, cvRect(0, img1->height, img2->width, img2->height));
    //cvAdd(img2, stacked, stacked, NULL);
    //cvResetImageROI(stacked);
    //return stacked;
    //����
    IplImage* stacked = cvCreateImage(cvSize(img1->width + img2->width,
                                      MAX(img1->height, img2->height)),
                                      IPL_DEPTH_8U, 3);
    cvZero(stacked);
    cvSetImageROI(stacked, cvRect(0, 0, img1->width, img1->height));
    cvAdd(img1, stacked, stacked, NULL);
    //����
    cvSetImageROI(stacked, cvRect(img1->width, 0, img2->width, img2->height));
    cvAdd(img2, stacked, stacked, NULL);
    cvResetImageROI(stacked);
    return stacked;
}

/*  (�Լ�д�ĺ���)
    ������ͼ��ϳ�Ϊһ�ţ�ˮƽ�ŷ�
    ������img1��λ����ߵ�ͼ���ָ�룬img2��λ���ұߵ�ͼ���ָ��
    ����ֵ���ϳ�ͼ��
*/
extern IplImage* stack_imgs_horizontal(IplImage* img1, IplImage* img2)
{
    //���ɺϳ�ͼ��
    IplImage* stacked = cvCreateImage(cvSize(img1->width + img2->width, MAX(img1->height, img2->height)),
                                      IPL_DEPTH_8U, 3);
    cvZero(stacked);    //����
    cvSetImageROI(stacked, cvRect(0, 0, img1->width, img1->height));
    cvAdd(img1, stacked, stacked, NULL);    //���ӵ�һ��ͼ��
    cvSetImageROI(stacked, cvRect(img1->width, 0, img2->width, img2->height));
    cvAdd(img2, stacked, stacked, NULL);    //���ӵڶ���ͼ��
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
    ���ļ��а�����С���ȶ��е�ʵ��
*/

//���غ���
/************************* Local Function Prototypes *************************/

static void restore_minpq_order(struct pq_node*, int, int);
static void decrease_pq_node_key(struct pq_node*, int, int);


//��������
/************************** Local Inline Functions ***************************/

//����i�ڵ�ĸ��ڵ���±�
/* returns the array index of element i's parent */
static inline int parent(int i)
{
    return (i - 1) / 2;
}

//����i�ڵ���Һ��ӵ��±�
/* returns the array index of element i's right child */
static inline int right(int i)
{
    return 2 * i + 2;
}

//����i�ڵ�����ӵ��±�
/* returns the array index of element i's left child */
static inline int left(int i)
{
    return 2 * i + 1;
}


/********************** Functions prototyped in minpq.h **********************/

/*  ��ʼ����С���ȼ����У�����Ĭ�ϴ�С�Ŀռ�
*/
/*
    Creates a new minimizing priority queue.
*/
struct min_pq* minpq_init()
{
    struct min_pq* min_pq;
    min_pq = (struct min_pq*) malloc(sizeof(struct min_pq));
    //Ĭ�Ϸ���MINPQ_INIT_NALLOCD���ռ�
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
    ���ļ��а����ĺ���ʵ���У�
    1��RANSAC�㷨����ƥ��ɸѡ
    2������һЩ��RANSAC�㷨��غ���
*/

/************************ δ��¶�ӿڵ�һЩ���غ��������� **************************/
/************************* Local Function Prototypes *************************/

//���ݸ�����ƥ�����ͣ���������������feat��ƥ���
static inline struct feature* get_match(struct feature*, int);
//�ҵ����о���mtype����ƥ���������㣬�����ǵ�ָ��洢��matched������
static int get_matched_features(struct feature*, int, int, struct feature***);
//���㱣֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
static int calc_min_inliers(int, int, double, double);
//����n�Ľ׳˵���Ȼ����
static inline double log_factorial(int);
//�Ӹ����������㼯�������ѡһ��RANSAC����(��һ��4�������������)
static struct feature** draw_ransac_sample(struct feature**, int, int);
//�������������л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
static void extract_corresp_pts(struct feature**, int, int, CvPoint2D64f**, CvPoint2D64f**);
//���ڸ�����ģ�ͺʹ�������������������㼯�����ҳ�һ�¼�
static int find_consensus(struct feature**, int, int, CvMat*, ransac_err_fn, double, struct feature***);
//�ͷ���������Ĵ洢�ռ�
static inline void release_mem(CvPoint2D64f*, CvPoint2D64f*, struct feature**);

/********************** Functions prototyped in model.h **********************/


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
    inliers�����������ָ�����飬ָ�����������յ��ڵ㼯�ϣ���Ϊ�գ���ʾû���������Ҫ���һ�¼�
    ��������ڴ潫�ڱ������б����䣬ʹ���������ڵ��ó��ͷţ�free(*inliers)
    n_in��������������ռ�������ڵ����Ŀ
    ����ֵ��RANSAC�㷨������ı任������Ϊ�գ���ʾ������޷�������ɽ��ܾ���
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
    //matched�����о���mtype����ƥ��������������飬Ҳ����������
    //sample��������������4�������������
    //consensus����ǰһ�¼���
    //consensus_max����ǰ���һ�¼�(����ǰ����ý����һ�¼�)
    struct feature** matched, ** sample, ** consensus, ** consensus_max = NULL;
    struct ransac_data* rdata;//ÿ���������feature_data���ransac����ָ��
    CvPoint2D64f* pts, *mpts;//ÿ��������Ӧ�������������飺��������������pts��ƥ�����������mpts
    CvMat* M = NULL;//��ǰ�任����
    //p����ǰ�������ģ�͵Ĵ�����ʣ���pС��p_badxformʱ����ֹͣ
    //in_frac���ڵ���Ŀռ��������Ŀ�İٷֱ�
    double p, in_frac = RANSAC_INLIER_FRAC_EST;
    //nm������������������о���mtype����ƥ�������������
    //in����ǰһ�¼���Ԫ�ظ���
    //in_min��һ�¼���Ԫ�ظ����������Сֵ����֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
    //in_max����ǰ����һ�¼�(���һ�¼�)��Ԫ�صĸ���
    //k����������������㵱ǰģ�͵Ĵ�������й�
    int i, nm, in, in_min, in_max = 0, k = 0;
    //�ҵ�����������features�����о���mtype����ƥ���������㣬�ŵ�matched����(������)�У�����ֵnm��matched�����Ԫ�ظ���
    nm = get_matched_features(features, n, mtype, &matched);
    //���ҵ��ľ���ƥ�������������С�ڼ���任������Ҫ����С������Ը���������
    if (nm < m)
    {
        //������������Ը�������
        fprintf(stderr, "Warning: not enough matches to compute xform, %s" \
                " line %d\n", __FILE__, __LINE__);
        goto end;
    }
    /* initialize random number generator */
    srand(time(NULL));       //��ʼ�������������  linux srandom
    //���㱣֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
    in_min = calc_min_inliers(nm, m, RANSAC_PROB_BAD_SUPP, p_badxform);
    //��ǰ�������ģ�͵Ĵ������,�ڵ���ռ����in_fracԽ�󣬴������ԽС����������kԽ�󣬴������ԽС
    p = pow(1.0 - pow(in_frac, m), k);
    i = 0;
    //��ǰ������ʴ������������������p_badxform����������
    while (p > p_badxform)
    {
        //��������matched�������ѡһ��RANSAC����(��һ��4�������������)���ŵ���������sample��
        sample = draw_ransac_sample(matched, nm, m);
        //�������л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
        extract_corresp_pts(sample, m, mtype, &pts, &mpts);
        //���ò����д���ĺ���xform_fn�����㽫m���������pts�任Ϊmpts�ľ��󣬷��ر任�����M
        M = xform_fn(pts, mpts, m);    //һ�㴫��lsq_homog()����
        if (!M)   //�����ж�
            goto iteration_end;
        //���������㼯���任�����������������ǰһ�¼�consensus������һ�¼���Ԫ�ظ�����in
        in = find_consensus(matched, nm, mtype, M, err_fn, err_tol, &consensus);
        //����ǰһ�¼�������ʷ����һ�¼�������ǰһ�¼�Ϊ���ţ����������һ�¼�consensus_max
        if (in > in_max)
        {
            if (consensus_max)   //��֮ǰ������ֵ���ͷ���ռ�
                free(consensus_max);
            consensus_max = consensus;//��������һ�¼�
            in_max = in;//��������һ�¼���Ԫ�ظ���
            in_frac = (double) in_max / nm;   //����һ�¼���Ԫ�ظ���ռ�����ܸ����İٷֱ�
        }
        else//����ǰһ�¼�С����ʷ����һ�¼����ͷŵ�ǰһ�¼�
            free(consensus);
        cvReleaseMat(&M);
iteration_end:
        release_mem(pts, mpts, sample);
        p = pow(1.0 - pow(in_frac, m), ++k);       //���µ�ǰ�������
    }
    //��������һ�¼��������յı任����
    /* calculate final transform based on best consensus set */
    //������һ�¼���Ԫ�ظ���������ͱ�׼��������Ҫ��
    if (in_max >= in_min)
    {
        //������һ�¼��л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
        extract_corresp_pts(consensus_max, in_max, mtype, &pts, &mpts);
        //���ò����д���ĺ���xform_fn�����㽫in_max���������pts�任Ϊmpts�ľ��󣬷��ر任�����M
        M = xform_fn(pts, mpts, in_max);
        /***********������ٽ���һ�ε���**********/
        //���ݱ任����M��������matched�м����һ�¼�consensus������һ�¼���Ԫ�ظ�����in
        in = find_consensus(matched, nm, mtype, M, err_fn, err_tol, &consensus);
        cvReleaseMat(&M);
        release_mem(pts, mpts, consensus_max);
        //��һ�¼��л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
        extract_corresp_pts(consensus, in, mtype, &pts, &mpts);
        //���ò����д���ĺ���xform_fn�����㽫in���������pts�任Ϊmpts�ľ��󣬷��ر任�����M
        M = xform_fn(pts, mpts, in);
        if (inliers)
        {
            *inliers = consensus;//������һ�¼���ֵ�����������inliers�����ڵ㼯��
            consensus = NULL;
        }
        if (n_in)
            *n_in = in;//������һ�¼���Ԫ�ظ�����ֵ�����������n_in�����ڵ����
        release_mem(pts, mpts, consensus);
    }
    else if (consensus_max)
    {
        //û�м��������Ҫ���һ�¼�
        if (inliers)
            *inliers = NULL;
        if (n_in)
            *n_in = 0;
        free(consensus_max);
    }
    //RANSAC�㷨�������ָ��������б����ĵ�������feature_data�������ر任����M
end:
    for (i = 0; i < nm; i++)
    {
        //���ú�feat_ransac_data����ȡmatched[i]�е�feature_data��Ա��ת��Ϊransac_data��ʽ��ָ��
        rdata = feat_ransac_data(matched[i]);
        //�ָ�feature_data��Ա����ǰ��ֵ
        matched[i]->feature_data = rdata->orig_feat_data;
        free(rdata);    //�ͷ��ڴ�
    }
    free(matched);
    return M;//��������ı任����M
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


/*  ����4������������С����ƽ�浥Ӧ�Ա任����
    ������
    pts�����������
    mpts����Ӧ�����飬pts[i]��mpts[i]һһ��Ӧ
    n��pts��mpts�����е�ĸ�����pts��mpts�е�ĸ���������ͬ��һ����4
    ����ֵ��һ��3*3�ı任���󣬽�pts�е�ÿһ����ת��Ϊmpts�еĶ�Ӧ�㣬����ֵΪ�ձ�ʾʧ��
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
    double x[9];//����x�е�Ԫ�ؾ��Ǳ任����H�е�ֵ
    int i;
    //�����Ը�������4
    if (n < 4)
    {
        fprintf(stderr, "Warning: too few points in lsq_homog(), %s line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }
    //���任����Hչ����һ��8ά������X�У�ʹ��AX=B������ֻ��һ�ν����Է����鼴�����X��Ȼ���ٸ���X�ָ�H
    /* set up matrices so we can unstack homography into X; AX = B */
    A = cvCreateMat(2 * n, 8, CV_64FC1);    //����2n*8�ľ���һ����8*8
    B = cvCreateMat(2 * n, 1, CV_64FC1);    //����2n*1�ľ���һ����8*1
    X = cvMat(8, 1, CV_64FC1, x);    //����8*1�ľ���ָ������Ϊx
    H = cvCreateMat(3, 3, CV_64FC1);    //����3*3�ľ���
    cvZero(A);    //��A����
    //������չ�����㣬��Ҫ����ԭ���ľ�����㷨�����·������A��B��ֵ������
    for (i = 0; i < n; i++)
    {
        cvmSet(A, i, 0, pts[i].x);    //���þ���A��i��0�е�ֵΪpts[i].x
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
    //����OpenCV�����������Է�����
    cvSolve(A, B, &X, CV_SVD);    //��X��ʹ��AX=B
    x[8] = 1.0;//�任�����[3][3]λ�õ�ֵΪ�̶�ֵ1
    X = cvMat(3, 3, CV_64FC1, x);
    cvConvert(&X, H);    //������ת��Ϊ����
    cvReleaseMat(&A);
    cvReleaseMat(&B);
    return H;
}


/*  ���ڸ����ĵ�Ӧ�Ծ���H�����������pt��H�任��ĵ�����ƥ���mpt֮������
    ���磺������x�����Ӧ��x'����Ӧ�Ծ���H�������x'��Hx֮��ľ����ƽ����d(x', Hx)^2
    ������
    pt��һ����
    mpt��pt�Ķ�Ӧƥ���
    H����Ӧ�Ծ���
    ����ֵ��ת�����
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
    CvPoint2D64f xpt = persp_xform_pt(pt, H);    //pt���任����H�任��ĵ�xpt,��H����x��Ӧ������
    return sqrt(dist_sq_2D(xpt, mpt));
}


/*  �����pt��͸�ӱ任��ĵ㣬������һ��pt��͸�ӱ任����T������任��ĵ�
    ������(x,y)���任����M������[x',y',w']^T = M * [x,y,1]^T(^T��ʾת��)��
    ��任��ĵ���(u,v) = (x'/w', y'/w')
    ע�⣺����任��͸�ӱ任������
    ������
    pt��һ����ά��
    T��͸�ӱ任����
    ����ֵ��pt��͸�ӱ任��ĵ�
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
    //XY����pt��Ӧ��3*1��������UV��pt�任��ĵ��Ӧ��3*1������
    CvMat XY, UV;
    double xy[3] = { pt.x, pt.y, 1.0 }, uv[3] = { 0 };//��Ӧ������
    CvPoint2D64f rslt;//���
    //��ʼ������ͷ
    cvInitMatHeader(&XY, 3, 1, CV_64FC1, xy, CV_AUTOSTEP);
    cvInitMatHeader(&UV, 3, 1, CV_64FC1, uv, CV_AUTOSTEP);
    cvMatMul(T, &XY, &UV);    //�������˷���T*XY,�������UV��
    rslt = cvPoint2D64f(uv[0] / uv[2], uv[1] / uv[2]);    //�õ�ת����ĵ�
    return rslt;
}


/************************ ���غ�����ʵ��**************************************/
/************************ Local funciton definitions *************************/

/*  ���ݸ�����ƥ�����ͣ���������������feat��ƥ���
    ������
    feat������������
    mtype��ƥ�����ͣ���FEATURE_FWD_MATCH��FEATURE_BCK_MATCH��FEATURE_MDL_MATCH֮һ
    ����ֵ��feat��ƥ����ָ�룬��Ϊ�ձ�ʾmtype��������
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
    //FEATURE_MDL_MATCH������feature�ṹ�е�mdl_match���Ƕ�Ӧ��ƥ���
    if (mtype == FEATURE_MDL_MATCH)
        return feat->mdl_match;
    //FEATURE_BCK_MATCH������feature�ṹ�е�bck_match���Ƕ�Ӧ��ƥ���
    if (mtype == FEATURE_BCK_MATCH)
        return feat->bck_match;
    //FEATURE_FWD_MATCH������feature�ṹ�е�fwd_match���Ƕ�Ӧ��ƥ���
    if (mtype == FEATURE_FWD_MATCH)
        return feat->fwd_match;
    return NULL;
}


/*  �ҵ����о���mtype����ƥ���������㣬�����ǵ�ָ��洢��matched�����У�
    ����ʼ��matched������ÿ���������feature_data��Ϊransac_data���͵�����ָ��
    ������
    features������������
    n�����������
    mtype��ƥ������
    matched���������������mtype����ƥ�����������ָ������
    ����ֵ��matched������������ĸ���
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
    struct feature** _matched;//������飬����mtype����ƥ����������ָ������
    struct ransac_data* rdata;//ransac_data��������ָ��
    int i, m = 0;
    _matched = (struct feature**) calloc(n, sizeof(struct feature*));
    //�������������������
    for (i = 0; i < n; i++)
    {
        //�ҵ�i���������mtype����ƥ��㣬������ȷ�ҵ���������������mtype���͵�ƥ��㣬�������_matched����
        if (get_match(features + i, mtype))
        {
            rdata = (struct ransac_data*) malloc(sizeof(struct ransac_data));          //Ϊransac_data�ṹ����ռ�
            memset(rdata, 0, sizeof(struct ransac_data));       //����
            rdata->orig_feat_data = features[i].feature_data;//�����i���������feature_data��֮ǰ��ֵ
            _matched[m] = features + i;//�ŵ�_matched����
            _matched[m]->feature_data = rdata;//��feature_data��ֵΪransac_data�������ݵ�ָ��
            m++;//_matched������Ԫ�ظ���
        }
    }
    *matched = _matched;//���������ֵ
    return m;//����ֵ��Ԫ�ظ���
}


/*  ���㱣֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
    ������
    n���ƶ���ƥ���Եĸ���
    m������ģ���������С��Ը���
    p_badsupp�����ʣ�����ģ�ͱ�һ��ƥ����֧�ֵĸ���
    p_badxform�����ʣ����ռ������ת�������Ǵ���ĵĸ���
    ����ֵ����֤RANSAC���ռ������ת���������ĸ���С��p_badxform�������С�ڵ���Ŀ
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
    //�������ģ�Chum, O. and Matas, J.  Matching with PROSAC -- Progressive Sample Consensus
    //�е�һ����ʽ���㣬������
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


//����n�Ľ׳˵���Ȼ����
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


/*  �Ӹ����������㼯�������ѡһ��RANSAC����(��һ��4�������������)
    ������
    features����Ϊ������������������
    n��features��Ԫ�ظ���
    m�����������ĳߴ磬������4(������Ҫ4����������任����)
    ����ֵ��һ��ָ�����飬��Ԫ��ָ��ѡΪ�����������㣬��ѡΪ�������������feature_data���sampled����Ϊ1
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
    struct feature** sample, *feat;//sample����ѡΪ�����ĵ������
    struct ransac_data* rdata;
    int i, x;
    //�������������feature_data���sampledֵ����ʼ��Ϊ0����δ��ѡΪ������
    for (i = 0; i < n; i++)
    {
        //���ú�feat_ransac_data����ȡ�����е�feature_data��Ա��ת��Ϊransac_data��ʽ��ָ��
        rdata = feat_ransac_data(features[i]);
        rdata->sampled = 0;
    }
    sample = (struct feature**) calloc(m, sizeof(struct feature*));           //Ϊ�����������ռ�
    //�����ȡm����������Ϊһ������������ָ�뱣����sample������
    for (i = 0; i < m; i++)
    {
        do
        {
            //linux  x = random() % n;
            x = rand() % n; //����±�
            feat = features[x];
            rdata = feat_ransac_data(feat);    //���feature_data��Ա��ת��Ϊransac_data��ʽ��ָ��
        }
        while (rdata->sampled);  //����ȡ���������sampledֵΪ1������ѡȡ������ֹͣ��������Ϊ�����е�һ����
        sample[i] = feat;//����sample����
        rdata->sampled = 1;//�õ��feature_data��Ա��sampled��ֵ��Ϊ1
    }
    return sample;//�������ѡȡ������
}


/*  �������������л�ȡ����������Ӧƥ���Ķ�ά���꣬�ֱ�ŵ��������pts��mpts��
    ������
    features�����������飬�������г�ȡ��������ƥ��㣬�����������������㶼Ӧ����mtype���͵�ƥ���
    n��feantures�����������
    mtype��ƥ�����ͣ�����FEATURE_MDL_MATCH����Ӧ��ƥ����������ÿ���������img_pt�����ƥ����mdl_pt��
    ���򣬶�Ӧ��ƥ������ÿ���������img_pt�����ƥ����img_pt��
    pts�����������������������features�л�ȡ�Ķ�ά��������
    mpts�����������������������features�Ķ�Ӧƥ����л�ȡ�Ķ�ά��������
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
    struct feature* match;//ÿ���������Ӧ��ƥ���
    CvPoint2D64f* _pts, *_mpts;
    int i;
    _pts = (CvPoint2D64f*) calloc(n, sizeof(CvPoint2D64f));          //���������������
    _mpts = (CvPoint2D64f*) calloc(n, sizeof(CvPoint2D64f));          //��Ӧƥ������������
    //ƥ��������FEATURE_MDL_MATCH��ƥ����������mdl_pt��
    if (mtype == FEATURE_MDL_MATCH)
        for (i = 0; i < n; i++)
        {
            //���ݸ�����ƥ�����ͣ����������������ƥ���
            match = get_match(features[i], mtype);
            if (!match)
                fatal_error("feature does not have match of type %d, %s line %d",
                            mtype, __FILE__, __LINE__);
            _pts[i] = features[i]->img_pt;//�����������
            _mpts[i] = match->mdl_pt;//��Ӧƥ��������
        }
    //ƥ�����Ͳ���FEATURE_MDL_MATCH��ƥ����������img_pt��
    else
        for (i = 0; i < n; i++)
        {
            //���ݸ�����ƥ�����ͣ����������������ƥ���
            match = get_match(features[i], mtype);
            if (!match)
                fatal_error("feature does not have match of type %d, %s line %d",
                            mtype, __FILE__, __LINE__);
            _pts[i] = features[i]->img_pt;//�����������
            _mpts[i] = match->img_pt;//��Ӧƥ��������
        }
    *pts = _pts;
    *mpts = _mpts;
}



/*  ���ڸ�����ģ�ͺʹ�������������������㼯�����ҳ�һ�¼�
    ������
    features�������㼯�ϣ����е������㶼����mtype���͵�ƥ���
    n��������ĸ���
    mtype��ƥ�����ͣ�����FEATURE_MDL_MATCH����Ӧ��ƥ����������ÿ���������img_pt�����ƥ����mdl_pt��
    ���򣬶�Ӧ��ƥ������ÿ���������img_pt�����ƥ����img_pt��
    M��������ģ�ͣ���һ���任����
    err_fn������������������ڸ����ı任���󣬼����ƶ���ƥ����֮��ı任���
    err_tol���ݴ�ȣ���������err_fn�ķ���ֵ��С��err_tol�ı�����һ�¼�
    consensus�����������һ�¼�����һ�¼��������㹹�ɵ�����
    ����ֵ��һ�¼���������ĸ���
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
    struct feature** _consensus;//һ�¼�
    struct feature* match;//ÿ���������Ӧ��ƥ���
    CvPoint2D64f pt, mpt;//pt:��ǰ����������꣬mpt����ǰ�������ƥ��������
    double err;//�任���
    int i, in = 0;
    _consensus = (struct feature**) calloc(n, sizeof(struct feature*));           //��һ�¼�����ռ�
    //ƥ��������FEATURE_MDL_MATCH��ƥ����������mdl_pt��
    if (mtype == FEATURE_MDL_MATCH)
        for (i = 0; i < n; i++)
        {
            //���ݸ�����ƥ�����ͣ����������������ƥ���
            match = get_match(features[i], mtype);
            if (!match)
                fatal_error("feature does not have match of type %d, %s line %d",
                            mtype, __FILE__, __LINE__);
            pt = features[i]->img_pt;//�����������
            mpt = match->mdl_pt;//��Ӧƥ��������
            err = err_fn(pt, mpt, M);    //����"pt��M�任��ĵ�"��mpt֮��ľ����ƽ�������任���
            if (err <= err_tol)   //���任���С���ݴ�ȣ��������һ�¼�
                _consensus[in++] = features[i];
        }
    //ƥ�����Ͳ���FEATURE_MDL_MATCH��ƥ����������img_pt��
    else
        for (i = 0; i < n; i++)
        {
            //���ݸ�����ƥ�����ͣ����������������ƥ���
            match = get_match(features[i], mtype);
            if (!match)
                fatal_error("feature does not have match of type %d, %s line %d",
                            mtype, __FILE__, __LINE__);
            pt = features[i]->img_pt;//�����������
            mpt = match->img_pt;//��Ӧƥ��������
            err = err_fn(pt, mpt, M);    //����"pt��M�任��ĵ�"��mpt֮��ľ����ƽ�������任���
            if (err <= err_tol)   //���任���С���ݴ�ȣ��������һ�¼�
                _consensus[in++] = features[i];
        }
    *consensus = _consensus;
    return in;//����һ�¼���Ԫ�ظ���
}


/*  �ͷ���������Ĵ洢�ռ�
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
    ���ļ����м���������ʵ�֣�������ĵ��뵼����������Ļ���
*/

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif

#define CV_RGB( r, g, b )  cvScalar( (b), (g), (r), 0 )

/************************ δ��¶�ӿڵ�һЩ���غ��������� **************************/
static int import_oxfd_features(char*, struct feature**);      //����OXFD��ʽ������
static int export_oxfd_features(char*, struct feature*, int);      //����OXFD��ʽ������
static void draw_oxfd_features(IplImage*, struct feature*, int);      //��OXFD��ʽ������
static void draw_oxfd_feature(IplImage*, struct feature*, CvScalar);      //��������

static int import_lowe_features(char*, struct feature**);      //����LOWE��ʽ������
static int export_lowe_features(char*, struct feature*, int);      //����LOWE��ʽ������
static void draw_lowe_features(IplImage*, struct feature*, int);      //��LOWE��ʽ������
static void draw_lowe_feature(IplImage*, struct feature*, CvScalar);      //��������


/*  ���ļ��ж���ͼ������
    �ļ��е��������ʽ������FEATURE_OXFD��FEATURE_LOWE��ʽ
    ������
    filename���ļ���
    type������������
    feat�������洢�������feature�����ָ��
    ����ֵ����������������
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
    //�������������ͣ����ò�ͬ�ĺ�����ɵ��빦��
    switch (type)
    {
        case FEATURE_OXFD:
            n = import_oxfd_features(filename, feat);    //���ú���������OXFD��ʽ������
            break;
        case FEATURE_LOWE:
            n = import_lowe_features(filename, feat);    //���ú���������LOWE��ʽ������
            break;
        default: //�������ʽ�޷�ʶ��
            fprintf(stderr, "Warning: import_features(): unrecognized feature" \
                    "type, %s, line %d\n", __FILE__, __LINE__);
            return -1;
    }
    //����ʧ��
    if (n == -1)
        fprintf(stderr, "Warning: unable to import features from %s,"   \
                " %s, line %d\n", filename, __FILE__, __LINE__);
    return n;
}


/*  ����feature���鵽�ļ�
    ������
    filename���ļ���
    feat����������
    n�����������
    ����ֵ��0���ɹ���1��ʧ��
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
    //�����Ϸ��Լ��
    if (n <= 0 || !feat)
    {
        fprintf(stderr, "Warning: no features to export, %s line %d\n",
                __FILE__, __LINE__);
        return 1;
    }
    type = feat[0].type;//����������͡�
    //�������������ͣ����ò�ͬ�ĺ�����ɵ�������
    switch (type)
    {
        case FEATURE_OXFD:
            r = export_oxfd_features(filename, feat, n);    //���ú���������OXFD��ʽ������
            break;
        case FEATURE_LOWE:
            r = export_lowe_features(filename, feat, n);    //���ú���������LOWE��ʽ������
            break;
        default:
            fprintf(stderr, "Warning: export_features(): unrecognized feature" \
                    "type, %s, line %d\n", __FILE__, __LINE__);
            return -1;
    }
    if (r)   //������������ֵ��0����ʾ����ʧ��
        fprintf(stderr, "Warning: unable to export features to %s," \
                " %s, line %d\n", filename, __FILE__, __LINE__);
    return r;
}


/*  ��ͼƬ�ϻ���������
    ������
    img��ͼ��
    feat������������
    n�����������
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
    //�����Ϸ��Լ��
    if (n <= 0 || !feat)
    {
        fprintf(stderr, "Warning: no features to draw, %s line %d\n",
                __FILE__, __LINE__);
        return;
    }
    type = feat[0].type;//�����������
    //�������������ͣ����ò�ͬ�ĺ�����ɻ�ͼ����
    switch (type)
    {
        case FEATURE_OXFD:
            draw_oxfd_features(img, feat, n);    //���ú�������ͼ���ϻ�OXFD��ʽ������
            break;
        case FEATURE_LOWE:
            draw_lowe_features(img, feat, n);    //���ú�������ͼ���ϻ�LOWE��ʽ������
            break;
        default:
            fprintf(stderr, "Warning: draw_features(): unrecognized feature" \
                    " type, %s, line %d\n", __FILE__, __LINE__);
            break;
    }
}


/*  �����������������Ӽ��ŷ�Ͼ����ƽ��
    ������
    f1:��һ��������
    f2:�ڶ���������
    ����ֵ��ŷ�Ͼ����ƽ��
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
    d = f1->d;//f1�����������ӵĳ���
    if (f2->d != d)   //��f1��f2�����������ӳ��Ȳ�ͬ������
        return DBL_MAX;
    descr1 = f1->descr;//f1�����������ӣ�һ��double����
    descr2 = f2->descr;//f2�����������ӣ�һ��double����
    //����ŷ�Ͼ����ƽ��������ӦԪ�صĲ��ƽ����
    for (i = 0; i < d; i++)
    {
        diff = descr1[i] - descr2[i];
        dsq += diff * diff;
    }
    return dsq;
}


/***************************** һЩδ��¶�ӿڵ��ڲ����� *******************************/
/***************************** Local Functions *******************************/


/*  ���ļ��ж���OXFD��ʽ��ͼ������
    ������
    filename���ļ���
    features�������洢�������feature�����ָ��
    ����ֵ����������������
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
    struct feature* f;//��һ���������ָ��
    int i, j, n, d;
    double x, y, a, b, c, dv;
    FILE* file;//�ļ�ָ��
    if (!features)
        fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);
    //���ļ�
    if (!(file = fopen(filename, "r")))
    {
        fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
                filename, __FILE__, __LINE__);
        return -1;
    }
    //��������������ά�������������
    /* read dimension and number of features */
    if (fscanf(file, " %d %d ", &d, &n) != 2)
    {
        fprintf(stderr, "Warning: file read error, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    //����������ά�����ڶ�������ά��������
    if (d > FEATURE_MAX_D)
    {
        fprintf(stderr, "Warning: descriptor too long, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    //�����ڴ棬n��feature�ṹ��С�������׵�ַ��f
    f = (struct feature*) calloc(n, sizeof(struct feature));
    //�����ļ��е�n��������
    for (i = 0; i < n; i++)
    {
        //��������������
        /* read affine region parameters */
        if (fscanf(file, " %lf %lf %lf %lf %lf ", &x, &y, &a, &b, &c) != 5)
        {
            fprintf(stderr, "Warning: error reading feature #%d, %s, line %d\n",
                    i + 1, __FILE__, __LINE__);
            free(f);    //����������ͷ��ڴ�
            return -1;
        }
        //����i�������㸳ֵ
        f[i].img_pt.x = f[i].x = x;//�������x����
        f[i].img_pt.y = f[i].y = y;//�������y����
        f[i].a = a;
        f[i].b = b;
        f[i].c = c;
        f[i].d = d;
        f[i].type = FEATURE_OXFD;//����������
        //��������������
        /* read descriptor */
        for (j = 0; j < d; j++)
        {
            if (!fscanf(file, " %lf ", &dv))
            {
                fprintf(stderr, "Warning: error reading feature descriptor" \
                        " #%d, %s, line %d\n", i + 1, __FILE__, __LINE__);
                free(f);    //����������ͷ��ڴ�
                return -1;
            }
            f[i].descr[j] = dv;//������i��������ĵ�j������������
        }
        //����һЩûʲô�õĲ���
        f[i].scl = f[i].ori = 0;//OXFD�������޴˲���
        f[i].category = 0;
        f[i].fwd_match = f[i].bck_match = f[i].mdl_match = NULL;
        f[i].mdl_pt.x = f[i].mdl_pt.y = -1;
        f[i].feature_data = NULL;
    }
    //�ر��ļ�
    if (fclose(file))
    {
        fprintf(stderr, "Warning: file close error, %s, line %d\n",
                __FILE__, __LINE__);
        free(f);    //����������ͷ��ڴ�
        return -1;
    }
    *features = f;//����һ���������ָ�븳��*feature
    return n;//���ض�������������
}


/*  ����OXFD��ʽ�������㼯���ļ�
    ������
    filename���ļ���
    feat����������
    n�����������
    ����ֵ��0���ɹ���1��ʧ��
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
    //���ļ�
    if (!(file = fopen(filename, "w")))
    {
        fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
                filename, __FILE__, __LINE__);
        return 1;
    }
    d = feat[0].d;//���������ӵ�ά��
    fprintf(file, "%d\n%d\n", d, n);    //����д�����������ӵ�ά�������������
    //����д��ÿ�����������Ϣ
    for (i = 0; i < n; i++)
    {
        //д������������
        fprintf(file, "%f %f %f %f %f", feat[i].x, feat[i].y, feat[i].a,
                feat[i].b, feat[i].c);
        //д��d�����������ӵ�Ԫ��
        for (j = 0; j < d; j++)
            fprintf(file, " %f", feat[i].descr[j]);
        fprintf(file, "\n");    //����
    }
    //�ر��ļ�
    if (fclose(file))
    {
        fprintf(stderr, "Warning: file close error, %s, line %d\n",
                __FILE__, __LINE__);
        return 1;
    }
    return 0;
}


/*  ��ͼ���ϻ���OXFD���͵�������
    ������
    img��ͼ��ָ��
    feat����������
    n����������
*/
/*
    Draws Oxford-type affine features

    @param img image on which to draw features
    @param feat array of Oxford-type features
    @param n number of features
*/
static void draw_oxfd_features(IplImage* img, struct feature* feat, int n)
{
    CvScalar color = CV_RGB(255, 255, 255);    //��ɫ
    int i;
    if (img->nChannels > 1)
        color = FEATURE_OXFD_COLOR;
    //���ú��������λ���ÿ��������
    for (i = 0; i < n; i++)
        draw_oxfd_feature(img, feat + i, color);
}


/*  ��ͼ���ϻ�����OXFD������
    ������
    img��ͼ��ָ��
    feat��Ҫ����������
    color����ɫ
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
    double v[4] = { 0 };//��������������
    double e[2] = { 0 };//����ֵ������
    CvMat M, V, E;
    double alpha, l1, l2;
    //������Բ�����ߺͷ���
    /* compute axes and orientation of ellipse surrounding affine region */
    cvInitMatHeader(&M, 2, 2, CV_64FC1, m, CV_AUTOSTEP);    //����
    cvInitMatHeader(&V, 2, 2, CV_64FC1, v, CV_AUTOSTEP);    //2��2*1������������ɵľ���
    cvInitMatHeader(&E, 2, 1, CV_64FC1, e, CV_AUTOSTEP);    //����ֵ
    cvEigenVV(&M, &V, &E, DBL_EPSILON, 0, 0);    //��������ֵ����������
    l1 = 1 / sqrt(e[1]);
    l2 = 1 / sqrt(e[0]);
    alpha = -atan2(v[1], v[0]);
    alpha *= 180 / M_PI;    //CV_PI
    //����Բ��ʮ����
    cvEllipse(img, cvPoint(feat->x, feat->y), cvSize(l2, l1), alpha,
              0, 360, CV_RGB(0, 0, 0), 3, 8, 0);
    cvEllipse(img, cvPoint(feat->x, feat->y), cvSize(l2, l1), alpha,
              0, 360, color, 1, 8, 0);
    cvLine(img, cvPoint(feat->x + 2, feat->y), cvPoint(feat->x - 2, feat->y),
           color, 1, 8, 0);
    cvLine(img, cvPoint(feat->x, feat->y + 2), cvPoint(feat->x, feat->y - 2),
           color, 1, 8, 0);
}


/*  ���ļ��ж���LOWE������
    ������
    filename:�ļ���
    features���������������������ָ��
    ����ֵ����������������
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
    struct feature* f;//��һ���������ָ��
    int i, j, n, d;
    double x, y, s, o, dv;
    FILE* file;
    if (!features)
        fatal_error("NULL pointer error, %s, line %d", __FILE__, __LINE__);
    //���ļ�
    if (!(file = fopen(filename, "r")))
    {
        fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
                filename, __FILE__, __LINE__);
        return -1;
    }
    //���ȶ������������������������ά��
    /* read number of features and dimension */
    if (fscanf(file, " %d %d ", &n, &d) != 2)
    {
        fprintf(stderr, "Warning: file read error, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    //����������ά�����ڶ�������ά��������
    if (d > FEATURE_MAX_D)
    {
        fprintf(stderr, "Warning: descriptor too long, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    //�����ڴ棬n��feature�ṹ��С�������׵�ַ��f
    f = (struct feature*) calloc(n, sizeof(struct feature));
    //���ζ���n��������
    for (i = 0; i < n; i++)
    {
        //���������������(ע��x,y˳��)���߶Ⱥͷ���
        /* read affine region parameters */
        if (fscanf(file, " %lf %lf %lf %lf ", &y, &x, &s, &o) != 4)
        {
            fprintf(stderr, "Warning: error reading feature #%d, %s, line %d\n",
                    i + 1, __FILE__, __LINE__);
            free(f);    //������ͷ��ڴ�
            return -1;
        }
        //����i�������㸳ֵ
        f[i].img_pt.x = f[i].x = x;//�������x����
        f[i].img_pt.y = f[i].y = y;//�������y����
        f[i].scl = s;//������Ĵ�С��������������ݶȵ�ģֵ
        f[i].ori = o;//������ķ��򣬼���������
        f[i].d = d;//���������ӵ�ά��
        f[i].type = FEATURE_LOWE;//����
        //��������������
        /* read descriptor */
        for (j = 0; j < d; j++)
        {
            if (!fscanf(file, " %lf ", &dv))
            {
                fprintf(stderr, "Warning: error reading feature descriptor" \
                        " #%d, %s, line %d\n", i + 1, __FILE__, __LINE__);
                free(f);    //������ͷ��ڴ�
                return -1;
            }
            f[i].descr[j] = dv;
        }
        //����һЩûʲô�õĲ���
        f[i].a = f[i].b = f[i].c = 0;
        f[i].category = 0;
        f[i].fwd_match = f[i].bck_match = f[i].mdl_match = NULL;
        f[i].mdl_pt.x = f[i].mdl_pt.y = -1;
        f[i].feature_data = NULL;
    }
    //�ر��ļ�
    if (fclose(file))
    {
        fprintf(stderr, "Warning: file close error, %s, line %d\n",
                __FILE__, __LINE__);
        free(f);    //������ͷ��ڴ�
        return -1;
    }
    *features = f;//�׵�ַ����*features
    return n;//���ض�������������
}


/*  ����LOWE��ʽ�����㼯�ϵ��ļ�
    ������
    filename���ļ���
    feat������������
    n�����������
    ����ֵ��0���ɹ���1��ʧ��
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
    //���ļ�
    if (!(file = fopen(filename, "w")))
    {
        fprintf(stderr, "Warning: error opening %s, %s, line %d\n",
                filename, __FILE__, __LINE__);
        return 1;
    }
    d = feat[0].d;//����������ά��
    fprintf(file, "%d %d\n", n, d);    //����д�����������������������ά��
    //����д��ÿ�����������Ϣ
    for (i = 0; i < n; i++)
    {
        //д������������(ע��x,y˳��),�߶ȣ�����
        fprintf(file, "%f %f %f %f", feat[i].y, feat[i].x,
                feat[i].scl, feat[i].ori);
        //д������������
        for (j = 0; j < d; j++)
        {
            //ÿ��20��Ԫ��
            /* write 20 descriptor values per line */
            if (j % 20 == 0)
                fprintf(file, "\n");
            fprintf(file, " %d", (int)(feat[i].descr[j]));
        }
        fprintf(file, "\n");
    }
    //�ر��ļ�
    if (fclose(file))
    {
        fprintf(stderr, "Warning: file close error, %s, line %d\n",
                __FILE__, __LINE__);
        return 1;
    }
    return 0;
}


/*  ��ͼ���ϻ�LOWE������
    ������
    img��ͼ��ָ��
    feat������������
    n�����������
*/
/*
    Draws Lowe-type features

    @param img image on which to draw features
    @param feat array of Oxford-type features
    @param n number of features
*/
static void draw_lowe_features(IplImage* img, struct feature* feat, int n)
{
    CvScalar color = CV_RGB(255, 255, 255);    //��ɫ
    int i;
    if (img->nChannels > 1)
        color = FEATURE_LOWE_COLOR;
    //���ú��������λ�n��������
    for (i = 0; i < n; i++)
        draw_lowe_feature(img, feat + i, color);
}


/*  ������LOWE������
    ������
    img��ͼ��ָ��
    feat��Ҫ����������
    color����ɫ
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
    //��ͷ�˵���������
    start_x = cvRound(feat->x);
    start_y = cvRound(feat->y);
    scl = feat->scl;//������Ĵ�С
    ori = feat->ori;//������ķ��򣬻���
    len = cvRound(scl * scale);    //��ͷ�˵ĳ���
    hlen = cvRound(scl * hscale);    //��ͷ�ֲ�ĳ���
    blen = len - hlen;
    //��ͷ�˵��յ������
    end_x = cvRound(len *  cos(ori)) + start_x;
    end_y = cvRound(len * -sin(ori)) + start_y;
    //��ͷ���ҷֲ����������
    h1_x = cvRound(blen *  cos(ori + CV_PI / 18.0)) + start_x;
    h1_y = cvRound(blen * -sin(ori + CV_PI / 18.0)) + start_y;
    //��ͷ����ֲ����������
    h2_x = cvRound(blen *  cos(ori - CV_PI / 18.0)) + start_x;
    h2_y = cvRound(blen * -sin(ori - CV_PI / 18.0)) + start_y;
    start = cvPoint(start_x, start_y);    //��ͷ�˵����
    end = cvPoint(end_x, end_y);    //��ͷ�˵��յ�
    h1 = cvPoint(h1_x, h1_y);    //��ͷ���ҷֲ�����
    h2 = cvPoint(h2_x, h2_y);    //��ͷ����ֲ�����
    cvLine(img, start, end, color, 1, 8, 0);    //����ͷ��
    cvLine(img, end, h1, color, 1, 8, 0);    //���ҷֲ�
    cvLine(img, end, h2, color, 1, 8, 0);    //����ֲ�
}




















//============================================================
/*
    ���ļ�����k-d���Ľ�����BBF���Һ�����ʵ��
*/

//BBF���õ��Ľṹ���ɴ洢��ǰ�㵽Ŀ���ľ���
//��kd�����������У����������ݻᱻ��ֵ��feature�ṹ��feature_data��Ա
struct bbf_data
{
    double d;  //�������㵽Ŀ����ŷʽ����ֵ
    void* old_data; //������������feature_data�����ǰ��ֵ
};

/************************ δ��¶�ӿڵ�һЩ���غ��������� **************************/
/************************* Local Function Prototypes *************************/

//�ø����������㼯��ʼ��k-d���ڵ�
static struct kd_node* kd_node_init(struct feature*, int);
//��չָ����k-d���ڵ㼰�����Һ���
static void expand_kd_node_subtree(struct kd_node*);
//ȷ������ڵ����������������ֵ
static void assign_part_key(struct kd_node*);
//�ҵ������������ֵ
static double median_select(double*, int);
//�ҵ����������е�rС����
static double rank_select(double*, int, int);
//�ò��뷨���������������������
static void insertion_sort(double*, int);
//���ݸ���������ֵ�ָ����飬ʹ����ǰ����С��pivot���󲿷ִ���pivot
static int partition_array(double*, int, double);
//��ָ����k-d���ڵ��ϻ��������㼯
static void partition_features(struct kd_node*);
//�Ӹ����������k-d��ֱ��Ҷ�ڵ㣬���������н�δ�����Ľڵ�������ȼ��������
static struct kd_node* explore_to_leaf(struct kd_node*, struct feature*, struct min_pq*);
//����һ�������㵽��������飬ʹ�����еĵ㰴��Ŀ���ľ�����������
static int insert_into_nbr_array(struct feature*, struct feature**, int, int);
//�жϸ������Ƿ���ĳ������
static int within_rect(CvPoint2D64f, CvRect);


/******************** ����kdtree.h�������ĺ��� **********************/
/******************** Functions prototyped in kdtree.h **********************/

/*  ���ݸ����������㼯�Ͻ���k-d��
    ������
    features�����������飬ע�⣺�˺�������ı�features������Ԫ�ص�����˳��
    n�����������
    ����ֵ�������õ�k-d��������ָ��
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
    //����������
    if (!features || n <= 0)
    {
        fprintf(stderr, "Warning: kdtree_build(): no features, %s, line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }
    //���ú������ø����������㼯��ʼ��k-d���ڵ㣬����ֵ��Ϊ����
    kd_root = kd_node_init(features, n);
    //���ú�������չ���ڵ�kd_root�������Һ���
    expand_kd_node_subtree(kd_root);
    return kd_root;
}


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
    struct kd_node* expl; //expl�ǵ�ǰ�����ڵ�
    struct min_pq* min_pq; //���ȼ�����
    struct feature* tree_feat, ** _nbrs; //tree_feat�ǵ���SIFT������_nbrs�д���Ų��ҳ����Ľ��������ڵ�
    struct bbf_data* bbf_data; //bbf_data��һ�����������ʱ�������ݺ����������Ļ���ṹ
    int i, t = 0, n = 0; //t����������������n�ǵ�ǰ����������е�Ԫ�ظ���
    //����������
    if (!nbrs || !feat || !kd_root)
    {
        fprintf(stderr, "Warning: NULL pointer error, %s, line %d\n",
                __FILE__, __LINE__);
        return -1;
    }
    _nbrs = (struct feature**) calloc(k, sizeof(struct feature*));           //�����ҽ��������Ӧ��С���ڴ�
    min_pq = minpq_init(); //min_pq���г�ʼ��������Ĭ�ϴ�С�Ŀռ�
    minpq_insert(min_pq, kd_root, 0);    //�����ڵ��Ȳ��뵽min_pq���ȼ�������
    //min_pq����û�л�������δ�ﵽ����������
    while (min_pq->n > 0 && t < max_nn_chks)
    {
        //��min_pq����ȡ(���Ƴ�)���ȼ���ߵĽڵ㣬��ֵ����ǰ�ڵ�expl
        expl = (struct kd_node*) minpq_extract_min(min_pq);
        if (!expl)
        {
            //������
            fprintf(stderr, "Warning: PQ unexpectedly empty, %s line %d\n", __FILE__, __LINE__);
            goto fail;
        }
        //�ӵ�ǰ�����ڵ�explһֱ������Ҷ�ӽڵ㣬���������н�δ�����Ľڵ�������ȼ�������У�����ֵΪҶ�ӽڵ�
        expl = explore_to_leaf(expl, feat, min_pq);
        if (!expl)
        {
            //������
            fprintf(stderr, "Warning: PQ unexpectedly empty, %s line %d\n", __FILE__, __LINE__);
            goto fail;
        }
        //�Ƚϲ��������
        for (i = 0; i < expl->n; i++)
        {
            tree_feat = &expl->features[i];//��i���������ָ��
            bbf_data = (struct bbf_data*) malloc(sizeof(struct bbf_data));          //�½�bbf�ṹ
            if (!bbf_data)
            {
                //������
                fprintf(stderr, "Warning: unable to allocate memory," " %s line %d\n", __FILE__, __LINE__);
                goto fail;
            }
            bbf_data->old_data = tree_feat->feature_data;//�����i���������feature_data����ǰ��ֵ
            bbf_data->d = descr_dist_sq(feat, tree_feat);    //��ǰ�������Ŀ���֮���ŷ�Ͼ���
            tree_feat->feature_data = bbf_data;//��bbf�ṹ�������������feature_data��
            //�жϲ�������������������㵽���������_nbrs��,����ɹ�����1
            //�������������Ԫ�ظ����Ѵﵽkʱ����������Ԫ�ظ����������ӣ��������Ԫ�ص�ֵ
            n += insert_into_nbr_array(tree_feat, _nbrs, n, k);
        }
        t++;//��������
    }
    minpq_release(&min_pq);    //�ͷ����ȶ���
    //��������������е������㣬�ָ���feature_data���ֵ
    for (i = 0; i < n; i++)
    {
        bbf_data = (struct bbf_data*)(_nbrs[i]->feature_data);
        _nbrs[i]->feature_data = bbf_data->old_data;//��֮ǰ�����ݸ�ֵ��feature_data��
        free(bbf_data);
    }
    *nbrs = _nbrs;
    return n;
    //ʧ�ܴ���
fail:
    minpq_release(&min_pq);
    //��������������е������㣬�ָ���feature_data���ֵ
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


/*  �ͷ�k-d��ռ�õĴ洢�ռ�
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


/************************ δ��¶�ӿڵ�һЩ���غ��� **************************/
/************************ Functions prototyped here **************************/

/*  �ø����������㼯��ʼ��k-d���ڵ�
    ������
    features�������㼯
    n�����������
    ����ֵ��k-d���ڵ�ָ��
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
    kd_node = (struct kd_node*) malloc(sizeof(struct kd_node));          //�����ڴ�
    memset(kd_node, 0, sizeof(struct kd_node));
    kd_node->ki = -1;//��������
    kd_node->features = features;//�ڵ��Ӧ�������㼯
    kd_node->n = n;//������ĸ���
    return kd_node;
}


/*  �ݹ����չָ����k-d���ڵ㼰�����Һ���
*/
/*
    Recursively expands a specified kd tree node into a tree whose leaves
    contain one entry each.

    @param kd_node an unexpanded node in a kd tree
*/
static void expand_kd_node_subtree(struct kd_node* kd_node)
{
    //���������Ҷ�ӽڵ�
    /* base case: leaf node */
    if (kd_node->n == 1 || kd_node->n == 0)
    {
        kd_node->leaf = 1;//Ҷ�ڵ��־λ��Ϊ1
        return;
    }
    //���ú�����ȷ���ڵ����������������ֵ
    assign_part_key(kd_node);
    //��ָ��k-d���ڵ��ϻ��������㼯(������ָ���ڵ��ki��kvֵ�����������㼯)
    partition_features(kd_node);
    //������չ���Һ���
    if (kd_node->kd_left)
        expand_kd_node_subtree(kd_node->kd_left);
    if (kd_node->kd_right)
        expand_kd_node_subtree(kd_node->kd_right);
}


/*  ȷ������ڵ����������������ֵ
    ������kd_node�������k-d���ڵ�
    ����ִ����󽫸�kd_node��ki��kv��Ա����
*/
/*
    Determines the descriptor index at which and the value with which to
    partition a kd tree node's features.

    @param kd_node a kd tree node
*/
static void assign_part_key(struct kd_node* kd_node)
{
    struct feature* features;
    //�����ֵkv����ֵmean������var���������ֵvar_max
    double kv, x, mean, var, var_max = 0;
    double* tmp;
    int d, n, i, j, ki = 0; //��������ki
    features = kd_node->features;
    n = kd_node->n;//������
    d = features[0].d;//����������ά��
    //���������ֵ���Ƿ���������һά��ά��,��n��128ά�����������У�����kά�ķ��������k��������(�ָ�λ��)
    /* partition key index is that along which descriptors have most variance */
    for (j = 0; j < d; j++)
    {
        mean = var = 0;
        //���jά�ľ�ֵ
        for (i = 0; i < n; i++)
            mean += features[i].descr[j];
        mean /= n;
        //���jά�ķ���
        for (i = 0; i < n; i++)
        {
            x = features[i].descr[j] - mean;
            var += x * x;
        }
        var /= n;
        //�ҵ���󷽲��ά��
        if (var > var_max)
        {
            ki = j;//��󷽲��ά����������
            var_max = var;
        }
    }
    //�����ֵ������������������kiά����ֵ(��kiά������м���Ǹ�ֵ)
    /* partition key value is median of descriptor values at ki */
    tmp = (double*) calloc(n, sizeof(double));
    for (i = 0; i < n; i++)
        tmp[i] = features[i].descr[ki];
    //���ú�������tmp�������ֵ
    kv = median_select(tmp, n);
    free(tmp);
    kd_node->ki = ki;//�����ά������
    kd_node->kv = kv;//�����ֵ
}


/*  �ҵ������������ֵ
    ������
    array���������飬Ԫ��˳�򽫻ᱻ�Ķ�
    n��Ԫ�ظ���
    ����ֵ����ֵ
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
    //���ú�������array�����еĵ�(n-1)/2С����������ֵ
    return rank_select(array, n, (n - 1) / 2);
}


/*  �ҵ����������е�rС����
    ������
    array���������飬Ԫ��˳�򽫻ᱻ�Ķ�
    n��Ԫ�ظ���
    r���ҵ�rСԪ��
    ����ֵ����rС��Ԫ��ֵ
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
    //������ֳ�5��һ�飬��gr_tot��
    /* divide array into groups of 5 and sort them */
    gr_5 = n / 5; //��ĸ���-1��n/5����ȡ��
    gr_tot = cvCeil(n / 5.0);    //��ĸ�����n/5����ȡ��
    rem_elts = n % 5;//���һ���е�Ԫ�ظ���
    tmp = array;
    //��ÿ����в�������
    for (i = 0; i < gr_5; i++)
    {
        insertion_sort(tmp, 5);
        tmp += 5;
    }
    //���һ��
    insertion_sort(tmp, rem_elts);
    //����ֵ����ֵ
    /* recursively find the median of the medians of the groups of 5 */
    tmp = (double*) calloc(gr_tot, sizeof(double));
    //��ÿ��5Ԫ���е���ֵ(���±�Ϊ2,2+5,...��Ԫ��)���Ƶ�temp����
    for (i = 0, j = 2; i < gr_5; i++, j += 5)
        tmp[i] = array[j];
    //���һ�����ֵ
    if (rem_elts)
        tmp[i++] = array[n - 1 - rem_elts / 2];
    //��temp�е���ֵmed������ֵ����ֵ
    med = rank_select(tmp, i, (i - 1) / 2);
    free(tmp);
    //������ֵ����ֵ�������飬�����ֽ���Ƿ��ǵ�rС��������������ݹ����rank_select����ѡ��
    /* partition around median of medians and recursively select if necessary */
    j = partition_array(array, n, med);    //�������飬����med���������е�����
    if (r == j)   //����ǵ�rС����
        return med;
    else if (r < j)   //��rС������ǰ�벿��
        return rank_select(array, j, r);
    else//��rС�����ں�벿��
    {
        array += j + 1;
        return rank_select(array, (n - j - 1), (r - j - 1));
    }
}


/*  �ò��뷨���������������������
    ������
    array����������
    n��Ԫ�ظ���
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


/*  ���ݸ���������ֵ�ָ����飬ʹ����ǰ����С��pivot���󲿷ִ���pivot
    ������
    array����������
    n�������Ԫ�ظ���
    pivot������ֵ
    ����ֵ���ָ��������±�
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
                p = i;//p����������±�
        }
    //����������һ��С����������Ի�
    array[p] = array[i];
    array[i] = pivot;
    return i;
}


/*  ��ָ����k-d���ڵ��ϻ��������㼯
    ʹ�������㼯��ǰ�벿���ǵ�kiάС������ĵ㣬��벿���ǵ�kiά��������ĵ�
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
    features = kd_node->features;//����������
    n = kd_node->n;//���������
    //printf("%d\n",n);
    ki = kd_node->ki;//�����ά������(��һά������)
    kv = kd_node->kv;//�����ֵ
    for (i = 0; i < n; i++)
    {
        //����i������������������ĵ�kiά��ֵС��kv
        if (features[i].descr[ki] <= kv)
        {
            tmp = features[++j];
            features[j] = features[i];
            features[i] = tmp;
            if (features[j].descr[ki] == kv)
                p = j;//p�����������ڵ�λ��
        }
    }
    //������features[p]�����һ��С������ĵ�features[j]�Ի�
    tmp = features[p];
    features[p] = features[j];
    features[j] = tmp;
    //�˺������λ���±�Ϊj
    //����������������ͬһ�࣬��˽ڵ��ΪҶ�ڵ�
    /* if all records fall on same side of partition, make node a leaf */
    if (j == n - 1)
    {
        kd_node->leaf = 1;
        return;
    }
    //��ʼ�����ӵĸ��ڵ㣬���ӹ�j+1��������
    kd_node->kd_left = kd_node_init(features, j + 1);
    //��ʼ���Һ��ӵĸ��ڵ㣬�Һ��ӹ�n-j-1��������
    kd_node->kd_right = kd_node_init(features + (j + 1), (n - j - 1));
}


/*  �Ӹ����������k-d��ֱ��Ҷ�ڵ㣬���������н�δ�����Ľڵ�������ȼ��������
    ���ȼ����к�����·����ͬʱ���ɵģ���Ҳ��BBF�㷨�ľ������ڣ��ڶ���������ʱ
    ������·����һ��ķ�֧���뵽���ȼ������У�������ʱ���ҡ������ȼ����е���
    ����Ǹ���Ŀ��������ָƽ��ľ���ABS(kv - feat->descr[ki])
    ������
    kd_node��Ҫ����������������
    feat��Ŀ��������
    min_pq�����ȼ�����
    ����ֵ��Ҷ�ӽڵ��ָ��
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
    //unexpl�д�������ȼ����еĺ�ѡ�ڵ�(��δ�����Ľڵ�)��explΪ��ǰ�����ڵ�
    struct kd_node* unexpl, *expl = kd_node;
    double kv;//�ָ������ֵ
    int ki;//�ָ������ά������
    //һֱ������Ҷ�ӽڵ㣬���������н�δ�����Ľڵ�������ȼ��������
    while (expl  &&  !expl->leaf)
    {
        ki = expl->ki;
        kv = expl->kv;
        //�����ά�����������������ά��������
        if (ki >= feat->d)
        {
            fprintf(stderr, "Warning: comparing imcompatible descriptors, %s" " line %d\n", __FILE__, __LINE__);
            return NULL;
        }
        //Ŀ��������kiά������С�ڵ���kv����������������
        if (feat->descr[ki] <= kv)
        {
            unexpl = expl->kd_right;//��ѡ�����ڵ���expl���������ĸ�
            expl = expl->kd_left;//��ǰ�����ڵ���expl���������ĸ�
        }
        else//Ŀ��������kiά�����ݴ���kv����������������
        {
            unexpl = expl->kd_left;//��ѡ�����ڵ���expl��������
            expl = expl->kd_right;//��ǰ�����ڵ���expl��������
        }
        //����ѡ�ڵ�unexpl����Ŀ��������kiά���丸�ڵ�ľ�����뵽���ȶ����У�����ԽС�����ȼ�Խ��
        if (minpq_insert(min_pq, unexpl, ABS(kv - feat->descr[ki])))
        {
            fprintf(stderr, "Warning: unable to insert into PQ, %s, line %d\n", __FILE__, __LINE__);
            return NULL;
        }
    }
    return expl;//����Ҷ�ӽڵ��ָ��
}


/*  ����һ�������㵽��������飬ʹ�����еĵ㰴��Ŀ���ľ�����������
    ������
    feat��Ҫ����������㣬��feature_data��Ӧ��ָ��bbf_data�ṹ��ָ�룬���е�dֵʱfeat��Ŀ���ľ����ƽ��
    nbrs�����������
    n����������������е�Ԫ�ظ���
    k�����������Ԫ�ظ��������ֵ
    ����ֵ����feat�ɹ����룬����1�����򷵻�0
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
    struct bbf_data* fdata, *ndata;//fdata��Ҫ����ĵ��bbf�ṹ��ndata������������еĵ��bbf�ṹ
    double dn, df; //dn��������������������bbf�ṹ�еľ���ֵ��df��Ҫ������������bbf�ṹ�еľ���ֵ
    int i, ret = 0;
    //ԭ���������Ϊ��
    if (n == 0)
    {
        nbrs[0] = feat;
        return 1;//����ɹ�������1
    }
    /* check at end of array */
    fdata = (struct bbf_data*) feat->feature_data;    //Ҫ������bbf�ṹ
    df = fdata->d;//Ҫ������������bbf�ṹ�еľ���ֵ
    ndata = (struct bbf_data*) nbrs[n - 1]->feature_data;    //����������еĵ��bbf�ṹ
    dn = ndata->d;//��������������һ���������bbf�ṹ�еľ���ֵ
    //df>=dn��˵��Ҫ����������������ĩβ
    if (df >= dn)
    {
        //�����������Ԫ�ظ����Ѵﵽ���ֵ���޷�����
        if (n == k)
        {
            feat->feature_data = fdata->old_data;//�����������Ǹ�ʲô��
            free(fdata);
            return 0;//����ʧ�ܣ�����0
        }
        nbrs[n] = feat;//���뵽ĩβ
        return 1;//����ɹ�������1
    }
    //���е��˴�˵������λ�ò�������ĩβ
    /* find the right place in the array */
    if (n < k)   //�����������Ԫ�ظ���С�����ֵ���ɲ���
    {
        nbrs[n] = nbrs[n - 1];//ԭ�������һ�������
        ret = 1;//��������Ϊ1
    }
    else//�����������Ԫ�ظ������ڻ�������ֵ���޷����룬������ret����0
    {
        //��ʵҲ�����޷����룬���������������Ԫ�ظ������䣬��ֵ�����
        nbrs[n - 1]->feature_data = ndata->old_data;
        free(ndata);
    }
    i = n - 2;
    //������������в���Ҫ�����λ��
    while (i >= 0)
    {
        ndata = (struct bbf_data*) nbrs[i]->feature_data;
        dn = ndata->d;
        if (dn <= df)   //�ҵ������
            break;
        nbrs[i + 1] = nbrs[i];//һ�κ���
        i--;
    }
    i++;
    nbrs[i] = feat;//����
    return ret;//���ؽ��
}


/*  �жϸ������Ƿ���ĳ������
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
