// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "MKUI_DeveloperSettings.generated.h"

class UMKUI_W_ActivatableBase;
class UTexture2D;

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="UI Settings"))
class MK_UI_API UMKUI_DeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
    UPROPERTY(Config, EditAnywhere, Category="Widget Reference", meta=(ForceInlineRow, Categories="MKUI.widget"))
    TMap<FGameplayTag, TSoftClassPtr<UMKUI_W_ActivatableBase>> mWidgetMap;

    UPROPERTY(Config, EditAnywhere, Category="MKUI Options Image Reference", meta=(ForceInlineRow, Categories="MKUI.image.testImage"))
    TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> mOptionsScreenSoftImageMap;

};
