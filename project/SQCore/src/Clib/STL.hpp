//-----------------------------------------------------------------------------
//
//                           STL学习使用记录
//                                          2014-06-16 AGAN
//
//-----------------------------------------------------------------------------

#ifndef STL_HPP
#define STL_HPP
#include <vector>
#include <iostream>
#include <memory>
#include <map>
using namespace std;
using std::vector;

template <typename T>
class CVectorTool
{
public:
    vector<T> vdT;
public:
    CVectorTool(){};
    ~CVectorTool(){};
    void SetVecData(vector<T>& _vdt)
    {
        vdT = _vdt;
    };
public:
    //写出vector内存数据
    void WriteVectorDat(const char* _filename)
    {
        FILE* fp = fopen(_filename, "w+");
        //写出vector里所有数据 (下面两种方法都行)
        fwrite(&vdT[0], 1, vdT.size()* sizeof(T), fp);
        //fwrite((char*)&(vdT[0]), 1, vdT.size()* sizeof(T), fp);
        fclose(fp);
    };
    //计算vec項中最大最小值
    void GetVectorMaxMin(T& _max, T& _min)
    {
        //方法1 stl算法
        _max = *(std::max_element(std::begin(vdT), std::end(vdT)));
        _min = *(std::min_element(std::begin(vdT), std::end(vdT)));

        //方法2 遍歷
        //_min = _max = vdT[0];       //這裏不太好寫
        //for (vector<T>::const_iterator it = vdT.begin();
        //    it != vdT.end(); ++it)
        //{
        //    if (*it < _min) _min = *it;
        //    if (*it > _max) _max = *it;
        //}
    };

    //定义比较函数
    bool CmpAge(T& p1, T& p2) //(参数不要 const，否则报错)
    {
        return p1 < p2;//小于号表示：从小到大排序（小的在前，大的在后）
        //大于号相反
    }
    //计算vec中结构体字段最大最小值
    void GetVectorItemMaxMin(T& _max, T& _min)
    {
        //排序后应该明白了吧
        sort(vdT.begin(), vdT.end(), CmpAge);
    };
};

//vector学习类
class CStudyVector
{
public:
    CStudyVector(){};
    ~CStudyVector(){};
public:
    vector<string> vdata;
    //vector初始化size
    void VecSize_Demo(void)
    {
        vdata.reserve(1000);       //手动设置容量
        vdata.resize(2000);        //手动设置数据空间
        cout << "vdata.size     真实数据空间" << vdata.size() << endl;
        cout << "vdata.capacity 容量(>=size)" << vdata.capacity() << endl;
    };
    //vector增加使用
    void VecADD_Demo(void)
    {
        //向vector末尾添加数据
        vdata.push_back("ok");
        vdata.push_back("one");
        vdata.push_back("one");
        vdata.push_back("two");
        vdata.push_back("three");
        vdata.push_back("错误数据");
        vdata.insert(vdata.end(), "结尾插入数据");
        vdata.insert(vdata.begin(), "头部插入数据");
    };
    //vector删除使用
    void VecDel_Demo(void)
    {
        //删除末尾一个数据
        vdata.pop_back();
        //删除一个迭代器的元素
        vdata.erase(vdata.end());
        //删除迭代器区间元素
        vdata.erase(vdata.begin(), vdata.end());
        //方法1
        for (vector<string>::iterator iter = vdata.begin(); iter != vdata.end();)
        {
            if (1/*符合条件*/)
                iter = vdata.erase(iter);
            else
                iter++;
        }
        //方法2
        for (vector<string>::iterator iter = vdata.begin(); iter != vdata.end();)
        {
            if (1/*符合条件*/)
                vdata.erase(iter++);
            else
                iter++;
        }
    };
    //vector显示数据
    void VecShow_Demo(void)
    {
        //用迭代器遍历取vector的值 迭代器解引用就是值
        for (vector<string>::const_iterator it = vdata.begin(); it != vdata.end(); ++it)
            cout << "iterator" << it->c_str() << endl;
        //用数组索引取vector值(不推荐使用,小心越界)
        for (size_t i = 0; i < vdata.size(); i++)
            cout << "array" << vdata[i].c_str() << endl;
        //用反向迭代器遍历取vector的值
        for (vector<string>::const_reverse_iterator rit = vdata.rbegin(); rit != vdata.rend(); ++rit)
            cout << "reverse_iterator" << rit->c_str() << endl;
    };
    //vector查找数据
    void VecFind_Demo(void)
    {
        //计算vector中有几个 f_key
        cout <<"vdata中 one 个数:"<< count(vdata.begin(), vdata.end(), "one") << endl;
        //查找vector<string>中 f_key
        vector<string>::const_iterator findk = find(vdata.begin(), vdata.end(), "two");
        if (findk == vdata.end())
            cout << "not find" << endl;
        else
            cout << findk->c_str() << endl;
    };
};

//map学习类
class CStudyMap
{
public:
    CStudyMap(){};
    ~CStudyMap(){};
private:
    map<int, string> MDat;
    //Map增加使用
    void MapADD_Demo(void)
    {
        //通过成员函数insert和STL的pair赋值
        MDat.insert(pair<int, string>(1, "张三"));
        //通过value_type赋值
        MDat.insert(map<int, string>::value_type(2, "李四"));
        //通过make_pair赋值
        MDat.insert(make_pair(3, "王五"));
    };
    //Map显示数据
    void MapShow_Demo(void)
    {
        //用迭代器遍历取map的值
        for (map<int, string> ::const_iterator it = MDat.begin();
            it != MDat.end(); it++)
            cout << it->first << "  " << it->second.c_str() << endl;
    };
    //vector查找数据
    void MapFind_Demo(void)
    {
        int findnum(2);         //map查找标号为2的项
        map<int, string>::const_iterator findr = MDat.find(findnum);
        if (findr != MDat.end())
            cout << findr->first << "  " << findr->second.c_str() << endl;
        else
            cout << "没有找到啦！" << endl;

        //multimap[可1对多关系]
        cout << "multimap查找标号为" << findnum << endl;
        multimap<int, string> MutiMapData;
        MutiMapData.insert(pair<int, string>(1, "张三"));
        MutiMapData.insert(multimap<int, string>::value_type(2, "李四"));
        MutiMapData.insert(make_pair(2, "王五"));
        MutiMapData.insert(map<int, string>::value_type(3, "王五2"));
        MutiMapData.insert(make_pair(4, "AAA"));
        MutiMapData.insert(make_pair(5, "AAA"));
        multimap<int, string> ::const_iterator mfindr = MutiMapData.find(findnum);
        for (int k = 0; k != MutiMapData.count(findnum); k++, mfindr++)
            cout << mfindr->first << "  " << mfindr->second.c_str() << endl;
        cout << "multimap全部数据:" << findnum << endl;
    };
};

//数组分n段数组表
template <typename T>
void SPLITARRAY(const vector<T>& datas, size_t n, vector<vector<T> >& table)
{
    if (n == 0)
        return;
#define PLANFUN3
    vector<T> tmp;
#ifdef PLANFUN1 //方法一
    for (size_t i = 1; i <= datas.size(); ++i)
    {
        tmp.push_back(datas[i - 1]);                  //注意索引从0开始的
        if (i % n == 0)                               //与N整除
        {
            table.push_back(tmp);
            tmp.clear();                              //必须归零
        }
    }
    table.push_back(tmp);
    tmp.clear();                                      //归零
#endif 

#ifdef PLANFUN2 //方法二 测试时间得到效率高于一
    size_t k = datas.size() / n, pos(0);
    //注意K小于1时让p去处理 此处不好理解
    size_t p = k < 1 ? datas.size() : datas.size() % n;
    for (size_t i = 0; i < k; ++i)
    {
        for (size_t pt = 0; pt < n; ++pt)
            tmp.push_back(datas[pos++]);
        table.push_back(tmp);
        tmp.clear();                                    //必须归零
    }
    if (p > 0)
    {
        for (size_t pt = 0; pt < p; ++pt)
            tmp.push_back(datas[pos++]);
        table.push_back(tmp);
        tmp.clear();
    }
#endif 

#ifdef PLANFUN3 //方法三 STL
    size_t k = datas.size() / n, pos(0);
    //注意K小于1时让p去处理 此处不好理解
    size_t p = k < 1 ? datas.size() : datas.size() % n;
    for (size_t i = 0; i < k; ++i)
        table.push_back(vector<T>(datas.begin() + n*i, datas.begin() + n*(i + 1)));
    if (p > 0)
        table.push_back(vector<T>(datas.begin() + n*k, datas.end()));
#endif 
}

//数组分段demos
struct SPLITARRAY_show
{
    static int main(int argc, char** argv)
    {
        //数据表
        vector<size_t> DATATABLE;
        //分解表
        vector<vector<size_t> > OutTABLE;
        //测试数据
        for (size_t i = 0; i < 17; ++i)
            DATATABLE.push_back(i + 1);
        //线性显示
        for (vector<size_t>::const_iterator it = DATATABLE.begin();
            it != DATATABLE.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
        std::cout << "请输入分段数："<<std::endl;
        //测试分3段
        int DD(3);
        std::cin >> DD;
        //分解算法
        SPLITARRAY(DATATABLE, DD, OutTABLE);
        //显示
        for (vector<vector<size_t> >::const_iterator it = OutTABLE.begin();
            it != OutTABLE.end(); ++it)
        {
            for (vector<size_t>::const_iterator iit = it->begin();
                iit != it->end(); ++iit)
                std::cout << *iit << " ";
            std::cout << std::endl;
        }
        return 0;
    };
};
















#endif

