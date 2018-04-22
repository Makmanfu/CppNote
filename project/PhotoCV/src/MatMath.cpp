
#include "stdafx.h"
#include "MatMath.h"


MatMath::MatMath()
{
}

MatMath::~MatMath()
{
}

void MatMath::MatrixCalcTest(void)
{
    float src[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Mat src_(3, 3, CV_32F, src);
    cout << src_ << endl;
    uchar mask[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    Mat mask_(3, 3, CV_8U, mask);
    cout << mask_ << endl;
    Mat dst, dst1;
    add(src_, Scalar(1.f), dst, mask_);
    cout << dst << endl;
    src_.copyTo(dst1, mask_);
    cout << dst1 << endl;
}

void MatMath::GetPointsRect(const std::vector<cv::Point>& contours, cv::Rect& Out_Rect)
{
    //区域坐标
    Point B(999999, 999999), E(0, 0);
    for (std::vector<cv::Point>::const_iterator it = contours.begin();
         it != contours.end(); ++it)
    {
        if (it->x < B.x) B.x = it->x;
        if (it->x > E.x) E.x = it->x;
        if (it->y < B.y) B.y = it->y;
        if (it->y > E.y) E.y = it->y;
    }
    Out_Rect = cv::Rect(B.x, B.y, E.x - B.x, E.y - B.y);
}

int MatMath::BFMatch(char* s, char* p)
{
    int m, n;
    m = strlen(s);
    n = strlen(p);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (s[i + j] != p[j])
                break;
            if (j == n - 1)
                return i;
        }
    }
    return -1;
    //测试用例
    //char s[100] = { "ababcababa" }; //目标串
    //char p[100] = { "ababa" }; //模式串
    //int flag = BFMatch(s, p);
    //if (flag == -1)
    //    printf("匹配失败！\n");
    //else
    //    printf("匹配成功！从目标串的s[%d] = %c 处开始匹配\n", flag, s[flag]);
}




