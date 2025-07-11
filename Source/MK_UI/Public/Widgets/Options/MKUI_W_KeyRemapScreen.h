// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MKUI_W_ActivatableBase.h"
#include "MKUI_W_KeyRemapScreen.generated.h"

enum class ECommonInputType : uint8;
class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;
/**
 * Widget that is pushed to to screen when button for binding keymap was pressed
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_W_KeyRemapScreen : public UMKUI_W_ActivatableBase
{
    GENERATED_BODY()
public:
    void setDesiredInputTypeToFilter(ECommonInputType desiredInputType);

    DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /*keyPressed*/)
    FOnKeyRemapScreenKeyPressedDelegate mOnKeyRemapScreenKeyPressed;

    DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCanceledDelegate, const FString& /*canceledReason*/)
    FOnKeyRemapScreenKeySelectCanceledDelegate mOnKeyRemapScreenKeySelectCanceled;
protected:
    virtual void NativeOnActivated() override;
    virtual void NativeOnDeactivated() override;

private:
    /*************** start bound widgets *********************/

    UPROPERTY(meta=(BindWidget))
    UCommonRichTextBlock* mDisplayText; // message that will be displayed for the user
    
    /*************** end bound widgets *********************/

    // delay a tick to make sure the input key is captured properly before widget deactivation
    void requestDeactivateWidget(TFunction<void()> preDeactivateCallback);
    
    void handleValidKeyPressed(const FKey& pressedKey);
    void handleKeySelectionCanceled(const FString& cancelReason);
    
    TSharedPtr<FKeyRemapScreenInputPreprocessor> mCachedInputPreprocessor; // input processor to listen to any user input

    ECommonInputType mCachedDesiredInputType;
};
