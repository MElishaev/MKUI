// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectScalar.h"
#include "Widgets/Options/MKUI_OptionsDataInteractionHelper.h"

float UMKUI_ListDataObjectScalar::getCurrentValue() const
{
    if (mDataDynamicGetter) {
        return FMath::GetMappedRangeValueClamped(mOutputValueRange,
                                                 mDisplayValueRange,
                                                 stringToFloat(mDataDynamicGetter->getValueAsString()));
        // return stringToFloat(mDataDynamicGetter->getValueAsString());
    }
    return 0.f;
}

void UMKUI_ListDataObjectScalar::setCurrentValueFromSlider(float val)
{
    if (mDataDynamicSetter) {
        const float clampedVal = FMath::GetMappedRangeValueClamped(mDisplayValueRange, mOutputValueRange, val);
        // const float clampedVal = FMath::Clamp(val, mDisplayValueRange.GetLowerBoundValue(), mDisplayValueRange.GetUpperBoundValue());
        mDataDynamicSetter->setValueFromString(LexToString(clampedVal));
        notifyDataModified(this); // todo- the notify here should be moved to the event where mouse capture of slider end 
    }
}

void UMKUI_ListDataObjectScalar::handleSliderMouseCaptureEnd()
{
    applySettingsManually();
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

bool UMKUI_ListDataObjectScalar::canResetBackToDefaultValue() const
{
    if (hasDefaultValue() && mDataDynamicGetter) {
        const float defaultVal = stringToFloat(getDefaultValueAsString());
        const float currVal = stringToFloat(mDataDynamicGetter->getValueAsString());
        return !FMath::IsNearlyEqual(defaultVal, currVal, 0.01f);
    }
    return false;
}

bool UMKUI_ListDataObjectScalar::tryResetBackToDefaultValue()
{
    if (canResetBackToDefaultValue() && mDataDynamicSetter) {
        mDataDynamicSetter->setValueFromString(getDefaultValueAsString());
        notifyDataModified(this, EOptionsListDataModifiedReason::ResetToDefaults);
        return true;
    }
    return false;
}
