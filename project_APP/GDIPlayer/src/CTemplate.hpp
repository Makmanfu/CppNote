//*********************************************************************
//
//          templateʹ�ü�¼ ��ģ��Ϊȫ��ģ���޸������
//                                          @2016-09-14��������
//*********************************************************************
#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include <vector>
#include <iostream>
#include <memory>
#include <map>
#include <functional>
#include <algorithm>
using namespace std;
using std::vector;

#define STR_VECT vector<string>
#define INT_VECT vector<int>

//����ģ��v1>v2 1 v1=v2 0 v1<v2 -1 
template <typename T>
int compare(const T& v1, const T& v2)
{
    if (v2 < v1) return 1;
    if (v1 < v2) return -1;
    return 0;
}

//˳�����[����,����,Ҫ�ҵ�ֵ]
template <typename T>
int SearchKey(const T* pArray, int n, T key)
{
    for (int i = 0; i <= n; i++)
        if (pArray[i] == key)
            return i;
    return -1;
}

//��������
template <typename T>
void Swap(T& A, T& B)
{
    T temp(A);
    A = B;
    B = temp;
}

template <typename T>
inline void PSwap(T* A, T* B)
{
    T temp = *A;
    *A = *B;
    *B = temp;
}


//ģ�庯��::�ͷ�ָ�������е��ڴ棬��vector,list,set��
template <typename PairType>
struct deletor : public std::unary_function < PairType, void >
{
    void operator()(PairType p)
    {
        delete p;
    }
};

template <typename Container_type>
void free_PointerContainer_memory(Container_type& Container)
{
    std::for_each(Container.begin(), Container.end(), deletor<Container_type::value_type>());
    Container.clear();
}

//ģ�庯��::�ͷ�MAP������value_typeΪָ��������ռ�õ��ڴ�
template <typename PairType>
struct deletor_pair2nd : public std::unary_function < PairType, void >
{
    void operator()(PairType p)
    {
        delete p.second;
    }
};

template <typename Container_type>
void free_MapPointer_memory(Container_type& Container)
{
    typedef std::pair<Container_type::key_type, Container_type::mapped_type > mapPair;
    std::for_each(Container.begin(), Container.end(), deletor_pair2nd<mapPair>());
    Container.clear();
}

//��ֹ������ �ο�boost
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable&);
    noncopyable& operator=(const noncopyable&);
};

#define _REMIND_ME0(a) #a
#define _REMIND_ME1(a) __FILE__ "(" _REMIND_ME0(a) ") : "
#define REMIND_ME(a) message(_REMIND_ME1(__LINE__)a)

//��ʾ���÷�
//#pragma REMIND_ME("��ֻ��һ����ʾ�꺯��!")

//ɾ��ָ��
#define SAFEDELETE(p)  {if (p) { delete[] (p); (p) = NULL;} }
//ɾ������ָ��
#define DELETEOBJ(OBJ) {if (OBJ) { delete OBJ; OBJ = NULL;} }



#endif
