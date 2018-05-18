

#include "stdafx.h"
#include <Shlobj.h>     //���Ŀ¼��ʹ��
#include "MainDlg.h"

//ÿ��һ����ã����ý�������ǰλ��
#ifdef _WIN64
    void CALLBACK MusicFrm::AdjustProgessPos(HWND hWnd, UINT uMsg, UINT_PTR iTimeID, DWORD dwTime)
#else
    void CALLBACK MusicFrm::AdjustProgessPos(HWND hWnd, UINT uMsg, UINT iTimeID, DWORD dwTime)
#endif
{
    //long LCurTime = MP3->CurPlayingSong.GetCurPlayingPos();
    ////���ý�����
    //HWND hWndsld = GetDlgItem(hWnd, IDC_SLIDER);
    //SendMessage(hWndsld, TBM_SETPOS, TRUE, LCurTime);
    ////����ʱ���
    //char szCurTime[256];
    //MP3->ConvLTime2Str(LCurTime, szCurTime);
    //SetDlgItemTextA(hWnd, IDC_PLAYTIME, szCurTime);
    ////����Ƿ񲥷����
    //if (LCurTime == 0) //������Ϻ󻬿��Զ��ص���ͷ
    //{
    //    switch (MP3->iPlayMode)
    //    {
    //        case ONCE:      //����һ��
    //            MP3->StopMusic(hWnd);
    //            break;
    //        case REPATE:    //����ѭ��
    //            MP3->CurPlayingSong.Play(SONG_PLAYING);
    //            break;
    //        case RANDOM:    //�������
    //        {
    //            int Count = ListBox_GetCount(GetDlgItem(hWnd, IDC_LIST));
    //            int RanIndex = MP3->GetRandNum(0, Count - 1);
    //            MP3->PlayMusicInList(hWnd, RanIndex);
    //        }
    //        break;
    //        case ONCEALL:   //ȫ��һ��
    //        {
    //            int Count = ListBox_GetCount(GetDlgItem(hWnd, IDC_LIST));
    //            if (Count - 1 == MP3->PlayingIndex)
    //                MP3->StopMusic(hWnd);
    //            else
    //                MP3->PlayMusicInList(hWnd, MP3->PlayingIndex + 1);
    //        }
    //        break;
    //        case REPATEALL: //ȫ��ѭ��
    //        default:
    //        {
    //            int Count = ListBox_GetCount(GetDlgItem(hWnd, IDC_LIST));
    //            if (Count - 1 == MP3->PlayingIndex)
    //                MP3->PlayMusicInList(hWnd, 0);
    //            else
    //                MP3->PlayMusicInList(hWnd, MP3->PlayingIndex + 1);
    //        }
    //        break;
    //    }
    //}
}

MusicFrm::MusicFrm() : iExitFlag(0)//Ĭ�ϲ��˳�������
    , hInstance(NULL)
    , hWndDlg(NULL)
    , hProgress(NULL)
    , hSound(NULL)
    , hWndList(NULL)
    , hWndShow(NULL)
    //, myTrayClass(hWndDlg, MAINICON,NULL)
    //, rect()
    , iVolume(1000)          //Ĭ���������
    , iMenuFlag(1)           //���˵���ʾ
    , eDrawMode(DGPI)
    , iPlayMode(REPATEALL)   //Ĭ�� ȫ��ѭ��
    , PlayingIndex(-1)       //��ǰ��������
{
}

MusicFrm::~MusicFrm()
{
    DestroyMenu(hTrayMenu);
    DestroyMenu(hPopMenu);
}

void MusicFrm::GethInstance(HWND hWnd)
{
#ifdef _WIN64
    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
#else
    hInstance = (HINSTANCE) GetWindowLong(hWnd, GWLP_HINSTANCE);
#endif
    hWndDlg = hWnd;
    hWndList = GetDlgItem(hWndDlg, IDC_LIST);
    hProgress = GetDlgItem(hWndDlg, IDC_SLIDER);
    hWndShow = GetDlgItem(hWndDlg, IDC_SHOW);
    hSound = GetDlgItem(hWndDlg, IDC_SOUND_SLIDER);
    //��û�������С
    GetClientRect(hWndShow, &rect);
}

void MusicFrm::GetMyMenu(HWND hWnd)
{
    //��ȡ�˵�
    hMainMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
    hTrayMenu = GetSubMenu(hMainMenu, 0);
    //myTrayClass.SetMenu(hTrayMenu);
    hPopMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_LISTMENU));
    hPopMenu = GetSubMenu(hPopMenu, 0);
    //DestroyMenu(hTrayMenu);
    //DestroyMenu(hPopMenu);
    SetMenu(hWnd, iMenuFlag ? hMainMenu : NULL);
    //���ݶ�����iExitFlag���ò˵�
    HMENU hMenu = GetSubMenu(hMainMenu, 1);
    CheckMenuItem(hMenu, 0, MF_BYPOSITION | (iExitFlag ? MF_CHECKED : MF_UNCHECKED));
    //���ݶ�����ģʽ����
    if (iPlayMode == ONCE)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("����һ��"));
    else if (iPlayMode == REPATE)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("����ѭ��"));
    else if (iPlayMode == RANDOM)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("�������"));
    else if (iPlayMode == ONCEALL)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("ȫ��һ��"));
    else if (iPlayMode == REPATEALL)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("ȫ��ѭ��"));
}

void MusicFrm::Initsound(void)
{
    SendMessage(hProgress, TBM_SETPAGESIZE, 0, (LPARAM)500);
    SendMessage(hProgress, TBM_SETLINESIZE, 0, (LPARAM)500);
    SendMessage(hSound, TBM_SETRANGEMIN, (WPARAM)TRUE, 0);      //�趨��Сֵ
    SendMessage(hSound, TBM_SETRANGEMAX, (WPARAM)TRUE, 1000);   //�趨���ֵ
    SendMessage(hSound, TBM_SETPAGESIZE, 0, (LPARAM)100);       //���� PAGE UP or PAGE DOWN key�Ľ���
    SendMessage(hSound, TBM_SETLINESIZE, 0, (LPARAM)100);       //����RIGHT ARROW or DOWN ARROW key�Ľ���
    SendMessage(hSound, TBM_SETPOS, TRUE, iVolume);             //�趨��ǰ�Ĺ���ֵ
}

void MusicFrm::InitTitIcon(HWND hWnd)
{
    //�������
#ifdef _WIN64
    SetWindowText(hWnd, TEXT("GDIPlayer64"));
#else
    SetWindowText(hWnd, TEXT("GDIPlayer"));
#endif
    //����ͼ��
    HICON hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(MAINICON));
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

void MusicFrm::ReadSettingInfo(HWND hWnd)
{
    char exePath[MAX_PATH], SetFilePath[MAX_PATH + 10];
    GetExeP(exePath, sizeof(exePath));
    wsprintfA(SetFilePath, "%s\\mp3lst.ini", exePath);
    FILE* fp = fopen(SetFilePath, "r");
    if (NULL == fp)
        return;
    fscanf(fp, "%d\n", &iVolume);
    fscanf(fp, "%d\n", &iPlayMode);
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

void TcharToChar(const TCHAR* tchar, char* _char)
{
    //��ȡ�ֽڳ���
    int iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
    //��tcharֵ����_char
    WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}

void MusicFrm::WriteSettingInfo(HWND hWnd)
{
    char exePath[MAX_PATH];
    GetExeP(exePath, sizeof(exePath));
    char SetFilePath[MAX_PATH + 10];
    wsprintfA(SetFilePath, "%s\\mp3lst.ini", exePath);
    FILE* fp = fopen(SetFilePath, "w");
    if (NULL == fp)
        return;
    fprintf(fp, "%d\n", iVolume);
    fprintf(fp, "%d\n", iPlayMode);
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

BOOL MusicFrm::AddDirMusic(HWND hWnd)
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

BOOL MusicFrm::AddMusicFileToList(HWND hWnd)
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

BOOL MusicFrm::SearchFileAddtoList(HWND hWnd, char path[], char format[])
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
    }
    while (TRUE == FindNextFileA(hFind, &FindFileData));
    FindClose(hFind);
    return TRUE;
}

void MusicFrm::RemoveSelectedMusic(HWND hWnd)
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

void MusicFrm::PlayMusicInList(HWND hWnd, int index)
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
    CurPlayingSong.SetSong(smp3Dir, hWndDlg);    //CurPlayingSong.SetSong(smp3Dir, hWndShow );
    CurPlayingSong.Play(SONG_PLAYING);
    //���ò�����ͣ��ť��ͼ��
    SetDlgItemTextA(hWnd, IDC_PLAYPAUSE, "��ͣ");
    //�趨ʱ��
    char szLen[10];
    ConvLTime2Str(CurPlayingSong.iLength, szLen);
    SetDlgItemTextA(hWnd, IDC_LENTIME, szLen);
    //���ò��Ž������������Сֵ��ʼֵ
    SendMessage(hProgress, TBM_SETPOS, TRUE, 0);
    SendDlgItemMessageA(hWnd, IDC_SLIDER, TBM_SETRANGEMIN, TRUE, 0);
    SendDlgItemMessageA(hWnd, IDC_SLIDER, TBM_SETRANGEMAX, TRUE, CurPlayingSong.iLength);
    ////�趨��ʱ����ÿ��һ���趨��������λ��
    //SetTimer(hWnd, 1, 1000, const_cast<TIMERPROC> (&MusicFrm::AdjustProgessPos));
    ////��ǰ���ŵ�����
    //PlayingIndex = index;
}

void MusicFrm::InitOFN(HWND hWnd, OPENFILENAMEA* Pofn, char OpenSaveFileName[])
{
    ZeroMemory(Pofn, sizeof(OPENFILENAME));
    Pofn->lStructSize = sizeof(OPENFILENAME);
    Pofn->lpstrFile = OpenSaveFileName;
    Pofn->lpstrFile[0] = TEXT('\0');
    Pofn->nMaxFile = sizeof(OpenSaveFileName);
    Pofn->hwndOwner = hWnd;
    Pofn->lpstrFileTitle = NULL;
}

void MusicFrm::InitWelcomSound(void)
{
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

void MusicFrm::ConvLTime2Str(LONG LTime, char* StrTime)
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

int MusicFrm::GetRandNum(int a, int b)
{
    srand((unsigned long)time(NULL));
    int num = rand();
    num = a + num % (b - a + 1);
    return num;
}

void MusicFrm::GetExeP(char* exePath, int iLen)
{
    GetModuleFileNameA(NULL, exePath, iLen); //�õ�ȫ·��
    char* CutPos = strrchr(exePath, '\\');
    *CutPos = '\0';
}

void MusicFrm::ClearMusicList(HWND hWnd)
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

void MainMsg::WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void MainMsg::WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //-------------swith����Ҳ��������ӳ��----------------
    //BOOL Main_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam);
    //void Main_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);
    //void Main_OnHScroll(HWND hWnd, HWND hWndCtl, UINT code, int pos);
    //void Main_OnDropFiles(HWND hWnd, HDROP hDropInfo);
    ////��ʼ��
    //HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
    ////�¼�
    //HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
    ////ˮƽ������
    //HANDLE_MSG(hWnd, WM_HSCROLL, Main_OnHScroll);
    ////�Ϸ��ļ�
    //HANDLE_MSG(hWnd, WM_DROPFILES, Main_OnDropFiles);
    //-------------------------------------------------
    GethInstance(hWnd);
    InitTitIcon(hWndDlg);                       //���ñ���ͼ��
    Initsound();                                //��ʼ������������
    ReadSettingInfo(hWndDlg);                   //��ȡ������������ģʽ���б����
    GetMyMenu(hWndDlg);                         //����˵�
    DragAcceptFiles(hWndDlg, TRUE);             //����֧����ק�ļ�
    InitWelcomSound();                          //���Ż�ӭ����
    //����listboxĬ��ѡ����
    SendMessage(hWndList, LB_SETCURSEL, 0, 0);
    //opengl����
    if (DGL == eDrawMode)
    {
        HDC hDrawDC = GetDC(hWndShow);
        m_OpenGL.SetDC(hDrawDC);
        m_OpenGL.InitOpenGL();
        m_OpenGL.ChangeSize(rect.right - rect.left, rect.bottom - rect.top);
    }
    //����listbox
    //addlist(hWnd, uMsg, wParam, lParam);
}

void MainMsg::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WriteSettingInfo(hWnd);
    EndDialog(hWnd, 0);
    //myTrayClass.ShowTray(false);
    PostQuitMessage(0);
}

void MainMsg::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (eDrawMode)
    {
        case MusicFrm::DGPI:
            GDIPaint(hWndShow);
            break;
        case MusicFrm::DGPIBMP:
            GDIBMPPaint(hWndShow);
            break;
        case MusicFrm::DGL:
            OpenGlPaint(hWndShow);
            break;
        case MusicFrm::DDX9:
            break;
        case MusicFrm::DNULL:
        default:
            break;
    }
}

void MainMsg::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    switch (ctrlID)
    {
#ifndef PLAYPROC //���Ų������
        case IDC_PLAYPAUSE:
        {
            switch (CurPlayingSong.state)
            {
                case SONG_PLAYING:     //���ڲ���
                {
                    CurPlayingSong.Play(SONG_PAUSE);
                    SetDlgItemText(hWnd, IDC_PLAYPAUSE, TEXT("����"));
                }
                break;
                case SONG_PAUSE:
                {
                    CurPlayingSong.Play(SONG_PLAYING);
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
            CurPlayingSong.Play(SONG_STOP);
            SetDlgItemTextA(hWnd, IDC_PLAYPAUSE, "����");
            //KillTimer(hWnd, 1);
        }
        break;
        case IDC_BTNPRE:
        {
            break;
            int Count = ListBox_GetCount(hWndList);
            if (0 == PlayingIndex)
                PlayMusicInList(hWnd, Count - 1);
            else
                PlayMusicInList(hWnd, PlayingIndex - 1);
        }
        break;
        case IDC_BTNNEXT:
        {
            break;
            int Count = ListBox_GetCount(hWndList);
            if (Count - 1 == PlayingIndex)
                PlayMusicInList(hWnd, 0);
            else
                PlayMusicInList(hWnd, PlayingIndex + 1);
        }
        break;
        case IDC_CHECKSOUND://���þ���
        {
            HWND hWndcheck = GetDlgItem(hWnd, IDC_CHECKSOUND);
            int CheckState = Button_GetCheck(hWndcheck);
            if (BST_CHECKED == CheckState) //���þ���
                CurPlayingSong.SetSound(true);
            else if (BST_UNCHECKED == CheckState)
                CurPlayingSong.SetSound(false);
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
            GetClientRect(hWndDlg, &tmprect);
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
            if (!CurPlayingSong.GetMp3Info(PszFilePathName, &SongInfo))
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
            iPlayMode = ONCE;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("����һ��"));
        }
        break;
        case IDM_PLAYREPART:            //����ѭ��
        {
            iPlayMode = REPATE;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("����ѭ��"));
        }
        break;
        case IDM_PLAYRANDOM:            //���ѭ��
        {
            iPlayMode = RANDOM;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("�������"));
        }
        break;
        case IDM_PLAYONCEALL:           //ȫ��һ��
        {
            iPlayMode = ONCEALL;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("ȫ��һ��"));
        }
        break;
        case IDM_PLAYREPATEALL:         //ȫ��ѭ��
        {
            iPlayMode = REPATEALL;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("ȫ��ѭ��"));
        }
        break;
#endif
#ifndef SHOWMENUPROC//��ʾ�˵�
        case ID_OPENGL:
        {
            //opengl����
            HDC hDrawDC = GetDC(hWndShow);
            m_OpenGL.SetDC(hDrawDC);
            m_OpenGL.InitOpenGL();
            m_OpenGL.ChangeSize(rect.right - rect.left, rect.bottom - rect.top);
            eDrawMode = DGL;
            PostMessage(hWnd, WM_INVALIDATE, 0, 0);//SendMessage
            //InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
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
            SendMessage(hWndDlg, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        }
        break;
#endif
        default:
            break;
    }
}

void MainMsg::WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void MainMsg::WMSHOWTASK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //wParam���յ���ͼ���ID����lParam���յ���������Ϊ
    //myTrayClass.ShowTask(hWnd, wParam, lParam);
}

void MainMsg::WMHSCROLL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //���������ֹͣ״̬
    if (CurPlayingSong.state == SONG_INIT || CurPlayingSong.state == SONG_STOP)
        return;
    //���Ž������¼�
    if ((HWND)lParam == hProgress)
    {
        if (SB_ENDSCROLL == LOWORD(wParam) || SB_THUMBPOSITION == LOWORD(wParam))
        {
            LONG lProgress = SendMessage(hProgress, TBM_GETPOS, 0, 0);  //���λ��
            CurPlayingSong.JumpTo(lProgress);                           //��ת
        }
    }
    //���������¼�
    else if ((HWND)lParam == hSound)
    {
        iVolume = SendMessage(hSound, TBM_GETPOS, 0, 0);
        CurPlayingSong.SetVolume(iVolume);
    }
}

void MainMsg::WMDROPFILES(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void MainMsg::GDIPaint(HWND DrawhWnd)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = BeginPaint(DrawhWnd, &ps);
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
    EndPaint(DrawhWnd, &ps);
}

void MainMsg::GDIBMPPaint(HWND DrawhWnd)
{
    PAINTSTRUCT ps;
    //����DC���ڴ�DC
    HDC hDrawDC = BeginPaint(DrawhWnd, &ps), memDc = CreateCompatibleDC(NULL);
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
    EndPaint(DrawhWnd, &ps);
}

void MainMsg::addlist(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //���Դ��� ����ɾ��
    HWND hWndList = GetDlgItem(hWnd, IDC_LIST);
    TCHAR szBuf[10];
    for (int k = 1; k < 10; ++k)
    {
        _itot(k * 100, szBuf, 10);
        SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)szBuf);
    }
    SendMessage(hWndList, LB_SETCURSEL, 0, 0);  // ����Ĭ��ѡ����
}

void MainMsg::WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //��BUG
    if (LPRECT(lParam)->right - LPRECT(lParam)->left < 326)
        LPRECT(lParam)->right = LPRECT(lParam)->left + 326;
    if (LPRECT(lParam)->bottom - LPRECT(lParam)->top < 558)
        LPRECT(lParam)->bottom = LPRECT(lParam)->top + 558;
}

void MainMsg::OpenGlPaint(HWND hDrawWnd)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = GetDC(hDrawWnd);
    //RECT rect;
    //GetClientRect(hDrawWnd, &rect);//GetClientRect  GetWindowRect
    //opengl��ͼ����
    hDrawDC = BeginPaint(hDrawWnd, &ps);
    m_OpenGL.RenderScene();
    SwapBuffers(hDrawDC);
    DeleteDC(hDrawDC);
    EndPaint(hDrawWnd, &ps);
}

IMPLEMENT_SINGLETON(MainMsg);

extern "C" MainMsg* GetMP3()
{
    return MainMsg::Instance();
}






