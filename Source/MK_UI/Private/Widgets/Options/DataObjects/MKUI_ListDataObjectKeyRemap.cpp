// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectKeyRemap.h"

#include "UserSettings/EnhancedInputUserSettings.h"

void UMKUI_ListDataObjectKeyRemap::initKeyRemapData(UEnhancedInputUserSettings* inputUserSettings,
                                                    UEnhancedPlayerMappableKeyProfile* mappableKeyProfile,
                                                    ECommonInputType inputKeyType,
                                                    const FPlayerKeyMapping& keyMapping)
{
    mCachedMappingName = keyMapping.GetMappingName();
    mCachedMappableKeySlot = keyMapping.GetSlot();
    mCachedUserSettings = inputUserSettings;
    mCachedMappableKeyProfile = mappableKeyProfile;
    mCachedDesiredInputKeyType = inputKeyType;
}
