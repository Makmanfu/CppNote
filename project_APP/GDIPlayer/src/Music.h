//-----------------------------------------------------------------------------
//
//         简单播放器模块
//
//-----------------------------------------------------------------------------

#ifndef MUSIC_H
#define MUSIC_H

#include <commdlg.h>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib,"winmm")

//播放状态
enum PLAYSTATE { SONG_INIT, SONG_PLAYING, SONG_PAUSE, SONG_STOP };
//播放模式 单曲一次,单曲循环,随机播放,全部一次,全部循环
enum PLAYMODE { ONCE, REPATE, RANDOM, ONCEALL, REPATEALL };

//标记歌曲信息的结构体
typedef struct _MP3_ID3v1_STRUCT
{
    char Header[4];     //标签头必须是"TAG"否则认为没有标签
    char Title[31];     //标题
    char Artist[31];    //作者
    char Album[31];     //专集
    char Year[5];       //出品年代
    char Comment[29];   //备注
    char reserve;       //保留
    char track;;        //音轨
    char Genre;         //类型
} MP3_ID3v1_STRUCT, * pMP3_ID3v1_STRUCT;

//歌曲类C++版
class Song : public noncopyable
{
private:
    int typeID;                         //文件类型
    char szShortFileDir[MAX_PATH];      //全路径的短路径形式

    //----------视频播放需要结构-------------
    MCI_DGV_OPEN_PARMSA mciOpen;        //MCI打开参数结构体
    MCI_DGV_STATUS_PARMSA  mciStatus;   //MCI状态参数结构体
    UINT uDeviceID;                     //设备ID
    RECT rClient;                       //父窗口客户区大小
    HWND hVideo;                        //播放句柄

public:
    string sName;                       //文件名
    string sFileDir;                    //全路径
    long iLength;                       //歌曲长度
    PLAYSTATE state;                    //播放状态
private:
    char cmd[MAX_PATH + 10];            //执行命令
public:
    Song();
    Song(string _mp3PathName, HWND hParent);
    ~Song();
    void SetSong(string _mp3PathName, HWND hParent);
public:
    //播放音乐控制[SONG_PLAYING播放, SONG_PAUSE暂停, SONG_STOP停止 ]
    void Play(PLAYSTATE setState);
    //设置静音
    void SetSound(bool bok);
    //设置声音
    void SetVolume(int iVolume);
    //跳到指定位置
    void JumpTo(LONG lPos);
    //获得当前播放位置
    long GetCurPlayingPos();
    //获取歌曲信息的函数
    BOOL GetMp3Info(char* szFilePath, pMP3_ID3v1_STRUCT pMp3Info);
private:
    //内部状态检查
    inline bool CheckMyState(void);
    //获得歌曲长度
    inline bool GetSongLen(long& llen);
};

VOID openFile(HWND);
VOID openFileDialog(HWND);


#endif