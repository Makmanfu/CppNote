

#include "stdafx.h"
#include "FunAll.h"

//数据结构
#include "CLIST.h"
#include "CDList.h"
#include "CQueue.h"
#include "CStack.h"
#include "CBITree.h"
#include "CRBTree.h"
#include "CDijkstra.h"
#include "CAStar.h"
//设计模式
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
//C/C++总结
#include "Effective.hpp"            //類中的問題
#include "Sorting.h"                //排序总结
#include "WinService.h"             //关机服务
#include "MiniTools.hpp"            //小工具集合
//GUI
#include "WinGUI.h"                 //WINGUI
#include "Harkscr.hpp"              //黑客帝国屏保
#include "HelloDUI.h"               //DUILIB


void FunCMDAll(CCMDParam* pCMD)
{
    //没别的意思 就是取个别名
    typedef CCMDParam::FunProcInfo FUNP;
    //数据结构
    pCMD->ComandAdd("Struct_LIST", FUNP(&CList::main, (void*)         "单链表"));
    pCMD->ComandAdd("Struct_DList", FUNP(&CDList::main, (void*)       "双向链表"));
    pCMD->ComandAdd("Struct_Queue", FUNP(&CQueue::main, (void*)       "队列"));
    pCMD->ComandAdd("Struct_Stack", FUNP(&CStack::main, (void*)       "链栈"));
    pCMD->ComandAdd("Struct_BITree", FUNP(&CBITree::main, (void*)     "二叉树"));
    pCMD->ComandAdd("Struct_RBTree", FUNP(&CRBTree::main, (void*)     "红黑树"));
    pCMD->ComandAdd("Struct_AStar", FUNP(&CAStar::main, (void*)       "A*"));
    //设计模式原形
    pCMD->ComandAdd("Mode_Factory", FUNP(&Mode_Factory, (void*)       "工厂模式"));
    pCMD->ComandAdd("Mode_AbstactFac", FUNP(&Mode_AbstactFac, (void*) "抽象工厂模式"));
    pCMD->ComandAdd("Mode_Builder", FUNP(&Mode_Builder, (void*)       "建造者模式"));
    pCMD->ComandAdd("Mode_Prototype", FUNP(&Mode_Prototype, (void*)   "原型模式"));
    pCMD->ComandAdd("Mode_Singleton", FUNP(&Mode_Singleton, (void*)   "单例模式"));
    pCMD->ComandAdd("Mode_Adapter", FUNP(&Mode_Adapter, (void*)       "适配器模式"));
    pCMD->ComandAdd("Mode_Bridge", FUNP(&Mode_Bridge, (void*)         "桥接模式"));
    pCMD->ComandAdd("Mode_Composite", FUNP(&Mode_Composite, (void*)   "组合模式"));
    pCMD->ComandAdd("Mode_Decorator", FUNP(&Mode_Decorator, (void*)   "装饰模式"));
    pCMD->ComandAdd("Mode_Facade", FUNP(&Mode_Facade, (void*)         "外观模式"));
    pCMD->ComandAdd("Mode_Flyweight", FUNP(&Mode_Flyweight, (void*)   "享元模式"));
    pCMD->ComandAdd("Mode_Proxy", FUNP(&Mode_Proxy, (void*)           "代理模式"));
    //C/C++
    pCMD->ComandAdd("C_Virtual", FUNP(&ClassVirtual::main, (void*)    "虚函数的理解"));
    pCMD->ComandAdd("C_ClassCopy", FUNP(&ClassCopy::main, (void*)     "类的拷贝问题"));
    pCMD->ComandAdd("C_StrChar", FUNP(&StrChar::main, (void*)         "模仿string字符"));
    pCMD->ComandAdd("C_ClassOOP", FUNP(&ClassOOP::main, (void*)       "一个例子理解OOP"));
    pCMD->ComandAdd("C_AutoSort", FUNP(&AutoSort::main, (void*)       "排序算法") );
    pCMD->ComandAdd("C_WinSvrShut", FUNP(&WinSvrShut::main, (void*)   "系统服务[关机]"));
    pCMD->ComandAdd("C_TypeSize", FUNP(&TypeSize, (void*)             "数据类型大小"));
    //app
    pCMD->ComandAdd("App_SignEXE", FUNP(&SignFlagexe::main, (void*)   "程序簽名"));
}

void FunWINAll(CWINParam* pWIN)
{
    //没别的意思 就是取个别名
    typedef CWINParam::FunProcInfo FUNP;
    pWIN->ComandAdd("XqWindowHarkscr", FUNP(&WinHarkscr::WINMAIN, (void*)"黑客帝国屏保"));
    pWIN->ComandAdd("XqWindow", FUNP(&XqWindowEx::WINMAIN, (void*)"非模态窗口"));
    pWIN->ComandAdd("XqWindowDlg", FUNP(&XqWindowDlg::WINMAIN, (void*)"模态窗口(有BUG)"));
    //DUI
    pWIN->ComandAdd("DuiTest01", FUNP(&DuiTest01::WINMAIN, (void*)"DUI001"));
    pWIN->ComandAdd("DuiTest02", FUNP(&DuiTest02::WINMAIN, (void*)"DUI002"));
}



