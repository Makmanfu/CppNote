//*************************************************************************
//
//                  数学计算函数
//
//                                      @阿甘整理于 2018-03-23
//
//*************************************************************************
#ifndef CALCFUN_H
#define CALCFUN_H

//求n! [num数列项 flag是否递归]
SQC_API size_t FactorialN(size_t num, bool flag = true);
//斐波那契算法 [num数列项 flag是否递归]
SQC_API size_t Fibonacci(size_t num, bool flag = true);
//求组合排列 注意依赖阶乘
SQC_API double Permutation(size_t n, size_t i);
//计算直线的斜率
SQC_API double LineGradient(double x1, double y1, double x2, double y2);
//欧式距离计算
SQC_API double EuclideanMetric(int x1, int y1, int x2, int y2, int z1 = 0, int z2 = 0);
//校验身份证算法 (必须18位)
SQC_API bool CheckCID(char* f_cid);
//调用自动计数循环1到f_n
SQC_API int LoopNum(int f_n);
//正态分布(高斯分布)
SQC_API double Gaussrand(void);


//杨辉三角算法
void PascalTriangle(size_t Num);
//计算圆周率10000位
void PIcalc(int Num);

#endif


