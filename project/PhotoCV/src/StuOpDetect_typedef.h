
#ifndef STUOPDETECT_TYPE_H__
#define STUOPDETECT_TYPE_H__

//���������
typedef struct _Face_Info
{
    int x1, y1, x2, y2;
    _Face_Info()
    {
        x1 = x2 = y1 = y2 = 0;
    }
} Face_Info;

//��ʶ�Ƽ��ṹ��
typedef struct _Check_Info_old
{
    typedef struct _Sign_Location
    {
        //�����
        int x_left, y_down, x_right, y_top;
        _Sign_Location()
        {
            x_left = x_right = y_down = y_top = 0;
        }
    } Speed_Sign;
    Speed_Sign* nSigns; //ʶ������
    struct _recognized_Location
    {
        //ͼ��������
        int lefttop_x;
        int lefttop_y;
        int rightdown_x;
        int rightdown_y;
        _recognized_Location()
        {
            lefttop_x = lefttop_y = rightdown_x = rightdown_y = 0;
        }
    } InRegion;
    Point2f* center_P;
    int iSign;
} Check_Info_old;

typedef struct _Sign_Rect
{
    int bx, by, ex, ey;
    _Sign_Rect(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0)
    {
        bx = x1;
        by = y1;
        ex = x2;
        ey = y2;
    }
} Sign_Rect;

//��ʶ���ṹ��
typedef struct _Check_Info
{
    //�������
    vector<Sign_Rect> OutRegions;
    //ͼ��������
    Sign_Rect InRegion;
} Check_Info;



#endif
