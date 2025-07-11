// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectKeyRemap.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "MKUI_DebugHelper.h"
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

void UMKUI_ListDataObjectKeyRemap::bindNewInputKey(const FKey& newKey)
{
    check(mCachedUserSettings);

    FMapPlayerKeyArgs keyArgs;
    keyArgs.MappingName = mCachedMappingName;
    keyArgs.Slot = mCachedMappableKeySlot;
    keyArgs.NewKey = newKey;
    FGameplayTagContainer container;
    
    mCachedUserSettings->MapPlayerKey(keyArgs, container);
    mCachedUserSettings->SaveSettings();

    notifyDataModified(this);
}

FSlateBrush UMKUI_ListDataObjectKeyRemap::getIconFromCurrentKey() const
{
    FSlateBrush foundBrush;
    auto commonInputSubsystem = UCommonInputSubsystem::Get(mCachedUserSettings->GetLocalPlayer());
    check(commonInputSubsystem);
    const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(foundBrush,
                                                                                      getOwningKeyMapping()->GetCurrentKey(),
                                                                                      mCachedDesiredInputKeyType,
                                                                                      commonInputSubsystem->GetCurrentGamepadName());
    if (!bHasFoundBrush) {
        MKUI_Debug::print(TEXT("Unable to find an icon for the key ") + getOwningKeyMapping()->GetMappingName().ToString());
    }

    return foundBrush;
}

FPlayerKeyMapping* UMKUI_ListDataObjectKeyRemap::getOwningKeyMapping() const
{
    check(mCachedMappableKeyProfile);

    FMapPlayerKeyArgs keyArgs;
    keyArgs.MappingName = mCachedMappingName;
    keyArgs.Slot = mCachedMappableKeySlot;

    return mCachedMappableKeyProfile->FindKeyMapping(keyArgs);
}
