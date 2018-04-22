
#include "stdafx.h"
#include "BoostConfig.h"
#include "Boost_Text.h"
#include <iostream>
#include <vector>
using namespace std;
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost;

#pragma warning(disable:4996)

void castUserType(void)
{
    int x = lexical_cast<int> ("100");                  //�ַ���->����
    long y = lexical_cast<long> ("2000");               //�ַ���->������
    float pai = lexical_cast<float> ("3.14159e5");      //�ַ���->float
    double e = lexical_cast<double> ("2.71828");        //�ַ���->double
    cout << x << y << pai << e << endl;
    string str = lexical_cast<string> (456);            //����->�ַ���
    cout << str << endl;
    cout << lexical_cast<string> (0.618) << endl;       //float->�ַ���
    cout << lexical_cast<string> (0x10) << endl;        //16��������->�ַ���
}

//һ���ı����� ����[spliteString]�и�װ��vField�б��� ����boost
vector<string> splitField(const string& strValue, const char* spliteString)
{
    std::vector <std::string> result;
    if (!strValue.empty())
    {
        string split_str(spliteString);
        //boost::split(result, strValue, boost::is_any_of(split_str));
    }
    return result;
}

//һ���ı����� ���ݶ����и�װ��vField�б���
void GetField(string& strSource, vector<string>& vField)
{
    size_t sSubstrStart(0), sSubstrEnd(0), sOldStart(0);
    vField.clear();
    while (1)
    {
        sSubstrEnd = strSource.find(',', sSubstrStart);
        if (sSubstrEnd != std::string::npos)
        {
            std::string field = strSource.substr(sOldStart, sSubstrEnd - sOldStart);
            size_t find = field.find('\"');
            if ((find != std::string::npos) && (find == field.rfind('\"')))
                sSubstrStart = sSubstrEnd + 1;
            else
            {
                sOldStart = sSubstrEnd + 1;
                sSubstrStart = sOldStart;
                size_t nPos = 0;
                while ((nPos = field.find('\"', nPos)) != std::string::npos)
                    field.erase(nPos, 1);
                vField.push_back(field);
            }
        }
        else
            break;
    }
    size_t nPos = 0;
    std::string lastField = strSource.substr(sSubstrStart);
    while ((nPos = lastField.find('\"', nPos)) != std::string::npos)
        lastField.erase(nPos, 1);
    vField.push_back(lastField);
    //����2
    //vField.clear();
    //string::const_iterator i = strSource.begin(), last = i;
    //for (; i != strSource.end(); ++i)
    //{
    //  if (*i == ',')
    //  {   // ȥ������
    //      vField.push_back(string(last, i));
    //      last = i + 1;
    //  }
    //}
    //if (last != i)
    //{
    //  vField.push_back(string(last, i));
    //}
}
