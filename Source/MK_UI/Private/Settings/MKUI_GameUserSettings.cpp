// MAAKU Studio all rights reserved


#include "Settings/MKUI_GameUserSettings.h"

UMKUI_GameUserSettings::UMKUI_GameUserSettings() : mOverallVolume(1.f)
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
