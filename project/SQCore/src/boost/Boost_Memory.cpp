
#include "stdafx.h"
#include "BoostConfig.h"
#include "Boost_Memory.h"
#include <iostream>
#include <vector>
using namespace std;
#include <boost\smart_ptr.hpp>
using namespace boost;

void boost_autoptr(void)
{
    //auto_ptr从C98开始就在stl支持了
    std::auto_ptr<int> ptr(new int);                //定义int智能指针 return前自动释放
    //scoped_ptr 不能拷贝的和比较计算 容器等操作
    boost::scoped_ptr<string> ch(new string(""));
    *ch = "适合将复杂类丢进去";
    if (!ch)                                        //如果不为空指针 注意不能做其他比较的
        cout << ch->c_str() << endl;                //可以像用普通指针一样解引用
    //shared_ptr是最接近指针的智能指针 使用跟指针一样 支持容器
    boost::shared_ptr<string> ch2(new string);
    //用make_shared工厂消除new
    boost::shared_ptr<std::vector<int> > sqv = boost::make_shared<std::vector<int> > (10, 2);
    //C11标准stl已经完全内置支持shared_ptr指针了
    std::shared_ptr<string> stlstr(new string);
    *stlstr = "C11内置智能指针";
    cout << stlstr->c_str() << endl;
}