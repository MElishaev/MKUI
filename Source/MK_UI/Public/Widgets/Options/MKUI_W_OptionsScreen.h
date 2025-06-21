// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MKUI_W_ActivatableBase.h"
#include "MKUI_W_OptionsScreen.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_W_OptionsScreen : public UMKUI_W_ActivatableBase
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;
    
private:

    void onResetBoundActionTriggered();
    void onBackBoundActionTriggered();
    
    // taking inspiration from how commonUI does it in the CommonUIInputData class, we will add custom bound actions
    UPROPERTY(EditDefaultsOnly, Category = "MKUI Options Screen", meta = (EditCondition = "CommonInput.CommonInputSettings.IsEnhancedInputSupportEnabled", EditConditionHides))
    TObjectPtr<UInputAction> mEnhancedInputResetAction; // reset settings input actions

    FUIActionBindingHandle mResetActionHandle; // handle for removal of the action button from the BoundActionsBar
};
