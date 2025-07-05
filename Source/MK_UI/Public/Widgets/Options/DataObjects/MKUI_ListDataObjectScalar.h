// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListDataObjectValue.h"
#include "CommonNumericTextBlock.h"
#include "MKUI_ListDataObjectScalar.generated.h"

/**
 * 
 */
UCLASS()
class MK_UI_API UMKUI_ListDataObjectScalar : public UMKUI_ListDataObjectValue
{
    GENERATED_BODY()
public:
    FORCEINLINE void setmDisplayValueRange(const TRange<float>& val) { mDisplayValueRange =  val; }
    FORCEINLINE void setmOutputValueRange(const TRange<float>& val) { mOutputValueRange =  val; }
    FORCEINLINE void setmSliderStepSize(const float val) { mSliderStepSize =  val; }
    FORCEINLINE void setmDisplayNumericType(const ECommonNumericType val) { mDisplayNumericType =  val; }
    FORCEINLINE void setmNumberFormattingOptions(const FCommonNumberFormattingOptions& val) { mNumberFormattingOptions =  val; }

    FORCEINLINE TRange<float> getmDisplayValueRange() const { return mDisplayValueRange; }
    FORCEINLINE TRange<float> getmOutputValueRange() const { return mOutputValueRange; }
    FORCEINLINE float getmSliderStepSize() const { return mSliderStepSize; }
    FORCEINLINE ECommonNumericType getmDisplayNumericType() { return mDisplayNumericType; }
    FORCEINLINE FCommonNumberFormattingOptions getmNumberFormattingOptions() const { return mNumberFormattingOptions; }

    float getCurrentValue() const;
    void setCurrentValueFromSlider(float val);
    void handleSliderMouseCaptureEnd();
    
    static FCommonNumberFormattingOptions noDecimal();
    static FCommonNumberFormattingOptions withDecimal(int32 precision); // precision indicate how many digits after .
    
private:
    float stringToFloat(const FString& str) const;
    virtual bool canResetBackToDefaultValue() const override;
    virtual bool tryResetBackToDefaultValue() override;

    
    TRange<float> mDisplayValueRange = TRange<float>(0.f, 1.f); // range to display for the user
    TRange<float> mOutputValueRange = TRange<float>(0.f, 1.f); // for when UE internal ranges are different from the display, we need a mapping between them
    float mSliderStepSize = 0.1f;
    ECommonNumericType mDisplayNumericType = ECommonNumericType::Number;
    FCommonNumberFormattingOptions mNumberFormattingOptions;
};
