// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MKUI_AsyncAction_PushSoftWidget.generated.h"

class UMKUI_W_ActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UMKUI_W_ActivatableBase*, pushedWidget);

/**
 * 
 */
UCLASS()
class MK_UI_API UMKUI_AsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, meta=(WorldContext="wco", HidePin="wco", BlueprintInternalUseOnly="true", DisplayName="Push Soft Widget to Stack"))
    static UMKUI_AsyncAction_PushSoftWidget* pushSoftWidget(const UObject* wco,
                                                            APlayerController* owningPlayerController,
                                                            TSoftClassPtr<UMKUI_W_ActivatableBase> widgetClass,
                                                            UPARAM(meta=(Categories="MKUI.widgetStack")) FGameplayTag widgetStackTag,
                                                            bool bFocusOnNewlyPushedWidget = true);

    virtual void Activate() override;
    
public:

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
