// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectScalar.h"
#include "Widgets/Options/MKUI_OptionsDataInteractionHelper.h"

float UMKUI_ListDataObjectScalar::getCurrentValue() const
{
    if (mDataDynamicGetter) {
        return FMath::GetMappedRangeValueClamped(mOutputValueRange,
                                                 mDisplayValueRange,
                                                 stringToFloat(mDataDynamicGetter->getValueAsString()));
    }
    return 0.f;
}

void UMKUI_ListDataObjectScalar::setCurrentValueFromSlider(float val)
{
    if (mDataDynamicSetter) {
        const float clampedVal = FMath::GetMappedRangeValueClamped(mDisplayValueRange, mOutputValueRange, val);
        mDataDynamicSetter->setValueFromString(LexToString(clampedVal));
        notifyDataModified(this);
    }
}

FCommonNumberFormattingOptions UMKUI_ListDataObjectScalar::noDecimal()
{
    FCommonNumberFormattingOptions options;
    options.MaximumFractionalDigits = 0;
    return options;
}

FCommonNumberFormattingOptions UMKUI_ListDataObjectScalar::withDecimal(int32 precision)
{
    FCommonNumberFormattingOptions options;
    options.MaximumFractionalDigits = precision;

    return options;
}

float UMKUI_ListDataObjectScalar::stringToFloat(const FString& str) const
{
    float val = 0;
    LexFromString(val, str);
    return val;
}
