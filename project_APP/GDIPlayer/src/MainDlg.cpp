

#include "stdafx.h"
#include <Shlobj.h>     //添加目录是使用
#include "MainDlg.h"

//每隔一秒调用，设置进度条当前位置
#ifdef _WIN64
    void CALLBACK MusicFrm::AdjustProgessPos(HWND hWnd, UINT uMsg, UINT_PTR iTimeID, DWORD dwTime)
#else
    void CALLBACK MusicFrm::AdjustProgessPos(HWND hWnd, UINT uMsg, UINT iTimeID, DWORD dwTime)
#endif
{
    //long LCurTime = MP3->CurPlayingSong.GetCurPlayingPos();
    ////设置进度条
    //HWND hWndsld = GetDlgItem(hWnd, IDC_SLIDER);
    //SendMessage(hWndsld, TBM_SETPOS, TRUE, LCurTime);
    ////设置时间框
    //char szCurTime[256];
    //MP3->ConvLTime2Str(LCurTime, szCurTime);
    //SetDlgItemTextA(hWnd, IDC_PLAYTIME, szCurTime);
    ////检查是否播放完毕
    //if (LCurTime == 0) //播放完毕后滑块自动回到开头
    //{
    //    switch (MP3->iPlayMode)
    //    {
    //        case ONCE:      //单曲一次
    //            MP3->StopMusic(hWnd);
    //            break;
    //        case REPATE:    //单曲循环
    //            MP3->CurPlayingSong.Play(SONG_PLAYING);
    //            break;
    //        case RANDOM:    //随机播放
    //        {
    //            int Count = ListBox_GetCount(GetDlgItem(hWnd, IDC_LIST));
    //            int RanIndex = MP3->GetRandNum(0, Count - 1);
    //            MP3->PlayMusicInList(hWnd, RanIndex);
    //        }
    //        break;
    //        case ONCEALL:   //全部一次
    //        {
    //            int Count = ListBox_GetCount(GetDlgItem(hWnd, IDC_LIST));
    //            if (Count - 1 == MP3->PlayingIndex)
    //                MP3->StopMusic(hWnd);
    //            else
    //                MP3->PlayMusicInList(hWnd, MP3->PlayingIndex + 1);
    //        }
    //        break;
    //        case REPATEALL: //全部循环
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

MusicFrm::MusicFrm() : iExitFlag(0)//默认不退出到托盘
    , hInstance(NULL)
    , hWndDlg(NULL)
    , hProgress(NULL)
    , hSound(NULL)
    , hWndList(NULL)
    , hWndShow(NULL)
    //, myTrayClass(hWndDlg, MAINICON,NULL)
    //, rect()
    , iVolume(1000)          //默认最大声音
    , iMenuFlag(1)           //主菜单显示
    , eDrawMode(DGPI)
    , iPlayMode(REPATEALL)   //默认 全部循环
    , PlayingIndex(-1)       //当前播放索引
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
    //获得绘制区大小
    GetClientRect(hWndShow, &rect);
}

void MusicFrm::GetMyMenu(HWND hWnd)
{
    //获取菜单
    hMainMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
    hTrayMenu = GetSubMenu(hMainMenu, 0);
    //myTrayClass.SetMenu(hTrayMenu);
    hPopMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_LISTMENU));
    hPopMenu = GetSubMenu(hPopMenu, 0);
    //DestroyMenu(hTrayMenu);
    //DestroyMenu(hPopMenu);
    SetMenu(hWnd, iMenuFlag ? hMainMenu : NULL);
    //根据读出的iExitFlag设置菜单
    HMENU hMenu = GetSubMenu(hMainMenu, 1);
    CheckMenuItem(hMenu, 0, MF_BYPOSITION | (iExitFlag ? MF_CHECKED : MF_UNCHECKED));
    //根据读出的模式设置
    if (iPlayMode == ONCE)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("单曲一次"));
    else if (iPlayMode == REPATE)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("单曲循环"));
    else if (iPlayMode == RANDOM)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("随机播放"));
    else if (iPlayMode == ONCEALL)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("全部一次"));
    else if (iPlayMode == REPATEALL)
        SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("全部循环"));
}

void MusicFrm::Initsound(void)
{
    SendMessage(hProgress, TBM_SETPAGESIZE, 0, (LPARAM)500);
    SendMessage(hProgress, TBM_SETLINESIZE, 0, (LPARAM)500);
    SendMessage(hSound, TBM_SETRANGEMIN, (WPARAM)TRUE, 0);      //设定最小值
    SendMessage(hSound, TBM_SETRANGEMAX, (WPARAM)TRUE, 1000);   //设定最大值
    SendMessage(hSound, TBM_SETPAGESIZE, 0, (LPARAM)100);       //设置 PAGE UP or PAGE DOWN key的进度
    SendMessage(hSound, TBM_SETLINESIZE, 0, (LPARAM)100);       //设置RIGHT ARROW or DOWN ARROW key的进度
    SendMessage(hSound, TBM_SETPOS, TRUE, iVolume);             //设定当前的滚动值
}

void MusicFrm::InitTitIcon(HWND hWnd)
{
    //界面标题
#ifdef _WIN64
    SetWindowText(hWnd, TEXT("GDIPlayer64"));
#else
    SetWindowText(hWnd, TEXT("GDIPlayer"));
#endif
    //界面图标
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
        //不能用fscanf 因为有空格
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
    //获取字节长度
    int iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
    //将tchar值赋给_char
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
        SHGetPathFromIDListA(lpitem, Buffer); //得到目录的路劲放在Buffer中
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
    //初始化ofn
    ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.lpstrFile = szOpenFileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szOpenFileName);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrFilter = "all\0*.*\0常见音频(mp3,wma,wav,wmv)\0*.mp3;*.wma;*.wav*.,wmv\0";
    ofn.nFilterIndex = 2;
    ofn.lpstrTitle = "添加歌曲";
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
    if (GetOpenFileNameA(&ofn))
    {
        char szMusicName[MAX_PATH];                             //歌曲名
        LPSTR PszFilePathName = (LPSTR)malloc(2 * MAX_PATH);    //歌曲全路径(包含歌曲名)
        size_t len = strlen(szOpenFileName), index, i = len;
        //单选
        if (szOpenFileName[len + 1] == '\0')
        {
            char* pszMusicName = strrchr(szOpenFileName, '\\');
            index = (int)SendMessageA(hWndList, LB_INSERTSTRING, WPARAM(-1), (LPARAM)(pszMusicName + 1));
            lstrcpyA(PszFilePathName, szOpenFileName);
            SendMessageA(hWndList, LB_SETITEMDATA, WPARAM(index), (LPARAM)PszFilePathName);
        }
        //多选
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
                    //或ListBox_SetItemData(hWndlist, index, (LPARAM)PszFilePathName);
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
    LPSTR PszFilePathName = NULL;  //歌曲全路径(包含歌曲名)
    WIN32_FIND_DATAA FindFileData;
    wsprintfA(SearchFormat, "%s\\%s", path, format);
    HANDLE hFind = FindFirstFileA(SearchFormat, &FindFileData); //注意目录的形式后面要有*.*
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
        pSelectIndexes[i] -= i; //再此之前已删除i条记录，下面记录的索引也要发生改变
        //free((LPTSTR)ListBox_GetItemData(hWnd, pSelectIndexes[i]));//内存泄漏
        ListBox_DeleteString(hWnd, pSelectIndexes[i]);      //SendMessage(hWnd, LB_DELETESTRING, pSelectIndexes[i], 0);
        //if (pSelectIndexes[i] < PlayingIndex) //正在播放的歌曲的索引减1
        //    PlayingIndex--;
        //else if (pSelectIndexes[i] == PlayingIndex) //删除的是正在播放的音乐
        //    StopMusic(hWnd);
    }
}

void MusicFrm::PlayMusicInList(HWND hWnd, int index)
{
    if (index <= LB_ERR)    //id非法
        return;
    //mp3文件名获取
    TCHAR swmp3name[MAX_PATH];
    char smp3name[2 * MAX_PATH];
    ListBox_GetText(hWndList, index, swmp3name);
    TcharToChar(swmp3name, smp3name);
    //路径获取
    char* smp3Dir = (char*)ListBox_GetItemData(hWndList, index);    //MessageBoxA(hWnd, smp3Dir, smp3name, MB_OK);
    CurPlayingSong.SetSong(smp3Dir, hWndDlg);    //CurPlayingSong.SetSong(smp3Dir, hWndShow );
    CurPlayingSong.Play(SONG_PLAYING);
    //设置播放暂停按钮的图标
    SetDlgItemTextA(hWnd, IDC_PLAYPAUSE, "暂停");
    //设定时间
    char szLen[10];
    ConvLTime2Str(CurPlayingSong.iLength, szLen);
    SetDlgItemTextA(hWnd, IDC_LENTIME, szLen);
    //设置播放进度条的最大最小值初始值
    SendMessage(hProgress, TBM_SETPOS, TRUE, 0);
    SendDlgItemMessageA(hWnd, IDC_SLIDER, TBM_SETRANGEMIN, TRUE, 0);
    SendDlgItemMessageA(hWnd, IDC_SLIDER, TBM_SETRANGEMAX, TRUE, CurPlayingSong.iLength);
    ////设定计时器，每隔一秒设定进度条的位置
    //SetTimer(hWnd, 1, 1000, const_cast<TIMERPROC> (&MusicFrm::AdjustProgessPos));
    ////当前播放的索引
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
    LTime = LTime / 1000; //由于LTime代表的微秒数，故除以1000
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
    GetModuleFileNameA(NULL, exePath, iLen); //得到全路径
    char* CutPos = strrchr(exePath, '\\');
    *CutPos = '\0';
}

void MusicFrm::ClearMusicList(HWND hWnd)
{
    if (IDYES != MessageBox(hWnd, TEXT("是否清空播放列表？"), TEXT("消息"), MB_YESNO | MB_ICONQUESTION))
        return;
    int Count = ListBox_GetCount(hWnd);
    for (int i = 0; i < Count; i++)
        ListBox_DeleteString(hWnd, i);  //SendMessage(hWnd, LB_DELETESTRING, 0, 0);
    //free( (LPTSTR)ListBox_GetItemData(hWnd, i) ); //内存泄漏
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
    //-------------swith里面也可以这样映射----------------
    //BOOL Main_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam);
    //void Main_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);
    //void Main_OnHScroll(HWND hWnd, HWND hWndCtl, UINT code, int pos);
    //void Main_OnDropFiles(HWND hWnd, HDROP hDropInfo);
    ////初始化
    //HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
    ////事件
    //HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
    ////水平滚动条
    //HANDLE_MSG(hWnd, WM_HSCROLL, Main_OnHScroll);
    ////拖放文件
    //HANDLE_MSG(hWnd, WM_DROPFILES, Main_OnDropFiles);
    //-------------------------------------------------
    GethInstance(hWnd);
    InitTitIcon(hWndDlg);                       //设置标题图标
    Initsound();                                //初始化声音滚动条
    ReadSettingInfo(hWndDlg);                   //读取到声音、播放模式、列表歌曲
    GetMyMenu(hWndDlg);                         //处理菜单
    DragAcceptFiles(hWndDlg, TRUE);             //设置支持拖拽文件
    InitWelcomSound();                          //播放欢迎声音
    //设置listbox默认选中项
    SendMessage(hWndList, LB_SETCURSEL, 0, 0);
    //opengl测试
    if (DGL == eDrawMode)
    {
        HDC hDrawDC = GetDC(hWndShow);
        m_OpenGL.SetDC(hDrawDC);
        m_OpenGL.InitOpenGL();
        m_OpenGL.ChangeSize(rect.right - rect.left, rect.bottom - rect.top);
    }
    //测试listbox
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
#ifndef PLAYPROC //播放操作相关
        case IDC_PLAYPAUSE:
        {
            switch (CurPlayingSong.state)
            {
                case SONG_PLAYING:     //正在播放
                {
                    CurPlayingSong.Play(SONG_PAUSE);
                    SetDlgItemText(hWnd, IDC_PLAYPAUSE, TEXT("播放"));
                }
                break;
                case SONG_PAUSE:
                {
                    CurPlayingSong.Play(SONG_PLAYING);
                    SetDlgItemText(hWnd, IDC_PLAYPAUSE, TEXT("暂停"));
                }
                break;
                case  SONG_STOP://歌曲未进行过操作或者已经停止
                case SONG_INIT:
                default:
                {
                    //默认打开列表选中的
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
            SetDlgItemTextA(hWnd, IDC_PLAYPAUSE, "播放");
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
        case IDC_CHECKSOUND://设置静音
        {
            HWND hWndcheck = GetDlgItem(hWnd, IDC_CHECKSOUND);
            int CheckState = Button_GetCheck(hWndcheck);
            if (BST_CHECKED == CheckState) //设置静音
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
            MessageBoxA(NULL, buf, "提示", MB_OK);
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
#ifndef LISTMENUPROC //list菜单
        case IDC_ADDFILE:               //添加文件
            AddMusicFileToList(hWnd);
            break;
        case IDC_ADDDIR:                //添加目录
            AddDirMusic(hWnd);
            break;
        case IDC_LISTREMOVE:            //移出列表
            RemoveSelectedMusic(hWndList);
            break;
        case IDC_LISTREMOVEALL:         //清空播放列表
            ClearMusicList(hWndList);
            break;
        case IDM_VIEWINFO:              //查看歌曲信息
        {
            MP3_ID3v1_STRUCT SongInfo;
            int index = ListBox_GetCurSel(hWndList);
            char* PszFilePathName = (char*)ListBox_GetItemData(hWndList, index); //歌曲全路径
            if (!CurPlayingSong.GetMp3Info(PszFilePathName, &SongInfo))
            {
                MessageBox(hWnd, TEXT("获取信息失败！"), TEXT("消息"), MB_OK | MB_ICONERROR);
                return;
            }
            //HINSTANCE hInstance = ( HINSTANCE ) GetWindowLong ( hWnd, GWLP_HINSTANCE );
            //DialogBoxParam ( hInstance, MAKEINTRESOURCE ( IDD_SONGINFO ), hWnd, SongInfo_Proc, ( LPARAM ) &SongInfo );
        }
        break;
        case IDM_OPENLOCATION:          //打开歌曲的所在路径
        {
            char szDirPath[MAX_PATH];//目录路径
            ZeroMemory(szDirPath, sizeof(szDirPath));
            int index = ListBox_GetCurSel(hWndList);
            char* PszFilePathName = (char*)ListBox_GetItemData(hWndList, index); //歌曲全路径
            char* Ptemp = strrchr(PszFilePathName, '\\');
            strncpy(szDirPath, PszFilePathName, Ptemp - PszFilePathName);
            //MessageBox(hWnd,szDirPath,szDirPath,0);
            ShellExecuteA(hWnd, "open", szDirPath, 0, 0, SW_SHOWNORMAL);
        }
        break;
#endif
#ifndef PLAYMODEPROC//播放模式菜单
        case IDC_STATICPLAYMODE:        //播放模式相关按钮
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
        case IDM_PLAYONCE:              //单曲一次
        {
            iPlayMode = ONCE;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("单曲一次"));
        }
        break;
        case IDM_PLAYREPART:            //单曲循环
        {
            iPlayMode = REPATE;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("单曲循环"));
        }
        break;
        case IDM_PLAYRANDOM:            //随机循环
        {
            iPlayMode = RANDOM;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("随机播放"));
        }
        break;
        case IDM_PLAYONCEALL:           //全部一次
        {
            iPlayMode = ONCEALL;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("全部一次"));
        }
        break;
        case IDM_PLAYREPATEALL:         //全部循环
        {
            iPlayMode = REPATEALL;
            SetDlgItemText(hWnd, IDC_STATICPLAYMODE, TEXT("全部循环"));
        }
        break;
#endif
#ifndef SHOWMENUPROC//显示菜单
        case ID_OPENGL:
        {
            //opengl测试
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
#ifndef SYSABOUTPROC//系统相关
        case IDC_README:                //关于
        {
#ifdef _WIN64
            MessageBox(hWnd, TEXT("GDIPlayer64播放器1.0\n阿甘C++API版"), TEXT("关于"), MB_OK);
#else
            MessageBox(hWnd, TEXT("GDIPlayer播放器1.0\n阿甘C++API版"), TEXT("关于"), MB_OK);
#endif
        }
        break;
        case IDC_EXIT:                  //退出菜单
        {
            //myTrayClass.ShowTray(false);
            WriteSettingInfo(hWnd);
            EndDialog(hWnd, 0);
        }
        break;
        case IDC_SETSYSEXIT:            //最小化托盘
        {
            HMENU hMenu = GetMenu(hWnd);
            hMenu = GetSubMenu(hMenu, 1);
            if (iExitFlag) //关闭时最小到托盘，菜单已复选
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
#ifndef LISTBOXPROC//处理ListBox的鼠标事件
        case IDC_LIST:
        {
            int index = LB_ERR;
            if (HIWORD(wParam) == LBN_SELCHANGE)        //鼠标单击
            {
                //记下index
                index = ListBox_GetCurSel(hWndList);
            }
            else if (HIWORD(wParam) == LBN_DBLCLK)          //鼠标双击
            {
                //获得点击ID
                index = ListBox_GetCurSel(hWndList);
                PlayMusicInList(hWnd, index);
            }
        }
        break;
#endif
#ifndef PLAYSHOWPROC
        case IDC_SHOW:
        {
            //模拟拖动窗体 WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
        //检查ListBox选中几个，并设置菜单的状态
        int iSelCount = ListBox_GetSelCount(GetDlgItem(hWnd, IDC_LIST));
        if (iSelCount == 0)         //没有选中的项
        {
            EnableMenuItem(hPopMenu, IDC_LISTREMOVE, MF_GRAYED);
            EnableMenuItem(hPopMenu, IDM_VIEWINFO, MF_GRAYED);
            EnableMenuItem(hPopMenu, IDM_OPENLOCATION, MF_GRAYED);
        }
        else if (iSelCount != 1)    //选中多个
        {
            EnableMenuItem(hPopMenu, IDM_VIEWINFO, MF_GRAYED);
            EnableMenuItem(hPopMenu, IDM_OPENLOCATION, MF_GRAYED);
        }
        TrackPopupMenu(hPopMenu, TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
    }
}

void MainMsg::WMSHOWTASK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //wParam接收的是图标的ID，而lParam接收的是鼠标的行为
    //myTrayClass.ShowTask(hWnd, wParam, lParam);
}

void MainMsg::WMHSCROLL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //如果现在是停止状态
    if (CurPlayingSong.state == SONG_INIT || CurPlayingSong.state == SONG_STOP)
        return;
    //播放进度条事件
    if ((HWND)lParam == hProgress)
    {
        if (SB_ENDSCROLL == LOWORD(wParam) || SB_THUMBPOSITION == LOWORD(wParam))
        {
            LONG lProgress = SendMessage(hProgress, TBM_GETPOS, 0, 0);  //获得位置
            CurPlayingSong.JumpTo(lProgress);                           //跳转
        }
    }
    //调节声音事件
    else if ((HWND)lParam == hSound)
    {
        iVolume = SendMessage(hSound, TBM_GETPOS, 0, 0);
        CurPlayingSong.SetVolume(iVolume);
    }
}

void MainMsg::WMDROPFILES(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // DragQueryFile原型为：
    //UINT DragQueryFile(HDROP hDrop, UINT iFile, LPTSTR lpszFile, UINT cch)
    //hDrop     是指向拖动文件结构的句柄；
    //iFile     是以0开始的被托动文件的序号，因一次可拖多个文件。此参数设为0xFFFFFFFF返回拖动文件数
    //lpszFile  是指向文件名的缓冲区；
    //cch       是文件名缓冲区的大小，即文件名的字符数。
    HDROP hDropInfo = (HDROP)wParam;
    LPSTR PszFilePathName = NULL;  //歌曲全路径(包含歌曲名)
    int index = -1;
    HWND hWndlist = GetDlgItem(hWnd, IDC_LIST);
    //得到文件个数
    UINT nNumOfFiles = DragQueryFileA(hDropInfo, 0xFFFFFFFF, NULL, 0);
    for (UINT nIndex = 0; nIndex < nNumOfFiles; nIndex++)
    {
        PszFilePathName = (LPSTR)malloc(2 * MAX_PATH);
        //得到文件名
        DragQueryFileA((HDROP)hDropInfo, nIndex, (LPSTR)PszFilePathName, _MAX_PATH);
        //将歌曲加入列表
        char* pszMusicName = strrchr((char*)PszFilePathName, '\\');
        index = (int)SendMessageA(hWndlist, LB_INSERTSTRING, WPARAM(-1), (LPARAM)(pszMusicName + 1));
        SendMessageA(hWndlist, LB_SETITEMDATA, WPARAM(index), (LPARAM)PszFilePathName);
    }
    //播放列表
    //PlayMusicInList(hWnd, index);
    DragFinish(hDropInfo);
}

void MainMsg::GDIPaint(HWND DrawhWnd)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = BeginPaint(DrawhWnd, &ps);
    int w = rect.right - rect.left, h = rect.bottom - rect.top;
    // 创建画刷
    HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
    // 画刷选择到当前DC中
    HBRUSH orgBrs = (HBRUSH)SelectObject(ps.hdc, hb);
    // 填充矩形图形区为黑色背景
    Rectangle(ps.hdc, 0, 0, w, h);
    // 绘制中心播放器三角
    POINT aptTriangle[4] = { { (w /= 2) - 10, (h /= 2) - 15 }, { w - 10, h + 15 }, { w + 10, h }, { w - 10, h - 15 } };
    int num[1] = { 4 };
    HBRUSH hbTriangle = CreateSolidBrush(RGB(250, 250, 250));
    SelectObject(hDrawDC, hbTriangle);
    PolyPolygon(hDrawDC, aptTriangle, num, 1);
    // 选回原先的画刷
    SelectObject(ps.hdc, orgBrs);
    //清理对象
    DeleteObject(hb);
    DeleteObject(orgBrs);
    DeleteDC(hDrawDC);
    EndPaint(DrawhWnd, &ps);
}

void MainMsg::GDIBMPPaint(HWND DrawhWnd)
{
    PAINTSTRUCT ps;
    //创建DC和内存DC
    HDC hDrawDC = BeginPaint(DrawhWnd, &ps), memDc = CreateCompatibleDC(NULL);
    //加载图片
    HBITMAP hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BMPLOGO));
    //图片放入内存DC
    SelectObject(memDc, hBmp);
    //设置在指定设备内容中的伸展模式。解决失真问题。
    SetStretchBltMode(hDrawDC, COLORONCOLOR);
    //获得图片尺寸
    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);
    //内存DC拷贝到Dc
    StretchBlt(hDrawDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
               memDc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    DeleteObject(hBmp);
    DeleteDC(memDc);
    DeleteDC(hDrawDC);
    EndPaint(DrawhWnd, &ps);
}

void MainMsg::addlist(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //测试代码 后面删除
    HWND hWndList = GetDlgItem(hWnd, IDC_LIST);
    TCHAR szBuf[10];
    for (int k = 1; k < 10; ++k)
    {
        _itot(k * 100, szBuf, 10);
        SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)szBuf);
    }
    SendMessage(hWndList, LB_SETCURSEL, 0, 0);  // 设置默认选中项
}

void MainMsg::WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //有BUG
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
    //opengl绘图代码
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






