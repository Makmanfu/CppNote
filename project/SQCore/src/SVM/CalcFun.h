//*************************************************************************
//
//                  ��ѧ���㺯��
//
//                                      @���������� 2018-03-23
//
//*************************************************************************
#ifndef CALCFUN_H
#define CALCFUN_H

//��n! [num������ flag�Ƿ�ݹ�]
SQC_API size_t FactorialN(size_t num, bool flag = true);
//쳲������㷨 [num������ flag�Ƿ�ݹ�]
SQC_API size_t Fibonacci(size_t num, bool flag = true);
//��������� ע�������׳�
SQC_API double Permutation(size_t n, size_t i);
//����ֱ�ߵ�б��
SQC_API double LineGradient(double x1, double y1, double x2, double y2);
//ŷʽ�������
SQC_API double EuclideanMetric(int x1, int y1, int x2, int y2, int z1 = 0, int z2 = 0);
//У�����֤�㷨 (����18λ)
SQC_API bool CheckCID(char* f_cid);
//�����Զ�����ѭ��1��f_n
SQC_API int LoopNum(int f_n);
//��̬�ֲ�(��˹�ֲ�)
SQC_API double Gaussrand(void);


//��������㷨
void PascalTriangle(size_t Num);
//����Բ����10000λ
void PIcalc(int Num);

#endif


