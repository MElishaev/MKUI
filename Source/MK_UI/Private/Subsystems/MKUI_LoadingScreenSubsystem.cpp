// MAAKU Studio all rights reserved

#include "Subsystems/MKUI_LoadingScreenSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "Interfaces/MKUI_LoadingScreenObserver.h"
#include "Misc/PackageName.h"
#include "PreLoadScreenManager.h"
#include "Settings/MKUI_LoadingScreenSettings.h"
#include "Subsystems/MKUI_LoadingTask.h"
#include "UObject/UObjectHash.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UMKUI_LoadingTask* UMKUI_LoadingScreenSubsystem::beginLoadingTask(const FText& loadingReason)
{
    if (!ensureAlwaysMsgf(IsInGameThread(), TEXT("MK_UI loading tasks must begin on the game thread."))) {
        return nullptr;
    }

    const FText resolvedReason = loadingReason.IsEmpty() ? FText::FromString(TEXT("Loading")) : loadingReason;

    UMKUI_LoadingTask* loadingTask = NewObject<UMKUI_LoadingTask>(this);
    loadingTask->initialize(this, resolvedReason);
    mActiveLoadingTasks.Add(loadingTask);

    mHoldLoadingScreenStartupTime = -1.0;
    SetTickableTickType(ETickableTickType::Conditional);
    tryUpdateLoadingScreen();

    return loadingTask;
}

int32 UMKUI_LoadingScreenSubsystem::getActiveLoadingTaskCount() const { return mActiveLoadingTasks.Num(); }

bool UMKUI_LoadingScreenSubsystem::ShouldCreateSubsystem(UObject* outer) const
{
    const UGameInstance* gameInstance = CastChecked<UGameInstance>(outer);
    const UMKUI_LoadingScreenSettings* settings = GetDefault<UMKUI_LoadingScreenSettings>();

    if (!Super::ShouldCreateSubsystem(outer) || gameInstance->IsDedicatedServerInstance() || !settings ||
        !settings->mbEnableLoadingScreen) {
        return false;
    }

    if (settings->mSoftLoadingScreenWidgetClass.IsNull()) {
        UE_LOG(LogTemp, Error, TEXT("MK_UI loading screens are enabled, but no loading-screen widget is configured."));
        return false;
    }

    // Prefer an explicitly derived subsystem over this base implementation.
    TArray<UClass*> derivedClasses;
    GetDerivedClasses(GetClass(), derivedClasses, false);
    return derivedClasses.IsEmpty();
}

void UMKUI_LoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
    Super::Initialize(collection);

    FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::handleMapPreloaded);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::handleMapPostLoaded);
}

void UMKUI_LoadingScreenSubsystem::Deinitialize()
{
    FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
    FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

    for (UMKUI_LoadingTask* loadingTask : mActiveLoadingTasks) {
        if (loadingTask) {
            loadingTask->invalidate();
        }
    }
    mActiveLoadingTasks.Reset();

    tryRemoveLoadingScreen();
    setWorldRenderingDisabled(false);

    Super::Deinitialize();
}

UWorld* UMKUI_LoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
    if (const UGameInstance* gameInstance = GetGameInstance()) {
        return gameInstance->GetWorld();
    }

    return nullptr;
}

void UMKUI_LoadingScreenSubsystem::Tick(float) { tryUpdateLoadingScreen(); }

ETickableTickType UMKUI_LoadingScreenSubsystem::GetTickableTickType() const
{
    return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Conditional;
}

bool UMKUI_LoadingScreenSubsystem::IsTickable() const { return GetGameInstance() && GetGameInstance()->GetGameViewportClient(); }

TStatId UMKUI_LoadingScreenSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UMKUI_LoadingScreenSubsystem, STATGROUP_Tickables);
}

void UMKUI_LoadingScreenSubsystem::completeLoadingTask(UMKUI_LoadingTask* loadingTask)
{
    if (!loadingTask || mActiveLoadingTasks.RemoveSingleSwap(loadingTask) == 0) {
        return;
    }

    loadingTask->invalidate();
    SetTickableTickType(ETickableTickType::Conditional);
    tryUpdateLoadingScreen();
}

void UMKUI_LoadingScreenSubsystem::handleMapPreloaded(const FWorldContext&, const FString& mapName)
{
    mCurrentLoadingLevelName = FPackageName::GetShortName(mapName);
    mbCurrentlyLoadingLevel = true;
    mHoldLoadingScreenStartupTime = -1.0;

    UE_LOG(LogTemp, Log, TEXT("MK_UI is loading map %s."), *mCurrentLoadingLevelName);

    SetTickableTickType(ETickableTickType::Conditional);
    tryUpdateLoadingScreen();
}

void UMKUI_LoadingScreenSubsystem::handleMapPostLoaded(UWorld* loadedWorld)
{
    if (!loadedWorld || loadedWorld->GetGameInstance() != GetGameInstance()) {
        return;
    }

    mbCurrentlyLoadingLevel = false;
    UE_LOG(LogTemp, Log, TEXT("MK_UI finished loading map %s."), *mCurrentLoadingLevelName);

    SetTickableTickType(ETickableTickType::Conditional);
}

void UMKUI_LoadingScreenSubsystem::tryUpdateLoadingScreen()
{
    if (isPreloadScreenActive()) {
        return;
    }

    logOverdueLoadingTasks();

    if (shouldShowLoadingScreen()) {
        const bool bLoadingScreenVisible = tryDisplayLoadingScreenIfNone();
        setWorldRenderingDisabled(bLoadingScreenVisible && mbShouldDisableWorldRendering);
        broadcastLoadingReasonIfChanged();
        return;
    }

    tryRemoveLoadingScreen();
    setWorldRenderingDisabled(false);
    mHoldLoadingScreenStartupTime = -1.0;
    mCurrentLoadingLevelName.Reset();
    SetTickableTickType(ETickableTickType::Never);
}

bool UMKUI_LoadingScreenSubsystem::tryDisplayLoadingScreenIfNone()
{
    if (mCachedCreatedLoadingScreenWidget) {
        return true;
    }

    UGameInstance* gameInstance = GetGameInstance();
    UGameViewportClient* viewportClient = gameInstance ? gameInstance->GetGameViewportClient() : nullptr;
    const UMKUI_LoadingScreenSettings* settings = GetDefault<UMKUI_LoadingScreenSettings>();
    const TSubclassOf<UUserWidget> widgetClass = settings ? settings->getLoadingScreenWidgetClass() : nullptr;

    if (!gameInstance || !viewportClient || !widgetClass) {
        if (!mbWidgetLoadFailureLogged) {
            UE_LOG(LogTemp, Error, TEXT("MK_UI could not create the configured loading-screen widget."));
            mbWidgetLoadFailureLogged = true;
        }
        return false;
    }

    UUserWidget* widgetInstance = UUserWidget::CreateWidgetInstance(*gameInstance, widgetClass, NAME_None);
    if (!widgetInstance) {
        if (!mbWidgetLoadFailureLogged) {
            UE_LOG(LogTemp, Error, TEXT("MK_UI could not instantiate the configured loading-screen widget."));
            mbWidgetLoadFailureLogged = true;
        }
        return false;
    }

    mCachedCreatedLoadingScreenWidget = widgetInstance->TakeWidget();
    viewportClient->AddViewportWidgetContent(mCachedCreatedLoadingScreenWidget.ToSharedRef(), 9999);
    notifyLoadingScreenVisibilityChanged(true);
    return true;
}

void UMKUI_LoadingScreenSubsystem::tryRemoveLoadingScreen()
{
    if (!mCachedCreatedLoadingScreenWidget) {
        return;
    }

    if (UGameInstance* gameInstance = GetGameInstance()) {
        if (UGameViewportClient* viewportClient = gameInstance->GetGameViewportClient()) {
            viewportClient->RemoveViewportWidgetContent(mCachedCreatedLoadingScreenWidget.ToSharedRef());
        }
    }

    mCachedCreatedLoadingScreenWidget.Reset();
    notifyLoadingScreenVisibilityChanged(false);
}

bool UMKUI_LoadingScreenSubsystem::isPreloadScreenActive() const
{
    const FPreLoadScreenManager* preloadScreenManager = FPreLoadScreenManager::Get();
    return preloadScreenManager && preloadScreenManager->HasValidActivePreLoadScreen();
}

bool UMKUI_LoadingScreenSubsystem::shouldShowLoadingScreen()
{
    const UMKUI_LoadingScreenSettings* settings = GetDefault<UMKUI_LoadingScreenSettings>();
    mbShouldDisableWorldRendering = false;

#if WITH_EDITOR
    if (GEditor && settings && !settings->mbShowLoadingScreenInEditor) {
        return false;
    }
#endif

    if (!areLoadingRequirementsMet()) {
        mbShouldDisableWorldRendering = true;
        return true;
    }

    mLoadingReason = FText::FromString(TEXT("Waiting for streaming to settle"));
    return settings && shouldHoldLoadingScreen(settings->mSecsToHoldLoadingScreenAfterLoad);
}

bool UMKUI_LoadingScreenSubsystem::areLoadingRequirementsMet()
{
    if (mbCurrentlyLoadingLevel) {
        mLoadingReason = FText::FromString(TEXT("Loading level"));
        return false;
    }

    UWorld* owningWorld = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
    if (!owningWorld) {
        mLoadingReason = FText::FromString(TEXT("Loading world"));
        return false;
    }

    if (!owningWorld->HasBegunPlay()) {
        mLoadingReason = FText::Format(FText::FromString(TEXT("Starting world {0}")), FText::FromName(owningWorld->GetFName()));
        return false;
    }

    if (!mActiveLoadingTasks.IsEmpty()) {
        const UMKUI_LoadingTask* loadingTask = mActiveLoadingTasks[0];
        mLoadingReason = loadingTask ? loadingTask->getReason() : FText::FromString(TEXT("Finishing game initialization"));
        return false;
    }

    return true;
}

bool UMKUI_LoadingScreenSubsystem::shouldHoldLoadingScreen(const float secondsToHold)
{
    if (secondsToHold <= 0.0f) {
        return false;
    }

    const double currentTimeSeconds = FPlatformTime::Seconds();
    if (mHoldLoadingScreenStartupTime < 0.0) {
        mHoldLoadingScreenStartupTime = currentTimeSeconds;
    }

    return currentTimeSeconds - mHoldLoadingScreenStartupTime < secondsToHold;
}

void UMKUI_LoadingScreenSubsystem::broadcastLoadingReasonIfChanged()
{
    if (mLoadingReason.EqualTo(mLastBroadcastLoadingReason)) {
        return;
    }

    mLastBroadcastLoadingReason = mLoadingReason;
    const FString loadingReason = mLoadingReason.ToString();
    OnLoadingReasonUpdated.Broadcast(loadingReason);
}

void UMKUI_LoadingScreenSubsystem::logOverdueLoadingTasks()
{
    const UMKUI_LoadingScreenSettings* settings = GetDefault<UMKUI_LoadingScreenSettings>();
    if (!settings || settings->mLoadingTaskWarningTimeout <= 0.0f) {
        return;
    }

    const double currentTimeSeconds = FPlatformTime::Seconds();
    for (UMKUI_LoadingTask* loadingTask : mActiveLoadingTasks) {
        if (!loadingTask || !loadingTask->shouldLogTimeoutWarning(currentTimeSeconds, settings->mLoadingTaskWarningTimeout)) {
            continue;
        }

        UE_LOG(LogTemp,
               Warning,
               TEXT("MK_UI loading task '%s' has remained active for at least %.1f seconds."),
               *loadingTask->getReason().ToString(),
               settings->mLoadingTaskWarningTimeout);
        loadingTask->markTimeoutWarningLogged();
    }
}

void UMKUI_LoadingScreenSubsystem::setWorldRenderingDisabled(const bool bDisabled) const
{
    if (const UGameInstance* gameInstance = GetGameInstance()) {
        if (UGameViewportClient* viewportClient = gameInstance->GetGameViewportClient()) {
            viewportClient->bDisableWorldRendering = bDisabled;
        }
    }
}

void UMKUI_LoadingScreenSubsystem::notifyLoadingScreenVisibilityChanged(const bool bVisible)
{
    UGameInstance* gameInstance = GetGameInstance();
    if (!gameInstance) {
        return;
    }

    for (ULocalPlayer* localPlayer : gameInstance->GetLocalPlayers()) {
        if (!localPlayer) {
            continue;
        }

        APlayerController* playerController = localPlayer->GetPlayerController(gameInstance->GetWorld());
        if (!playerController) {
            continue;
        }

        if (playerController->Implements<UMKUI_LoadingScreenObserver>()) {
            if (bVisible) {
                IMKUI_LoadingScreenObserver::Execute_handleLoadingScreenActivated(playerController);
            }
            else {
                IMKUI_LoadingScreenObserver::Execute_handleLoadingScreenDeactivated(playerController);
            }
        }

        APawn* pawn = playerController->GetPawn();
        if (!pawn || !pawn->Implements<UMKUI_LoadingScreenObserver>()) {
            continue;
        }

        if (bVisible) {
            IMKUI_LoadingScreenObserver::Execute_handleLoadingScreenActivated(pawn);
        }
        else {
            IMKUI_LoadingScreenObserver::Execute_handleLoadingScreenDeactivated(pawn);
        }
    }
}
