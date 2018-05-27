

#ifndef __VLCWRAPPERIMPL_H__
#define __VLCWRAPPERIMPL_H__

#include "VLCWrapper.h"

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_media_t;
struct libvlc_event_manager_t;

class VLCWrapperImpl
{
    libvlc_instance_t*       pVLCInstance_;        ///< The VLC instance.
    libvlc_media_player_t*   pMediaPlayer_;        ///< The VLC media player object.
    libvlc_media_t*          pMedia_;              ///< The media played by the media player.
    libvlc_event_manager_t*  pEventManager_;       ///< The event manger for the loaded media file.    
    VLCEventHandler          eventHandler;         ///< An event handler for the media player.    

public:
    VLCWrapperImpl(void);
    ~VLCWrapperImpl(void);
    void SetOutputWindow(void* pHwnd);
    void SetEventHandler(VLCEventHandler event, void* pUserData);
    void Play();
    void Pause();
    void Stop();
    int64_t GetLength();
    int64_t GetTime();
    void SetTime(int64_t newTime);
    void Mute(bool mute = true);
    bool GetMute();
    int  GetVolume();
    void SetVolume(int volume);
    void OpenMedia(const char* pMediaPathName);
};

#endif 


