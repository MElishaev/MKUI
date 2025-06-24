// MAAKU Studio all rights reserved


#include "Settings/MKUI_GameUserSettings.h"

UMKUI_GameUserSettings* UMKUI_GameUserSettings::getInstance()
{
    if (GEngine) {
        return CastChecked<UMKUI_GameUserSettings>(GEngine->GetGameUserSettings());
    }
    return nullptr;
}
