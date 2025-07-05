#pragma once

#include "MKUI_StructTypes.generated.h"

/**
 * This struct represents in what conditions the entry option in the options menu can
 * be modified. If the conditions are met, the entry can be modified, otherwise it can't.
 * Example: if the screen resolution set to borderless window, it will take up the whole
 * screen and will be set to the resolution of the screen (not sure if this is correct usage
 * in this case because i think borderless doesn't necessarily have to be max resolution of the screen)
 */
USTRUCT()
struct FOptionsDataEditConditionDescriptor
{
    GENERATED_BODY()
public:
    void setmEditConditionFunc(const TFunction<bool()>& func) { mEditConditionFunc = func; };
    bool isValid() const { return mEditConditionFunc != nullptr; }
    bool isEditConditionMet() const { return isValid() ? mEditConditionFunc() : true; }

    FString getmDisabledRichStringReason() const { return mDisabledRichStringReason; }
    void setmDisabledRichStringReason(const FString& reasonStr) { mDisabledRichStringReason = reasonStr; }

    bool hasForcedStringValueSet() const { return mDisabledForcedStringValue.IsSet(); }
    FString getmDisabledForcedStringValue() const { return mDisabledForcedStringValue.GetValue(); }
    void setmDisabledForcedStringValue(const FString& disabledForcedVal) { mDisabledForcedStringValue = disabledForcedVal; }
private:
    TFunction<bool()> mEditConditionFunc;
    FString mDisabledRichStringReason;
    // there may be an enforced value in case the option is disabled (like in the resolution example in struct description)
    TOptional<FString> mDisabledForcedStringValue;
};