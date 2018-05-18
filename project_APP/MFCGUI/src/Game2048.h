


class Game2048
{
public:
    Game2048();
    ~Game2048();
private:
    //���ݽṹ
    CFont m_font, m_headFont, m_smallFont;
    int GridData[4][4], GridDataTran[4][4], PriorData1[4][4], PriorData2[4][4];
    int m_nScore, m_nBestScore, m_nBackCounts, m_nSteps;
    COLORREF colors[14];
private:
    int GenRand2or4(void);
    void Gen2or4InDataGrid(void);
    void GridDataCopy(int(*data1)[4], int(*data2)[4]);
    bool IsSameData(int(*data1)[4], int(*data2)[4]);
    int getColorIndex(int num);
    bool IsInDeathSituation(void);
public:
    void initdata(void);
    //����
    void OnRestartBtn(void);

    void MoveToRight(void);
    void MoveToLeft(void);
    void MoveToDown(void);
    void MoveToUp(void);
    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


//��Ϸ2048
class Gameer
{
public:
    Gameer();
    ~Gameer();
private:
    void DrawHeadTile(CDC* pDC);
    

    //����ʱ
    void Create(void);
    //����
    void DrawGrids(CDC* pDC);





};




