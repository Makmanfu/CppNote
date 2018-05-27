
#include "Castar.h"
#include <algorithm>

int map[20][20] =
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    , {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}
    , {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1}
    , {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}
    , {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    , {1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    , {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

CAStar::CAStar(void) : m_pBegin(NULL)
{
}

bool CAStar::MySearch(vector<stNode*>& vecNode, stNode*& p)
{
    for (int i = 0; i < vecNode.size(); ++i)
    {
        if ((vecNode[i]->xPos == p->xPos) && (vecNode[i]->yPos == p->yPos))
            return true;
    }
    return false;
}

bool Sortt(stNode* node1, stNode* node2)
{
    if (node1->nF < node2->nF)
        return true;
    return false;
}

void CAStar::MyInsert(vector<stNode*>& vecNode, stNode* p)
{
    vecNode.push_back(p);
    sort(vecNode.begin(), vecNode.end(), Sortt);
}

stNode* CAStar::GetNext()
{
    stNode* node = m_pBegin;
    if (m_pBegin)
        m_pBegin = m_pBegin->pParent;
    return node;
}

bool CAStar::Search(int xBegin, int yBegin, int xEnd, int yEnd)
{
    stNode* stBegin = new stNode(xBegin, yBegin);
    stNode* stEnd = new stNode(xEnd, yEnd);
    vecClose.clear();
    vecOpen.clear();
    stBegin->nG = 0;
    stBegin->nH = abs(yEnd - yBegin) * 10 + abs(xEnd - xBegin) * 10;
    stBegin->nF = stBegin->nG + stBegin->nH;
    vecOpen.push_back(stBegin);
    while (!vecOpen.empty())
    {
        stNode* NodeTemp = vecOpen[0];
        vecOpen.erase(vecOpen.begin());
        //循环周围的3*3格子
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if ((i == 0) && (j == 0))       //如果是自己的话就跳出
                    continue;
                if (map[NodeTemp->xPos + i][NodeTemp->yPos + j] == 1)     //如果周围格子是障碍物跳出
                    continue;
                stNode* node = new stNode(NodeTemp->xPos + i, NodeTemp->yPos + j);
                if (MySearch(vecClose, node))       //是否在关闭列表
                    continue;
                int add = (i != 0 && j != 0) ? 14 : 10;   //如果在与当前节点在同一行或同一列为10 否则为14
                if (!MySearch(vecOpen, node))
                {
                    node->nG = add + NodeTemp->nG;
                    node->nH = abs(node->xPos - xEnd) * 10 + abs(node->yPos - yEnd) * 10;
                    node->nF = node->nG + node->nH;
                    node->pParent = NodeTemp;
                    MyInsert(vecOpen, node);
                }
                if ((node->xPos == xEnd) && (node->yPos == yEnd))
                {
                    m_pBegin = node;
                    int size = vecOpen.size();
                    vecOpen.clear();
                    return true;
                }
            }
        }
        vecClose.push_back(NodeTemp);
    }
    return false;
}

CAStar::~CAStar(void)
{
}
