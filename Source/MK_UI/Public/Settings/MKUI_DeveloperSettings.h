// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "MKUI_DeveloperSettings.generated.h"

class UInputMappingContext;
class UMKUI_W_ActivatableBase;
class UMKUI_W_PrimaryLayout;
class UTexture2D;

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="UI Settings"))
class MK_UI_API UMKUI_DeveloperSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    /** Root layout created once for each local player. Leave unset only for the deprecated controller-created path. */
    UPROPERTY(Config, EditAnywhere, Category = "Layout")
    TSoftClassPtr<UMKUI_W_PrimaryLayout> mPrimaryLayoutClass;

    /** Generic input mapping used for UI navigation and button actions. */
    UPROPERTY(Config, EditAnywhere, Category = "Input")
    TSoftObjectPtr<UInputMappingContext> mInputMappingContext;
    
    UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta=(ForceInlineRow, Categories="MKUI.widget"))
    TMap<FGameplayTag, TSoftClassPtr<UMKUI_W_ActivatableBase>> mWidgetMap;

    UPROPERTY(Config, EditAnywhere, Category = "MKUI Options Image Reference", meta=(ForceInlineRow, Categories="MKUI.image.testImage"))
    TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> mOptionsScreenSoftImageMap;

    UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
    FSoftObjectPath mMasterSoundClass;
 
    UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
    FSoftObjectPath mMusicSoundClass;
 
    UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundClass"))
    FSoftObjectPath mSoundFXSoundClass;
 
    UPROPERTY(Config, EditAnywhere, Category = "Audio", meta = (AllowedClasses = "/Script/Engine.SoundMix"))
    FSoftObjectPath mDefaultSoundMix;
};
