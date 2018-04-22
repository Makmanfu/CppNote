//*************************************************************************
//
//                   �����9�������㷨�����ܽ��о�
//
//                                                  2017-06-11      ����
//
//*************************************************************************
#pragma once

//ֱ�Ӳ�������
template <typename T>
inline void InsertSort(T* data, size_t n)
{
    size_t i, j;
    for (size_t i = 1; i < n; ++i)                      //1.A1-AN����
    {
        T pos = data[i];                                //2.�����м�Ƚ�ֵPOS
        for (j = i; j > 0 && data[j - 1] > pos; j--)    //3.��A0������λ�����Ƚ�
            data[j] = data[j - 1];                      //4.������λ����
        data[j] = pos;                                  //5.ȷ����Сֵλ��
    }
};

//���ֲ�������
template <typename T>
inline void BiInsertSort(T* data, size_t n)
{
    int i(0);
    while (i < n)
    {
        T pos = data[i];
        int head = 0, tail = i - 1;
        while (head <= tail)
        {
            int mid = (head + tail) / 2;
            if (pos < data[mid])
                tail = mid - 1;
            else
                head = mid + 1;
        }
        for (int j = i++ - 1; j >= head; j--)
            data[j + 1] = data[j];
        data[head] = pos;
    }
};

//ϣ������
template <typename T>
inline void ShellSort(T* data, size_t n)
{
    size_t i, j, gap;
    for (gap = n / 2; gap > 0; gap /= 2)            //�ָ��
        for (i = gap; i < n; i++)                   //�������gap��Ԫ�ؿ�ʼֱ�Ӳ�������
        {                                           //����Ͳ���˵�� ֱ�Ӳ����������
            T pos = data[i];
            for (j = i; j > gap - 1 && data[j - gap] > pos; j -= gap)
                data[j] = data[j - gap];
            data[j] = pos;
        }
};

//ð������
template <typename T>
inline void BubbleSort(T* data, size_t n)
{
    size_t j, k, flag(n);
    while (flag > 0)
    {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++)
            if (data[j - 1] > data[j])
            {
                //Swap(data[j], data[j - 1]);
                T tmp = data[j];
                data[j] = data[j - 1];
                data[j - 1] = tmp;
                flag = j;
            }
    }
};

//��������[LΪȡ����һ���Ƚ�ֵindex]
template <typename T>
inline void QuickSort(T* data, size_t n, size_t L = 0)
{
    if (n <= 1)
        return;
    T value(data[0]);
    size_t start(L), end(n - 1);
    while (start < end)
    {
        for (; start < end; --end)
        {
            if (data[end] < value)
            {
                data[start++] = data[end];
                break;
            }
        }
        for (; start < end; ++start)
        {
            if (data[start] > value)
            {
                data[end--] = data[start];
                break;
            }
        }
    }
    data[start] = value;
    InsertSort(data, start);
    InsertSort(data + start + 1, n - start - 1);
};

//ѡ������
template <typename T>
inline void SelectSort(T* data, size_t n)
{
    for (size_t i = 0; i < n - 1; ++i)              //�ӵ�һ���������ڶ�������ȡ�Ƚ�ֵ
    {
        size_t min = i;                             //ȡ����ǱȽ�ֵ
        for (size_t j = i + 1; j < n; ++j)          //��������Ƚ�ֵ����
        {
            if (data[j] < data[min])
                min = j;                            //����С�Ļ�����
        }
        if (min != i)                               //��min=i��֤���Ѿ����������
        {
            //Swap(data[i] , data[min]);
            T tmp = data[i];
            data[i] = data[min];
            data[min] = tmp;
        }
    }
};

//������������
template <typename T>
class TArraySort
{
private:
    T* pData;
    size_t num;
private:
    inline void Swap(T& A, T& B)
    {
        T temp(A);
        A = B;
        B = temp;
    }
    inline void PSwap(T* A, T* B)
    {
        T tmp = *A;
        *A = *B;
        *B = tmp;
    }
public:
    TArraySort(T* pUser, size_t n) : pData(pUser), num(n) {};
    ~TArraySort(){};
public:
    //ֱ�Ӳ�������
    void SortInsert(void)           { InsertSort(pData, num); };
    //���ֲ�������
    void SortBiInsert(void)         { BiInsertSort(pData, num); };
    //ϣ������
    void SortShell(void)            { ShellSort(pData, num); };
    //ð������
    void SortBubble(void)           { BubbleSort(pData, num); };
    //��������
    void SortQuick(void)            { QuickSort(pData, num); };
    //ѡ������
    void SortSelect(void)           { SelectSort(pData, num); };
};






