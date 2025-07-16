// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "MKUI_LoadingScreenSubsystem.generated.h"

class SWidget;

/**
 * Subsystem used to control loading screens in game. Accessible throughout the whole game instance
 */
UCLASS()
class MK_UI_API UMKUI_LoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, loadingReason);
    UPROPERTY(BlueprintAssignable)
    FOnLoadingReasonUpdatedDelegate mOnLoadingReasonUpdated;
    
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

    void handleMapPreloaded(const FWorldContext& wc, const FString& mapName);
    void handleMapPostLoaded(UWorld* loadedWorld);
    
    void tryUpdateLoadingScreen();
    void tryDisplayLoadingScreenIfNone(); // try to display loading screen if already not showing one
    void tryRemoveLoadingScreen(); // if there is a cached loading screen removes it (if cached it is shown)
    bool isPreloadScreenActive() const;
    bool shouldShowLoadingScreen(); // encapsulates all the checks that tell if need to show loading screen
    bool checkTheNeedToShowLoadingScreen(); // checks if there are objects that need loading screen
    bool shouldHoldLoadingScreen(const float secsToHold); // checks if loading screen should be still held after loading finished (for streaming textures for example)

    void notifyLoadingScreenVisibilityChanged(bool bVisible);
    
    bool mbCurrentlyLoadingMap = false;
    float mHoldLoadingScreenStartupTime = -1.f;
    FString mLoadingReason;
    TSharedPtr<SWidget> mCachedCreatedLoadingScreenWidget;
};
