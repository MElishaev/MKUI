// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "MKUI_W_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

/**
 * This class acts as a container that holds all the other widgets we use
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_W_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    UCommonActivatableWidgetContainerBase* findWidgetStackByTag(const FGameplayTag& tag) const;
    
protected:
    UFUNCTION(BlueprintCallable)
    void registerWidgetStack(UPARAM(meta=(Categories="MKUI.widgetStack"))const FGameplayTag tag, UCommonActivatableWidgetContainerBase* stack);
    
private:
    UPROPERTY(Transient)
    TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> mRegisteredWidgetStackMap;
};
