
#include "stdafx.h"
#include "VLCWrapper.h"
#include "VLCWrapperImpl.h"

VLCWrapper::VLCWrapper(void)
:   pImpl_(std::auto_ptr<VLCWrapperImpl>(new VLCWrapperImpl()))
{
}

VLCWrapper::~VLCWrapper(void)
{
}

void VLCWrapper::SetOutputWindow(void* pHwnd)
{
    pImpl_->SetOutputWindow(pHwnd);
}

void VLCWrapper::SetEventHandler(VLCEventHandler event, void* pUserData)
{
    pImpl_->SetEventHandler(event, pUserData);
}

void VLCWrapper::Play()
{
    pImpl_->Play();
}

void VLCWrapper::Pause()
{
    pImpl_->Pause();
}

void VLCWrapper::Stop()
{
    pImpl_->Stop();
}

int64_t VLCWrapper::GetLength()
{
    return pImpl_->GetLength();
}

int64_t VLCWrapper::GetTime()
{
    return pImpl_->GetTime();
}

void VLCWrapper::SetTime(int64_t newTime)
{
    pImpl_->SetTime(newTime);
}

void VLCWrapper::Mute(bool mute)
{
    pImpl_->Mute(mute);
}

bool VLCWrapper::GetMute()
{
    return pImpl_->GetMute();
}

int  VLCWrapper::GetVolume()
{
    return pImpl_->GetVolume();
}

void VLCWrapper::SetVolume(int volume)
{
    pImpl_->SetVolume(volume);
}

void VLCWrapper::OpenMedia(const char* pMediaPathName)
{
    pImpl_->OpenMedia(pMediaPathName);
}