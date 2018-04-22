
#include "stdafx.h"
#include "Sorting.h"
#include <sys/timeb.h>


int AutoSort::main(int argc, char** argv)
{
#define NUMDATA 10000
    int data[NUMDATA];//10000
    for (int ii = 0; ii < NUMDATA; ++ii)
        data[ii] = NUMDATA - ii;
    struct timeb startTime, endTime;
    ftime(&startTime);

    AutoSort st;
    st.HeapSort(data, sizeof(data) / sizeof(int));

    ftime(&endTime);
    for (int i = 0; i < 100; ++i)
        cout << data[i] << " ";
    printf("\n运行了：%d 毫秒\n", (endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm));
    //system("pause");
    return 1;
}

void AutoSort::InsertSort(int data[], int n)
{
#define FUNInsert2

    /* 1 直接插入排序**************************************************
    空间复杂度：只有辅助变量， 没有与问题规模相关的辅存消耗，O(1)
    时间复杂度：最好情况，初始数组为正序（此处为递增），O(n)；最坏情况，初始数组为反
    序，O(n2)；平均时间复杂度为O(n2).
    稳定性：当data[i]=datda[i-1]时，相对位置不变，所以是稳定的排序
    思想：将原序列分为有序区和无序区，每次外部循环将无序区的第一个元素插入到有序区的适
    当位置，同时有序区元素加1，无序区元素减1，这样直到无序区的元素为0
    *******************************************************************/
#ifdef FUNInsert1 //原理算法
    for (int i = 1; i < n; i++)                 //1.遍历A1-AN,i插入次数,共进行n-1次插入
    {
        int pos = data[i];                      //2.取出临时比较值POS
        int j = i - 1;                          //3.向A0倒序遍历索引
        while (pos < data[j] && j >= 0)         //4.这里是比较条件
        {
            data[j + 1] = data[j];              //5.这里就是交换咯
            //data[j] = pos;                    //6.循环里是需要优化不需要的操作的 见8
            j--;                                //7.这里移动位置 向前移动
        }
        data[j + 1] = pos;                      //8.确定最终的POS位置(优化上面的6)
    }
#endif
#ifdef FUNInsert2 //极限缩减优化 最快
    //优化思路后算法
    //for (int i = 1; i < n; ++i)
    //{
    //    if (data[i - 1] > data[i])              //处理该比较时候
    //    {
    //        int pos = data[i];
    //        int j = i;
    //        while (j > 0 && data[j - 1] > pos)
    //        {
    //            data[j] = data[j - 1];
    //            j--;
    //        }
    //        data[j] = pos;
    //    }
    //}
    //简写成这样
    //for (int i = 1; i < num; i++)
    //    for (int j = i - 1; j >= 0 && pData[j] > pData[j + 1]; j--)
    //        Swap(pData[j], pData[j + 1]);
    for (int i = 1; i < n; ++i)                         //1.A1-AN遍历
    {
        int j, pos = data[i];                           //2.记下中间比较值POS
        for (j = i; j > 0 && data[j - 1] > pos; j--)    //3.向A0倒序移位遍历比较
            data[j] = data[j - 1];                      //4.交互移位操作
        data[j] = pos;                                  //5.确定最小值位置
    }
#endif
#ifdef FUNInsert3 //while作死变态缩版
    //将上面推导
    //int i(0);
    //while (i<n)
    //{
    //    int j = i - 1, pos = data[i];
    //    while (j >= 0 && data[j]>pos)
    //    {
    //        data[j + 1] = data[j];
    //        j--;
    //    }
    //    data[j + 1] = pos;
    //    i++;
    //}
    int i(0);
    while (i<n)
    {
        int j = i - 1, pos = data[i++];             //前一位ID,取比较量自动移位
        while (j >= 0 && data[j]>pos)               //比较前一位的值
            data[j + 1] = data[j--];                //交互左移数据并自动移位J
        data[j + 1] = pos;                          //处理最后一个比较的位置
    }
#endif
}

void AutoSort::BiInsertSort(int data[], int n)
{
#define FUNBiInsert2
    /*2 二分（折半）插入排序 *******************************************
    时空复杂度及稳定性与上面是一样的
    思想：对于有序的序列二分查找效率比顺序查找高很多，基于此，在将无序区的第一个元素插
    入到有序区相应位置时，用二分查找寻找该位置而不是顺序查找，可以减少关
    键字比较的次数但是关键字移动的次数仍然是没有改变的，所以其实际的效果与直接插
    入排序相当，只需注意二分查找思想的运用。
    *******************************************************************/
#ifdef FUNBiInsert1 //原理算法
    for (int i = 1; i < n; ++i)                             //从A1-AN遍历
    {
        int pos = data[i], low = 0, high = i - 1;           //取比较量和前面排好的区间标记
        while (low <= high)                                 //下面计算前面排序的标记左右向POS靠近
        {
            int mid = (low + high) / 2;                     //一直折半计算中点
            if (pos < data[mid])
                high = mid - 1;                             //得到POS需要靠近插入的位置点high
            else
                low = mid + 1;
        }
        for (int j = i - 1; j >= high; j--)                 //high+1 is mid(中点)
            data[j + 1] = data[j];                          //擦写位置点到POS之间的交互排列
        data[low] = pos;
        //for (j = i - 1; j >= high + 1; j--)
        //    data[j + 1] = data[j];
        //data[high + 1] = pos;
    }
#endif
#ifdef FUNBiInsert2 //while作死算法 
    int i(0);
    while (i < n)
    {
        int pos = data[i], head = 0, tail = i - 1;
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
#endif
}

void AutoSort::ShellSort(int data[], int n)
{
#define FUNShell2
    /* 3 希尔排序 *****************************************************
    空间复杂度：只用到了i,j,gap,tmp4个辅助变量，与问题规模无关，空间复杂度为O(1).
    时间复杂度：分析较复杂，一般认为平均时间复杂度为O(n^1.3).
    稳定性：不稳定
    思想：本质上还是属于插入排序，只不过是先对序列分组，然后组内直接插入，同时，分组数
    由多到少，组内元素由少到多，顺序性由差到好，直到最后一步组间距为1时，
    直接插入排序的数组已经基本有序了
    *******************************************************************/
#ifdef FUNShell1 //原理算法
    for (int gap = n / 2; gap > 0; gap /= 2)        //分割步长
#ifdef 0
        for (int i = 0; i < gap; i++)               //分组成gap组
            for (int j = i + gap; j < n; j += gap)  //对每组进行插入排序
#else   //可以优化成下面这样一行
        for (int j = gap; j < n; j++)               //从数组第gap个元素开始
#endif
                if (data[j] < data[j - gap])        //每个元素与自己组内的数据进行 直接插入排序
                {
                    int pos = data[j];
                    int k = j - gap;
                    while (k >= 0 && data[k] > pos)
                    {
                        data[k + gap] = data[k];
                        k -= gap;
                    }
                    data[k + gap] = pos;
                }
#endif
#ifdef FUNShell2 //极限优化
    //思路算法
    //for (int gap = num / 2; gap > 0; gap /= 2)
    //    for (int i = gap; i < num; i++)
    //        for (int j = i - gap; j >= 0 && pData[j] > pData[j + gap]; j -= gap)
    //            Swap(pData[j], pData[j + gap]);               //交换两个数的值

    for (int gap = n / 2; gap > 0; gap /= 2)        //分割步长
        for (int i = gap; i < n; i++)               //从数组第gap个元素开始直接插入排序
        {                                           //下面就不用说了 直接插入排序里的
            int j, pos = data[i];
            for (j = i; j > gap - 1 && data[j - gap] > pos; j -= gap)    //for (k = j - gap; k > gap-2 && data[k] > pos; k -= gap)
                data[j] = data[j - gap];                                 //data[k + gap] = data[k];
            data[j] = pos;                                               //data[k + gap] = pos;
        }
#endif
}

void AutoSort::BubbleSort(int data[], int n)
{
#define FUNBubble3
/* 4 冒泡排序 ******************************************************
空间复杂度：只有三个辅助变量，与问题规模无关，空间复杂度为O(1)
时间复杂度：最好情况，数组本身是正序的，O(n)；最坏情况，数组是反序的，O(n^2)；平
均时间复杂度为O(n^2)。
稳定性：稳定
思想：将数组头部看成水面，数组尾部看成水底，最小（或最大）的元素上浮（或下沉）直到
结束，采用的是比较元素大小然后交换元素值的思想，每次都选择未排序的
元素中最小或最大元素送达指定的位置。
*******************************************************************/
#ifdef FUNBubble1     //理解原理算法篇
    int i, j, tmp;
    for (i = 0; i < n; i++)                     //原素计数遍历  [0~n-1]
#if 1                                           //最大的向右靠
        for (j = 1; j < n - i; j++)             //从第二个向右已经排好的(n-i之后的)遍历 [从1~n-1]
#else                                           //最小的向左靠
        for (j = n - 1; j > i; j--)             //倒数二个向左已经排好的(i之前的)遍历 [从n-1~1]
#endif
            if (data[j - 1] > data[j])          //比较进行交换 最大的向右靠
            {
                tmp = data[j];
                data[j] = data[j - 1];
                data[j - 1] = tmp;
            }
#endif
#ifdef FUNBubble2     //标记优化
    int i, j, tmp;
    bool flag(true);
    for (i = 0; i < n && flag; i++)             //flag判断是否有必要排序
    {
        flag = false;
        for (j = 1; j < n - i; j++)
            if (data[j - 1] > data[j])
            {
                tmp = data[j];
                data[j] = data[j - 1];
                data[j - 1] = tmp;
                flag = true;
            }
    }
#endif
#ifdef FUNBubble3     //极限优化
    //进一步优化 消掉FOR中的赋值和判断
    //int j, k(n), tmp;
    //bool flag(true);
    //while (flag)
    //{
    //    flag = false;
    //    for (j = 1; j < k; j++)
    //        if (data[j - 1] > data[j])
    //        {
    //            tmp = data[j];
    //            data[j] = data[j - 1];
    //            data[j - 1] = tmp;
    //            flag = true;
    //        }
    //    k--;
    //}

    //再做进一步的优化。如果有100个数的数组，仅前面10个无序，
    //后面90个都已排好序且都大于前面10个数字，那么在第一趟遍历后，
    //最后发生交换的位置必定小于10，且这个位置之后的数据必定已经有序了，
    //记录下这位置，第二次只要从数组头部遍历到这个位置就可以了。
    int j, k, flag(n), tmp;
    while (flag > 0)
    {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++)
            if (data[j - 1] > data[j])
            {
                tmp = data[j];
                data[j] = data[j - 1];
                data[j - 1] = tmp;
                flag = j;
            }
    }
#endif
}

void AutoSort::QuickSort(int data[], int n, int L)
{
#define FUNQuick2
/* 5 快速排序 *****************************************************
空间复杂度：主要是递归时所需的栈空间，平均空间复杂度为O(nlongn)。
时间复杂度：主要的时间都花费在划分上面，最好情况，每次划分的基准都是无序区的‘中
值’记录，O(nlogn);最坏情况，原数组本身是有序的，此时O(n^2)。
平均时间复杂度为O(nlogn)。
稳定性： 不稳定
思想：分治的思想，将大问题转化为小问题，递归的思想，最重要的过程就是划分，划分结束
了，数组也就排好序了，快速排序是排序算法中非常重要的一种
*******************************************************************/
#ifdef FUNQuick1    //原理标准版本 快排，数据结构书上的方法,递归，以后以这个为准
    int start(L), end(n);                               //注意这里为了统一接口才这么干的
    if (start < end)
    {
        int i(start), j(end), pivot = data[start];      //pivot比较值取出
#if 0                                                   //pivot比较值也可以用随机取的方法
        srand((unsigned int)time(NULL));
        swap(&data[i + rand() % (j - i + 1)], &data[i]);//与第一个交换
#endif
        while (i != j)
        {
            while (j > i && data[j] > pivot)            //从右向左找<=pivot的 j
                --j;
            data[i] = data[j];
            while (i < j && data[i] < pivot)            //从左向右找>=pivot的 i
                ++i;
            data[j] = data[i];
        }
        data[i] = pivot;                                //这时候i等于j i为中间一个分割位置
        QuickSort(data, i - 1, start);      //QuickSort(data, start, i - 1);
        QuickSort(data, end, i + 1);        //QuickSort(data, i + 1, end);
    }
#endif
#ifdef FUNQuick2    //用一个参数的方法
    if (n <= 1)
        return;
    int value(data[0]), start(L), end(n - 1);
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
#endif
#ifdef FUNQuick3    //左右理解法(一次找到两个值交换)
    int start(left), end(right);                //注意这里为了统一接口才这么干的
    if (left > right)
        return;
    int i(left), j(right), temp = data[left];   //temp中存的就是基准数 
    while (i != j)
    {
        while (data[j] >= temp && i < j)        //顺序很重要，要先从右边开始找 
            j--;
        while (data[i] <= temp && i < j)        //再找左边的 
            i++;
        if (i < j)
            Swap(data[i],data[j]);              //交换两个数在数组中的位置 
    }
    Swap(data[left],data[i]);                   //最终将基准数归位 不然排序就是反着的
    quicksort(data,left, i - 1);                //继续处理左边的，这里是一个递归的过程 
    quicksort(data,i + 1, right);               //继续处理右边的，这里是一个递归的过程 
#endif
#ifdef FUNQuick4    //改非递归版
#endif
}

void AutoSort::SelectSort(int data[], int n)
{
/* 6 直接选择排序 **************************************************
空间复杂度：只用到了i,j,min,tmp四个辅助变量，故空间复杂度为O(1).
时间复杂度：无论表的初始状态如何，比较次数都达到O(n^2),故直接选择排序的最好和最坏
时间复杂度都是O(n^2).
稳定性：不稳定，如将{5，3，2，5，4，1}排序，第一趟就改变了两个5的相对位置。可以
看成是交换排序和直接插入排序的综合，但是直接插入和冒泡排序都是稳定的，而该算法是不稳定的
思想：每一趟从待排序的记录中选择关键字最小的记录，顺序放在已排好序子表的最后，知道全部记录排序完毕
适用性：适合从大量记录中选择一部分排序记录，如从10000个记录中选择关键字大小为前10的记录
*******************************************************************/
    for (int i = 0; i < n - 1; ++i)                 //从第一个到倒数第二个遍历取比较值
    {
        int min = i;                                //取出标记比较值
        for (int j = i + 1; j < n; ++j)             //其他的与比较值遍历
        {
            if (data[j] < data[min])
                min = j;                            //把最小的换出来
        }
        if (min != i)                               //若min=i则证明已经是有序的了
        {
            int tmp = data[i];
            data[i] = data[min];
            data[min] = tmp;
        }
    }
}

//建立大根堆，每次将最大的元素移动到末尾
void heapAdjust(int data[], int start, int end)
{
    int tmp = data[start];
    for (int i = 2 * start + 1; i <= end; i *= 2)
    {
        //这个i<end的判断很重要，若i=end，则证明当前节点start只有一个左孩子节点，就不用继续比较了
        if (i < end && data[i] < data[i + 1])
            ++i;
        if (tmp > data[i])
            break;
        data[start] = data[i];
        start = i;
    }
    data[start] = tmp;
}

void AutoSort::HeapSort(int data[], int n)
{
/* 7 堆排序 ********************************************************
空间复杂度：只用到了四个辅助变量，空间复杂度是O(1).
时间复杂度：最好，最坏，和平均时间复杂度都是O(nlogn).
稳定性：不稳定
思想：本质上是一种树形选择排序思想，将原数组看成为一个完全二叉树的顺序存储结构，利
用完全二叉树中双亲节点和孩子节点之间的内在关系，在当前无序区中选择关键字
最大（大根堆）或者最小（小根堆）的记录移动到数组的末尾，然后对剩余的元素作同
样的操作
适用性：不适宜记录数较少的表，与直接选择排序算法类似
*******************************************************************/
/*
//算法分为两个主要部分，堆调整（采用筛选算法），与排序
void heapAdjust1(int data[], int low, int high)
{
    int i = low, j = 2 * i + 1;
    int tmp = data[i];
    while (j <= high)
    {
        if (j < high && data[j] < data[j + 1])
            ++j;
        if (tmp < data[j])
        {
            data[i] = data[j];
            i = j;
            j = 2 * i;
        }
        else
            break;
    }
    data[i] = tmp;
}
*/

    //建立初始堆
    for (int i = n / 2; i >= 0; --i)
        heapAdjust(data, i, n - 1);
    //堆排序过程
    for (int i = n - 1; i >= 0; --i)
    {
        //交换堆顶和最后一个元素
        int tmp = data[0];
        data[0] = data[i];
        data[i] = tmp;
        //调整堆满足大根堆的性质
        heapAdjust(data, 0, i - 1);
    }
}

void AutoSort::MergeSort(int data[], int n)
{
/* 8 归并排序 ******************************************************
空间复杂度：O(n),需要一个辅助的数组来存放合并两个有序表之后生成的新表，故归并排序不是就地排序
时间复杂度：最好，最坏，平均时间复杂度均是O(nlogn)
稳定性：归并排序是稳定的排序算法
思想：将两个或两个以上的有序表合并为一个新的有序表，递归的思想
*******************************************************************/
    /*
    //迭代版本,有问题
    //void mergeSort_iter(int data[], int n)
    //{
    //    int *b = new int[n];
    //    int *a = data;
    //    //外层for循环，一共进行logn趟归并
    //    for (int seg = 1; seg < n; seg += seg)
    //    {
    //        //一趟归并排序
    //        for (int start = 0; start < n; start += seg + seg)
    //        {
    //            int low = start, mid = (start + seg) < n ? (start + seg) : n, high = (start + seg + seg) < n ? (start + seg + seg) : n;
    //            int k = low;
    //            int start1 = low, end1 = mid;
    //            int start2 = mid, end2 = high;
    //            while (start1 < end1 && start2 < end2)
    //                b[k++] = a[start1] < a[start2] ? a[start1] : a[start2];
    //            while (start1 < end1)
    //                b[k++] = a[start1++];
    //            while (start2 < end2)
    //                b[k++] = a[start2++];
    //        }
    //        //交换a和b
    //        int *tmp = a;
    //        a = b;
    //        b = tmp;
    //    }
    //    //若发生交换了
    //    if (a != data)
    //    {
    //        for (int i = 0; i < n; ++i)
    //            b[i] = a[i];
    //        b = a;
    //    }
    //    delete b;
    //}

    //一趟归并过程，将两个有序的子表合成一个新的有序表
    void merge(int data[], int low, int mid, int high)
    {
        int i = low, j = mid + 1, k = 0;
        //临时存储排好序的数组
        int *tmp = new int[high - low + 1];
        while (i <= mid && j <= high)
        {
            if (data[i] < data[j])
                tmp[k++] = data[i++];
            else
                tmp[k++] = data[j++];
        }
        while (i <= mid)
            tmp[k++] = data[i++];
        while (j <= high)
            tmp[k++] = data[j++];

        for (int i = low, k = 0; i <= high; i++, k++)
            data[i] = tmp[k];
        delete tmp;
    }

    //递归形式分别对数组的左右两个子数组归并排序，然后merge成一个新的有序数组
    void mergeSortR(int data[], int low, int high)
    {
        int mid;
        if (low < high)
        {
            mid = (low + high) / 2;
            mergeSortR(data, low, mid);
            mergeSortR(data, mid + 1, high);
            merge(data, low, mid, high);
        }
    }
    //自顶向下的二路归并排序算法
    void mergeSort(int data[], int n)
    {
        mergeSortR(data, 0, n - 1);
    }
    */

    ////归并排序
    //void merge(int* pArray, int start, int mid, int end)
    //{
    //    if (start > mid || mid > end) return;
    //    int i = start, j = mid + 1, k = 0;
    //    int* L = (int*)malloc((end - start + 1) * sizeof(int));
    //    while (i <= mid && j <= end)
    //    {
    //        if (pArray[i] < pArray[j])
    //            L[k++] = pArray[i++];
    //        else
    //            L[k++] = pArray[j++];
    //    }
    //    while (i <= mid)
    //        L[k++] = pArray[i++];
    //    while (j <= end)
    //        L[k++] = pArray[j++];
    //    for (i = start, j = 0; i <= end; i++, j++)
    //        pArray[i] = L[j];
    //    free(L);
    //}

    //void mergeSort(int* pArray, int start, int end)
    //{
    //    if (start < end)
    //    {
    //        int mid = (start + end) / 2;
    //        mergeSort(pArray, start, mid);
    //        mergeSort(pArray, mid + 1, end);
    //        merge(pArray, start, mid, end);
    //    }
    //}
}

void AutoSort::RadixSort(int data[], int n)
{
/* 9 基数排序 ******************************************************
空间复杂度：空间复杂度为O(n)
时间复杂度：最好、最坏、平均的时间复杂度都是O(d(n+r)),其中d是待排序元素的最大位
数，n是元素的个数，r是基数（十进制r=10，二进制r=2）。
稳定性：基数排序是稳定的排序方法
思想：通过"分配"和"收集"过程实现排序，不需要进行关键字之间的比较，是一种借助于多
关键字排序的思想对单关键字排序的方法，分为最低位优先(LSD）和最高位优(MSD)
*******************************************************************/
    /*
//辅助函数，求数据的最大位数d
int maxbit(int data[], int n)
{
    int d = 1;//保存最大位数，初始为1
    int p = 10;
    for (int i = 0; i < n; ++i)
    {
        while (data[i] >= p)
        {
            p *= 10;//有溢出的风险
            ++d;
        }
    }
    return d;
}

//基数排序
void radixSort(int data[], int n)
{
    //得到最大位数d
    int d = maxbit(data, n);
    int *tmp = new int[n];
    int *count = new int[10];//计数器
    int i, j, k;
    int radix = 1;
    for (i = 1; i <= d; ++i)
    {
        //清空计数器
        for (j = 0; j < 10; ++j)
            count[j] = 0;
        for (j = 0; j < n; j++)
        {
            k = (data[j] / radix) % 10;//统计每个桶中的记录数
            count[k]++;
        }
        for (j = 1; j < 10; j++)
            count[j] = count[j - 1] + count[j];
        for (j = n - 1; j >= 0; j--)
        {
            k = (data[j] / radix) % 10;
            tmp[count[k] - 1] = data[j];
            count[k]--;
        }
        for (j = 0; j < n; j++)
            data[j] = tmp[j];
        radix = radix * 10;
    }
    delete[]tmp;
    delete[]count;
}
*/
}

void AutoSort::print(int data[], int n)
{
    cout << "Num[" << n << "]: ";
    for (int i = 0; i < n; ++i)
        cout << data[i] << " ";
    cout << endl;
}

void AutoSort::TakeTime(void)
{
    struct timeb startTime, endTime;
    ftime(&startTime);
    //执行函数+++++++++++++++++++++++++++++++++++++++++++++
    //MessageBoxA(NULL, "ok", "ok", MB_OK);
    //=====================================================
    ftime(&endTime);
    printf("\n运行了：%d 毫秒\n", (endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm));
}



