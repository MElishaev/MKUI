// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "MKUI_LoadingScreenSubsystem.generated.h"

class SWidget;

/**
 * Represent a row in a data table which key should be the Level name.
 * Should hold the pre-requisites before starting the Level for the player.
 * The loading screen should be displayed until all conditions met.
 */
USTRUCT(BlueprintType)
struct FLoadingCondition : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MKUI")
    TArray<AActor*> mLoadedActors; // must-be loaded actors before starting level - if empty then no restriction

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MKUI")
    TArray<FName> mRequiredSystems; // array of names of the required systems that must be loaded before starting level
};

/**
 * Subsystem used to control loading screens in game. Accessible throughout the whole game instance
 */
UCLASS()
class MK_UI_API UMKUI_LoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, loadingReason);

    // should broadcast the loading reason - for example, loading level, loading assets, streaming textures etc.
    UPROPERTY(BlueprintAssignable)
    FOnLoadingReasonUpdatedDelegate mOnLoadingReasonUpdated;

    UFUNCTION(BlueprintCallable, Category="MKUI")
    void notifyStageComplete(const FName stageName);

    //~Begin UGameInstanceSubsystem interface
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    //~End UGameInstanceSubsystem interface

    //~Begin FTickableGameObject interface
    virtual UWorld* GetTickableGameObjectWorld() const override;
    virtual void Tick(float DeltaTime) override;
    virtual ETickableTickType GetTickableTickType() const override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;
    //~End FTickableGameObject interface

private:
    // this will execute the logic needed before loading started - e.g. showing loading screen, storing info etc.
    void handleMapPreloaded(const FWorldContext& wc, const FString& mapName);
    void handleMapPostLoaded(UWorld* loadedWorld);

    void tryUpdateLoadingScreen();
    void tryDisplayLoadingScreenIfNone(); // try to display loading screen if already not showing one
    void tryRemoveLoadingScreen();        // if there is a cached loading screen removes it (if cached it is shown)

    /** checks if any of the preload screen (the screens before the actual game loading),
     *  like engine loading, early startup screen or custom splash screen (like the screens from Movies folder)
     */
    bool isPreloadScreenActive() const; 

    bool shouldShowLoadingScreen();   // encapsulates all the checks that tell if need to show loading screen
    bool checkLoadingConditionsMet(); // checks if there are objects that need loading screen

    /**
     * checks if loading screen should be still held for some delay
     * after loading finished (for streaming textures for example)
     * @param secsToHold amount of delay in seconds to hold after the world has loaded
     * @return true if loading screen should be shown
     */
    bool shouldHoldLoadingScreen(const float secsToHold);

    // should be called when visibility of loading screen changed - added or removed from screen
    void notifyLoadingScreenVisibilityChanged(bool bVisible);

    bool mbCurrentlyLoadingLevel = false;
    float mHoldLoadingScreenStartupTime = -1.f;
    FString mLoadingReason;
    FString mCurrentLoadingLevelName;
    TSharedPtr<SWidget> mCachedCreatedLoadingScreenWidget;

    /** maps stage name to bool value represents if the stage is complete.
     *  this is initialized on every request for loading new level */
    TMap<FName, bool> mStagesState;

    UPROPERTY()
    UDataTable* mCachedLoadingConditionsDT;

};
