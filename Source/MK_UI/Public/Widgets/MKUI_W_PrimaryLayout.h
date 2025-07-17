// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "MKUI_W_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

/**
 * This class acts as a container that holds all the other widgets we use.
 * It holds the possible stacks for different UI sections of your game, for example, it can hold the UI stacks of
 * HUD, main menu, pause menu, modals etc.
 * Every UI element should be pushed to one of the available stacks and if needed, additional stacks can be added
 * through first adding declaring and defining the stack in the MKUI_GameplayTags.h.
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_W_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    UCommonActivatableWidgetContainerBase* findWidgetStackByTag(const FGameplayTag& tag) const;
    
protected:
    UFUNCTION(BlueprintCallable, Category="MKUI")
    void registerWidgetStack(UPARAM(meta=(Categories="MKUI.widgetStack"))const FGameplayTag tag, UCommonActivatableWidgetContainerBase* stack);
    
private:
    UPROPERTY(Transient)
    TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> mRegisteredWidgetStackMap;
};
