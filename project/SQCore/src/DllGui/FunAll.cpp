

#include "stdafx.h"
#include "FunAll.h"

//���ݽṹ
#include "CLIST.h"
#include "CDList.h"
#include "CQueue.h"
#include "CStack.h"
#include "CBITree.h"
#include "CRBTree.h"
#include "CDijkstra.h"
#include "CAStar.h"
//���ģʽ
#include "Factory.hpp"
#include "AbstactFactory.hpp"
#include "Builder.hpp"
#include "Prototype.hpp"
#include "Singleton.hpp"
#include "Adapter.hpp"
#include "Bridge.hpp"
#include "Composite.hpp"
#include "Decorator.hpp"
#include "Facade.hpp"
#include "Flyweight.hpp"
#include "Proxy.hpp"
//C/C++�ܽ�
#include "Effective.hpp"            //��еĆ��}
#include "Sorting.h"                //�����ܽ�
#include "WinService.h"             //�ػ�����
#include "MiniTools.hpp"            //С���߼���
//GUI
#include "WinGUI.h"                 //WINGUI
#include "Harkscr.hpp"              //�ڿ͵۹�����
#include "HelloDUI.h"               //DUILIB


void FunCMDAll(CCMDParam* pCMD)
{
    //û�����˼ ����ȡ������
    typedef CCMDParam::FunProcInfo FUNP;
    //���ݽṹ
    pCMD->ComandAdd("Struct_LIST", FUNP(&CList::main, (void*)         "������"));
    pCMD->ComandAdd("Struct_DList", FUNP(&CDList::main, (void*)       "˫������"));
    pCMD->ComandAdd("Struct_Queue", FUNP(&CQueue::main, (void*)       "����"));
    pCMD->ComandAdd("Struct_Stack", FUNP(&CStack::main, (void*)       "��ջ"));
    pCMD->ComandAdd("Struct_BITree", FUNP(&CBITree::main, (void*)     "������"));
    pCMD->ComandAdd("Struct_RBTree", FUNP(&CRBTree::main, (void*)     "�����"));
    pCMD->ComandAdd("Struct_AStar", FUNP(&CAStar::main, (void*)       "A*"));
    //���ģʽԭ��
    pCMD->ComandAdd("Mode_Factory", FUNP(&Mode_Factory, (void*)       "����ģʽ"));
    pCMD->ComandAdd("Mode_AbstactFac", FUNP(&Mode_AbstactFac, (void*) "���󹤳�ģʽ"));
    pCMD->ComandAdd("Mode_Builder", FUNP(&Mode_Builder, (void*)       "������ģʽ"));
    pCMD->ComandAdd("Mode_Prototype", FUNP(&Mode_Prototype, (void*)   "ԭ��ģʽ"));
    pCMD->ComandAdd("Mode_Singleton", FUNP(&Mode_Singleton, (void*)   "����ģʽ"));
    pCMD->ComandAdd("Mode_Adapter", FUNP(&Mode_Adapter, (void*)       "������ģʽ"));
    pCMD->ComandAdd("Mode_Bridge", FUNP(&Mode_Bridge, (void*)         "�Ž�ģʽ"));
    pCMD->ComandAdd("Mode_Composite", FUNP(&Mode_Composite, (void*)   "���ģʽ"));
    pCMD->ComandAdd("Mode_Decorator", FUNP(&Mode_Decorator, (void*)   "װ��ģʽ"));
    pCMD->ComandAdd("Mode_Facade", FUNP(&Mode_Facade, (void*)         "���ģʽ"));
    pCMD->ComandAdd("Mode_Flyweight", FUNP(&Mode_Flyweight, (void*)   "��Ԫģʽ"));
    pCMD->ComandAdd("Mode_Proxy", FUNP(&Mode_Proxy, (void*)           "����ģʽ"));
    //C/C++
    pCMD->ComandAdd("C_Virtual", FUNP(&ClassVirtual::main, (void*)    "�麯�������"));
    pCMD->ComandAdd("C_ClassCopy", FUNP(&ClassCopy::main, (void*)     "��Ŀ�������"));
    pCMD->ComandAdd("C_StrChar", FUNP(&StrChar::main, (void*)         "ģ��string�ַ�"));
    pCMD->ComandAdd("C_ClassOOP", FUNP(&ClassOOP::main, (void*)       "һ���������OOP"));
    pCMD->ComandAdd("C_AutoSort", FUNP(&AutoSort::main, (void*)       "�����㷨") );
    pCMD->ComandAdd("C_WinSvrShut", FUNP(&WinSvrShut::main, (void*)   "ϵͳ����[�ػ�]"));
    pCMD->ComandAdd("C_TypeSize", FUNP(&TypeSize, (void*)             "�������ʹ�С"));
    //app
    pCMD->ComandAdd("App_SignEXE", FUNP(&SignFlagexe::main, (void*)   "������"));
}

void FunWINAll(CWINParam* pWIN)
{
    //û�����˼ ����ȡ������
    typedef CWINParam::FunProcInfo FUNP;
    pWIN->ComandAdd("XqWindowHarkscr", FUNP(&WinHarkscr::WINMAIN, (void*)"�ڿ͵۹�����"));
    pWIN->ComandAdd("XqWindow", FUNP(&XqWindowEx::WINMAIN, (void*)"��ģ̬����"));
    pWIN->ComandAdd("XqWindowDlg", FUNP(&XqWindowDlg::WINMAIN, (void*)"ģ̬����(��BUG)"));
    //DUI
    pWIN->ComandAdd("DuiTest01", FUNP(&DuiTest01::WINMAIN, (void*)"DUI001"));
    pWIN->ComandAdd("DuiTest02", FUNP(&DuiTest02::WINMAIN, (void*)"DUI002"));
}



