// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "MKUI_LoadingScreenSubsystem.generated.h"

class SWidget;
class UMKUI_LoadingTask;

/**
 * Coordinates the loading screen for map travel and project-owned asynchronous tasks.
 * Its GameInstance lifetime allows the screen to remain available across map travel.
 */
UCLASS()
class MK_UI_API UMKUI_LoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, loadingReason);

    /** Broadcast when the reason keeping the loading screen visible changes. */
    UPROPERTY(BlueprintAssignable, Category = "MKUI|Loading Screen")
    FOnLoadingReasonUpdatedDelegate OnLoadingReasonUpdated;

    /**
     * Begins an asynchronous task that keeps the loading screen visible.
     * Complete the returned task on the game thread when the operation is ready.
     */
    UFUNCTION(BlueprintCallable, Category = "MKUI|Loading Screen")
    UMKUI_LoadingTask* beginLoadingTask(const FText& loadingReason);

    UFUNCTION(BlueprintPure, Category = "MKUI|Loading Screen")
    int32 getActiveLoadingTaskCount() const;

    //~Begin UGameInstanceSubsystem interface
    virtual bool ShouldCreateSubsystem(UObject* outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& collection) override;
    virtual void Deinitialize() override;
    //~End UGameInstanceSubsystem interface

    //~Begin FTickableGameObject interface
    virtual UWorld* GetTickableGameObjectWorld() const override;
    virtual void Tick(float deltaTime) override;
    virtual ETickableTickType GetTickableTickType() const override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;
    //~End FTickableGameObject interface

private:
    friend class UMKUI_LoadingTask;

    void completeLoadingTask(UMKUI_LoadingTask* loadingTask);
    void handleMapPreloaded(const FWorldContext& worldContext, const FString& mapName);
    void handleMapPostLoaded(UWorld* loadedWorld);
    void tryUpdateLoadingScreen();
    bool tryDisplayLoadingScreenIfNone();
    void tryRemoveLoadingScreen();
    bool isPreloadScreenActive() const;
    bool shouldShowLoadingScreen();
    bool areLoadingRequirementsMet();
    bool shouldHoldLoadingScreen(float secondsToHold);
    void broadcastLoadingReasonIfChanged();
    void logOverdueLoadingTasks();
    void setWorldRenderingDisabled(bool bDisabled) const;
    void notifyLoadingScreenVisibilityChanged(bool bVisible);

    UPROPERTY(Transient)
    TArray<TObjectPtr<UMKUI_LoadingTask>> mActiveLoadingTasks;

    TSharedPtr<SWidget> mCachedCreatedLoadingScreenWidget;
    FText mLoadingReason;
    FText mLastBroadcastLoadingReason;
    FString mCurrentLoadingLevelName;
    double mHoldLoadingScreenStartupTime = -1.0;
    bool mbCurrentlyLoadingLevel = false;
    bool mbShouldDisableWorldRendering = false;
    bool mbWidgetLoadFailureLogged = false;
};
