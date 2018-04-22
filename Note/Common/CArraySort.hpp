//*************************************************************************
//
//                   数组的9种排序算法泛型总结研究
//
//                                                  2017-06-11      阿甘
//
//*************************************************************************
#pragma once

//直接插入排序
template <typename T>
inline void InsertSort(T* data, size_t n)
{
    size_t i, j;
    for (size_t i = 1; i < n; ++i)                      //1.A1-AN遍历
    {
        T pos = data[i];                                //2.记下中间比较值POS
        for (j = i; j > 0 && data[j - 1] > pos; j--)    //3.向A0倒序移位遍历比较
            data[j] = data[j - 1];                      //4.交互移位操作
        data[j] = pos;                                  //5.确定最小值位置
    }
};

//二分插入排序
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

//希尔排序
template <typename T>
inline void ShellSort(T* data, size_t n)
{
    size_t i, j, gap;
    for (gap = n / 2; gap > 0; gap /= 2)            //分割步长
        for (i = gap; i < n; i++)                   //从数组第gap个元素开始直接插入排序
        {                                           //下面就不用说了 直接插入排序里的
            T pos = data[i];
            for (j = i; j > gap - 1 && data[j - gap] > pos; j -= gap)
                data[j] = data[j - gap];
            data[j] = pos;
        }
};

//冒泡排序
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

//快速排序[L为取其中一个比较值index]
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

//选择排序
template <typename T>
inline void SelectSort(T* data, size_t n)
{
    for (size_t i = 0; i < n - 1; ++i)              //从第一个到倒数第二个遍历取比较值
    {
        size_t min = i;                             //取出标记比较值
        for (size_t j = i + 1; j < n; ++j)          //其他的与比较值遍历
        {
            if (data[j] < data[min])
                min = j;                            //把最小的换出来
        }
        if (min != i)                               //若min=i则证明已经是有序的了
        {
            //Swap(data[i] , data[min]);
            T tmp = data[i];
            data[i] = data[min];
            data[min] = tmp;
        }
    }
};

//数组的排序管理
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
    //直接插入排序
    void SortInsert(void)           { InsertSort(pData, num); };
    //二分插入排序
    void SortBiInsert(void)         { BiInsertSort(pData, num); };
    //希尔排序
    void SortShell(void)            { ShellSort(pData, num); };
    //冒泡排序
    void SortBubble(void)           { BubbleSort(pData, num); };
    //快速排序
    void SortQuick(void)            { QuickSort(pData, num); };
    //选择排序
    void SortSelect(void)           { SelectSort(pData, num); };
};






