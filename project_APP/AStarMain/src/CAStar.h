//-----------------------------------------------------------------------------
//
//                  AStar
//                                          @2017-03-23��������
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
    int xPos, yPos;     //����
    int nF, nG, nH;     //����ֵ
    stNode* pParent;
};

//��A*�㷨
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
    vector<stNode*> vecClose;       //�ر��б� ����·��
    vector<stNode*> vecOpen;        //�����б�
    stNode* m_pBegin;
};







#endif