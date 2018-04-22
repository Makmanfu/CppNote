//===================================================
//
//              boost 解析文本
//
//===================================================

#ifndef BOOST_TEXT_H
#define BOOST_TEXT_H
#include <vector>
#include <iostream>
#include <string>
using namespace std;

//类型转换例子
void castUserType(void);

//一行文本数据 根据[spliteString]切割装到vField列表中 调用boost
std::vector<string> splitField(const string& strValue, const char* spliteString = "\t");

//一行文本数据 根据逗号切割装到vField列表中
void GetField(string& strSource, vector< string >& vField);






#endif