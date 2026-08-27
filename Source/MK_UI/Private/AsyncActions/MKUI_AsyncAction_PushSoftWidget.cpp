// MAAKU Studio all rights reserved


#include "AsyncActions/MKUI_AsyncAction_PushSoftWidget.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Subsystems/MKUI_LocalPlayerSubsystem.h"
#include "Widgets/MKUI_W_ActivatableBase.h"

UMKUI_AsyncAction_PushSoftWidget* UMKUI_AsyncAction_PushSoftWidget::pushSoftWidget(const UObject* wco,
                                                                                   APlayerController* owningPlayerController,
                                                                                   TSoftClassPtr<UMKUI_W_ActivatableBase> widgetClass,
                                                                                   UPARAM(meta=(Categories="MKUI.widgetStack")) FGameplayTag
                                                                                   widgetStackTag,
                                                                                   bool bFocusOnNewlyPushedWidget)
{
    checkf(!widgetClass.IsNull(), TEXT("passed null widget class"));

    if (GEngine) {
        if (const auto world = GEngine->GetWorldFromContextObject(wco, EGetWorldErrorMode::LogAndReturnNull)) {
            const auto node = NewObject<UMKUI_AsyncAction_PushSoftWidget>();
            node->mCachedOwningWorld = world;
            node->mCachedOwningPC = owningPlayerController;
            node->mCachedWidgetClass = widgetClass;
            node->mbCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
            node->mCachedGameplayTag = widgetStackTag;

            node->RegisterWithGameInstance(world);
            return node;
        }
    }

    return nullptr;
}

void UMKUI_AsyncAction_PushSoftWidget::Activate()
{
    // callback for pushing the widget (to control what happens just before the push and after the push)
    auto asyncPushStateCallback = [this](EAsyncPushWidgetState pushState, UMKUI_W_ActivatableBase* pushedWidget) {
        switch (pushState) {
            case EAsyncPushWidgetState::OnCreatedBeforePush:
                pushedWidget->SetOwningPlayer(mCachedOwningPC.Get());
                onWidgetCreatedBeforePush.Broadcast(pushedWidget); // executes the flow out of the corresponding pin of the BPNode
                break;
            case EAsyncPushWidgetState::AfterPush:
                afterPush.Broadcast(pushedWidget); // executes the flow out of the corresponding pin of the BPNode
                UE_LOG(LogTemp, Warning, TEXT("Pushed widget %s to %s"), *pushedWidget->GetName(), *mCachedGameplayTag.ToString());
                if (mbCachedFocusOnNewlyPushedWidget) {
                    if (const auto widgetToFocus = pushedWidget->GetDesiredFocusTarget()) {
                        widgetToFocus->SetFocus();
                    }
                }
                SetReadyToDestroy(); // destroy action after it is done its job of pushing widget to stack
                break;
        }
    };

    const ULocalPlayer* localPlayer = mCachedOwningPC.IsValid() ? mCachedOwningPC->GetLocalPlayer() : nullptr;
    if (UMKUI_LocalPlayerSubsystem* uiSubsystem =
            ULocalPlayer::GetSubsystem<UMKUI_LocalPlayerSubsystem>(localPlayer)) {
        uiSubsystem->pushSoftWidgetToStackAsync(mCachedGameplayTag, mCachedWidgetClass, asyncPushStateCallback);
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("MK_UI could not resolve the owning local player's UI subsystem."));
        SetReadyToDestroy();
    }
}
