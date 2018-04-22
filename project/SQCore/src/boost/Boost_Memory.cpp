
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
    //auto_ptr��C98��ʼ����stl֧����
    std::auto_ptr<int> ptr(new int);                //����int����ָ�� returnǰ�Զ��ͷ�
    //scoped_ptr ���ܿ����ĺͱȽϼ��� �����Ȳ���
    boost::scoped_ptr<string> ch(new string(""));
    *ch = "�ʺϽ������ඪ��ȥ";
    if (!ch)                                        //�����Ϊ��ָ�� ע�ⲻ���������Ƚϵ�
        cout << ch->c_str() << endl;                //����������ָͨ��һ��������
    //shared_ptr����ӽ�ָ�������ָ�� ʹ�ø�ָ��һ�� ֧������
    boost::shared_ptr<string> ch2(new string);
    //��make_shared��������new
    boost::shared_ptr<std::vector<int> > sqv = boost::make_shared<std::vector<int> > (10, 2);
    //C11��׼stl�Ѿ���ȫ����֧��shared_ptrָ����
    std::shared_ptr<string> stlstr(new string);
    *stlstr = "C11��������ָ��";
    cout << stlstr->c_str() << endl;
}