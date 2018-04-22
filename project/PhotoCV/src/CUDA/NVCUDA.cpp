
#include "stdafx.h"
#include "NVCUDA.h"

#ifdef DEMO_CUDA_001
#define N 10

void add(int *a, int *b, int *c)
{
    int tid = 0;    //这是第0个CPU,因此索引从0开始
    while (tid < N)
    {
        c[tid] = a[tid] + b[tid];
        tid += 1;   //由于只有一个CPU，因此每次递增1
    }
}

void Testmain1(void)
{
    int a[N], b[N], c[N];
    //在CPU上为数组'a'和'b'赋值
    for (int i = 0; i < N; i++)
    {
        a[i] = -i;
        b[i] = i*i;
    }
    add(a, b, c);
    //显示结果
    for (int i = 0; i < N; i++)
        printf("%d + %d = %d\n", a[i], b[i], c[i]);
}
#endif

#ifdef DEMO_CUDA_002

#define N 10

__global__ void add(int *a, int *b, int *c)
{
    int tid = blockIdx.x;    //计算该索引处的数据
    if (tid < N)
        c[tid] = a[tid] + b[tid];
}

void Testmain1(void)
{
    int a[N], b[N], c[N];
    int *dev_a, *dev_b, *dev_c;
    //在GPU上分配内存
    HANDLE_ERROR(cudaMalloc((void**)&dev_a, N * sizeof(int)));
    HANDLE_ERROR(cudaMalloc((void**)&dev_b, N * sizeof(int)));
    HANDLE_ERROR(cudaMalloc((void**)&dev_c, N * sizeof(int)));
    //在CPU上为数组'a'和'b'赋值
    for (int i = 0; i < N; i++)
    {
        a[i] = -i;
        b[i] = i*i;
    }
    //将数组'a'和'b'复制到GPU
    HANDLE_ERROR(cudaMemcpy(dev_a, a, N * sizeof(int), cudaMemcpyHostToDevice));
    HANDLE_ERROR(cudaMemcpy(dev_b, b, N * sizeof(int), cudaMemcpyHostToDevice));
    add << <N, 1 >> >(dev_a,dev_b,dev_c);
    //将数组'c'从GPU复制到CPU
    HANDLE_ERROR(cudaMemcpy(dev_c, c, N * sizeof(int), cudaMemcpyHostToDevice));
    //显示结果
    for (int i = 0; i < N; i++)
        printf("%d + %d = %d\n", a[i], b[i], c[i]);
    //释放在GPU上分配的内存
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);
}


#endif

#ifdef DEMO_CUDA_003

#define DIM 10

struct cuComplex
{
    float r;
    float i;
    cuComplex(float a, float b) :r(a), i(b){};
    float magnitude2(void)
    {
        return r*r + i*i;
    };
    cuComplex operator*(const cuComplex& a)
    {
        return cuComplex(r*a.r - i*a.i, i*a.r + r*a.i);
    };
    cuComplex operator+(const cuComplex& a)
    {
        return cuComplex(r + a.r, i + a.i);
    };
};

int julia(int x, int y)
{
    const float scale = 1.5;
    float jx = scale*(float)(DIM / 2 - x) / (DIM / 2);
    float jy = scale*(float)(DIM / 2 - y) / (DIM / 2);

    cuComplex c(-0.8, 0.156);
    cuComplex a(jx, jy);

    int i = 0;
    for (i = 0; i < 200;i++)
    {
        a = a*a + c;
        if (a.magnitude2() > 1000)
            return 0;
    }
    return 1;
}

void kernel(unsigned char *ptr)
{
    for (int y = 0; y < DIM; y++)
    {
        for (int x = 0; x < DIM; x++)
        {
            int offset = x + y*DIM;
            int juliaValue = julia(x, y);
            ptr[offset * 4 = 0] = 255 * juliaValue;
            ptr[offset * 4 = 1] = 0;
            ptr[offset * 4 = 2] = 0; 
            ptr[offset * 4 = 3] = 255; 
        }
    }
}

void Testmain1(void)
{
    CPUBitmap bitmap(DIM, DIM);
    unsigned char *ptr = bitmap.get_ptr();
    kernel(ptr);
    bitmap.display_and_exit();
}

#endif

#ifdef DEMO_CUDA_004


#endif