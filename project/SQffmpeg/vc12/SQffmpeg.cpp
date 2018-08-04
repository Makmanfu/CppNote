

#include "stdafx.h"
#include "CToolClass.hpp"
#include "ffmpeg_helloworld.hpp"
#include "ffmpeg_player.hpp"
#include "ffmpeg_player_su.hpp"
#include "ffmpeg_decoder.hpp"
#include "ffmpeg_decoder_pure.hpp"
#include "ffmpeg_video_play_sdl2.hpp"
#include "ffmpeg_audio_play_sdl2.hpp"
#include "ffmpeg_audio_decoder.hpp"
#include "ffmpeg_audio_player.hpp"
#include "ffmpeg_picture_encoder.hpp"


//exe����ں���
int main(int argc, char** argv)
{
    CMDProc fp = NULL, CmdMain = NULL, DefaultFun = &ffmpeg_player_su::start;
    CCMDParam cmds(argv[0], DefaultFun);               //����������
    cmds.ComandAdd("ffmpeg_helloworld", &ffmpeg_helloworld::start);
    cmds.ComandAdd("ffmpeg_player", &ffmpeg_player::start);
    cmds.ComandAdd("ffmpeg_player_su", &ffmpeg_player_su::start);
    cmds.ComandAdd("ffmpeg_decoder", &ffmpeg_decoder::start);
    cmds.ComandAdd("ffmpeg_decoder_pure", &ffmpeg_decoder_pure::start);
    cmds.ComandAdd("ffmpeg_video_play_sdl2", &ffmpeg_video_play_sdl2::start);
    cmds.ComandAdd("ffmpeg_audio_play_sdl2", &ffmpeg_audio_play_sdl2::start);
    cmds.ComandAdd("ffmpeg_audio_decoder", &ffmpeg_audio_decoder::start);
    cmds.ComandAdd("ffmpeg_audio_player", &ffmpeg_audio_player::start);
    cmds.ComandAdd("ffmpeg_picture_encoder", &ffmpeg_picture_encoder::start);

    string strcmd = argc > 1 ? argv[1] : "default";    //default  help
    cmds.GetCmdFunProc(strcmd, &fp);
    if (fp != NULL)
        fp(argc, argv);
    cmds.Loop();
    return 0;
}

//���ڽ������
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
//{
//    return 0;
//}

//dll����ں���
bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:        //�����DLL��ӳ�䵽�˽��̵ĵ�ַ�ռ�ʱ
        case DLL_THREAD_ATTACH:         //һ���߳����ڱ�����
        case DLL_PROCESS_DETACH:        //���DLL�ӽ��̵ĵ�ַ�ռ��н��ӳ��
            break;
        case DLL_THREAD_DETACH:         //�߳��ս�
            break;
    }
    return true;
}




