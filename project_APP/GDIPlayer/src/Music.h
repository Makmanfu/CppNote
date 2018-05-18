//-----------------------------------------------------------------------------
//
//         �򵥲�����ģ��
//
//-----------------------------------------------------------------------------

#ifndef MUSIC_H
#define MUSIC_H

#include <commdlg.h>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib,"winmm")

//����״̬
enum PLAYSTATE { SONG_INIT, SONG_PLAYING, SONG_PAUSE, SONG_STOP };
//����ģʽ ����һ��,����ѭ��,�������,ȫ��һ��,ȫ��ѭ��
enum PLAYMODE { ONCE, REPATE, RANDOM, ONCEALL, REPATEALL };

//��Ǹ�����Ϣ�Ľṹ��
typedef struct _MP3_ID3v1_STRUCT
{
    char Header[4];     //��ǩͷ������"TAG"������Ϊû�б�ǩ
    char Title[31];     //����
    char Artist[31];    //����
    char Album[31];     //ר��
    char Year[5];       //��Ʒ���
    char Comment[29];   //��ע
    char reserve;       //����
    char track;;        //����
    char Genre;         //����
} MP3_ID3v1_STRUCT, * pMP3_ID3v1_STRUCT;

//������C++��
class Song : public noncopyable
{
private:
    int typeID;                         //�ļ�����
    char szShortFileDir[MAX_PATH];      //ȫ·���Ķ�·����ʽ

    //----------��Ƶ������Ҫ�ṹ-------------
    MCI_DGV_OPEN_PARMSA mciOpen;        //MCI�򿪲����ṹ��
    MCI_DGV_STATUS_PARMSA  mciStatus;   //MCI״̬�����ṹ��
    UINT uDeviceID;                     //�豸ID
    RECT rClient;                       //�����ڿͻ�����С
    HWND hVideo;                        //���ž��

public:
    string sName;                       //�ļ���
    string sFileDir;                    //ȫ·��
    long iLength;                       //��������
    PLAYSTATE state;                    //����״̬
private:
    char cmd[MAX_PATH + 10];            //ִ������
public:
    Song();
    Song(string _mp3PathName, HWND hParent);
    ~Song();
    void SetSong(string _mp3PathName, HWND hParent);
public:
    //�������ֿ���[SONG_PLAYING����, SONG_PAUSE��ͣ, SONG_STOPֹͣ ]
    void Play(PLAYSTATE setState);
    //���þ���
    void SetSound(bool bok);
    //��������
    void SetVolume(int iVolume);
    //����ָ��λ��
    void JumpTo(LONG lPos);
    //��õ�ǰ����λ��
    long GetCurPlayingPos();
    //��ȡ������Ϣ�ĺ���
    BOOL GetMp3Info(char* szFilePath, pMP3_ID3v1_STRUCT pMp3Info);
private:
    //�ڲ�״̬���
    inline bool CheckMyState(void);
    //��ø�������
    inline bool GetSongLen(long& llen);
};

VOID openFile(HWND);
VOID openFileDialog(HWND);


#endif