// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MKUI_FunctionLibrary.generated.h"

class UMKUI_W_ActivatableBase;
/**
 * 
 */
UCLASS()
class MK_UI_API UMKUI_FunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Searches a widget mapped to the input tag in the project settings where you can map a tag to a widget
     * and then use this to decide what UI will be shown in different frontend screens.
     * @param widgetTag widget tag to search for in the map that is located in the project settings of this plugin
     * @return soft class ptr for the widget
     */ // todo change name to something like getWidgetClassFromSettingsByTag
    UFUNCTION(BlueprintPure, Category="MKUI Function Library")
    static TSoftClassPtr<UMKUI_W_ActivatableBase> getSoftWidgetClassByTag(UPARAM(meta=(Categories="MKUI.widget")) FGameplayTag widgetTag);

    UFUNCTION(BlueprintPure, Category="MKUI Function Library")
    static TSoftObjectPtr<UTexture2D> getOptionsSoftImageByTag(UPARAM(meta=(Categories="MKUI.image")) FGameplayTag imgTag);
};
