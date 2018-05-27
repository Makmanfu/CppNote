//-----------------------------------------------------------------------------
//
//                  AStar
//                                          @2017-03-23阿甘整理
//-----------------------------------------------------------------------------

#ifndef ASTAR_H
#define ASTAR_H
#include <vector>
#include <math.h>
#include <Windows.h>
using namespace std;

struct stNode
{
    stNode(int x, int y, stNode* p = NULL) : xPos(x), yPos(y), pParent(p) {}
    int xPos, yPos;     //坐标
    int nF, nG, nH;     //估价值
    stNode* pParent;
};

//简单A*算法
class CAStar
{
public:
    static int main(int argc, char** argv)
    {
        return 1;
    };
public:
    CAStar(void);
    ~CAStar(void);
public:
    bool MySearch(vector<stNode*>& vecNode, stNode*& p);
    bool Search(int xBegin, int yBegin, int xEnd, int yEnd);
    void MyInsert(vector<stNode*>& vecNode, stNode* p);
    stNode* GetNext();
private:
    vector<stNode*> vecClose;       //关闭列表 最后的路径
    vector<stNode*> vecOpen;        //开放列表
    stNode* m_pBegin;
};







#endif