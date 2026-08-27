// MAAKU Studio all rights reserved

#pragma once

#include "CommonActivatableWidget.h"
#include "CoreMinimal.h"
#include "MKUI_W_ActivatableBase.generated.h"

class APlayerController;

/**
 * Common base for MK_UI screens owned by any consumer-provided Player Controller.
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class MK_UI_API UMKUI_W_ActivatableBase : public UCommonActivatableWidget
{
    GENERATED_BODY()

    UFUNCTION(BlueprintPure, Category = "MKUI")
    APlayerController* getOwningPlayerController();

private:
    TWeakObjectPtr<APlayerController> mCachedOwningPC;
};
