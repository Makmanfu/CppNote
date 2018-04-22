//***********************************************************************
//       图像处理数学计算
//
//***********************************************************************

#ifndef MATMATH_H__
#define MATMATH_H__

//数学方法记录
class MatMath
{
public:
    MatMath();
    virtual ~MatMath();
public:
    //数学矩阵计算测试
    void MatrixCalcTest(void);
    //过滤一个点集的最小区域出来
    void GetPointsRect(const std::vector<cv::Point>& contours, cv::Rect& Out_Rect);
    //BF 算法(Brute Force)
    int BFMatch(char* s, char* p);
};

//最小二乘法曲线拟合曲线
class LeastSquares
{
private:
    //法方程矩阵
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
    //增广矩阵
    class AddMatrix
    {
    public:
        AddMatrix(void)
        {
            m = NULL;
        };
        ~AddMatrix(void) {};
        double** m;
        //矩阵对角化
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
    //点数据
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
    //录入点数据
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
    //计算出方程式
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
        //    //系数
        //    cout<<result[i];
        //    //x的n次方
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
            //系数
            sprintf(buf, "%d", (int) result[i]);
            result_str += buf;
            //x的n次方
            for (int z = 0; z < i; z++)
                result_str += "*x";
            if (i != n - 1)
                result_str += "+";
        }
        cout << "曲线拟合最小二乘法，拟合曲线为" << endl;
        cout << result_str << endl;
        return result_str;
    }
    //演示
    void CalcEquationShow(void)
    {
        double x = 0.0;
        double y = 0.0;
        cout << "曲线拟合的最小二乘法" << endl;
        cout << "请输入点的信息" << endl;
        cin >> x;
        cin >> y;
        while (x > 0.0)
        {
            add(x, y);
            cout << "请输入点的信息" << endl;
            cin >> x;
            cin >> y;
        }
        cout << "下面开始曲线拟合" << endl;
        cout << "请输入次数" << endl;
        cin >> n;
        n++;
        CalcEquation(v, n);
    }
};


#endif