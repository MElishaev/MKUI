// MAAKU Studio all rights reserved


#include "Subsystems/MKUI_LoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/MKUI_LoadingScreenObserver.h"
#include "Settings/MKUI_LoadingScreenSettings.h"


bool UMKUI_LoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance()) {
        // return true to create the subsystem only in case there are no instantiated classes already
        TArray<UClass*> classes;
        GetDerivedClasses(GetClass(), classes);
        return classes.IsEmpty();
    }
    return false;
}

void UMKUI_LoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::handleMapPreloaded);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::handleMapPostLoaded);
}

void UMKUI_LoadingScreenSubsystem::Deinitialize()
{
    FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
    FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

UWorld* UMKUI_LoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
    if (const auto gi = GetGameInstance()) {
        return gi->GetWorld();
    }
    return nullptr;
}

void UMKUI_LoadingScreenSubsystem::Tick(float DeltaTime)
{
    tryUpdateLoadingScreen();
}

ETickableTickType UMKUI_LoadingScreenSubsystem::GetTickableTickType() const
{
    if (IsTemplate()) {
        return ETickableTickType::Never;
    }
    return ETickableTickType::Conditional; // returning conditional will mark flag to call IsTickable() to determine if tick is needed
}

bool UMKUI_LoadingScreenSubsystem::IsTickable() const
{
    // todo - why game viewport client is the condition for determining if should tick? 
    return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UMKUI_LoadingScreenSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UMKUI_LoadingScreenSubsystem, STATGROUP_Tickables);
}

void UMKUI_LoadingScreenSubsystem::handleMapPreloaded(const FWorldContext& wc, const FString& mapName)
{
    SetTickableTickType(ETickableTickType::Conditional);
    mbCurrentlyLoadingMap = true;
    tryUpdateLoadingScreen();
}

void UMKUI_LoadingScreenSubsystem::handleMapPostLoaded(UWorld* loadedWorld)
{
    // if this condition is true it means the loading is complete - why?
    if (loadedWorld && loadedWorld->GetGameInstance() == GetGameInstance()) {
        mbCurrentlyLoadingMap = false;
    }
}

void UMKUI_LoadingScreenSubsystem::tryUpdateLoadingScreen()
{
    if (isPreloadScreenActive()) {
        return;
    }

    if (shouldShowLoadingScreen()) {
        tryDisplayLoadingScreenIfNone();
        mOnLoadingReasonUpdated.Broadcast(mLoadingReason);
    }
    else {
        tryRemoveLoadingScreen();
        notifyLoadingScreenVisibilityChanged(false);
        mHoldLoadingScreenStartupTime = -1.f;
        SetTickableTickType(ETickableTickType::Never);
    }
}

void UMKUI_LoadingScreenSubsystem::tryDisplayLoadingScreenIfNone()
{
    if (mCachedCreatedLoadingScreenWidget) {
        return;
    }

    const auto loadingScreenSettings = GetDefault<UMKUI_LoadingScreenSettings>();

    // will crash if widget wasn't set in the project settings
    const auto loadedWidgetClass = loadingScreenSettings->getLoadingScreenWidgetClassChecked();
    if (auto gi = GetGameInstance()) {
        const auto widgetInst = UUserWidget::CreateWidgetInstance(*gi, loadedWidgetClass, NAME_None);
        check(widgetInst);

        mCachedCreatedLoadingScreenWidget = widgetInst->TakeWidget();
        gi->GetGameViewportClient()->AddViewportWidgetContent(mCachedCreatedLoadingScreenWidget.ToSharedRef(), 9999);
        notifyLoadingScreenVisibilityChanged(true);
    }
}

void UMKUI_LoadingScreenSubsystem::tryRemoveLoadingScreen()
{
    if (mCachedCreatedLoadingScreenWidget) {
        GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(mCachedCreatedLoadingScreenWidget.ToSharedRef());
        mCachedCreatedLoadingScreenWidget.Reset();
    }
}

bool UMKUI_LoadingScreenSubsystem::isPreloadScreenActive() const
{
    if (const auto preloadScreenManager = FPreLoadScreenManager::Get()) {
        return preloadScreenManager->HasValidActivePreLoadScreen();
    }
    return false;
}

bool UMKUI_LoadingScreenSubsystem::shouldShowLoadingScreen()
{
    const auto loadingScreenSettings = GetDefault<UMKUI_LoadingScreenSettings>();
#if WITH_EDITOR
    if (GEditor && !loadingScreenSettings->mbShowLoadingScreenInEditor) {
        return false;
    }
#endif
    // check if the objects in the world need loading screen
    if (checkTheNeedToShowLoadingScreen()) {
        GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
        return true;
    }

    mLoadingReason = TEXT("Waiting for streaming textures");

    // at this point, we found that there is no need to show loading screen, allow world to render to our viewport
    GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;
    
    return shouldHoldLoadingScreen(loadingScreenSettings->mSecsToHoldLoadingScreenAfterLoad);
}

bool UMKUI_LoadingScreenSubsystem::checkTheNeedToShowLoadingScreen()
{
    if (mbCurrentlyLoadingMap) {
        mLoadingReason = TEXT("Loading Level");
        return true;
    }

    UWorld* owningWorld = GetGameInstance()->GetWorld();
    if (!owningWorld) {
        mLoadingReason = TEXT("Loading World");
        return true;        
    }

    if (!owningWorld->HasBegunPlay()) {
        mLoadingReason = TEXT("World hasn't begun play yet");
        return true;
    }

    // we can check here more stuff if we want to display this in the loading screen for user feedback - but this is optional

    return false;    
}

bool UMKUI_LoadingScreenSubsystem::shouldHoldLoadingScreen(const float secsToHold)
{
    const float currentTime = FPlatformTime::Seconds();
    if (mHoldLoadingScreenStartupTime < 0) {
        mHoldLoadingScreenStartupTime = currentTime;
    }
    const float elapsedTime = currentTime - mHoldLoadingScreenStartupTime;
    if (elapsedTime < secsToHold) {
        return true;
    }
    return false;
}

void UMKUI_LoadingScreenSubsystem::notifyLoadingScreenVisibilityChanged(bool bVisible)
{
    auto gi = GetGameInstance();
    for (auto localPlayer : gi->GetLocalPlayers()) {
        if (!localPlayer) {
            continue;
        }

        if (auto pc = localPlayer->GetPlayerController(gi->GetWorld())) {
            if (pc->Implements<UMKUI_LoadingScreenObserver>()) {
                if (bVisible) {
                    IMKUI_LoadingScreenObserver::Execute_handleLoadingScreenActivated(pc);
                }
                else {
                    IMKUI_LoadingScreenObserver::Execute_handleLoadingScreenDeactivated(pc);
                }
            }

            if (auto pawn = pc->GetPawn()) {
                if (pawn->Implements<UMKUI_LoadingScreenObserver>()) {
                    if (bVisible) {
                        IMKUI_LoadingScreenObserver::Execute_handleLoadingScreenActivated(pawn);
                    }
                    else {
                        IMKUI_LoadingScreenObserver::Execute_handleLoadingScreenDeactivated(pawn);
                    }
                }
            }
        }
    }

    // todo - for notifying other objects that needs to know - this happens here
}
