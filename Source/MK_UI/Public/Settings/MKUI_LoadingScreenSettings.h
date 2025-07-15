// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MKUI_LoadingScreenSettings.generated.h"

/**
 * This class represents the settings for loading screen.
 * This is exposed in the project settings under the "Game" section inside "MKUI Loading Screen Settings"  
 */
UCLASS(Config=Game, DefaultConfig)
class MK_UI_API UMKUI_LoadingScreenSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    // this class will load the soft ptr of loading screen widget and return it to the caller
    TSubclassOf<UUserWidget> getLoadingScreenWidgetClassChecked() const;

    
    UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
    TSoftClassPtr<UUserWidget> mSoftLoadingScreenWidgetClass; // the widget we want to show as the loading screen

    // amount of additional seconds to hold loading screen after loading complete (for texture streaming - not showing blurry texture hiccup)
    UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
    float mSecsToHoldLoadingScreenAfterLoad = 3.f;

    UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
    bool mbShowLoadingScreenInEditor = false;

};
