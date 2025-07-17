// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MKUI_AsyncAction_PushSoftWidget.generated.h"

class UMKUI_W_ActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UMKUI_W_ActivatableBase*, pushedWidget);

/**
 * An async action which task is to push a widget onto a specific UI stack.
 * The pushSoftWidget(...) is a static factory method that creates the async action and returns a pointer to it.
 * This method also responsible for UE to generate a corresponding BPNode, where the declared multicast delegates
 * represent the pins of this node in BP.
 * The output variables are the multicast delegate's parameters and exec pins are the delegates declared
 * as members inside the class.
 * The Activate() method is automatically called by UE's latent action execution pipeline immediately after the BP
 * node is created via its factory method pushSoftWidget(...).
 */
UCLASS()
class MK_UI_API UMKUI_AsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    // factory function for creating the async action instance and returning it
    UFUNCTION(BlueprintCallable, meta=(WorldContext="wco", HidePin="wco", BlueprintInternalUseOnly="true", DisplayName="Push Soft Widget to Stack"))
    static UMKUI_AsyncAction_PushSoftWidget* pushSoftWidget(const UObject* wco,
                                                            APlayerController* owningPlayerController,
                                                            TSoftClassPtr<UMKUI_W_ActivatableBase> widgetClass,
                                                            UPARAM(meta=(Categories="MKUI.widgetStack")) FGameplayTag widgetStackTag,
                                                            bool bFocusOnNewlyPushedWidget = true);

    // implementing the task of this async action
    virtual void Activate() override;
    
public:
    /**
     * Multicast delegates that act as output pins of the created BP node
     */
    UPROPERTY(BlueprintAssignable)
    FOnPushSoftWidgetDelegate onWidgetCreatedBeforePush;

    UPROPERTY(BlueprintAssignable)
    FOnPushSoftWidgetDelegate afterPush;

private:
    TWeakObjectPtr<APlayerController> mCachedOwningPC;
    TWeakObjectPtr<UWorld> mCachedOwningWorld;
    TSoftClassPtr<UMKUI_W_ActivatableBase> mCachedWidgetClass;
    FGameplayTag mCachedGameplayTag;
    bool mbCachedFocusOnNewlyPushedWidget = false;
};
