// MAAKU Studio all rights reserved

#include "Subsystems/MKUI_LocalPlayerSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "MKUI_FunctionLibrary.h"
#include "MKUI_GameplayTags.h"
#include "MKUITypes/MKUIEnumTypes.h"
#include "Settings/MKUI_DeveloperSettings.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/MKUI_W_ActivatableBase.h"
#include "Widgets/MKUI_W_ConfirmScreen.h"
#include "Widgets/MKUI_W_PrimaryLayout.h"

namespace
{
ULocalPlayer* resolveLocalPlayer(const UObject* worldContextObject)
{
    if (const UUserWidget* widget = Cast<UUserWidget>(worldContextObject)) {
        return widget->GetOwningLocalPlayer();
    }

    if (const APlayerController* playerController = Cast<APlayerController>(worldContextObject)) {
        return playerController->GetLocalPlayer();
    }

    if (const ULocalPlayer* localPlayer = Cast<ULocalPlayer>(worldContextObject)) {
        return const_cast<ULocalPlayer*>(localPlayer);
    }

    if (GEngine && worldContextObject) {
        const UWorld* world = GEngine->GetWorldFromContextObject(
            worldContextObject,
            EGetWorldErrorMode::LogAndReturnNull);
        if (world) {
            if (const UGameInstance* gameInstance = world->GetGameInstance()) {
                return gameInstance->GetFirstGamePlayer();
            }
        }
    }

    return nullptr;
}
}
UMKUI_LocalPlayerSubsystem* UMKUI_LocalPlayerSubsystem::getInstance(const UObject* worldContextObject)
{
    return ULocalPlayer::GetSubsystem<UMKUI_LocalPlayerSubsystem>(resolveLocalPlayer(worldContextObject));
}

void UMKUI_LocalPlayerSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
    Super::Initialize(collection);
    collection.InitializeDependency<UEnhancedInputLocalPlayerSubsystem>();
}

void UMKUI_LocalPlayerSubsystem::Deinitialize()
{
    removeGenericInputMapping();

    if (mPrimaryLayout) {
        mPrimaryLayout->RemoveFromParent();
        mPrimaryLayout = nullptr;
    }

    Super::Deinitialize();
}

void UMKUI_LocalPlayerSubsystem::PlayerControllerChanged(APlayerController* newPlayerController)
{
    Super::PlayerControllerChanged(newPlayerController);

    // A new controller requires a layout and input mapping owned by that controller.
    removeGenericInputMapping();
    if (mPrimaryLayout) {
        mPrimaryLayout->RemoveFromParent();
        mPrimaryLayout = nullptr;
    }

    if (!newPlayerController || !newPlayerController->IsLocalPlayerController()) {
        return;
    }

    applyGenericInputMapping();
    createPrimaryLayout(newPlayerController);
}

void UMKUI_LocalPlayerSubsystem::createPrimaryLayout(APlayerController* owningPlayerController)
{
    const UMKUI_DeveloperSettings* settings = GetDefault<UMKUI_DeveloperSettings>();
    if (!settings || settings->mPrimaryLayoutClass.IsNull()) {
        // An unset class intentionally preserves the legacy controller-created layout path.
        return;
    }

    const TSubclassOf<UMKUI_W_PrimaryLayout> layoutClass = settings->mPrimaryLayoutClass.LoadSynchronous();
    if (!layoutClass) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI could not load the configured primary layout class."));
        return;
    }

    mPrimaryLayout = CreateWidget<UMKUI_W_PrimaryLayout>(owningPlayerController, layoutClass);
    if (!mPrimaryLayout) {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("MK_UI could not create the primary layout for local player %s."),
            *GetNameSafe(GetLocalPlayer()));
        return;
    }

    if (!mPrimaryLayout->AddToPlayerScreen()) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI could not add the primary layout to the local player's screen."));
        mPrimaryLayout = nullptr;
    }
}

void UMKUI_LocalPlayerSubsystem::applyGenericInputMapping()
{
    const UMKUI_DeveloperSettings* settings = GetDefault<UMKUI_DeveloperSettings>();
    if (!settings || settings->mInputMappingContext.IsNull()) {
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* inputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!inputSubsystem) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI could not resolve Enhanced Input for its local player."));
        return;
    }

    mAppliedInputMappingContext = settings->mInputMappingContext.LoadSynchronous();
    if (!mAppliedInputMappingContext) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI could not load the configured generic input mapping context."));
        return;
    }

    inputSubsystem->AddMappingContext(mAppliedInputMappingContext, 0);
}

void UMKUI_LocalPlayerSubsystem::removeGenericInputMapping()
{
    if (!mAppliedInputMappingContext) {
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* inputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (inputSubsystem) {
        inputSubsystem->RemoveMappingContext(mAppliedInputMappingContext);
    }

    mAppliedInputMappingContext = nullptr;
}

void UMKUI_LocalPlayerSubsystem::registerPrimaryLayoutWidget(UMKUI_W_PrimaryLayout* widget)
{
    if (!widget || widget == mPrimaryLayout) {
        return;
    }

    if (widget->GetOwningLocalPlayer() && widget->GetOwningLocalPlayer() != GetLocalPlayer()) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI rejected a primary layout owned by a different local player."));
        return;
    }

    if (mPrimaryLayout) {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("MK_UI already created a primary layout for this local player; removing the duplicate legacy layout."));
        widget->RemoveFromParent();
        return;
    }

    mPrimaryLayout = widget;
}

void UMKUI_LocalPlayerSubsystem::removeAllWidgetsFromStack(const FGameplayTag widgetStackTag)
{
    if (!mPrimaryLayout) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI primary layout has not been created for this local player."));
        return;
    }

    mPrimaryLayout->findWidgetStackByTag(widgetStackTag)->ClearWidgets();
}

void UMKUI_LocalPlayerSubsystem::pushSoftWidgetToStackAsync(
    const FGameplayTag& widgetStackTag,
    TSoftClassPtr<UMKUI_W_ActivatableBase> widgetClass,
    TFunction<void(EAsyncPushWidgetState, UMKUI_W_ActivatableBase*)> asyncPushStateCallback)
{
    if (widgetClass.IsNull()) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI cannot push a null widget class."));
        return;
    }

    if (!mPrimaryLayout) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI primary layout has not been created for this local player."));
        return;
    }

    // Keep only a weak subsystem reference while the soft widget class loads.
    const TWeakObjectPtr<UMKUI_LocalPlayerSubsystem> weakThis(this);
    UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
        widgetClass.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [weakThis, widgetClass, widgetStackTag, asyncPushStateCallback]() {
                UMKUI_LocalPlayerSubsystem* subsystem = weakThis.Get();
                UClass* loadedWidgetClass = widgetClass.Get();
                if (!subsystem || !subsystem->mPrimaryLayout || !loadedWidgetClass) {
                    UE_LOG(
                        LogTemp,
                        Error,
                        TEXT("MK_UI widget push could not finish because its player UI is unavailable."));
                    return;
                }

                UCommonActivatableWidgetContainerBase* widgetStack =
                    subsystem->mPrimaryLayout->findWidgetStackByTag(widgetStackTag);
                UMKUI_W_ActivatableBase* createdWidget = widgetStack->AddWidget<UMKUI_W_ActivatableBase>(
                    loadedWidgetClass,
                    [&asyncPushStateCallback](UMKUI_W_ActivatableBase& widgetInstance) {
                        if (asyncPushStateCallback) {
                            asyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &widgetInstance);
                        }
                    });

                if (asyncPushStateCallback) {
                    asyncPushStateCallback(EAsyncPushWidgetState::AfterPush, createdWidget);
                }
            }));
}

void UMKUI_LocalPlayerSubsystem::pushConfirmScreenToModalStackAsync(
    EConfirmScreenType screenType,
    const FText& screenTitle,
    const FText& screenMsg,
    TFunction<void(EConfirmScreenButtonType)> buttonClickedCallback)
{
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

    if (!screenInfo) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI cannot create an unknown confirmation-screen type."));
        return;
    }

    // Initialize the confirmation screen before its Common UI stack activates it.
    auto pushStateCallback = [screenInfo, buttonClickedCallback](
                                 EAsyncPushWidgetState pushState,
                                 UMKUI_W_ActivatableBase* pushedWidget) {
        if (pushState == EAsyncPushWidgetState::OnCreatedBeforePush) {
            UMKUI_W_ConfirmScreen* confirmScreen = CastChecked<UMKUI_W_ConfirmScreen>(pushedWidget);
            confirmScreen->initConfirmScreen(screenInfo, buttonClickedCallback);
        }
    };

    pushSoftWidgetToStackAsync(
        MKUI_GameplayTags::MKUI_widgetStack_modal,
        UMKUI_FunctionLibrary::getSoftWidgetClassByTag(MKUI_GameplayTags::MKUI_widget_confirmScreen),
        pushStateCallback);
}
