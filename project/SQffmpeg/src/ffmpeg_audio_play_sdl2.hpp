/**
    最简单的SDL2播放音频的例子（SDL2播放PCM）

    本程序使用SDL2播放PCM音频采样数据。SDL实际上是对底层绘图
    API（Direct3D，OpenGL）的封装，使用起来明显简单于直接调用底层
    API。

    函数调用步骤如下:

    [初始化]
    SDL_Init(): 初始化SDL。
    SDL_OpenAudio(): 根据参数（存储于SDL_AudioSpec）打开音频设备。
    [循环播放数据]
    SDL_PauseAudio(): 播放音频数据。
    SDL_Delay(): 延时等待播放完成。
*/

#pragma once
#include "stdafx.h"

//Buffer:
//|-----------|-------------|
//chunk-------pos---len-----|
static  Uint8*  sdl_audio_chunk;
static  Uint32  sdl_audio_len;
static  Uint8*  sdl_audio_pos;

struct ffmpeg_audio_play_sdl2
{


    /*  Audio Callback
    The audio function callback takes the following parameters:
    stream: A pointer to the audio buffer to be filled
    len: The length (in bytes) of the audio buffer
    */
    static void fill_audio(void* udata, Uint8* stream, int len)
    {
        //SDL 2.0
        SDL_memset(stream, 0, len);
        if (sdl_audio_len == 0)     /*  Only  play  if  we  have  data  left  */
            return;
        len = (len > sdl_audio_len ? sdl_audio_len : len); /*  Mix  as  much  data  as  possible  */
        SDL_MixAudio(stream, sdl_audio_pos, len, SDL_MIX_MAXVOLUME);
        sdl_audio_pos += len;
        sdl_audio_len -= len;
    }

    static int start(int argc, char** argv)
    {
        //Init
        if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER))
        {
            printf("Could not initialize SDL - %s\n", SDL_GetError());
            return -1;
        }
        //SDL_AudioSpec
        SDL_AudioSpec wanted_spec;
        wanted_spec.freq = 44100;
        wanted_spec.format = AUDIO_S16SYS;
        wanted_spec.channels = 2;
        wanted_spec.silence = 0;
        wanted_spec.samples = 1024;
        wanted_spec.callback = fill_audio;
        if (SDL_OpenAudio(&wanted_spec, NULL) < 0)
        {
            printf("can't open audio.\n");
            return -1;
        }
        FILE* fp = fopen("NocturneNo2inEflat_44.1k_s16le.pcm", "rb+");
        if (fp == NULL)
        {
            printf("cannot open this file\n");
            return -1;
        }
        //For YUV420P
        int pcm_buffer_size = 4096;
        char* pcm_buffer = (char*)malloc(pcm_buffer_size);
        int data_count = 0;
        while (1)
        {
            if (fread(pcm_buffer, 1, pcm_buffer_size, fp) != pcm_buffer_size)
            {
                // Loop
                fseek(fp, 0, SEEK_SET);
                fread(pcm_buffer, 1, pcm_buffer_size, fp);
                data_count = 0;
            }
            printf("Now Playing %10d Bytes data.\n", data_count);
            data_count += pcm_buffer_size;
            //Set audio buffer (PCM data)
            sdl_audio_chunk = (Uint8*)pcm_buffer;
            //Audio buffer length
            sdl_audio_len = pcm_buffer_size;
            sdl_audio_pos = sdl_audio_chunk;
            //Play
            SDL_PauseAudio(0);
            while (sdl_audio_len > 0) //Wait until finish
                SDL_Delay(1);
        }
        free(pcm_buffer);
        SDL_Quit();
        return 0;
    };
};

