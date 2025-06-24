// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_OptionsDataInteractionHelper.h"

#include "Settings/MKUI_GameUserSettings.h"

MKUI_FOptionsDataInteractionHelper::MKUI_FOptionsDataInteractionHelper(const FString& funcPath) : mCachedDynamicFunctionPath(funcPath)
{
    mCachedWeakGameUserSettings = UMKUI_GameUserSettings::getInstance();
}

FString MKUI_FOptionsDataInteractionHelper::getValueAsString() const
{
    FString value;
    // isn't it dangerous to extract a raw pointer from a smart pointer and use it like below? 
    //  the answer for this is that the pointer is to GameUserSettings, which seems to be an instance that is available throughout
    //  the life time of the game instance. Moreover, this ptr only used to access some property and not to be stored inside,
    //  so it needs to be valid only for the lifetime of this function call which minuscule, and anyway, like said before
    //  about the type of object that this ptr stores - game instance lifetime.
    PropertyPathHelpers::GetPropertyValueAsString(mCachedWeakGameUserSettings.Get(), mCachedDynamicFunctionPath, value);
    return value;
}

void MKUI_FOptionsDataInteractionHelper::setValueFromString(const FString& value)
{
    PropertyPathHelpers::SetPropertyValueFromString(mCachedWeakGameUserSettings.Get(), mCachedDynamicFunctionPath, value);
}
