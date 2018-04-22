//-----------------------------------------------------------------------------
//
//          C中的例子
//
//-----------------------------------------------------------------------------
#ifndef CEXAM_HPP
#define CEXAM_HPP

#include "CLinearList.hpp"

//链表演示
class CStudent
{
public:
    static int main(int argc, char** argv)
    {
        CList<CStudent>* list = new CList<CStudent>();
        list->Insert(new CStudent(1));
        list->Insert(new CStudent(2));
        list->Insert(new CStudent(3));
        //链表中查找
        CStudent* s = list->Find(&CStudent::GetStudentId, 2);
        if (s != NULL)
        {
            // s FOUND
        }
        return 0;
    }

    static int CLinearList(int argc, char** argv)
    {
        CQueue<int>* cQueue = new CQueue<int>();
        CStack<double>* cStack = new CStack<double>();

        for (int i = 0; i < 10; i++)
        {
            cQueue->Enqueue(new int(i));            //入队10个元素
            cStack->Push(new double(i / 10.0));     //入栈10个元素
        }

        cout << "Queue - integer collection:" << endl;
        for (; cQueue->Count();)
        {
            cout << *cQueue->Dequeue() << " ";
        }

        cout << endl << endl << "Stack - double collection:" << endl;
        for (; cStack->Count();)
        {
            cout << *cStack->Pop() << " ";
        }

        delete cQueue;
        delete cStack;

        cout << endl << endl;
        return system("pause");
    }

private:
    DWORD dwStudentId;
public:
    CStudent(DWORD dwStudentId) : dwStudentId(dwStudentId){ }
    static DWORD GetStudentId(CStudent* student)
    {
        DWORD dwValue = student->GetId();
        return dwValue;
    }
    DWORD GetId() const
    {
        return dwStudentId;
    }
};













#endif



