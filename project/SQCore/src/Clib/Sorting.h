//*************************************************************************
//
//                     �����9�������㷨�ܽ��о�
//
//                                                  2017-06-06      ����
//
//*************************************************************************
#pragma once
#include <string>
#include <iostream>
#include <time.h>
using namespace std;

//�����9������ԭ��ƪ
class AutoSort
{
public:
    static int main(int argc, char** argv);
public:
    AutoSort() {};
    ~AutoSort() {};
public:
    //ֱ�Ӳ�������
    void InsertSort(int data[], int n);
    //���ֲ�������
    void BiInsertSort(int data[], int n);
    //ϣ������
    void ShellSort(int data[], int n);
    //ð������
    void BubbleSort(int data[], int n);
    //��������[LΪȡ����һ���Ƚ�ֵindex]
    void QuickSort(int data[], int n, int L = 0);
    //ѡ������
    void SelectSort(int data[], int n);
    //������
    void HeapSort(int data[], int n);
    //�鲢����
    void MergeSort(int data[], int n);
    //��������
    void RadixSort(int data[], int n);
    //��������
    void CountSort(void);
private:
    //��ӡ����
    void print(int data[], int n);
    //����ʱ��
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








