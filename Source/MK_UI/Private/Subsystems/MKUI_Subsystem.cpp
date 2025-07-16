// MAAKU Studio all rights reserved

#include "Subsystems/MKUI_Subsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "MKUI_FunctionLibrary.h"
#include "MKUI_GameplayTags.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/MKUI_W_ActivatableBase.h"
#include "Widgets/MKUI_W_PrimaryLayout.h"
#include "MKUITypes/MKUIEnumTypes.h"
#include "Widgets/MKUI_W_ConfirmScreen.h"


UMKUI_Subsystem* UMKUI_Subsystem::getInstance(const UObject* worldContextObject)
{
    if (GEngine) {
        const auto world = GEngine->GetWorldFromContextObject(worldContextObject, EGetWorldErrorMode::Assert);
        return world->GetGameInstance()->GetSubsystem<UMKUI_Subsystem>();
    }
    return nullptr;
}

bool UMKUI_Subsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance()) {
        // return true to create the subsystem only in case there are no instantiated classes already
        TArray<UClass*> classes;
        GetDerivedClasses(GetClass(), classes);
        return classes.IsEmpty();
    }
    return false;
}

void UMKUI_Subsystem::registerPrimaryLayoutWidget(UMKUI_W_PrimaryLayout* widget)
{
    check(widget);
    mPrimaryLayout = widget;
}

void UMKUI_Subsystem::pushSoftWidgetToStackAsync(const FGameplayTag& widgetTag,
                                                 TSoftClassPtr<UMKUI_W_ActivatableBase> widgetClass,
                                                 TFunction<void(EAsyncPushWidgetState, UMKUI_W_ActivatableBase*)> asyncPushStateCallback)
{
    check(!widgetClass.IsNull());

    UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
        widgetClass.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda( // called when loading is finished
            [this, widgetClass, widgetTag, asyncPushStateCallback]() {
                const auto loadedWidget = widgetClass.Get();
                check(loadedWidget && mPrimaryLayout);

                const auto widgetStack = mPrimaryLayout->findWidgetStackByTag(widgetTag);
                auto widgetInitFunc = [&](UMKUI_W_ActivatableBase& createdWidgetInstance) {
                    asyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &createdWidgetInstance);
                };
                const auto createdWidget = widgetStack->AddWidget<UMKUI_W_ActivatableBase>(loadedWidget, widgetInitFunc);

                asyncPushStateCallback(EAsyncPushWidgetState::AfterPush, createdWidget);
            }
            )
        );
}

void UMKUI_Subsystem::pushConfirmScreenToModalStackAsync(EConfirmScreenType screenType,
                                                         const FText& screenTitle,
                                                         const FText& screenMsg,
                                                         TFunction<void(EConfirmScreenButtonType)> buttonClickedCallback)
{
    // create screen info for the pushed screen
    UConfirmScreenInfoObject* screenInfo = nullptr;
    switch (screenType) {
        case EConfirmScreenType::Ok:
            screenInfo = UConfirmScreenInfoObject::createOkScreenInfo(screenTitle, screenMsg);
            break;
        case EConfirmScreenType::YesNo:
            screenInfo = UConfirmScreenInfoObject::createYesNoScreenInfo(screenTitle, screenMsg);
            break;
        case EConfirmScreenType::OkCancel:
            screenInfo = UConfirmScreenInfoObject::createOkCancelScreenInfo(screenTitle, screenMsg);
            break;
        case EConfirmScreenType::Unknown:
            break;
    }
    check(screenInfo);

    // create push state callback and push the widget to the UI stack
    auto buttonPushStateCallback = [screenInfo, buttonClickedCallback](EAsyncPushWidgetState pushState,
                                                                       UMKUI_W_ActivatableBase* pushedWidget) {
        if (pushState == EAsyncPushWidgetState::OnCreatedBeforePush) {
            const auto createConfirmScreen = CastChecked<UMKUI_W_ConfirmScreen>(pushedWidget);
            createConfirmScreen->initConfirmScreen(screenInfo, buttonClickedCallback);
        }
    };

    pushSoftWidgetToStackAsync(MKUI_GameplayTags::MKUI_widgetStack_modal,
                               UMKUI_FunctionLibrary::getSoftWidgetClassByTag(MKUI_GameplayTags::MKUI_widget_confirmScreen),
                               buttonPushStateCallback);
}
