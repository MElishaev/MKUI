// MAAKU Studio all rights reserved


#include "AsyncActions/MKUI_AsyncAction_PushSoftWidget.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Subsystems/MKUI_Subsystem.h"
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

    const auto uiSubsystem = mCachedOwningWorld->GetGameInstance()->GetSubsystem<UMKUI_Subsystem>();
    uiSubsystem->pushSoftWidgetToStackAsync(mCachedGameplayTag, mCachedWidgetClass, asyncPushStateCallback);
}
