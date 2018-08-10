
#include "stdafx.h"
#include <shellapi.h>   //��ק�ļ�ʱ�õ���HDROP�����ڴ��ļ�(Ҫд��windowsx.h֮ǰ��HANDLE_WM_DROPFILES���ܱ�ʶ��)
#include <windowsx.h>
#include <Shlobj.h>     //���Ŀ¼��ʹ��
#include "GDIPlayer.h"


void TcharToChar(const TCHAR* tchar, char* _char)
{
    //��ȡ�ֽڳ���
    int iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
    //��tcharֵ����_char
    WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}

int GDIPlayer::WIN_MAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    GDIPlayer MsgDlg(IDD_GDIMAIN);
    DialogBoxA(hInstance, MAKEINTRESOURCEA(XqWindowDlg::ResDlgID), NULL, XqWindowDlg::Main_Proc);
    return 0;
}

GDIPlayer::GDIPlayer(int tResDlgID) :XqWindowDlg(tResDlgID)
, iExitFlag(0)//Ĭ�ϲ��˳�������
, hProgress(NULL)
, hSound(NULL)
, hWndList(NULL)
, hWndShow(NULL)
, iVolume(1000)          //Ĭ���������
, iMenuFlag(1)           //���˵���ʾ
, eDrawMode(DGPI)
{
}

GDIPlayer::~GDIPlayer()
{
}

void GDIPlayer::ConvLTime2Str(LONG LTime, char* StrTime)
{
    LTime = LTime / 1000; //����LTime�����΢�������ʳ���1000
    struct Time
    {
        int hour;
        int mini;
        int sect;
    } t;
    t.hour = LTime / 3600;
    t.mini = (LTime - t.hour * 3600) / 60;
    t.sect = LTime % 60;
    char h[5], m[3], si[3];
    wsprintfA(h, (t.hour < 10) ? "0%d" : "%d", t.hour);
    wsprintfA(m, (t.mini < 10) ? "0%d" : "%d", t.mini);
    wsprintfA(si, (t.sect < 10) ? "0%d" : "%d", t.sect);
    wsprintfA(StrTime, "%s:%s:%s", h, m, si);
}

int GDIPlayer::GetRandNum(int a, int b)
{
    srand((unsigned long)time(NULL));
    int num = rand();
    num = a + num % (b - a + 1);
    return num;
}

BOOL GDIPlayer::SearchFileAddtoList(HWND hWnd, char path[], char format[])
{
    char szMusicName[MAX_PATH], SearchFormat[MAX_PATH + 15];
    LPSTR PszFilePathName = NULL;  //����ȫ·��(����������)
    WIN32_FIND_DATAA FindFileData;
    wsprintfA(SearchFormat, "%s\\%s", path, format);
    HANDLE hFind = FindFirstFileA(SearchFormat, &FindFileData); //ע��Ŀ¼����ʽ����Ҫ��*.*
    do
    {
        if (hFind == INVALID_HANDLE_VALUE)
            return FALSE;
        else
        {
            PszFilePathName = (LPSTR)malloc(2 * MAX_PATH);
            strcpy(szMusicName, FindFileData.cFileName);
            wsprintfA(PszFilePathName, "%s\\%s", path, szMusicName);
            int index = (int)SendDlgItemMessageA(hWnd, IDC_LIST, LB_INSERTSTRING, WPARAM(-1), (LPARAM)szMusicName);
            SendDlgItemMessageA(hWnd, IDC_LIST, LB_SETITEMDATA, WPARAM(index), (LPARAM)PszFilePathName);
        }
    } while (TRUE == FindNextFileA(hFind, &FindFileData));
    FindClose(hFind);
    return TRUE;
}

void GDIPlayer::GetExeP(char* exePath, int iLen)
{
    GetModuleFileNameA(NULL, exePath, iLen); //�õ�ȫ·��
    char* CutPos = strrchr(exePath, '\\');
    *CutPos = '\0';
}

void GDIPlayer::InitOFN(HWND hWnd, OPENFILENAMEA* Pofn, char OpenSaveFileName[])
{
    ZeroMemory(Pofn, sizeof(OPENFILENAME));
    Pofn->lStructSize = sizeof(OPENFILENAME);
    Pofn->lpstrFile = OpenSaveFileName;
    Pofn->lpstrFile[0] = TEXT('\0');
    Pofn->nMaxFile = sizeof(OpenSaveFileName);
    Pofn->hwndOwner = hWnd;
    Pofn->lpstrFileTitle = NULL;
}

void GDIPlayer::initGui(void)
{
    hWndList = GetDlgItem(this->GetDlgHWND(), IDC_LIST);
    hProgress = GetDlgItem(this->GetDlgHWND(), IDC_SLIDER);
    hWndShow = GetDlgItem(this->GetDlgHWND(), IDC_SHOW);
    hSound = GetDlgItem(this->GetDlgHWND(), IDC_SOUND_SLIDER);
    //��û�������С
    GetClientRect(hWndShow, &rect);
}

void GDIPlayer::Initsound(void)
{
    SendMessage(hProgress, TBM_SETPAGESIZE, 0, (LPARAM)500);
    SendMessage(hProgress, TBM_SETLINESIZE, 0, (LPARAM)500);
    SendMessage(hSound, TBM_SETRANGEMIN, (WPARAM)TRUE, 0);      //�趨��Сֵ
    SendMessage(hSound, TBM_SETRANGEMAX, (WPARAM)TRUE, 1000);   //�趨���ֵ
    SendMessage(hSound, TBM_SETPAGESIZE, 0, (LPARAM)100);       //���� PAGE UP or PAGE DOWN key�Ľ���
    SendMessage(hSound, TBM_SETLINESIZE, 0, (LPARAM)100);       //����RIGHT ARROW or DOWN ARROW key�Ľ���
    SendMessage(hSound, TBM_SETPOS, TRUE, iVolume);             //�趨��ǰ�Ĺ���ֵ
}

void GDIPlayer::ReadSettingInfo(HWND hWnd)
{
    char exePath[MAX_PATH], SetFilePath[MAX_PATH + 10];
    GetExeP(exePath, sizeof(exePath));
    wsprintfA(SetFilePath, "%s\\mp3lst.ini", exePath);
    FILE* fp = fopen(SetFilePath, "r");
    if (NULL == fp)
        return;
    fscanf(fp, "%d\n", &iVolume);
    fscanf(fp, "%d\n", &m_mp3.iPlayMode);
    fscanf(fp, "%d\n", &iExitFlag);
    fscanf(fp, "%d\n", &iMenuFlag);
    fscanf(fp, "%d\n", &eDrawMode);
    int index;
    char szMusicName[MAX_PATH], szTempPath[2 * MAX_PATH];
    char* PszPath = (char*)malloc(2 * MAX_PATH);
    while (fgets(szMusicName, MAX_PATH, fp) != NULL)
    {
        //������fscanf ��Ϊ�пո�
        fgets(szTempPath, 2 * MAX_PATH, fp);
        szMusicName[strlen(szMusicName) - 1] = '\0';
        szTempPath[strlen(szTempPath) - 1] = '\0';
        strcpy(PszPath, szTempPath); //lstrcpyA
        index = (int)SendDlgItemMessageA(hWnd, IDC_LIST, LB_INSERTSTRING, WPARAM(-1), (LPARAM)szMusicName);
        SendDlgItemMessage(hWnd, IDC_LIST, LB_SETITEMDATA, WPARAM(index), (LPARAM)PszPath);
        PszPath = (char*)malloc(2 * MAX_PATH);
    }
    fclose(fp);
}

void GDIPlayer::InitWelcomSound(void)
{
    //���Ż�ӭ����
    //char ExePath[MAX_PATH];
    //char WelcomSoundPath[MAX_PATH + 10];
    //char WelcomSoundShortPath[MAX_PATH + 10];
    //GetExeP(ExePath, sizeof(ExePath));
    //wsprintfA(WelcomSoundPath, "%s\\login.wav", ExePath);
    //GetShortPathNameA(WelcomSoundPath, WelcomSoundShortPath, sizeof(WelcomSoundShortPath));
    //char WelcomCmd[MAX_PATH + 20];
    //wsprintfA(WelcomCmd, "play %s", WelcomSoundShortPath);
    //mciSendStringA(WelcomCmd, "", 0, NULL);
}

void GDIPlayer::GetMyMenu(HWND hWnd)
{
    //��ȡ�˵�
    hMainMenu = LoadMenu(this->GetDlgInst(), MAKEINTRESOURCE(IDR_GDIMENU));
    hTrayMenu = GetSubMenu(hMainMenu, 0);
    //myTrayClass.SetMenu(hTrayMenu);
    hPopMenu = LoadMenu(this->GetDlgInst(), MAKEINTRESOURCE(IDR_LISTMENU));
    hPopMenu = GetSubMenu(hPopMenu, 0);
    SetMenu(hWnd, iMenuFlag ? hMainMenu : NULL);
    //���ݶ�����iExitFlag���ò˵�
    HMENU hMenu = GetSubMenu(hMainMenu, 1);
    CheckMenuItem(hMenu, 0, MF_BYPOSITION | (iExitFlag ? MF_CHECKED : MF_UNCHECKED));
    //���ݶ�����ģʽ����
    if (m_mp3.iPlayMode == ONCE)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("����һ��"));
    else if (m_mp3.iPlayMode == REPATE)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("����ѭ��"));
    else if (m_mp3.iPlayMode == RANDOM)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("�������"));
    else if (m_mp3.iPlayMode == ONCEALL)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("ȫ��һ��"));
    else if (m_mp3.iPlayMode == REPATEALL)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("ȫ��ѭ��"));
}

void GDIPlayer::WriteSettingInfo(HWND hWnd)
{
    char exePath[MAX_PATH];
    GetExeP(exePath, sizeof(exePath));
    char SetFilePath[MAX_PATH + 10];
    wsprintfA(SetFilePath, "%s\\mp3lst.ini", exePath);
    FILE* fp = fopen(SetFilePath, "w");
    if (NULL == fp)
        return;
    fprintf(fp, "%d\n", iVolume);
    fprintf(fp, "%d\n", m_mp3.iPlayMode);
    fprintf(fp, "%d\n", iExitFlag);
    fprintf(fp, "%d\n", iMenuFlag);
    fprintf(fp, "%d\n", eDrawMode);
    int count = ListBox_GetCount(hWndList);
    TCHAR szMusicName[MAX_PATH];
    char* PszPath, MusicName[MAX_PATH];
    for (int i = 0; i < count; i++)
    {
        ListBox_GetText(hWndList, i, szMusicName);
        TcharToChar(szMusicName, MusicName);
        fprintf(fp, "%s\n", MusicName);
        PszPath = (char*)ListBox_GetItemData(hWndList, i);
        fprintf(fp, "%s\n", PszPath);
    }
    fclose(fp);
}

BOOL GDIPlayer::AddDirMusic(HWND hWnd)
{
    BOOL flag = TRUE;
    BROWSEINFOA NewBro;
    char   Buffer[MAX_PATH];
    LPITEMIDLIST   lpitem;
    RtlZeroMemory(&Buffer, sizeof(Buffer));
    RtlZeroMemory(&NewBro, sizeof(BROWSEINFO));
    NewBro.hwndOwner = hWnd;
    NewBro.ulFlags = BIF_RETURNONLYFSDIRS;
    lpitem = SHBrowseForFolderA(&NewBro);
    if (lpitem != NULL)
    {
        SHGetPathFromIDListA(lpitem, Buffer); //�õ�Ŀ¼��·������Buffer��
        flag = SearchFileAddtoList(hWnd, Buffer, "*.mp3");
        flag = SearchFileAddtoList(hWnd, Buffer, "*.wma");
        flag = SearchFileAddtoList(hWnd, Buffer, "*.wav");
        flag = SearchFileAddtoList(hWnd, Buffer, "*.wmv");
    }
    return flag;
}

BOOL GDIPlayer::AddMusicFileToList(HWND hWnd)
{
    char szOpenFileName[50 * MAX_PATH];
    OPENFILENAMEA ofn;
    //��ʼ��ofn
    ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.lpstrFile = szOpenFileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szOpenFileName);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrFilter = "all\0*.*\0������Ƶ(mp3,wma,wav,wmv)\0*.mp3;*.wma;*.wav*.,wmv\0";
    ofn.nFilterIndex = 2;
    ofn.lpstrTitle = "��Ӹ���";
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
    if (GetOpenFileNameA(&ofn))
    {
        char szMusicName[MAX_PATH];                             //������
        LPSTR PszFilePathName = (LPSTR)malloc(2 * MAX_PATH);    //����ȫ·��(����������)
        size_t len = strlen(szOpenFileName), index, i = len;
        //��ѡ
        if (szOpenFileName[len + 1] == '\0')
        {
            char* pszMusicName = strrchr(szOpenFileName, '\\');
            index = (int)SendMessageA(hWndList, LB_INSERTSTRING, WPARAM(-1), (LPARAM)(pszMusicName + 1));
            lstrcpyA(PszFilePathName, szOpenFileName);
            SendMessageA(hWndList, LB_SETITEMDATA, WPARAM(index), (LPARAM)PszFilePathName);
        }
        //��ѡ
        while (szOpenFileName[i + 1] != '\0')
        {
            for (int j = 0;; j++)
            {
                i++;
                szMusicName[j] = szOpenFileName[i];
                if (szOpenFileName[i] == '\0')
                {
                    PszFilePathName = (LPSTR)malloc(2 * MAX_PATH);
                    wsprintfA(PszFilePathName, (szOpenFileName[len - 1] == '\\'
                        ) ? "%s%s" : "%s\\%s", szOpenFileName, szMusicName);
                    index = (int)SendMessageA(hWndList, LB_INSERTSTRING, -1, (LPARAM)szMusicName);
                    SendMessage(hWndList, LB_SETITEMDATA, index, (LPARAM)PszFilePathName);
                    //��ListBox_SetItemData(hWndlist, index, (LPARAM)PszFilePathName);
                    break;
                }
            }
        }
    }
    return TRUE;
}

void GDIPlayer::RemoveSelectedMusic(HWND hWnd)
{
    int count = ListBox_GetSelCount(hWnd);
    int* pSelectIndexes = new int[count];
    ListBox_GetSelItems(hWnd, count, pSelectIndexes);
    for (int i = 0; i < count; i++)
    {
        pSelectIndexes[i] -= i; //�ٴ�֮ǰ��ɾ��i����¼�������¼������ҲҪ�����ı�
        //free((LPTSTR)ListBox_GetItemData(hWnd, pSelectIndexes[i]));//�ڴ�й©
        ListBox_DeleteString(hWnd, pSelectIndexes[i]);      //SendMessage(hWnd, LB_DELETESTRING, pSelectIndexes[i], 0);
        //if (pSelectIndexes[i] < PlayingIndex) //���ڲ��ŵĸ�����������1
        //    PlayingIndex--;
        //else if (pSelectIndexes[i] == PlayingIndex) //ɾ���������ڲ��ŵ�����
        //    StopMusic(hWnd);
    }
}

void GDIPlayer::ClearMusicList(HWND hWnd)
{
    if (IDYES != MessageBox(hWnd, TEXT("�Ƿ���ղ����б�"), TEXT("��Ϣ"), MB_YESNO | MB_ICONQUESTION))
        return;
    int Count = ListBox_GetCount(hWnd);
    for (int i = 0; i < Count; i++)
        ListBox_DeleteString(hWnd, i);  //SendMessage(hWnd, LB_DELETESTRING, 0, 0);
    //free( (LPTSTR)ListBox_GetItemData(hWnd, i) ); //�ڴ�й©
    ListBox_ResetContent(hWnd);
    //StopMusic(hWnd);
}

void GDIPlayer::PlayMusicInList(HWND hWnd, int index)
{
    if (index <= LB_ERR)    //id�Ƿ�
        return;
    //mp3�ļ�����ȡ
    TCHAR swmp3name[MAX_PATH];
    char smp3name[2 * MAX_PATH];
    ListBox_GetText(hWndList, index, swmp3name);
    TcharToChar(swmp3name, smp3name);
    //·����ȡ
    char* smp3Dir = (char*)ListBox_GetItemData(hWndList, index);    //MessageBoxA(hWnd, smp3Dir, smp3name, MB_OK);
    m_mp3.SetSong(smp3Dir, this->GetDlgHWND());    //CurPlayingSong.SetSong(smp3Dir, hWndShow );
    m_mp3.Play(SONG_PLAYING);
    //���ò�����ͣ��ť��ͼ��
    SetDlgItemTextA(hWnd, IDC_PLAYPAUSE, "��ͣ");
    //�趨ʱ��
    char szLen[10];
    ConvLTime2Str(m_mp3.iLength, szLen);
    SetDlgItemTextA(hWnd, IDC_LENTIME, szLen);
    //���ò��Ž������������Сֵ��ʼֵ
    SendMessage(hProgress, TBM_SETPOS, TRUE, 0);
    SendDlgItemMessageA(hWnd, IDC_SLIDER, TBM_SETRANGEMIN, TRUE, 0);
    SendDlgItemMessageA(hWnd, IDC_SLIDER, TBM_SETRANGEMAX, TRUE, m_mp3.iLength);
    ////�趨��ʱ����ÿ��һ���趨��������λ��
    //SetTimer(hWnd, 1, 1000, const_cast<TIMERPROC> (&MusicFrm::AdjustProgessPos));
    ////��ǰ���ŵ�����
    //PlayingIndex = index;
}

void GDIPlayer::GDIPaint(HWND DrawHWnd)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = BeginPaint(DrawHWnd, &ps);
    int w = rect.right - rect.left, h = rect.bottom - rect.top;
    // ������ˢ
    HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
    // ��ˢѡ�񵽵�ǰDC��
    HBRUSH orgBrs = (HBRUSH)SelectObject(ps.hdc, hb);
    // ������ͼ����Ϊ��ɫ����
    Rectangle(ps.hdc, 0, 0, w, h);
    // �������Ĳ���������
    POINT aptTriangle[4] = { { (w /= 2) - 10, (h /= 2) - 15 }, { w - 10, h + 15 }, { w + 10, h }, { w - 10, h - 15 } };
    int num[1] = { 4 };
    HBRUSH hbTriangle = CreateSolidBrush(RGB(250, 250, 250));
    SelectObject(hDrawDC, hbTriangle);
    PolyPolygon(hDrawDC, aptTriangle, num, 1);
    // ѡ��ԭ�ȵĻ�ˢ
    SelectObject(ps.hdc, orgBrs);
    //�������
    DeleteObject(hb);
    DeleteObject(orgBrs);
    DeleteDC(hDrawDC);
    EndPaint(DrawHWnd, &ps);
}

void GDIPlayer::GDIBMPPaint(HINSTANCE hInstance, HWND DrawHWnd)
{
    PAINTSTRUCT ps;
    //����DC���ڴ�DC
    HDC hDrawDC = BeginPaint(DrawHWnd, &ps), memDc = CreateCompatibleDC(NULL);
    //����ͼƬ
    HBITMAP hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BMPLOGO));
    //ͼƬ�����ڴ�DC
    SelectObject(memDc, hBmp);
    //������ָ���豸�����е���չģʽ�����ʧ�����⡣
    SetStretchBltMode(hDrawDC, COLORONCOLOR);
    //���ͼƬ�ߴ�
    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);
    //�ڴ�DC������Dc
    StretchBlt(hDrawDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
        memDc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    DeleteObject(hBmp);
    DeleteDC(memDc);
    DeleteDC(hDrawDC);
    EndPaint(DrawHWnd, &ps);
}

void GDIPlayer::WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INVALIDATE:
        InvalidateRect(hWnd, NULL, TRUE);
        break;
        //default:
        //    DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void GDIPlayer::WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    initGui();
    Initsound();
    InitTitIcon("GDIPlayer");                   //����
    SetIcon(GDIMAINICON);                          //����ͼ��
    ReadSettingInfo(this->GetDlgHWND());            //��ȡ������������ģʽ���б����
    GetMyMenu(this->GetDlgHWND());                  //����˵�
    DragAcceptFiles(this->GetDlgHWND(), TRUE);      //����֧����ק�ļ�
    SendMessage(hWndList, LB_SETCURSEL, 0, 0);  //����listboxĬ��ѡ����
    myTrayClass = new CTray(hWnd, GDIMAINICON, NULL);
    //����listbox
    //addlist(hWnd, uMsg, wParam, lParam);
}

void GDIPlayer::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    DestroyMenu(hTrayMenu);
    DestroyMenu(hPopMenu);
    WriteSettingInfo(hWnd);
    delete myTrayClass;
    EndDialog(hWnd, 0);
    PostQuitMessage(0);
}

void GDIPlayer::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    switch (ctrlID)
    {
#ifndef PLAYPROC //���Ų������
    case IDC_PLAYPAUSE:
    {
        switch (m_mp3.state)
        {
        case SONG_PLAYING:     //���ڲ���
        {
            m_mp3.Play(SONG_PAUSE);
            SetDlgItemText(hWnd, IDC_PLAYPAUSE, TEXT("����"));
        }
        break;
        case SONG_PAUSE:
        {
            m_mp3.Play(SONG_PLAYING);
            SetDlgItemText(hWnd, IDC_PLAYPAUSE, TEXT("��ͣ"));
        }
        break;
        case  SONG_STOP://����δ���й����������Ѿ�ֹͣ
        case SONG_INIT:
        default:
        {
            //Ĭ�ϴ��б�ѡ�е�
            if (ListBox_GetCount(hWndList) > 1)
                PlayMusicInList(hWnd, ListBox_GetCurSel(hWndList));
        }
        break;
        }
    }
    break;
    case IDC_STOP:
    {
        m_mp3.Play(SONG_STOP);
        SetDlgItemTextA(hWnd, IDC_PLAYPAUSE, "����");
        //KillTimer(hWnd, 1);
    }
    break;
    case IDC_BTNPRE:
    {
        break;
        int Count = ListBox_GetCount(hWndList);
        if (0 == m_mp3.PlayingIndex)
            PlayMusicInList(hWnd, Count - 1);
        else
            PlayMusicInList(hWnd, m_mp3.PlayingIndex - 1);
    }
    break;
    case IDC_BTNNEXT:
    {
        break;
        int Count = ListBox_GetCount(hWndList);
        if (Count - 1 == m_mp3.PlayingIndex)
            PlayMusicInList(hWnd, 0);
        else
            PlayMusicInList(hWnd, m_mp3.PlayingIndex + 1);
    }
    break;
    case IDC_CHECKSOUND://���þ���
    {
        HWND hWndcheck = GetDlgItem(hWnd, IDC_CHECKSOUND);
        int CheckState = Button_GetCheck(hWndcheck);
        if (BST_CHECKED == CheckState) //���þ���
            m_mp3.SetSound(true);
        else if (BST_UNCHECKED == CheckState)
            m_mp3.SetSound(false);
    }
    break;
    case IDC_SOUND_SLIDER:
    {
        break;
        iVolume = SendMessage(hSound, TBM_GETPOS, 0, 0);
        char buf[255] = { 0 };
        sprintf(buf, "%d", iVolume);
        MessageBoxA(NULL, buf, "��ʾ", MB_OK);
        //CurPlayingSong.SetVolume(MP.iVolume);
    }
    break;
    case IDC_BTNFULL:
    {
        MCI_OPEN_PARMSA mciOpen;
        UINT wDeviceID;
        mciOpen.lpstrDeviceType = "avivideo";
        mciOpen.lpstrElementName = "1.avi";
        mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
        wDeviceID = mciOpen.wDeviceID;
        MCI_PLAY_PARMS mciPlay;
        mciSendCommand(wDeviceID, MCI_PLAY, MCI_WAIT, (DWORD)&mciPlay);
        break;
        RECT tmprect;
        GetClientRect(this->GetDlgHWND(), &tmprect);
        MoveWindow(hWndShow, 0, 0, tmprect.right, tmprect.bottom, TRUE);
        SetWindowPos(hWndShow, HWND_TOP, 0, 0, 0, 0, TRUE);
    }
    break;
#endif
#ifndef LISTMENUPROC //list�˵�
    case IDC_ADDFILE:               //����ļ�
        AddMusicFileToList(hWnd);
        break;
    case IDC_ADDDIR:                //���Ŀ¼
        AddDirMusic(hWnd);
        break;
    case IDC_LISTREMOVE:            //�Ƴ��б�
        RemoveSelectedMusic(hWndList);
        break;
    case IDC_LISTREMOVEALL:         //��ղ����б�
        ClearMusicList(hWndList);
        break;
    case IDM_VIEWINFO:              //�鿴������Ϣ
    {
        MP3_ID3v1_STRUCT SongInfo;
        int index = ListBox_GetCurSel(hWndList);
        char* PszFilePathName = (char*)ListBox_GetItemData(hWndList, index); //����ȫ·��
        if (!m_mp3.GetMp3Info(PszFilePathName, &SongInfo))
        {
            MessageBox(hWnd, TEXT("��ȡ��Ϣʧ�ܣ�"), TEXT("��Ϣ"), MB_OK | MB_ICONERROR);
            return;
        }
        //HINSTANCE hInstance = ( HINSTANCE ) GetWindowLong ( hWnd, GWLP_HINSTANCE );
        //DialogBoxParam ( hInstance, MAKEINTRESOURCE ( IDD_SONGINFO ), hWnd, SongInfo_Proc, ( LPARAM ) &SongInfo );
    }
    break;
    case IDM_OPENLOCATION:          //�򿪸���������·��
    {
        char szDirPath[MAX_PATH];//Ŀ¼·��
        ZeroMemory(szDirPath, sizeof(szDirPath));
        int index = ListBox_GetCurSel(hWndList);
        char* PszFilePathName = (char*)ListBox_GetItemData(hWndList, index); //����ȫ·��
        char* Ptemp = strrchr(PszFilePathName, '\\');
        strncpy(szDirPath, PszFilePathName, Ptemp - PszFilePathName);
        //MessageBox(hWnd,szDirPath,szDirPath,0);
        ShellExecuteA(hWnd, "open", szDirPath, 0, 0, SW_SHOWNORMAL);
    }
    break;
#endif
#ifndef PLAYMODEPROC//����ģʽ�˵�
    case IDC_STATICPLAYMODE:        //����ģʽ��ذ�ť
    {
        RECT recttmp;
        GetWindowRect(GetDlgItem(hWnd, IDC_STATICPLAYMODE), &recttmp);
        HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE);
        HMENU hPopMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_PLAYMODEMENU));
        hPopMenu = GetSubMenu(hPopMenu, 0);
        TrackPopupMenu(hPopMenu, TPM_LEFTBUTTON, recttmp.left, recttmp.bottom, 0, hWnd, NULL);
        DestroyMenu(hPopMenu);
    }
    break;
    case IDM_PLAYONCE:              //����һ��
    {
        m_mp3.iPlayMode = ONCE;
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("����һ��"));
    }
    break;
    case IDM_PLAYREPART:            //����ѭ��
    {
        m_mp3.iPlayMode = REPATE;
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("����ѭ��"));
    }
    break;
    case IDM_PLAYRANDOM:            //���ѭ��
    {
        m_mp3.iPlayMode = RANDOM;
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("�������"));
    }
    break;
    case IDM_PLAYONCEALL:           //ȫ��һ��
    {
        m_mp3.iPlayMode = ONCEALL;
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("ȫ��һ��"));
    }
    break;
    case IDM_PLAYREPATEALL:         //ȫ��ѭ��
    {
        m_mp3.iPlayMode = REPATEALL;
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("ȫ��ѭ��"));
    }
    break;
#endif
#ifndef SHOWMENUPROC//��ʾ�˵�
    case ID_GDIPOINT:
    {
        eDrawMode = DGPI;
        PostMessage(hWnd, WM_INVALIDATE, 0, 0);//SendMessage
    }
    break;
    case ID_GDIBMPPOINT:
    {
        eDrawMode = DGPIBMP;
        PostMessage(hWnd, WM_INVALIDATE, 0, 0);//SendMessage
    }
    break;
#endif
#ifndef SYSABOUTPROC//ϵͳ���
    case IDC_README:                //����
    {
#ifdef _WIN64
        MessageBox(hWnd, TEXT("GDIPlayer64������1.0\n����C++API��"), TEXT("����"), MB_OK);
#else
        MessageBox(hWnd, TEXT("GDIPlayer������1.0\n����C++API��"), TEXT("����"), MB_OK);
#endif
    }
    break;
    case IDC_EXIT:                  //�˳��˵�
    {
        //myTrayClass.ShowTray(false);
        WriteSettingInfo(hWnd);
        EndDialog(hWnd, 0);
    }
    break;
    case IDC_SETSYSEXIT:            //��С������
    {
        HMENU hMenu = GetMenu(hWnd);
        hMenu = GetSubMenu(hMenu, 1);
        if (iExitFlag) //�ر�ʱ��С�����̣��˵��Ѹ�ѡ
        {
            iExitFlag = 0;
            CheckMenuItem(hMenu, 0, MF_BYPOSITION | MF_UNCHECKED);
            //myTrayClass.ShowTray(true);
        }
        else
        {
            iExitFlag = 1;
            CheckMenuItem(hMenu, 0, MF_BYPOSITION | MF_CHECKED);
        }
    }
    break;
#endif
#ifndef LISTBOXPROC//����ListBox������¼�
    case IDC_LIST:
    {
        int index = LB_ERR;
        if (HIWORD(wParam) == LBN_SELCHANGE)        //��굥��
        {
            //����index
            index = ListBox_GetCurSel(hWndList);
        }
        else if (HIWORD(wParam) == LBN_DBLCLK)          //���˫��
        {
            //��õ��ID
            index = ListBox_GetCurSel(hWndList);
            PlayMusicInList(hWnd, index);
        }
    }
    break;
#endif
#ifndef PLAYSHOWPROC
    case IDC_SHOW:
    {
        //ģ���϶����� WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        SendMessage(this->GetDlgHWND(), WM_NCLBUTTONDOWN, HTCAPTION, 0);
    }
    break;
#endif
    default:
        break;
    }
}

void GDIPlayer::WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //��BUG
    if (LPRECT(lParam)->right - LPRECT(lParam)->left < 326)
        LPRECT(lParam)->right = LPRECT(lParam)->left + 326;
    if (LPRECT(lParam)->bottom - LPRECT(lParam)->top < 558)
        LPRECT(lParam)->bottom = LPRECT(lParam)->top + 558;
}

void GDIPlayer::WMHSCROLL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //���������ֹͣ״̬
    if (m_mp3.state == SONG_INIT || m_mp3.state == SONG_STOP)
        return;
    //���Ž������¼�
    if ((HWND)lParam == hProgress)
    {
        if (SB_ENDSCROLL == LOWORD(wParam) || SB_THUMBPOSITION == LOWORD(wParam))
        {
            LONG lProgress = SendMessage(hProgress, TBM_GETPOS, 0, 0);  //���λ��
            m_mp3.JumpTo(lProgress);                           //��ת
        }
    }
    //���������¼�
    else if ((HWND)lParam == hSound)
    {
        iVolume = SendMessage(hSound, TBM_GETPOS, 0, 0);
        m_mp3.SetVolume(iVolume);
    }
}

void GDIPlayer::WMDROPFILES(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // DragQueryFileԭ��Ϊ��
    //UINT DragQueryFile(HDROP hDrop, UINT iFile, LPTSTR lpszFile, UINT cch)
    //hDrop     ��ָ���϶��ļ��ṹ�ľ����
    //iFile     ����0��ʼ�ı��ж��ļ�����ţ���һ�ο��϶���ļ����˲�����Ϊ0xFFFFFFFF�����϶��ļ���
    //lpszFile  ��ָ���ļ����Ļ�������
    //cch       ���ļ����������Ĵ�С�����ļ������ַ�����
    HDROP hDropInfo = (HDROP)wParam;
    LPSTR PszFilePathName = NULL;  //����ȫ·��(����������)
    int index = -1;
    HWND hWndlist = GetDlgItem(hWnd, IDC_LIST);
    //�õ��ļ�����
    UINT nNumOfFiles = DragQueryFileA(hDropInfo, 0xFFFFFFFF, NULL, 0);
    for (UINT nIndex = 0; nIndex < nNumOfFiles; nIndex++)
    {
        PszFilePathName = (LPSTR)malloc(2 * MAX_PATH);
        //�õ��ļ���
        DragQueryFileA((HDROP)hDropInfo, nIndex, (LPSTR)PszFilePathName, _MAX_PATH);
        //�����������б�
        char* pszMusicName = strrchr((char*)PszFilePathName, '\\');
        index = (int)SendMessageA(hWndlist, LB_INSERTSTRING, WPARAM(-1), (LPARAM)(pszMusicName + 1));
        SendMessageA(hWndlist, LB_SETITEMDATA, WPARAM(index), (LPARAM)PszFilePathName);
    }
    //�����б�
    //PlayMusicInList(hWnd, index);
    DragFinish(hDropInfo);
}

void GDIPlayer::WMSHOWTASK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //wParam���յ���ͼ���ID����lParam���յ���������Ϊ
    //myTrayClass.ShowTask(hWnd, wParam, lParam);
}

void GDIPlayer::WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ((HWND)wParam == GetDlgItem(hWnd, IDC_LIST))
    {
        //���ListBoxѡ�м����������ò˵���״̬
        int iSelCount = ListBox_GetSelCount(GetDlgItem(hWnd, IDC_LIST));
        if (iSelCount == 0)         //û��ѡ�е���
        {
            EnableMenuItem(hPopMenu, IDC_LISTREMOVE, MF_GRAYED);
            EnableMenuItem(hPopMenu, IDM_VIEWINFO, MF_GRAYED);
            EnableMenuItem(hPopMenu, IDM_OPENLOCATION, MF_GRAYED);
        }
        else if (iSelCount != 1)    //ѡ�ж��
        {
            EnableMenuItem(hPopMenu, IDM_VIEWINFO, MF_GRAYED);
            EnableMenuItem(hPopMenu, IDM_OPENLOCATION, MF_GRAYED);
        }
        TrackPopupMenu(hPopMenu, TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
    }
}

void GDIPlayer::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (eDrawMode)
    {
    case GDIPlayer::DGPI:
        GDIPaint(hWndShow);
        break;
    case GDIPlayer::DGPIBMP:
        GDIBMPPaint(this->GetDlgInst(), hWndShow);
        break;
    default:
        break;
    }
}


