// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MKUI_LoadingTask.generated.h"

class UMKUI_LoadingScreenSubsystem;

/**
 * Represents one asynchronous operation that needs the loading screen to remain visible.
 * Call complete when the owning operation is ready for the player to continue.
 */
UCLASS(BlueprintType)
class MK_UI_API UMKUI_LoadingTask : public UObject
{
    GENERATED_BODY()

public:
    /** Completes this task. Calling this more than once has no effect. */
    UFUNCTION(BlueprintCallable, Category = "MKUI|Loading Screen")
    void complete();

    UFUNCTION(BlueprintPure, Category = "MKUI|Loading Screen")
    bool isComplete() const;

    UFUNCTION(BlueprintPure, Category = "MKUI|Loading Screen")
    FText getReason() const;

private:
    friend class UMKUI_LoadingScreenSubsystem;

    void initialize(UMKUI_LoadingScreenSubsystem* owningSubsystem, const FText& reason);
    void invalidate();
    bool shouldLogTimeoutWarning(double currentTimeSeconds, float warningTimeoutSeconds) const;
    void markTimeoutWarningLogged();

    UPROPERTY(Transient)
    TWeakObjectPtr<UMKUI_LoadingScreenSubsystem> mOwningSubsystem;

    FText mReason;
    double mStartTimeSeconds = 0.0;
    bool mbIsComplete = false;
    bool mbTimeoutWarningLogged = false;
};
