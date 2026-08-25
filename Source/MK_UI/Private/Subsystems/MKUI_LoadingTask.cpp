// MAAKU Studio all rights reserved

#include "Subsystems/MKUI_LoadingTask.h"

#include "HAL/PlatformTime.h"
#include "Subsystems/MKUI_LoadingScreenSubsystem.h"

void UMKUI_LoadingTask::complete()
{
    if (mbIsComplete) {
        return;
    }

    if (!ensureAlwaysMsgf(IsInGameThread(), TEXT("MK_UI loading tasks must be completed on the game thread."))) {
        return;
    }

    mbIsComplete = true;

    if (UMKUI_LoadingScreenSubsystem* owningSubsystem = mOwningSubsystem.Get()) {
        owningSubsystem->completeLoadingTask(this);
    }

    mOwningSubsystem.Reset();
}

bool UMKUI_LoadingTask::isComplete() const { return mbIsComplete; }

FText UMKUI_LoadingTask::getReason() const { return mReason; }

void UMKUI_LoadingTask::initialize(UMKUI_LoadingScreenSubsystem* owningSubsystem, const FText& reason)
{
    mOwningSubsystem = owningSubsystem;
    mReason = reason;
    mStartTimeSeconds = FPlatformTime::Seconds();
}

void UMKUI_LoadingTask::invalidate()
{
    mbIsComplete = true;
    mOwningSubsystem.Reset();
}

bool UMKUI_LoadingTask::shouldLogTimeoutWarning(const double currentTimeSeconds, const float warningTimeoutSeconds) const
{
    return !mbIsComplete && !mbTimeoutWarningLogged && warningTimeoutSeconds > 0.0f &&
        currentTimeSeconds - mStartTimeSeconds >= warningTimeoutSeconds;
}

void UMKUI_LoadingTask::markTimeoutWarningLogged() { mbTimeoutWarningLogged = true; }
