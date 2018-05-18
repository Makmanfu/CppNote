
#include "stdafx.h"
#include "Game2048.h"

Gameer::Gameer()
{

}

Gameer::~Gameer()
{
}


void Gameer::DrawHeadTile(CDC* pDC)
{
    ////int x0, y0, i, j;
    ////CString strNum;
    ////for (i = 0; i < 2; i++)
    ////    for (j = 0; j < 3; j++)
    ////    {
    ////        x0 = 110 + 10 * (j + 1) + 80 * j;
    ////        y0 = 10 + 30 * i;
    ////        pDC->FillSolidRect(x0, y0, 80, 30, RGB(180, 180, 180));
    ////        if (i == 0 && j == 0)
    ////            pDC->DrawText("steps", CRect(CPoint(x0, y0), CPoint(x0 + 80, y0 + 30)), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ////        if (i == 1 && j == 0)
    ////        {
    ////            strNum.Format("%d", m_nSteps);
    ////            pDC->DrawText(strNum, CRect(CPoint(x0, y0), CPoint(x0 + 80, y0 + 30)), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ////        }
    ////        if (i == 0 && j == 1)
    ////            pDC->DrawText("score", CRect(CPoint(x0, y0), CPoint(x0 + 80, y0 + 30)), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ////        if (i == 1 && j == 1)
    ////        {
    ////            strNum.Format("%d", m_nScore);
    ////            pDC->DrawText(strNum, CRect(CPoint(x0, y0), CPoint(x0 + 80, y0 + 30)), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ////        }
    ////        if (i == 0 && j == 2)
    ////            pDC->DrawText("best", CRect(CPoint(x0, y0), CPoint(x0 + 80, y0 + 30)), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ////        if (i == 1 && j == 2)
    ////        {
    ////            strNum.Format("%d", m_nBestScore);
    ////            pDC->DrawText(strNum, CRect(CPoint(x0, y0), CPoint(x0 + 80, y0 + 30)), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ////        }
    ////    }
}



void Gameer::Create(void)
{
    //m_font.CreateFont(
    //    40,                        // nHeight
    //    0,                         // nWidth
    //    0,                         // nEscapement
    //    0,                         // nOrientation
    //    FW_NORMAL,                 // nWeight
    //    FALSE,                     // bItalic
    //    FALSE,                     // bUnderline
    //    0,                         // cStrikeOut
    //    ANSI_CHARSET,              // nCharSet
    //    OUT_DEFAULT_PRECIS,        // nOutPrecision
    //    CLIP_DEFAULT_PRECIS,       // nClipPrecision
    //    DEFAULT_QUALITY,           // nQuality
    //    DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
    //    "Arial");                 // lpszFacename
    //m_headFont.CreateFont(
    //    48,                        // nHeight
    //    0,                         // nWidth
    //    0,                         // nEscapement
    //    0,                         // nOrientation
    //    FW_NORMAL,                 // nWeight
    //    FALSE,                     // bItalic
    //    FALSE,                     // bUnderline
    //    0,                         // cStrikeOut
    //    ANSI_CHARSET,              // nCharSet
    //    OUT_DEFAULT_PRECIS,        // nOutPrecision
    //    CLIP_DEFAULT_PRECIS,       // nClipPrecision
    //    DEFAULT_QUALITY,           // nQuality
    //    DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
    //    "Arial");                 // lpszFacename
    //m_smallFont.CreateFont(
    //    25,                        // nHeight
    //    0,                         // nWidth
    //    0,                         // nEscapement
    //    0,                         // nOrientation
    //    FW_NORMAL,                 // nWeight
    //    FALSE,                     // bItalic
    //    FALSE,                     // bUnderline
    //    0,                         // cStrikeOut
    //    ANSI_CHARSET,              // nCharSet
    //    OUT_DEFAULT_PRECIS,        // nOutPrecision
    //    CLIP_DEFAULT_PRECIS,       // nClipPrecision
    //    DEFAULT_QUALITY,           // nQuality
    //    DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
    //    "Arial");                  // lpszFacename
    //Gen2or4InDataGrid();
    //Gen2or4InDataGrid();
    //GridDataCopy(GridDataTran, GridData);
    //GridDataCopy(PriorData1, GridData);
    //GridDataCopy(PriorData2, GridData);
}

void Gameer::DrawGrids(CDC* pDC)
{
    //int i, j;
    //CRect rc;
    //CString strNum;
    //GetClientRect(&rc);
    //int x = (400 - 5 * 10 - 4 * 80) / 2;
    //int y = 100 + (400 - 5 * 10 - 4 * 80) / 2;
    //CDC MemDC;
    //CBitmap MemBitmap;
    //MemDC.CreateCompatibleDC(NULL);
    //MemBitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
    //CBitmap* pOldBit = MemDC.SelectObject(&MemBitmap);
    //CFont* pOldFont = MemDC.SelectObject(&m_headFont);
    //MemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(255, 255, 255));
    //MemDC.DrawText("2048", CRect(CPoint(10, 10), CPoint(110, 60)), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    //MemDC.SelectObject(&m_smallFont);
    //MemDC.FillSolidRect(x, y, 370, 370, RGB(180, 180, 180));
    //int x0, y0;
    //DrawHeadTile(&MemDC);
    //MemDC.SelectObject(&m_font);
    //for (i = 0; i < 4; i++)
    //{
    //    for (j = 0; j < 4; j++)
    //    {
    //        x0 = x + 10 * (j + 1) + 80 * j;
    //        y0 = y + 10 * (i + 1) + 80 * i;
    //        int index = getColorIndex(GridData[i][j]);
    //        MemDC.FillSolidRect(x0, y0, 80, 80, colors[index]);
    //        if (GridData[i][j])
    //        {
    //            strNum.Format("%d", GridData[i][j]);
    //            if (GridData[i][j] == 2 || GridData[i][j] == 4)
    //                MemDC.SetTextColor(RGB(0, 0, 0));
    //            else
    //                MemDC.SetTextColor(RGB(255, 255, 255));
    //            MemDC.DrawText(strNum, CRect(CPoint(x0, y0), CPoint(x0 + 80, y0 + 80)), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    //            //  MemDC.SelectObject(pOldFont);
    //        }
    //    }
    //}
    //MemDC.SelectObject(&m_smallFont);
    //MemDC.SetTextColor(RGB(0, 0, 0));
    //strNum.Format("undo(%d)", m_nBackCounts);
    //MemDC.FillSolidRect(290, 80, 90, 30, RGB(180, 180, 180));
    //MemDC.DrawText(strNum, CRect(CPoint(290, 80), CPoint(290 + 90, 80 + 30)), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    //pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
    //MemBitmap.DeleteObject();
    //MemDC.DeleteDC();
}






Game2048::Game2048()
{
    initdata();
}

Game2048::~Game2048()
{
}

void Game2048::initdata(void)
{
    //初始化就不多说了
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            GridData[i][j] = 0;
            GridDataTran[i][j] = 0;
            PriorData1[i][j] = 0;
            PriorData2[i][j] = 0;
        }
    }
    //srand((unsigned)time(NULL));
    m_nSteps = 0;
    m_nBackCounts = 2;
    m_nScore = 0;
    m_nBestScore = 0;
    colors[0] = RGB(220, 220, 220);
    colors[1] = RGB(250, 230, 180);
    colors[2] = RGB(191, 191, 128);
    colors[3] = RGB(255, 152, 102);
    colors[4] = RGB(255, 128, 0);
    colors[5] = RGB(255, 0, 0);
    colors[6] = RGB(227, 0, 0);
    colors[7] = RGB(255, 255, 70);
    colors[8] = RGB(255, 128, 255);
    colors[9] = RGB(255, 128, 64);
    colors[10] = RGB(221, 221, 0);
    colors[11] = RGB(255, 255, 2);
    colors[12] = RGB(0, 255, 0);
}

int Game2048::GenRand2or4(void)
{
    if (rand() % 2 == 0 && rand() % 2 == 0)
        return 4;
    else
        return 2;
}

void Game2048::Gen2or4InDataGrid(void)
{
    int num = GenRand2or4();
    int index = rand() % 16;
    while (GridData[index / 4][index % 4])
        index = rand() % 16;
    GridData[index / 4][index % 4] = num;
}

void Game2048::GridDataCopy(int(*data1)[4], int(*data2)[4])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            data1[i][j] = data2[i][j];
}

bool Game2048::IsSameData(int(*data1)[4], int(*data2)[4])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (data1[i][j] != data2[i][j])
                return false;
    return true;
}

int Game2048::getColorIndex(int num)
{
    int k = 0;
    if (num == 0)
        return 0;
    while (num % 2 != 1)
    {
        num = num / 2;
        k++;
    }
    return k;
}

bool Game2048::IsInDeathSituation(void)
{
    int i, j;
    bool bMoveFlag;
    for (i = 0; i < 16; i++)
        if (GridData[i / 4][i % 4] == 0)
            return false;
    if (i == 16)
    {
        bMoveFlag = false;
        for (i = 0; i < 4 && !bMoveFlag; i++)
            for (j = 0; j < 3; j++)
            {
                if (GridData[i][j] == GridData[i][j + 1])
                {
                    bMoveFlag = true;
                    break;
                }
            }
        for (j = 0; j < 4 && !bMoveFlag; j++)
            for (i = 0; i < 3; i++)
            {
                if (GridData[i][j] == GridData[i + 1][j])
                {
                    bMoveFlag = true;
                    break;
                }
            }
    }
    return !bMoveFlag;
}

void Game2048::OnRestartBtn(void)
{
    m_nBackCounts = 2;
    m_nSteps = 0;
    m_nScore = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            GridData[i][j] = 0;
    Gen2or4InDataGrid();
    Gen2or4InDataGrid();
    GridDataCopy(GridDataTran, GridData);
    GridDataCopy(PriorData1, GridData);
    GridDataCopy(PriorData2, GridData);
    ////Invalidate(TRUE);
}

void Game2048::MoveToUp()
{
    int k;
    int index1, index2;
    for (int i = 0; i < 4; i++)
    {
        k = 0;
        index1 = 0;
        while (index1 < 4)
        {
            while (index1 < 4 && GridData[index1][i] == 0)
                index1++;
            index2 = index1 + 1;
            while (index2 < 4 && GridData[index2][i] == 0)
                index2++;
            if (index1 < 4)
            {
                if (index2 < 4)
                {
                    if (GridData[index1][i] == GridData[index2][i])
                    {
                        GridData[k][i] = GridData[index1][i] * 2;
                        m_nScore = m_nScore + GridData[k][i];
                        if (m_nScore > m_nBestScore)
                            m_nBestScore = m_nScore;
                        k++;
                        index1 = index2 + 1;
                    }
                    else
                    {
                        GridData[k++][i] = GridData[index1][i];
                        index1 = index2;
                    }
                }
                else
                {
                    GridData[k++][i] = GridData[index1][i];
                    break;
                }
            }
            else
                break;
        }
        while (k < 4)
            GridData[k++][i] = 0;
    }
}

void Game2048::MoveToDown()
{
    int k;
    int index1, index2;
    for (int i = 0; i < 4; i++)
    {
        k = 3;
        index1 = 3;
        while (index1 >= 0)
        {
            while (index1 >= 0 && GridData[index1][i] == 0)
                index1--;
            index2 = index1 - 1;
            while (index2 >= 0 && GridData[index2][i] == 0)
                index2--;
            if (index1 >= 0)
            {
                if (index2 >= 0)
                {
                    if (GridData[index1][i] == GridData[index2][i])
                    {
                        GridData[k][i] = GridData[index1][i] * 2;
                        m_nScore = m_nScore + GridData[k][i];
                        if (m_nScore > m_nBestScore)
                            m_nBestScore = m_nScore;
                        k--;
                        index1 = index2 - 1;
                    }
                    else
                    {
                        GridData[k--][i] = GridData[index1][i];
                        index1 = index2;
                    }
                }
                else
                {
                    GridData[k--][i] = GridData[index1][i];
                    break;
                }
            }
            else
                break;
        }
        while (k >= 0)
            GridData[k--][i] = 0;
    }
}

void Game2048::MoveToLeft()
{
    int k;
    int index1, index2;
    for (int i = 0; i < 4; i++)
    {
        k = 0;
        index1 = 0;
        while (index1 < 4)
        {
            while (index1 < 4 && GridData[i][index1] == 0)
                index1++;
            index2 = index1 + 1;
            while (index2 < 4 && GridData[i][index2] == 0)
                index2++;
            if (index1 < 4)
            {
                if (index2 < 4)
                {
                    if (GridData[i][index1] == GridData[i][index2])
                    {
                        GridData[i][k] = GridData[i][index1] * 2;
                        m_nScore = m_nScore + GridData[i][k];
                        if (m_nScore > m_nBestScore)
                            m_nBestScore = m_nScore;
                        k++;
                        index1 = index2 + 1;
                    }
                    else
                    {
                        GridData[i][k++] = GridData[i][index1];
                        index1 = index2;
                    }
                }
                else
                {
                    GridData[i][k++] = GridData[i][index1];
                    break;
                }
            }
            else
                break;
        }
        while (k < 4)
            GridData[i][k++] = 0;
    }
}

void Game2048::MoveToRight()
{
    int k;
    int index1, index2;
    for (int i = 0; i < 4; i++)
    {
        k = 3;
        index1 = 3;
        while (index1 >= 0)
        {
            while (index1 >= 0 && GridData[i][index1] == 0)
                index1--;
            index2 = index1 - 1;
            while (index2 >= 0 && GridData[i][index2] == 0)
                index2--;
            if (index1 >= 0)
            {
                if (index2 >= 0)
                {
                    if (GridData[i][index1] == GridData[i][index2])
                    {
                        GridData[i][k] = GridData[i][index1] * 2;
                        m_nScore = m_nScore + GridData[i][k];
                        if (m_nScore > m_nBestScore)
                            m_nBestScore = m_nScore;
                        k--;
                        index1 = index2 - 1;
                    }
                    else
                    {
                        GridData[i][k--] = GridData[i][index1];
                        index1 = index2;
                    }
                }
                else
                {
                    GridData[i][k--] = GridData[i][index1];
                    break;
                }
            }
            else
                break;
        }
        while (k >= 0)
            GridData[i][k--] = 0;
    }
}

void Game2048::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    MessageBoxA(NULL, "VK_UP", "OK", MB_OK);
    switch (nChar)
    {
    case VK_UP://VK_UP:
        MessageBoxA(NULL, "VK_UP", "OK", MB_OK);
        break;
        MoveToUp();
        ////if (IsInDeathSituation())
        ////    MessageBox("游戏结束!!!");
        if (!IsSameData(GridData, GridDataTran))
        {
            Gen2or4InDataGrid();
            m_nSteps++;
            GridDataCopy(PriorData2, PriorData1);
            GridDataCopy(PriorData1, GridDataTran);
            GridDataCopy(GridDataTran, GridData);
            //Invalidate(TRUE);
        }
        break;
    case VK_DOWN:
        MessageBoxA(NULL, "VK_DOWN", "OK", MB_OK);
        break;
        MoveToDown();
        ////if (IsInDeathSituation())
        ////    MessageBox("游戏结束!!!");
        if (!IsSameData(GridData, GridDataTran))
        {
            Gen2or4InDataGrid();
            m_nSteps++;
            GridDataCopy(PriorData2, PriorData1);
            GridDataCopy(PriorData1, GridDataTran);
            GridDataCopy(GridDataTran, GridData);
            //Invalidate(TRUE);
        }
        break;
    case VK_LEFT:
        MessageBoxA(NULL, "VK_LEFT", "OK", MB_OK);
        break;
        MoveToLeft();
        ////if (IsInDeathSituation())
        ////    MessageBox("游戏结束!!!");
        if (!IsSameData(GridData, GridDataTran))
        {
            Gen2or4InDataGrid();
            m_nSteps++;
            GridDataCopy(PriorData2, PriorData1);
            GridDataCopy(PriorData1, GridDataTran);
            GridDataCopy(GridDataTran, GridData);
            //Invalidate(TRUE);
        }
        break;
    case VK_RIGHT:
        MessageBoxA(NULL, "VK_RIGHT", "OK", MB_OK);
        break;
        MoveToRight();
        ////if (IsInDeathSituation())
        ////    MessageBox("游戏结束!!!");
        if (!IsSameData(GridData, GridDataTran))
        {
            Gen2or4InDataGrid();
            m_nSteps++;
            GridDataCopy(PriorData2, PriorData1);
            GridDataCopy(PriorData1, GridDataTran);
            GridDataCopy(GridDataTran, GridData);
            //Invalidate(TRUE);
        }
        break;
    case VK_ESCAPE:
        MessageBoxA(NULL, "VK_ESCAPE", "OK", MB_OK);
        break;
        if (m_nBackCounts && !IsSameData(GridData, PriorData1))
        {
            m_nBackCounts--;
            m_nSteps++;
            GridDataCopy(GridData, PriorData1);
            GridDataCopy(GridDataTran, GridData);
            GridDataCopy(PriorData1, PriorData2);
            //Invalidate(TRUE);
        }
        break;
    default:
        break;
    }
    //CView::OnKeyDown(nChar, nRepCnt, nFlags);
}





