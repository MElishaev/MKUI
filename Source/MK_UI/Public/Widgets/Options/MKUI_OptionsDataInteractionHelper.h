// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"
class UMKUI_GameUserSettings;
/**
 * This class holds a weak ptr for the game user settings and a function path
 * of one of the getters or setters of the game user settings.
 * It exposes access to the variables inside the game user settings hierarchy.
 */
class MK_UI_API MKUI_FOptionsDataInteractionHelper
{
public:
    MKUI_FOptionsDataInteractionHelper(const FString& funcPath);

    FString getValueAsString() const;

    // sets the input value to the property this helper instance represents (which is a setter for a property in MKUI user settings class).
    // this only modifies the values in the class but not yet writes or applies them? (not sure if the Config specifier somehow
    // forces a write on each change of value but whatever)
    void setValueFromString(const FString& value);
    
private:
    FCachedPropertyPath mCachedDynamicFunctionPath; // stores the path to the getter or setter functions
    TWeakObjectPtr<UMKUI_GameUserSettings> mCachedWeakGameUserSettings;
};
