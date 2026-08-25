// MAAKU Studio all rights reserved

#include "Subsystems/MKUI_Subsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Framework/Application/NavigationConfig.h"
#include "Subsystems/MKUI_LocalPlayerSubsystem.h"
#include "Widgets/MKUI_W_PrimaryLayout.h"


UMKUI_Subsystem* UMKUI_Subsystem::getInstance(const UObject* worldContextObject)
{
    if (GEngine) {
        const auto world = GEngine->GetWorldFromContextObject(worldContextObject, EGetWorldErrorMode::Assert);
        return world->GetGameInstance()->GetSubsystem<UMKUI_Subsystem>();
    }
    return nullptr;
}

bool UMKUI_Subsystem::ShouldCreateSubsystem(UObject* outer) const
{
    if (!CastChecked<UGameInstance>(outer)->IsDedicatedServerInstance()) {
        // return true to create the subsystem only in case there are no instantiated classes already
        TArray<UClass*> classes;
        GetDerivedClasses(GetClass(), classes);
        return classes.IsEmpty();
    }
    return false;
}

void UMKUI_Subsystem::Initialize(FSubsystemCollectionBase& collection)
{
    Super::Initialize(collection);

    const TSharedRef<FNavigationConfig> navigationConfig = FSlateApplication::Get().GetNavigationConfig();

    navigationConfig.Get().KeyEventRules.Emplace(EKeys::W, EUINavigation::Up);
    navigationConfig.Get().KeyEventRules.Emplace(EKeys::A, EUINavigation::Left);
    navigationConfig.Get().KeyEventRules.Emplace(EKeys::S, EUINavigation::Down);
    navigationConfig.Get().KeyEventRules.Emplace(EKeys::D, EUINavigation::Right);
    FSlateApplication::Get().SetNavigationConfig(navigationConfig);
}

void UMKUI_Subsystem::registerPrimaryLayoutWidget(UMKUI_W_PrimaryLayout* widget)
{
    if (UMKUI_LocalPlayerSubsystem* localPlayerSubsystem = UMKUI_LocalPlayerSubsystem::getInstance(widget)) {
        localPlayerSubsystem->registerPrimaryLayoutWidget(widget);
    }
}

void UMKUI_Subsystem::removeAllWidgetsFromStack(UPARAM(meta=(Categories="MKUI.widgetStack")) const FGameplayTag widgetStackTag)
{
    if (UMKUI_LocalPlayerSubsystem* localPlayerSubsystem = UMKUI_LocalPlayerSubsystem::getInstance(GetGameInstance())) {
        localPlayerSubsystem->removeAllWidgetsFromStack(widgetStackTag);
    }
}

void UMKUI_Subsystem::pushSoftWidgetToStackAsync(const FGameplayTag& widgetStackTag,
                                                 TSoftClassPtr<UMKUI_W_ActivatableBase> widgetClass,
                                                 TFunction<void(EAsyncPushWidgetState, UMKUI_W_ActivatableBase*)> asyncPushStateCallback)
{
    if (UMKUI_LocalPlayerSubsystem* localPlayerSubsystem = UMKUI_LocalPlayerSubsystem::getInstance(GetGameInstance())) {
        localPlayerSubsystem->pushSoftWidgetToStackAsync(widgetStackTag, widgetClass, asyncPushStateCallback);
    }
}

void UMKUI_Subsystem::pushConfirmScreenToModalStackAsync(EConfirmScreenType screenType,
                                                         const FText& screenTitle,
                                                         const FText& screenMsg,
                                                         TFunction<void(EConfirmScreenButtonType)> buttonClickedCallback)
{
    if (UMKUI_LocalPlayerSubsystem* localPlayerSubsystem = UMKUI_LocalPlayerSubsystem::getInstance(GetGameInstance())) {
        localPlayerSubsystem->pushConfirmScreenToModalStackAsync(
            screenType, screenTitle, screenMsg, buttonClickedCallback);
    }
}
