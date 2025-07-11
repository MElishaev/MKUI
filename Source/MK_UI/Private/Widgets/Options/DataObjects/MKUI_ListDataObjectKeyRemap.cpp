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

bool UMKUI_ListDataObjectKeyRemap::hasDefaultValue() const
{
    return getOwningKeyMapping()->GetDefaultKey().IsValid();
}

bool UMKUI_ListDataObjectKeyRemap::canResetBackToDefaultValue() const
{
    return hasDefaultValue() && getOwningKeyMapping()->IsCustomized();
}

bool UMKUI_ListDataObjectKeyRemap::tryResetBackToDefaultValue()
{
    // todo - what happens if the key to reset to is already bound?
    if (canResetBackToDefaultValue()) {
        check(mCachedUserSettings);
        getOwningKeyMapping()->ResetToDefault();
        mCachedUserSettings->SaveSettings();
        notifyDataModified(this, EOptionsListDataModifiedReason::ResetToDefaults);
        return true;
    }
    return false;
}

TTuple<bool, FText> UMKUI_ListDataObjectKeyRemap::isKeyAlreadyBound(const FKey& keyToCheck) const
{
    check(mCachedMappableKeyProfile);

    // get all the mappings, and for each mapping run over all mapped keys to see if key already mapped to another action
    for (const auto& mappingRowPair : mCachedMappableKeyProfile->GetPlayerMappingRows()) {
        for (const auto& playerKeyMapping : mappingRowPair.Value.Mappings) {
            if ((playerKeyMapping.GetMappingName() != mCachedMappingName) && (playerKeyMapping.GetCurrentKey() == keyToCheck)) {
                MKUI_Debug::print(FString::Printf(TEXT("%s is already rebount to %s"),
                                                  *(keyToCheck.GetDisplayName().ToString()),
                                                  *(playerKeyMapping.GetDisplayName().ToString())));
                return {true, playerKeyMapping.GetDisplayName()};
            }
        }
    }

    MKUI_Debug::print(FString::Printf(TEXT("%s is free for binding"), *(keyToCheck.GetDisplayName().ToString())));
    return {false, FText()};
}
