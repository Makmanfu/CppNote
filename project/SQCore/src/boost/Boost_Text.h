//===================================================
//
//              boost �����ı�
//
//===================================================

#ifndef BOOST_TEXT_H
#define BOOST_TEXT_H
#include <vector>
#include <iostream>
#include <string>
using namespace std;

//����ת������
void castUserType(void);

//һ���ı����� ����[spliteString]�и�װ��vField�б��� ����boost
std::vector<string> splitField(const string& strValue, const char* spliteString = "\t");

//һ���ı����� ���ݶ����и�װ��vField�б���
void GetField(string& strSource, vector< string >& vField);






#endif