/**
    ��򵥵�SDL2������Ƶ�����ӣ�SDL2����RGB/YUV��

    ������ʹ��SDL2����RGB/YUV��Ƶ�������ݡ�SDLʵ�����ǶԵײ��ͼ
    API��Direct3D��OpenGL���ķ�װ��ʹ���������Լ���ֱ�ӵ��õײ�
    API��

    �������ò�������:

    [��ʼ��]
    SDL_Init(): ��ʼ��SDL��
    SDL_CreateWindow(): �������ڣ�Window����
    SDL_CreateRenderer(): ���ڴ��ڴ�����Ⱦ����Render����
    SDL_CreateTexture(): ��������Texture����

    [ѭ����Ⱦ����]
    SDL_UpdateTexture(): ������������ݡ�
    SDL_RenderCopy(): �����Ƹ���Ⱦ����
    SDL_RenderPresent(): ��ʾ��
*/

#pragma once
#include "stdafx.h"

const int bpp = 12;

int screen_w = 500, screen_h = 500;
const int pixel_w = 320, pixel_h = 180;

unsigned char buffer[pixel_w * pixel_h * bpp / 8];

//Refresh Event
#define REFRESH_EVENT  (SDL_USEREVENT + 1)

#define BREAK_EVENT  (SDL_USEREVENT + 2)

int sdl_thread_exit = 0;

int refresh_video(void* opaque)
{
    sdl_thread_exit = 0;
    while (!sdl_thread_exit)
    {
        SDL_Event event;
        event.type = REFRESH_EVENT;
        SDL_PushEvent(&event);
        SDL_Delay(40);
    }
    sdl_thread_exit = 0;
    //Break
    SDL_Event event;
    event.type = BREAK_EVENT;
    SDL_PushEvent(&event);
    return 0;
}

struct ffmpeg_video_play_sdl2
{
    static int start(int argc, char** argv)
    {
        if (SDL_Init(SDL_INIT_VIDEO))
        {
            printf("Could not initialize SDL - %s\n", SDL_GetError());
            return -1;
        }
        SDL_Window* screen;
        //SDL 2.0 Support for multiple windows
        screen = SDL_CreateWindow("Simplest Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            screen_w, screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!screen)
        {
            printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
            return -1;
        }
        SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
        Uint32 pixformat = 0;
        //IYUV: Y + U + V  (3 planes)
        //YV12: Y + V + U  (3 planes)
        pixformat = SDL_PIXELFORMAT_IYUV;
        SDL_Texture* sdlTexture = SDL_CreateTexture(sdlRenderer, pixformat, SDL_TEXTUREACCESS_STREAMING, pixel_w, pixel_h);
        FILE* fp = NULL;
        fp = fopen("test_yuv420p_320x180.yuv", "rb+");
        if (fp == NULL)
        {
            printf("cannot open this file\n");
            return -1;
        }
        SDL_Rect sdlRect;
        SDL_Thread* refresh_thread = SDL_CreateThread(refresh_video, NULL, NULL);
        SDL_Event event;
        while (1)
        {
            //Wait
            SDL_WaitEvent(&event);
            if (event.type == REFRESH_EVENT)
            {
                if (fread(buffer, 1, pixel_w * pixel_h * bpp / 8, fp) != pixel_w * pixel_h * bpp / 8)
                {
                    // Loop
                    fseek(fp, 0, SEEK_SET);
                    fread(buffer, 1, pixel_w * pixel_h * bpp / 8, fp);
                }
                SDL_UpdateTexture(sdlTexture, NULL, buffer, pixel_w);
                //FIX: If window is resize
                sdlRect.x = 0;
                sdlRect.y = 0;
                sdlRect.w = screen_w;
                sdlRect.h = screen_h;
                SDL_RenderClear(sdlRenderer);
                SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
                SDL_RenderPresent(sdlRenderer);
            }
            else if (event.type == SDL_WINDOWEVENT)
            {
                //If Resize
                SDL_GetWindowSize(screen, &screen_w, &screen_h);
            }
            else if (event.type == SDL_QUIT)
                sdl_thread_exit = 1;
            else if (event.type == BREAK_EVENT)
                break;
        }
        SDL_Quit();
        return 0;
    };
};


