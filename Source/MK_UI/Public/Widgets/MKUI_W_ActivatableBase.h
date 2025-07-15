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
    AMKUI_PlayerController* getOwningPlayerController(); // todo change name and insert mkui pc 
    
private:
    // todo - is it ok to not flag it as UPROPERTY? it is a pointer
    TWeakObjectPtr<AMKUI_PlayerController> mCachedOwningPC;
};
