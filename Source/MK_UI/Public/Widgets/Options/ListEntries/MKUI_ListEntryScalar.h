// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListEntryBase.h"
#include "MKUI_ListEntryScalar.generated.h"

class UMKUI_ListDataObjectScalar;
class UCommonNumericTextBlock;
class UAnalogSlider;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_ListEntryScalar : public UMKUI_ListEntryBase
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;
    virtual void onOwningListDataObjectSet(UMKUI_ListDataObjectBase* listDataObject) override;
    virtual void onOwningListDataObjectModified(UMKUI_ListDataObjectBase* listDataObject, EOptionsListDataModifiedReason reason) override;
    
private:
    UFUNCTION()
    void handleSliderValueChanged(float value);

    UFUNCTION()
    void handleSliderMouseCaptureBegin();
    
    UFUNCTION()
    void handleSliderMouseCaptureEnd();
    
    /************** Bound widgets ****************/
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
    UCommonNumericTextBlock* mSettingValue;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
    UAnalogSlider* mSettingSlider;
    /************** Bound widgets ****************/

    UPROPERTY(Transient)
    UMKUI_ListDataObjectScalar* mCachedDataObject;
};
