
#include "stdafx.h"
#include "Music.h"

Music::Music() : typeID(0)
    , iLength(0)
    , state(SONG_INIT)
    , hVideo(NULL)
    , iPlayMode(REPATEALL)   //默认 全部循环
    , PlayingIndex(-1)       //当前播放索引
{
    //视频播放结构初始化
    mciStatus.dwItem = MCI_DGV_STATUS_HWND;
    //初始化mciOpen以及打开avi文件
    ZeroMemory(&mciOpen, sizeof(mciOpen));
    mciOpen.lpstrAlias = "movie";               //设备别名
    mciOpen.dwStyle = WS_CHILD;                 //窗口风格
}

Music::Music(string _mp3PathName, HWND hParent) : Music()
{
    SetSong(_mp3PathName, hParent);
}

Music::~Music()
{
}

void Music::SetSong(string _mp3PathName, HWND hParent)
{
    //先执行一次停止
    Play(SONG_STOP);
    //得到路径全名
    sName = sFileDir = _mp3PathName;
    size_t pos0 = sName.find_last_of("\\");
    //得到短路径(主要是解决路径空格问题)
    GetShortPathNameA(sFileDir.c_str(), szShortFileDir, sizeof(szShortFileDir));
    //得到文件名
    sName = sName.substr(pos0 + 1, sName.size() - pos0 - 1);
    //得到扩展名
    string mp3str = sName.substr(sName.size() - 3);
    if (0 == mp3str.compare("mp3"))
        typeID = 0;
    else if (0 == mp3str.compare("wmv") || 0 == mp3str.compare("avi"))
        typeID = 1;
    //获得长度
    if (GetSongLen(iLength))
        return;
    if (1 == typeID)
    {
        typeID = 1;
        mciOpen.dwCallback = (DWORD) hParent;
        mciOpen.lpstrElementName = szShortFileDir;  //avi文件地址
        mciOpen.hWndParent = hParent;               //父窗口
        mciSendCommandA(0, MCI_OPEN, MCI_OPEN_ALIAS | MCI_OPEN_ELEMENT |
                        MCI_DGV_OPEN_PARENT | MCI_DGV_OPEN_WS, (DWORD) &mciOpen);
        uDeviceID = mciOpen.wDeviceID;
        mciSendCommandA(uDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mciStatus);
        //获取播放视频的窗口句柄
        hVideo = (HWND) mciStatus.dwReturn;
        //设置播放区域大小
        GetClientRect(GetDlgItem(hParent, IDC_SHOW), &rClient);
        //GetClientRect(hParent, &rClient);
        MoveWindow(hVideo, 0, 0, rClient.right - rClient.left, rClient.bottom - rClient.top, TRUE);
        InvalidateRect(hVideo, NULL, TRUE);
        Play(SONG_PLAYING);
    }
}

void Music::Play(PLAYSTATE setState)
{
    if (CheckMyState())
        return;
    switch (setState)
    {
        case SONG_PLAYING:
        {
            if (0 == typeID)
                state = (0 == mciSendStringA((string("play ") + szShortFileDir).c_str(), "", 0, NULL)) ? SONG_PLAYING : SONG_INIT;
            else
                state = (0 == mciSendStringA("Play movie repeat", NULL, 0, 0)) ? SONG_PLAYING : SONG_INIT;
        }
        break;
        case SONG_PAUSE:
        {
            if (0 == typeID)
                state = (0 == mciSendStringA((string("pause ") + szShortFileDir).c_str(), "", 0, NULL)) ? SONG_PAUSE : SONG_INIT;
            else
                state = (0 == mciSendStringA("Pause movie", NULL, 0, 0)) ? SONG_PAUSE : SONG_INIT;
        }
        break;
        case SONG_STOP:
        {
            if (0 == typeID)
                state = (0 == mciSendStringA((string("stop ") + szShortFileDir).c_str(), "", 0, NULL)) ? SONG_STOP : SONG_INIT;
            else
                state = (0 == mciSendStringA("Close movie", NULL, 0, 0)) ? SONG_STOP : SONG_INIT;
        }
        break;
        case SONG_INIT:
        default:
        {
            if (0 == typeID)
                state = (0 == mciSendStringA((string("close ") + szShortFileDir).c_str(), "", 0, NULL)) ? SONG_STOP : SONG_INIT;
            else
                state = (0 == mciSendStringA("Close movie", NULL, 0, 0)) ? SONG_STOP : SONG_INIT;
        }
        break;
    }
}

void Music::SetSound(bool bok)
{
    if (CheckMyState())
        return;
    wsprintfA(cmd, bok ? " setaudio %s off" : " setaudio %s on", szShortFileDir);
    mciSendStringA(cmd, (LPSTR)NULL, 0, NULL);
}

void Music::SetVolume(int iVolume)
{
    if (CheckMyState())
        return;
    if (0 == typeID)
        wsprintfA(cmd, "setaudio %s volume to %d", szShortFileDir, iVolume);
    else
        wsprintfA(cmd, "setaudio movie volume to %ld", iVolume);
    mciSendStringA(cmd, "", 0, NULL);
}

void Music::JumpTo(LONG lPos)
{
    if (CheckMyState())
        return;
    if (0 == typeID)
        wsprintfA(cmd, "play %s from %ld", szShortFileDir, lPos);
    else
        wsprintfA(cmd, "play movie FROM %ld to %ld", lPos, iLength);
    //mciSendStringA ( cmd, NULL, 0, 0 );
    mciSendStringA(cmd, NULL, 0, 0);    //为什么要两次？？？
    //if ( state == SONG_PAUSE )          //如果之前歌曲是暂停的
    //    Play ( SONG_PAUSE );
}

bool Music::GetSongLen(long& llen)
{
    //得到歌曲的总时间
    char PlaySongLength[100];
    string commd = string("status ");
    //if (0 == typeID)
    commd += string(szShortFileDir) + " length";
    //else
    //    commd += string("movie length");
    //获得歌曲的长度
    state = (0 == mciSendStringA(commd.c_str(), PlaySongLength, sizeof(PlaySongLength), 0)) ? SONG_STOP : SONG_INIT;
    //得到的是歌曲的微秒数
    llen = atol(PlaySongLength);
    //检查歌曲打开是否正常
    return CheckMyState();
}

long Music::GetCurPlayingPos()
{
    if (CheckMyState())
        return 0;
    char pos[256];
    //得到当前播放的位置（时间）
    wsprintfA(cmd, "status %s position", szShortFileDir);
    mciSendStringA(cmd, pos, sizeof(pos), NULL);
    return atol(pos);
}

bool Music::CheckMyState(void)
{
    //如果什么连歌名都没有就报错了
    return (state == SONG_INIT);
}

BOOL Music::GetMp3Info(char* szFilePath, pMP3_ID3v1_STRUCT pMp3Info)
{
    //memset(&SongInfo, 0, sizeof(SongInfo)); //SongInfo初始化
    return TRUE;
}




