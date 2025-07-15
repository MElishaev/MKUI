// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MKUI_W_ActivatableBase.h"
#include "MKUITypes/MKUIEnumTypes.h"
#include "MKUI_W_ConfirmScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MKUI")
    EConfirmScreenButtonType mConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MKUI")
    FText mButtonText;
};

UCLASS()
class MK_UI_API UConfirmScreenInfoObject : public UObject
{
    GENERATED_BODY()

public:

    static UConfirmScreenInfoObject* createOkScreenInfo(const FText& title, const FText& msg);
    static UConfirmScreenInfoObject* createYesNoScreenInfo(const FText& title, const FText& msg);
    static UConfirmScreenInfoObject* createOkCancelScreenInfo(const FText& title, const FText& msg);
    
    UPROPERTY(Transient)
    FText mScreenTitle;

    UPROPERTY(Transient)
    FText mScreenMessage;

    UPROPERTY(Transient)
    TArray<FConfirmScreenButtonInfo> mAvailableButtons; // buttons to populate the confirm window
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_W_ConfirmScreen : public UMKUI_W_ActivatableBase
{
	GENERATED_BODY()
public:
    // gets called when this widget is constructed and before it is pushed to the stack
    void initConfirmScreen(UConfirmScreenInfoObject* infoObject, TFunction<void(EConfirmScreenButtonType)> clickedButtonCallback);

protected:
    virtual UWidget* NativeGetDesiredFocusTarget() const override;
    
private:
    UPROPERTY(meta=(BindWidget))
    UCommonTextBlock* mTitle;

    UPROPERTY(meta=(BindWidget))
    UCommonTextBlock* mMessage; // for the confirm screen

    UPROPERTY(meta=(BindWidget))
    UDynamicEntryBox* mButtons; // buttons dynamically added for the message box options (e.g. Confirm/Decline/Save/Save&Exit etc.)
};
