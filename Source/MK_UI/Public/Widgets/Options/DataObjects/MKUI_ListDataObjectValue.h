// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListDataObjectBase.h"
#include "MKUI_ListDataObjectValue.generated.h"

class MKUI_FOptionsDataInteractionHelper;
/**
 * This class represents the value of a setting in the option menu.
 * This class will act as a base class for different values possible in an options menu.
 */
UCLASS(Abstract)
class MK_UI_API UMKUI_ListDataObjectValue : public UMKUI_ListDataObjectBase
{
    GENERATED_BODY()

public:
    void setmDataDynamicGetter(const TSharedPtr<MKUI_FOptionsDataInteractionHelper>& dynamicGetter) { mDataDynamicGetter = dynamicGetter; }
    void setmDataDynamicSetter(const TSharedPtr<MKUI_FOptionsDataInteractionHelper>& dynamicSetter) { mDataDynamicSetter = dynamicSetter; }

    void setDefaultValueFromString(const FString& defaultVal) { mDefaultStringValue = defaultVal; }
    virtual bool hasDefaultValue() const override { return mDefaultStringValue.IsSet(); };
    
protected:
    /* Below members store the path to the getters and setters for the value in the "Game Settings" class
     * which role is to store the modified values in config file and apply the settings in game */
    TSharedPtr<MKUI_FOptionsDataInteractionHelper> mDataDynamicGetter;
    TSharedPtr<MKUI_FOptionsDataInteractionHelper> mDataDynamicSetter;
    
    FString getDefaultValueAsString() const { return mDefaultStringValue.GetValue(); }

private:
    // we use here string because it is easily convertible value (e.g. string->int/float and vise-versa)
    TOptional<FString> mDefaultStringValue;

};
