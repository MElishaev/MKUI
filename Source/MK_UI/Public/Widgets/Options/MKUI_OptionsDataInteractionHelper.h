// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"
class UMKUI_GameUserSettings;
/**
 * 
 */
class MK_UI_API MKUI_FOptionsDataInteractionHelper
{
public:
    MKUI_FOptionsDataInteractionHelper(const FString& funcPath);

    FString getValueAsString() const;
    void setValueFromString(const FString& value);
    
private:
    FCachedPropertyPath mCachedDynamicFunctionPath; // stores the path to the getter or setter functions
    TWeakObjectPtr<UMKUI_GameUserSettings> mCachedWeakGameUserSettings;
};
