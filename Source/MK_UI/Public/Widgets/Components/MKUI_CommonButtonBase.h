// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "MKUI_CommonButtonBase.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;

/**
 * A base class for creating CommonUI buttons
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_CommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="MKUI")
    void setButtonText(const FText& text);

    UFUNCTION(BlueprintCallable, Category="MKUI")
    void setButtonImg(const FSlateBrush& img);
    
    UFUNCTION(BlueprintCallable, Category="MKUI")
    FText getButtonText() const;
    
private:
    // this function will be called on every change that is done in the WBP in editor while designing
    virtual void NativePreConstruct() override;

    virtual void NativeOnCurrentTextStyleChanged() override;

    virtual void NativeOnHovered() override;
    virtual void NativeOnUnhovered() override;
private:
    /**** Bound widgets ****/
    // these are the widgets that could be bound in the WBP under the "Bind Widget" tab that is generally near the
    // widget Hierarchy graph
    UPROPERTY(meta=(BindWidgetOptional))
    UCommonTextBlock* mCommonButtonText;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"), Category="MKUI")
    UCommonLazyImage* mButtonImg;
    /**** Bound widgets ****/

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess))
    FText mButtonDisplayText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess))
    bool mbUseUpperCaseForButtonText = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess))
    FText mButtonDescription;
};
