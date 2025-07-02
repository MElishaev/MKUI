// MAAKU Studio all rights reserved


#include "Settings/MKUI_GameUserSettings.h"

UMKUI_GameUserSettings::UMKUI_GameUserSettings() :
    mOverallVolume(0.5f),
    mMusicVolume(0.5f), mSFXVolume(0.5f), mbAllowBackgroundAudio(true), mbUseHDRAudioMode(false)
{

}

UMKUI_GameUserSettings* UMKUI_GameUserSettings::getInstance()
{
    if (GEngine) {
        return CastChecked<UMKUI_GameUserSettings>(GEngine->GetGameUserSettings());
    }
    return nullptr;
}

void UMKUI_GameUserSettings::setOverallVolume(float vol)
{
    mOverallVolume = vol;

    // todo: the actual logic to control the volume in game goes here
}

void UMKUI_GameUserSettings::setMusicVolume(float vol)
{
    mMusicVolume = vol;
    // todo: the actual logic to control the volume in game goes here
}

void UMKUI_GameUserSettings::setSFXVolume(float vol)
{
    mSFXVolume = vol;
    // todo: the actual logic to control the volume in game goes here
}

void UMKUI_GameUserSettings::setAllowBackgroundAudio(bool bIsAllowed)
{
    mbAllowBackgroundAudio = bIsAllowed;
    // todo: the actual logic to control the volume in game goes here
}

void UMKUI_GameUserSettings::setUseHDRAudioMode(bool bIsAllowed)
{
    mbUseHDRAudioMode = bIsAllowed;
    // todo: the actual logic to control the volume in game goes here
}
