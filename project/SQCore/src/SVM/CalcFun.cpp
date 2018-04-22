
#include "stdafx.h"
#include "CalcFun.h"
#include <math.h>
#include <memory>

size_t FactorialN(size_t num, bool flag)
{
    if (num > 16)
        return -1;      //计算溢出
    if (flag)
    {
        //递归计算阶乘
        if (num < 1)
            return 1;
        else
            return num * FactorialN(num - 1, true);
    } else {
        //迭代方法求阶乘
        int ret = 1;
        while (num > 1)
        {
            ret *= num;
            num--;
        }
        return ret;
    }

    //下面是打数据计算算法
    //if (f_n >10000)
    //    f_n = 10000;
    //size_t n,a[10000];
    //size_t i,j,k,count,temp;
    //n = f_n;
    //while(n)
    //{
    //    a[0]=1;
    //    count=1;
    //    for(i=1;i<=n;i++)
    //    {
    //        k=0;
    //        for(j=0;j<count;j++)
    //        {
    //            temp=a[j]*i+k;
    //            a[j]=temp%10;
    //            k=temp/10;
    //        }
    //        while(k)//记录进位
    //        {
    //            a[count++]=k%10;
    //            k/=10;
    //        }
    //    }
    //    for(j=f_n-1;j>=0;j--)
    //        if(a[j])
    //            break;//忽略前导0
    //    for(i=count-1;i>=0;i--)
    //        printf("%d",a[i]);
    //    printf("\n");
    //}
    //return 0;
}

size_t Fibonacci(size_t num, bool flag)
{
    if (num < 2)
        return num;
    if (flag)
    {   //递归算法
        return FactorialN(num - 1, true) + FactorialN(num - 2, true);
    } else {
        //迭代方法
        int* pData = new int[num + 1];
        pData[0] = 0;
        pData[1] = 1;
        for (int i = 2; i <= num; i++)
            pData[i] = pData[i - 1] + pData[i - 2];
        int ret = pData[num];
        delete pData;
        return ret;
    }
}

double Permutation(size_t n, size_t i)
{
    return ((double)FactorialN(n)) / ((double)(FactorialN(i) * FactorialN(n - i)));
}

double LineGradient(double x1, double y1, double x2, double y2)
{
    //计算直线的斜率
    if (x1 - x2 > 1E-8)         //这里要处理
        return 0;
    return fabs((y1 - y2) / (x1 - x2));
}

double EuclideanMetric(int x1, int y1, int x2, int y2, int z1, int z2)
{
    //2维
    return sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2));
    //3维
    return sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2) + pow((double)(z1 - z2), 2));
}

bool CheckCID(char* f_cid)
{
    if (strlen(f_cid) != 18)
    {
        //cout << "号码不正确!" << endl;
        return false;
    }
    char t_cid[18] = { '\0' };
    strcpy(t_cid, f_cid);
    //cout << "18:   " << t_cid << endl;
    //cout << "la:   " << t_cid[18] << endl;
    char code[18] = { '\0' };
    int ch = 0, niSum = 0;
    int  Wi[17] = { 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };
    char checkcode[11] = { '1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2' };
    for (int i = 0; i < 17; i++)
    {
        ch = atoi(&t_cid[i]);
        niSum = ch * Wi[i] + niSum;
    }
    ch = /*12 - */ niSum % 11;
    if (t_cid[18] == checkcode[ch])     //最后一位校验
    {
        //cout << "CID正确: " << t_cid << endl;
        return true;
    }
    //cout << "CID错误!" << endl;
    return false;
}

int LoopNum(int f_n)
{
    static int k = 0;
    if (k == f_n)
        k = 1;
    else
        k += 1;
    return k;
}

double Gaussrand(void)
{
    static double V1, V2, S;
    static int phase = 0;
    double X;
    if (phase == 0)
    {
        do
        {
            double U1 = (double) rand() / RAND_MAX;
            double U2 = (double) rand() / RAND_MAX;
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        }
        while (S >= 1 || S == 0);
        X = V1 * sqrt(-2 * log(S) / S);
    }
    else
        X = V2 * sqrt(-2 * log(S) / S);
    phase = 1 - phase;
    return X;
}



//杨辉三角算法
void PascalTriangle(size_t Num)
{
    for (size_t n = 0; n <= Num; n++)
    {
        for (size_t r = 0; r <= n; r++)
        {
            if (r == 0)
            {
                //排版设定开始
                for (size_t i = 0; i <= (Num - n); i++)
                    printf("  ");
            }
            else
                printf(" ");    //排版设定结束
            //--------------------------------
            size_t p = 1;
            for (size_t j = 1; j <= r; j++)
                p = p * (n - j + 1) / j;
            printf("%3d", p);
            //--------------------------------
        }
        printf("\n");
    }
}

//计算圆周率10000位
void PIcalc(int Num)
{
    //const int Num = 10000;
    const int N = Num / 4 + 1;      //N是array长度
    int* s = new int[N + 3];
    memset(s, 0, sizeof(int) * (N + 3));
    int* w = new int[N + 3];
    memset(w, 0, sizeof(int) * (N + 3));
    int* v = new int[N + 3];
    memset(v, 0, sizeof(int) * (N + 3));
    int* q = new int[N + 3];
    memset(q, 0, sizeof(int) * (N + 3));
    //int s[N + 3] = { 0 };
    //int w[N + 3] = { 0 };
    //int v[N + 3] = { 0 };
    //int q[N + 3] = { 0 };
    int n = (int)(Num / 1.39793 + 1);
    int k;
    w[0] = 16 * 5;
    v[0] = 4 * 239;
    for (k = 1; k <= n; k++)
    {
        // 套用公式
        //div(w, 25, w);
        //div(v, 239, v);
        //div(v, 239, v);
        for (int i(0), tmp(0), remain(0); i <= N + 1; i++)
        {
            tmp = w[i] + remain;
            w[i] = tmp / 25;
            remain = (tmp % 25) * 10000;
        }
        for (int i(0), tmp(0), remain(0); i <= N + 1; i++)
        {
            tmp = v[i] + remain;
            v[i] = tmp / 239;
            remain = (tmp % 239) * 10000;
        }
        for (int i(0), tmp(0), remain(0); i <= N + 1; i++)
        {
            tmp = v[i] + remain;
            v[i] = tmp / 239;
            remain = (tmp % 239) * 10000;
        }
        //sub(w, v, q);
        for (int i = N + 1, borrow(0); i >= 0; i--)
        {
            q[i] = w[i] - v[i] - borrow;
            if (q[i] >= 0)
                borrow = 0;
            else   // 借位
            {
                q[i] = q[i] + 10000;
                borrow = 1;
            }
        }
        //div(q, 2 * k - 1, q);
        for (int i(0), tmp(0), remain(0); i <= N + 1; i++)
        {
            tmp = q[i] + remain;
            q[i] = tmp / (2 * k - 1);
            remain = (tmp % (2 * k - 1)) * 10000;
        }
        if (k % 2)   // 奇数项
        {
            //add(s, q, s);
            for (int i = N + 1, carry(0); i >= 0; i--)
            {
                s[i] = s[i] + q[i] + carry;
                if (s[i] < 10000)
                    carry = 0;
                else     // 进位
                {
                    s[i] = s[i] - 10000;
                    carry = 1;
                }
            }
        }
        else    // 偶数项
        {
            //sub(s, q, s);
            for (int i = N + 1, borrow(0); i >= 0; i--)
            {
                s[i] = s[i] - q[i] - borrow;
                if (s[i] >= 0)
                    borrow = 0;
                else   // 借位
                {
                    s[i] = s[i] + 10000;
                    borrow = 1;
                }
            }
        }
    }
    printf("%d.", s[0]);
    for (k = 1; k < N; k++)
        printf("%04d", s[k]);
    delete[] s;
    delete[] w;
    delete[] v;
    delete[] q;
    printf("\n");
}




