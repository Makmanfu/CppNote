//*************************************************************************
//
//                     数组的9种排序算法总结研究
//
//                                                  2017-06-06      阿甘
//
//*************************************************************************
#pragma once
#include <string>
#include <iostream>
#include <time.h>
using namespace std;

//数组的9种排序原理篇
class AutoSort
{
public:
    static int main(int argc, char** argv);
public:
    AutoSort() {};
    ~AutoSort() {};
public:
    //直接插入排序
    void InsertSort(int data[], int n);
    //二分插入排序
    void BiInsertSort(int data[], int n);
    //希尔排序
    void ShellSort(int data[], int n);
    //冒泡排序
    void BubbleSort(int data[], int n);
    //快速排序[L为取其中一个比较值index]
    void QuickSort(int data[], int n, int L = 0);
    //选择排序
    void SelectSort(int data[], int n);
    //堆排序
    void HeapSort(int data[], int n);
    //归并排序
    void MergeSort(int data[], int n);
    //基数排序
    void RadixSort(int data[], int n);
    //计数排序
    void CountSort(void);
private:
    //打印测试
    void print(int data[], int n);
    //测试时间
    void TakeTime(void);
public:
    template <typename T>
    void Swap(T& A, T& B)
    {
        T temp(A);
        A = B;
        B = temp;
    }
    template <typename T>
    void PSwap(T* A, T* B)
    {
        T tmp = *A;
        *A = *B;
        *B = tmp;
    }
};








