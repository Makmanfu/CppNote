//-----------------------------------------------------------------------------
//
//                           STLѧϰʹ�ü�¼
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
    //д��vector�ڴ�����
    void WriteVectorDat(const char* _filename)
    {
        FILE* fp = fopen(_filename, "w+");
        //д��vector���������� (�������ַ�������)
        fwrite(&vdT[0], 1, vdT.size()* sizeof(T), fp);
        //fwrite((char*)&(vdT[0]), 1, vdT.size()* sizeof(T), fp);
        fclose(fp);
    };
    //����vec��������Сֵ
    void GetVectorMaxMin(T& _max, T& _min)
    {
        //����1 stl�㷨
        _max = *(std::max_element(std::begin(vdT), std::end(vdT)));
        _min = *(std::min_element(std::begin(vdT), std::end(vdT)));

        //����2 ��v
        //_min = _max = vdT[0];       //�@�Y��̫�Ì�
        //for (vector<T>::const_iterator it = vdT.begin();
        //    it != vdT.end(); ++it)
        //{
        //    if (*it < _min) _min = *it;
        //    if (*it > _max) _max = *it;
        //}
    };

    //����ȽϺ���
    bool CmpAge(T& p1, T& p2) //(������Ҫ const�����򱨴�)
    {
        return p1 < p2;//С�ںű�ʾ����С��������С����ǰ������ں�
        //���ں��෴
    }
    //����vec�нṹ���ֶ������Сֵ
    void GetVectorItemMaxMin(T& _max, T& _min)
    {
        //�����Ӧ�������˰�
        sort(vdT.begin(), vdT.end(), CmpAge);
    };
};

//vectorѧϰ��
class CStudyVector
{
public:
    CStudyVector(){};
    ~CStudyVector(){};
public:
    vector<string> vdata;
    //vector��ʼ��size
    void VecSize_Demo(void)
    {
        vdata.reserve(1000);       //�ֶ���������
        vdata.resize(2000);        //�ֶ��������ݿռ�
        cout << "vdata.size     ��ʵ���ݿռ�" << vdata.size() << endl;
        cout << "vdata.capacity ����(>=size)" << vdata.capacity() << endl;
    };
    //vector����ʹ��
    void VecADD_Demo(void)
    {
        //��vectorĩβ�������
        vdata.push_back("ok");
        vdata.push_back("one");
        vdata.push_back("one");
        vdata.push_back("two");
        vdata.push_back("three");
        vdata.push_back("��������");
        vdata.insert(vdata.end(), "��β��������");
        vdata.insert(vdata.begin(), "ͷ����������");
    };
    //vectorɾ��ʹ��
    void VecDel_Demo(void)
    {
        //ɾ��ĩβһ������
        vdata.pop_back();
        //ɾ��һ����������Ԫ��
        vdata.erase(vdata.end());
        //ɾ������������Ԫ��
        vdata.erase(vdata.begin(), vdata.end());
        //����1
        for (vector<string>::iterator iter = vdata.begin(); iter != vdata.end();)
        {
            if (1/*��������*/)
                iter = vdata.erase(iter);
            else
                iter++;
        }
        //����2
        for (vector<string>::iterator iter = vdata.begin(); iter != vdata.end();)
        {
            if (1/*��������*/)
                vdata.erase(iter++);
            else
                iter++;
        }
    };
    //vector��ʾ����
    void VecShow_Demo(void)
    {
        //�õ���������ȡvector��ֵ �����������þ���ֵ
        for (vector<string>::const_iterator it = vdata.begin(); it != vdata.end(); ++it)
            cout << "iterator" << it->c_str() << endl;
        //����������ȡvectorֵ(���Ƽ�ʹ��,С��Խ��)
        for (size_t i = 0; i < vdata.size(); i++)
            cout << "array" << vdata[i].c_str() << endl;
        //�÷������������ȡvector��ֵ
        for (vector<string>::const_reverse_iterator rit = vdata.rbegin(); rit != vdata.rend(); ++rit)
            cout << "reverse_iterator" << rit->c_str() << endl;
    };
    //vector��������
    void VecFind_Demo(void)
    {
        //����vector���м��� f_key
        cout <<"vdata�� one ����:"<< count(vdata.begin(), vdata.end(), "one") << endl;
        //����vector<string>�� f_key
        vector<string>::const_iterator findk = find(vdata.begin(), vdata.end(), "two");
        if (findk == vdata.end())
            cout << "not find" << endl;
        else
            cout << findk->c_str() << endl;
    };
};

//mapѧϰ��
class CStudyMap
{
public:
    CStudyMap(){};
    ~CStudyMap(){};
private:
    map<int, string> MDat;
    //Map����ʹ��
    void MapADD_Demo(void)
    {
        //ͨ����Ա����insert��STL��pair��ֵ
        MDat.insert(pair<int, string>(1, "����"));
        //ͨ��value_type��ֵ
        MDat.insert(map<int, string>::value_type(2, "����"));
        //ͨ��make_pair��ֵ
        MDat.insert(make_pair(3, "����"));
    };
    //Map��ʾ����
    void MapShow_Demo(void)
    {
        //�õ���������ȡmap��ֵ
        for (map<int, string> ::const_iterator it = MDat.begin();
            it != MDat.end(); it++)
            cout << it->first << "  " << it->second.c_str() << endl;
    };
    //vector��������
    void MapFind_Demo(void)
    {
        int findnum(2);         //map���ұ��Ϊ2����
        map<int, string>::const_iterator findr = MDat.find(findnum);
        if (findr != MDat.end())
            cout << findr->first << "  " << findr->second.c_str() << endl;
        else
            cout << "û���ҵ�����" << endl;

        //multimap[��1�Զ��ϵ]
        cout << "multimap���ұ��Ϊ" << findnum << endl;
        multimap<int, string> MutiMapData;
        MutiMapData.insert(pair<int, string>(1, "����"));
        MutiMapData.insert(multimap<int, string>::value_type(2, "����"));
        MutiMapData.insert(make_pair(2, "����"));
        MutiMapData.insert(map<int, string>::value_type(3, "����2"));
        MutiMapData.insert(make_pair(4, "AAA"));
        MutiMapData.insert(make_pair(5, "AAA"));
        multimap<int, string> ::const_iterator mfindr = MutiMapData.find(findnum);
        for (int k = 0; k != MutiMapData.count(findnum); k++, mfindr++)
            cout << mfindr->first << "  " << mfindr->second.c_str() << endl;
        cout << "multimapȫ������:" << findnum << endl;
    };
};

//�����n�������
template <typename T>
void SPLITARRAY(const vector<T>& datas, size_t n, vector<vector<T> >& table)
{
    if (n == 0)
        return;
#define PLANFUN3
    vector<T> tmp;
#ifdef PLANFUN1 //����һ
    for (size_t i = 1; i <= datas.size(); ++i)
    {
        tmp.push_back(datas[i - 1]);                  //ע��������0��ʼ��
        if (i % n == 0)                               //��N����
        {
            table.push_back(tmp);
            tmp.clear();                              //�������
        }
    }
    table.push_back(tmp);
    tmp.clear();                                      //����
#endif 

#ifdef PLANFUN2 //������ ����ʱ��õ�Ч�ʸ���һ
    size_t k = datas.size() / n, pos(0);
    //ע��KС��1ʱ��pȥ���� �˴��������
    size_t p = k < 1 ? datas.size() : datas.size() % n;
    for (size_t i = 0; i < k; ++i)
    {
        for (size_t pt = 0; pt < n; ++pt)
            tmp.push_back(datas[pos++]);
        table.push_back(tmp);
        tmp.clear();                                    //�������
    }
    if (p > 0)
    {
        for (size_t pt = 0; pt < p; ++pt)
            tmp.push_back(datas[pos++]);
        table.push_back(tmp);
        tmp.clear();
    }
#endif 

#ifdef PLANFUN3 //������ STL
    size_t k = datas.size() / n, pos(0);
    //ע��KС��1ʱ��pȥ���� �˴��������
    size_t p = k < 1 ? datas.size() : datas.size() % n;
    for (size_t i = 0; i < k; ++i)
        table.push_back(vector<T>(datas.begin() + n*i, datas.begin() + n*(i + 1)));
    if (p > 0)
        table.push_back(vector<T>(datas.begin() + n*k, datas.end()));
#endif 
}

//����ֶ�demos
struct SPLITARRAY_show
{
    static int main(int argc, char** argv)
    {
        //���ݱ�
        vector<size_t> DATATABLE;
        //�ֽ��
        vector<vector<size_t> > OutTABLE;
        //��������
        for (size_t i = 0; i < 17; ++i)
            DATATABLE.push_back(i + 1);
        //������ʾ
        for (vector<size_t>::const_iterator it = DATATABLE.begin();
            it != DATATABLE.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
        std::cout << "������ֶ�����"<<std::endl;
        //���Է�3��
        int DD(3);
        std::cin >> DD;
        //�ֽ��㷨
        SPLITARRAY(DATATABLE, DD, OutTABLE);
        //��ʾ
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

