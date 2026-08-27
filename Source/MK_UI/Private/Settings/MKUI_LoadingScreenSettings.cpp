// MAAKU Studio all rights reserved

#include "Settings/MKUI_LoadingScreenSettings.h"

#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UMKUI_LoadingScreenSettings::getLoadingScreenWidgetClass() const
{
    return mSoftLoadingScreenWidgetClass.LoadSynchronous();
}
