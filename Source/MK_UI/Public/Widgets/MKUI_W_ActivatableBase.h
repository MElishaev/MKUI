// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MKUI_W_ActivatableBase.generated.h"

class AMKUI_PlayerController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_W_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

    UFUNCTION(BlueprintPure, Category="MKUI")
    AMKUI_PlayerController* getOwningPlayerController();
    
private:
    TWeakObjectPtr<AMKUI_PlayerController> mCachedOwningPC;
};
