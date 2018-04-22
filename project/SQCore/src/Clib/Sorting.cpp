
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
    printf("\n�����ˣ�%d ����\n", (endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm));
    //system("pause");
    return 1;
}

void AutoSort::InsertSort(int data[], int n)
{
#define FUNInsert2

    /* 1 ֱ�Ӳ�������**************************************************
    �ռ临�Ӷȣ�ֻ�и��������� û���������ģ��صĸ������ģ�O(1)
    ʱ�临�Ӷȣ�����������ʼ����Ϊ���򣨴˴�Ϊ��������O(n)����������ʼ����Ϊ��
    ��O(n2)��ƽ��ʱ�临�Ӷ�ΪO(n2).
    �ȶ��ԣ���data[i]=datda[i-1]ʱ�����λ�ò��䣬�������ȶ�������
    ˼�룺��ԭ���з�Ϊ����������������ÿ���ⲿѭ�����������ĵ�һ��Ԫ�ز��뵽����������
    ��λ�ã�ͬʱ������Ԫ�ؼ�1��������Ԫ�ؼ�1������ֱ����������Ԫ��Ϊ0
    *******************************************************************/
#ifdef FUNInsert1 //ԭ���㷨
    for (int i = 1; i < n; i++)                 //1.����A1-AN,i�������,������n-1�β���
    {
        int pos = data[i];                      //2.ȡ����ʱ�Ƚ�ֵPOS
        int j = i - 1;                          //3.��A0�����������
        while (pos < data[j] && j >= 0)         //4.�����ǱȽ�����
        {
            data[j + 1] = data[j];              //5.������ǽ�����
            //data[j] = pos;                    //6.ѭ��������Ҫ�Ż�����Ҫ�Ĳ����� ��8
            j--;                                //7.�����ƶ�λ�� ��ǰ�ƶ�
        }
        data[j + 1] = pos;                      //8.ȷ�����յ�POSλ��(�Ż������6)
    }
#endif
#ifdef FUNInsert2 //���������Ż� ���
    //�Ż�˼·���㷨
    //for (int i = 1; i < n; ++i)
    //{
    //    if (data[i - 1] > data[i])              //����ñȽ�ʱ��
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
    //��д������
    //for (int i = 1; i < num; i++)
    //    for (int j = i - 1; j >= 0 && pData[j] > pData[j + 1]; j--)
    //        Swap(pData[j], pData[j + 1]);
    for (int i = 1; i < n; ++i)                         //1.A1-AN����
    {
        int j, pos = data[i];                           //2.�����м�Ƚ�ֵPOS
        for (j = i; j > 0 && data[j - 1] > pos; j--)    //3.��A0������λ�����Ƚ�
            data[j] = data[j - 1];                      //4.������λ����
        data[j] = pos;                                  //5.ȷ����Сֵλ��
    }
#endif
#ifdef FUNInsert3 //while������̬����
    //�������Ƶ�
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
        int j = i - 1, pos = data[i++];             //ǰһλID,ȡ�Ƚ����Զ���λ
        while (j >= 0 && data[j]>pos)               //�Ƚ�ǰһλ��ֵ
            data[j + 1] = data[j--];                //�����������ݲ��Զ���λJ
        data[j + 1] = pos;                          //�������һ���Ƚϵ�λ��
    }
#endif
}

void AutoSort::BiInsertSort(int data[], int n)
{
#define FUNBiInsert2
    /*2 ���֣��۰룩�������� *******************************************
    ʱ�ո��Ӷȼ��ȶ�����������һ����
    ˼�룺������������ж��ֲ���Ч�ʱ�˳����Ҹߺܶ࣬���ڴˣ��ڽ��������ĵ�һ��Ԫ�ز�
    �뵽��������Ӧλ��ʱ���ö��ֲ���Ѱ�Ҹ�λ�ö�����˳����ң����Լ��ٹ�
    ���ֱȽϵĴ������ǹؼ����ƶ��Ĵ�����Ȼ��û�иı�ģ�������ʵ�ʵ�Ч����ֱ�Ӳ�
    �������൱��ֻ��ע����ֲ���˼������á�
    *******************************************************************/
#ifdef FUNBiInsert1 //ԭ���㷨
    for (int i = 1; i < n; ++i)                             //��A1-AN����
    {
        int pos = data[i], low = 0, high = i - 1;           //ȡ�Ƚ�����ǰ���źõ�������
        while (low <= high)                                 //�������ǰ������ı��������POS����
        {
            int mid = (low + high) / 2;                     //һֱ�۰�����е�
            if (pos < data[mid])
                high = mid - 1;                             //�õ�POS��Ҫ���������λ�õ�high
            else
                low = mid + 1;
        }
        for (int j = i - 1; j >= high; j--)                 //high+1 is mid(�е�)
            data[j + 1] = data[j];                          //��дλ�õ㵽POS֮��Ľ�������
        data[low] = pos;
        //for (j = i - 1; j >= high + 1; j--)
        //    data[j + 1] = data[j];
        //data[high + 1] = pos;
    }
#endif
#ifdef FUNBiInsert2 //while�����㷨 
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
    /* 3 ϣ������ *****************************************************
    �ռ临�Ӷȣ�ֻ�õ���i,j,gap,tmp4�������������������ģ�޹أ��ռ临�Ӷ�ΪO(1).
    ʱ�临�Ӷȣ������ϸ��ӣ�һ����Ϊƽ��ʱ�临�Ӷ�ΪO(n^1.3).
    �ȶ��ԣ����ȶ�
    ˼�룺�����ϻ������ڲ�������ֻ�������ȶ����з��飬Ȼ������ֱ�Ӳ��룬ͬʱ��������
    �ɶൽ�٣�����Ԫ�����ٵ��࣬˳�����ɲ�ã�ֱ�����һ������Ϊ1ʱ��
    ֱ�Ӳ�������������Ѿ�����������
    *******************************************************************/
#ifdef FUNShell1 //ԭ���㷨
    for (int gap = n / 2; gap > 0; gap /= 2)        //�ָ��
#ifdef 0
        for (int i = 0; i < gap; i++)               //�����gap��
            for (int j = i + gap; j < n; j += gap)  //��ÿ����в�������
#else   //�����Ż�����������һ��
        for (int j = gap; j < n; j++)               //�������gap��Ԫ�ؿ�ʼ
#endif
                if (data[j] < data[j - gap])        //ÿ��Ԫ�����Լ����ڵ����ݽ��� ֱ�Ӳ�������
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
#ifdef FUNShell2 //�����Ż�
    //˼·�㷨
    //for (int gap = num / 2; gap > 0; gap /= 2)
    //    for (int i = gap; i < num; i++)
    //        for (int j = i - gap; j >= 0 && pData[j] > pData[j + gap]; j -= gap)
    //            Swap(pData[j], pData[j + gap]);               //������������ֵ

    for (int gap = n / 2; gap > 0; gap /= 2)        //�ָ��
        for (int i = gap; i < n; i++)               //�������gap��Ԫ�ؿ�ʼֱ�Ӳ�������
        {                                           //����Ͳ���˵�� ֱ�Ӳ����������
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
/* 4 ð������ ******************************************************
�ռ临�Ӷȣ�ֻ�����������������������ģ�޹أ��ռ临�Ӷ�ΪO(1)
ʱ�临�Ӷȣ������������鱾��������ģ�O(n)�������������Ƿ���ģ�O(n^2)��ƽ
��ʱ�临�Ӷ�ΪO(n^2)��
�ȶ��ԣ��ȶ�
˼�룺������ͷ������ˮ�棬����β������ˮ�ף���С������󣩵�Ԫ���ϸ������³���ֱ��
���������õ��ǱȽ�Ԫ�ش�СȻ�󽻻�Ԫ��ֵ��˼�룬ÿ�ζ�ѡ��δ�����
Ԫ������С�����Ԫ���ʹ�ָ����λ�á�
*******************************************************************/
#ifdef FUNBubble1     //���ԭ���㷨ƪ
    int i, j, tmp;
    for (i = 0; i < n; i++)                     //ԭ�ؼ�������  [0~n-1]
#if 1                                           //�������ҿ�
        for (j = 1; j < n - i; j++)             //�ӵڶ��������Ѿ��źõ�(n-i֮���)���� [��1~n-1]
#else                                           //��С������
        for (j = n - 1; j > i; j--)             //�������������Ѿ��źõ�(i֮ǰ��)���� [��n-1~1]
#endif
            if (data[j - 1] > data[j])          //�ȽϽ��н��� �������ҿ�
            {
                tmp = data[j];
                data[j] = data[j - 1];
                data[j - 1] = tmp;
            }
#endif
#ifdef FUNBubble2     //����Ż�
    int i, j, tmp;
    bool flag(true);
    for (i = 0; i < n && flag; i++)             //flag�ж��Ƿ��б�Ҫ����
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
#ifdef FUNBubble3     //�����Ż�
    //��һ���Ż� ����FOR�еĸ�ֵ���ж�
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

    //������һ�����Ż��������100���������飬��ǰ��10������
    //����90�������ź����Ҷ�����ǰ��10�����֣���ô�ڵ�һ�˱�����
    //�����������λ�ñض�С��10�������λ��֮������ݱض��Ѿ������ˣ�
    //��¼����λ�ã��ڶ���ֻҪ������ͷ�����������λ�þͿ����ˡ�
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
/* 5 �������� *****************************************************
�ռ临�Ӷȣ���Ҫ�ǵݹ�ʱ�����ջ�ռ䣬ƽ���ռ临�Ӷ�ΪO(nlongn)��
ʱ�临�Ӷȣ���Ҫ��ʱ�䶼�����ڻ������棬��������ÿ�λ��ֵĻ�׼�����������ġ���
ֵ����¼��O(nlogn);������ԭ���鱾��������ģ���ʱO(n^2)��
ƽ��ʱ�临�Ӷ�ΪO(nlogn)��
�ȶ��ԣ� ���ȶ�
˼�룺���ε�˼�룬��������ת��ΪС���⣬�ݹ��˼�룬����Ҫ�Ĺ��̾��ǻ��֣����ֽ���
�ˣ�����Ҳ���ź����ˣ����������������㷨�зǳ���Ҫ��һ��
*******************************************************************/
#ifdef FUNQuick1    //ԭ���׼�汾 ���ţ����ݽṹ���ϵķ���,�ݹ飬�Ժ������Ϊ׼
    int start(L), end(n);                               //ע������Ϊ��ͳһ�ӿڲ���ô�ɵ�
    if (start < end)
    {
        int i(start), j(end), pivot = data[start];      //pivot�Ƚ�ֵȡ��
#if 0                                                   //pivot�Ƚ�ֵҲ���������ȡ�ķ���
        srand((unsigned int)time(NULL));
        swap(&data[i + rand() % (j - i + 1)], &data[i]);//���һ������
#endif
        while (i != j)
        {
            while (j > i && data[j] > pivot)            //����������<=pivot�� j
                --j;
            data[i] = data[j];
            while (i < j && data[i] < pivot)            //����������>=pivot�� i
                ++i;
            data[j] = data[i];
        }
        data[i] = pivot;                                //��ʱ��i����j iΪ�м�һ���ָ�λ��
        QuickSort(data, i - 1, start);      //QuickSort(data, start, i - 1);
        QuickSort(data, end, i + 1);        //QuickSort(data, i + 1, end);
    }
#endif
#ifdef FUNQuick2    //��һ�������ķ���
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
#ifdef FUNQuick3    //������ⷨ(һ���ҵ�����ֵ����)
    int start(left), end(right);                //ע������Ϊ��ͳһ�ӿڲ���ô�ɵ�
    if (left > right)
        return;
    int i(left), j(right), temp = data[left];   //temp�д�ľ��ǻ�׼�� 
    while (i != j)
    {
        while (data[j] >= temp && i < j)        //˳�����Ҫ��Ҫ�ȴ��ұ߿�ʼ�� 
            j--;
        while (data[i] <= temp && i < j)        //������ߵ� 
            i++;
        if (i < j)
            Swap(data[i],data[j]);              //�����������������е�λ�� 
    }
    Swap(data[left],data[i]);                   //���ս���׼����λ ��Ȼ������Ƿ��ŵ�
    quicksort(data,left, i - 1);                //����������ߵģ�������һ���ݹ�Ĺ��� 
    quicksort(data,i + 1, right);               //���������ұߵģ�������һ���ݹ�Ĺ��� 
#endif
#ifdef FUNQuick4    //�ķǵݹ��
#endif
}

void AutoSort::SelectSort(int data[], int n)
{
/* 6 ֱ��ѡ������ **************************************************
�ռ临�Ӷȣ�ֻ�õ���i,j,min,tmp�ĸ������������ʿռ临�Ӷ�ΪO(1).
ʱ�临�Ӷȣ����۱�ĳ�ʼ״̬��Σ��Ƚϴ������ﵽO(n^2),��ֱ��ѡ���������ú��
ʱ�临�Ӷȶ���O(n^2).
�ȶ��ԣ����ȶ����罫{5��3��2��5��4��1}���򣬵�һ�˾͸ı�������5�����λ�á�����
�����ǽ��������ֱ�Ӳ���������ۺϣ�����ֱ�Ӳ����ð���������ȶ��ģ������㷨�ǲ��ȶ���
˼�룺ÿһ�˴Ӵ�����ļ�¼��ѡ��ؼ�����С�ļ�¼��˳��������ź����ӱ�����֪��ȫ����¼�������
�����ԣ��ʺϴӴ�����¼��ѡ��һ���������¼�����10000����¼��ѡ��ؼ��ִ�СΪǰ10�ļ�¼
*******************************************************************/
    for (int i = 0; i < n - 1; ++i)                 //�ӵ�һ���������ڶ�������ȡ�Ƚ�ֵ
    {
        int min = i;                                //ȡ����ǱȽ�ֵ
        for (int j = i + 1; j < n; ++j)             //��������Ƚ�ֵ����
        {
            if (data[j] < data[min])
                min = j;                            //����С�Ļ�����
        }
        if (min != i)                               //��min=i��֤���Ѿ����������
        {
            int tmp = data[i];
            data[i] = data[min];
            data[min] = tmp;
        }
    }
}

//��������ѣ�ÿ�ν�����Ԫ���ƶ���ĩβ
void heapAdjust(int data[], int start, int end)
{
    int tmp = data[start];
    for (int i = 2 * start + 1; i <= end; i *= 2)
    {
        //���i<end���жϺ���Ҫ����i=end����֤����ǰ�ڵ�startֻ��һ�����ӽڵ㣬�Ͳ��ü����Ƚ���
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
/* 7 ������ ********************************************************
�ռ临�Ӷȣ�ֻ�õ����ĸ������������ռ临�Ӷ���O(1).
ʱ�临�Ӷȣ���ã������ƽ��ʱ�临�Ӷȶ���O(nlogn).
�ȶ��ԣ����ȶ�
˼�룺��������һ������ѡ������˼�룬��ԭ���鿴��Ϊһ����ȫ��������˳��洢�ṹ����
����ȫ��������˫�׽ڵ�ͺ��ӽڵ�֮������ڹ�ϵ���ڵ�ǰ��������ѡ��ؼ���
��󣨴���ѣ�������С��С���ѣ��ļ�¼�ƶ��������ĩβ��Ȼ���ʣ���Ԫ����ͬ
���Ĳ���
�����ԣ������˼�¼�����ٵı���ֱ��ѡ�������㷨����
*******************************************************************/
/*
//�㷨��Ϊ������Ҫ���֣��ѵ���������ɸѡ�㷨����������
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

    //������ʼ��
    for (int i = n / 2; i >= 0; --i)
        heapAdjust(data, i, n - 1);
    //���������
    for (int i = n - 1; i >= 0; --i)
    {
        //�����Ѷ������һ��Ԫ��
        int tmp = data[0];
        data[0] = data[i];
        data[i] = tmp;
        //�������������ѵ�����
        heapAdjust(data, 0, i - 1);
    }
}

void AutoSort::MergeSort(int data[], int n)
{
/* 8 �鲢���� ******************************************************
�ռ临�Ӷȣ�O(n),��Ҫһ����������������źϲ����������֮�����ɵ��±��ʹ鲢�����Ǿ͵�����
ʱ�临�Ӷȣ���ã����ƽ��ʱ�临�ӶȾ���O(nlogn)
�ȶ��ԣ��鲢�������ȶ��������㷨
˼�룺���������������ϵ������ϲ�Ϊһ���µ�������ݹ��˼��
*******************************************************************/
    /*
    //�����汾,������
    //void mergeSort_iter(int data[], int n)
    //{
    //    int *b = new int[n];
    //    int *a = data;
    //    //���forѭ����һ������logn�˹鲢
    //    for (int seg = 1; seg < n; seg += seg)
    //    {
    //        //һ�˹鲢����
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
    //        //����a��b
    //        int *tmp = a;
    //        a = b;
    //        b = tmp;
    //    }
    //    //������������
    //    if (a != data)
    //    {
    //        for (int i = 0; i < n; ++i)
    //            b[i] = a[i];
    //        b = a;
    //    }
    //    delete b;
    //}

    //һ�˹鲢���̣�������������ӱ�ϳ�һ���µ������
    void merge(int data[], int low, int mid, int high)
    {
        int i = low, j = mid + 1, k = 0;
        //��ʱ�洢�ź��������
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

    //�ݹ���ʽ�ֱ���������������������鲢����Ȼ��merge��һ���µ���������
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
    //�Զ����µĶ�·�鲢�����㷨
    void mergeSort(int data[], int n)
    {
        mergeSortR(data, 0, n - 1);
    }
    */

    ////�鲢����
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
/* 9 �������� ******************************************************
�ռ临�Ӷȣ��ռ临�Ӷ�ΪO(n)
ʱ�临�Ӷȣ���á����ƽ����ʱ�临�Ӷȶ���O(d(n+r)),����d�Ǵ�����Ԫ�ص����λ
����n��Ԫ�صĸ�����r�ǻ�����ʮ����r=10��������r=2����
�ȶ��ԣ������������ȶ������򷽷�
˼�룺ͨ��"����"��"�ռ�"����ʵ�����򣬲���Ҫ���йؼ���֮��ıȽϣ���һ�ֽ����ڶ�
�ؼ��������˼��Ե��ؼ�������ķ�������Ϊ���λ����(LSD�������λ��(MSD)
*******************************************************************/
    /*
//���������������ݵ����λ��d
int maxbit(int data[], int n)
{
    int d = 1;//�������λ������ʼΪ1
    int p = 10;
    for (int i = 0; i < n; ++i)
    {
        while (data[i] >= p)
        {
            p *= 10;//������ķ���
            ++d;
        }
    }
    return d;
}

//��������
void radixSort(int data[], int n)
{
    //�õ����λ��d
    int d = maxbit(data, n);
    int *tmp = new int[n];
    int *count = new int[10];//������
    int i, j, k;
    int radix = 1;
    for (i = 1; i <= d; ++i)
    {
        //��ռ�����
        for (j = 0; j < 10; ++j)
            count[j] = 0;
        for (j = 0; j < n; j++)
        {
            k = (data[j] / radix) % 10;//ͳ��ÿ��Ͱ�еļ�¼��
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
    //ִ�к���+++++++++++++++++++++++++++++++++++++++++++++
    //MessageBoxA(NULL, "ok", "ok", MB_OK);
    //=====================================================
    ftime(&endTime);
    printf("\n�����ˣ�%d ����\n", (endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm));
}



