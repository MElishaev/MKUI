// MAAKU Studio all rights reserved


#include "Subsystems/MKUI_LoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/MKUI_LoadingScreenObserver.h"
#include "Settings/MKUI_LoadingScreenSettings.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/Pawn.h"


void UMKUI_LoadingScreenSubsystem::notifyStageComplete(const FName stageName)
{
    if (mStagesState.Contains(stageName)) {
        mStagesState.Add(stageName, true);
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("System readiness doesn't contain %s for level %s"), *stageName.ToString(), *mCurrentLoadingLevelName);
    }
}

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

    const auto loadingScreenSettings = GetDefault<UMKUI_LoadingScreenSettings>();
    check(loadingScreenSettings);

    mCachedLoadingConditionsDT = loadingScreenSettings->getLoadingConditionsDataTable();
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
    UE_LOG(LogTemp, Warning, TEXT("Loading %s"), *mapName);

    // extract name from mapName which is a relative path to the level file in the project
    FString strippedName = mapName;
    if (const int32 nameStartPosition = mapName.Find("/", ESearchCase::IgnoreCase, ESearchDir::FromEnd); nameStartPosition > 0) {
        strippedName = mapName.RightChop(nameStartPosition + 1);
        UE_LOG(LogTemp, Warning, TEXT("Stripped level name %s"), *strippedName);
    }
    
    // based on mapName, prepare data for checking loading conditions
    mStagesState.Empty();
    if (mCachedLoadingConditionsDT) {
        if (const auto levelLoadingConditions = mCachedLoadingConditionsDT->FindRow<FLoadingCondition>(FName(strippedName), "")) {
            for (const FName sysName : levelLoadingConditions->mRequiredSystems) {
                mStagesState.Add(sysName, false);
            }
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Didn't find row for %s"), *strippedName);
        }
    }   
    
    mbCurrentlyLoadingLevel = true;
    mCurrentLoadingLevelName = strippedName;
    tryUpdateLoadingScreen();
}

void UMKUI_LoadingScreenSubsystem::handleMapPostLoaded(UWorld* loadedWorld)
{
    // if this condition is true it means the loading is complete - why?
    if (loadedWorld && loadedWorld->GetGameInstance() == GetGameInstance()) {
        mbCurrentlyLoadingLevel = false;
        UE_LOG(LogTemp, Warning, TEXT("Finished loading %s"), *mCurrentLoadingLevelName);
    }
}

void UMKUI_LoadingScreenSubsystem::tryUpdateLoadingScreen()
{
    if (isPreloadScreenActive()) {
        // early return if not even started loading game yet and still showing startup and splash screens
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
        mCurrentLoadingLevelName.Reset();
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
    // check if the objects in the world need loading screen - but only if we still loading the level.
    // there may be a case we finished loading but we still hold the loading screen to prevent displaying
    // not yet streamed in textures
    if (!checkLoadingConditionsMet()) {
        GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
        return true;
    }

    mLoadingReason = TEXT("Waiting for streaming textures");

    // at this point, we found that there is no need to show loading screen, allow world to render to our viewport
    GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;
    
    return shouldHoldLoadingScreen(loadingScreenSettings->mSecsToHoldLoadingScreenAfterLoad);
}

bool UMKUI_LoadingScreenSubsystem::checkLoadingConditionsMet()
{
    if (mbCurrentlyLoadingLevel) {
        mLoadingReason = TEXT("Loading Level");
        return false;
    }

    UWorld* owningWorld = GetGameInstance()->GetWorld();
    if (!owningWorld) {
        mLoadingReason = TEXT("Loading World");
        return false;        
    }

    if (!owningWorld->HasBegunPlay()) {
        mLoadingReason = FString::Printf(TEXT("World %s hasn't begun play yet"), *owningWorld->GetFName().ToString());
        return false;
    }

    // TODO: we can check here additional conditions that must be met for loading to complete

    if (mCachedLoadingConditionsDT) {
        if (auto loadingConditions = mCachedLoadingConditionsDT->FindRow<FLoadingCondition>(FName(mCurrentLoadingLevelName), "")) {
            for (const auto sysName : loadingConditions->mRequiredSystems) {
                const bool bSysReady = mStagesState.FindRef(sysName);
                if (!bSysReady) {
                    mLoadingReason = FString::Printf(TEXT("Loading %s"), *sysName.ToString());
                    return false;
                }
            }

            // todo - not implemented checking loaded actors yet
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Couldn't find loading conditions for level %s"), *mCurrentLoadingLevelName);
        }
    }
    
    return true;    
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
