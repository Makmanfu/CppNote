//***********************************************************************
//       ͼ������ѧ����
//
//***********************************************************************

#ifndef MATMATH_H__
#define MATMATH_H__

//��ѧ������¼
class MatMath
{
public:
    MatMath();
    virtual ~MatMath();
public:
    //��ѧ����������
    void MatrixCalcTest(void);
    //����һ���㼯����С�������
    void GetPointsRect(const std::vector<cv::Point>& contours, cv::Rect& Out_Rect);
    //BF �㷨(Brute Force)
    int BFMatch(char* s, char* p);
};

//��С���˷������������
class LeastSquares
{
private:
    //�����̾���
    class Matrix
    {
    public:
        Matrix(void)
        {
            m = NULL;
        };
        ~Matrix(void) {};
        double** m;
    } myMatrix;
    //�������
    class AddMatrix
    {
    public:
        AddMatrix(void)
        {
            m = NULL;
        };
        ~AddMatrix(void) {};
        double** m;
        //����Խǻ�
        void trapezoidal(int arrayLong)
        {
            for (int i = 0; i < arrayLong; i++)
                for (int ii = i + 1; ii < arrayLong; ii++)
                {
                    for (int jj = i + 1; jj < arrayLong + 1; jj++)
                    {
                        double d = m[ii][i] / m[i][i];
                        double d2 = d * m[i][jj];
                        m[ii][jj] = m[ii][jj] - d2;
                    }
                    m[ii][i] = 0.0;
                }
            for (int i = arrayLong - 1; i >= 0; i--)
                for (int j = 0; j < i; j++)
                    m[j][arrayLong] = m[j][arrayLong] - (m[j][i] / m[i][i]) * m[i][arrayLong];
        }
    } myAddMatrix;
    //������
    vector<Point> v;
    int n;
    int arrayLong;
    double* result;
public:
    LeastSquares(void) : n(0), arrayLong(0)
    {
        result = NULL;
    };
    ~LeastSquares(void) {};
    //¼�������
    void add(double xx, double yy)
    {
        cv::Point2d p(xx, yy);
        v.push_back(p);
    };
    void makeMatrix()
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                double result = 0;
                for (int l = 0; l < arrayLong; l++)
                {
                    double x = 1;
                    for (int z = 0; z < i + j; z++)
                        x = x * (v[l].x);
                    result += x;
                }
                myMatrix.m[i][j] = result;
            }
    };
    void makeAddMatrix()
    {
        for (int i = 0; i < n; i++)
        {
            int j = 0;
            for (; j < n; j++)
                myAddMatrix.m[i][j] = myMatrix.m[i][j];
            double result = 0;
            for (int l = 0; l < arrayLong; l++)
            {
                double x = v[l].y;
                for (int z = 0; z < i; z++)
                    x = x * v[l].x;
                result += x;
            }
            myAddMatrix.m[i][j] = result;
        }
    };
    //���������ʽ
    string CalcEquation(vector<Point> v, int n)
    {
        if (v.empty())
            return "0";
        arrayLong = v.size();
        myMatrix.m = new double*[n];
        for (int i = 0; i < n; i++)
            myMatrix.m[i] = new double[n];
        myAddMatrix.m = new double*[n];
        for (int i = 0; i < n; i++)
            myAddMatrix.m[i] = new double[n + 1];
        makeMatrix();
        makeAddMatrix();
        result = new double[n];
        myAddMatrix.trapezoidal(n);
        for (int i = 0; i < n; i++)
            result[i] = myAddMatrix.m[i][n] / myAddMatrix.m[i][i];
        //cout<<"f(x)=";
        //for(int i=0;i<n;i++)
        //{
        //    //ϵ��
        //    cout<<result[i];
        //    //x��n�η�
        //    for(int z=0;z<i;z++)
        //        cout<<"*x";
        //    if(i!=n-1)
        //        cout<<"+";
        //}
        string result_str("");
        result_str += "f(x)=";
        char buf[255] = {0};
        for (int i = 0; i < n; i++)
        {
            //ϵ��
            sprintf(buf, "%d", (int) result[i]);
            result_str += buf;
            //x��n�η�
            for (int z = 0; z < i; z++)
                result_str += "*x";
            if (i != n - 1)
                result_str += "+";
        }
        cout << "���������С���˷����������Ϊ" << endl;
        cout << result_str << endl;
        return result_str;
    }
    //��ʾ
    void CalcEquationShow(void)
    {
        double x = 0.0;
        double y = 0.0;
        cout << "������ϵ���С���˷�" << endl;
        cout << "����������Ϣ" << endl;
        cin >> x;
        cin >> y;
        while (x > 0.0)
        {
            add(x, y);
            cout << "����������Ϣ" << endl;
            cin >> x;
            cin >> y;
        }
        cout << "���濪ʼ�������" << endl;
        cout << "���������" << endl;
        cin >> n;
        n++;
        CalcEquation(v, n);
    }
};


#endif