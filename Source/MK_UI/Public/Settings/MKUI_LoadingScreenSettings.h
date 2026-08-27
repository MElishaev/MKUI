// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MKUI_LoadingScreenSettings.generated.h"

class UUserWidget;

/** Project settings for MK_UI's loading-screen coordinator. */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "MKUI Loading Screen Settings"))
class MK_UI_API UMKUI_LoadingScreenSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    TSubclassOf<UUserWidget> getLoadingScreenWidgetClass() const;

    /** Enables the loading-screen subsystem for map travel and project-owned loading tasks. */
    UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
    bool mbEnableLoadingScreen = false;

    /** Widget displayed above the viewport while loading requirements remain active. */
    UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings", meta = (EditCondition = "mbEnableLoadingScreen"))
    TSoftClassPtr<UUserWidget> mSoftLoadingScreenWidgetClass;

    /** Minimum additional display time after the world and all loading tasks become ready. */
    UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings", meta = (ClampMin = "0.0", EditCondition = "mbEnableLoadingScreen"))
    float mSecsToHoldLoadingScreenAfterLoad = 3.0f;

    /** Logs the reason for a loading task that remains active this long. Zero disables warnings. */
    UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings", meta = (ClampMin = "0.0", EditCondition = "mbEnableLoadingScreen"))
    float mLoadingTaskWarningTimeout = 30.0f;

    /** Allows loading screens to appear during Play In Editor sessions. */
    UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings", meta = (EditCondition = "mbEnableLoadingScreen"))
    bool mbShowLoadingScreenInEditor = false;
};
