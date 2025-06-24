// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectString.h"
#include "Widgets/Options/MKUI_OptionsDataInteractionHelper.h"

void UMKUI_ListDataObjectString::addOptionValue(const FString& stringVal, const FText& displayText)
{
    mAvailableOptionsStrings.Add(stringVal);
    mAvailableOptionsDisplayText.Add(displayText);
}

void UMKUI_ListDataObjectString::advanceToNextOption()
{
    if (mAvailableOptionsStrings.IsEmpty() || mAvailableOptionsDisplayText.IsEmpty()) {
        return;
    }

    const auto currentIndex = mAvailableOptionsStrings.IndexOfByKey(mCurrentValueString);
    const auto nextIndex = (currentIndex + 1) % mAvailableOptionsStrings.Num();
    mCurrentValueString = mAvailableOptionsStrings[nextIndex];

    trySetCurrentTextFromStringValue(mCurrentValueString);
    // updates the game user settings to store the vale for this setting
    if (mDataDynamicSetter) {
        mDataDynamicSetter->setValueFromString(mCurrentValueString);
        notifyDataModified(this);
    }
}

void UMKUI_ListDataObjectString::advanceToPrevOption()
{
    if (mAvailableOptionsStrings.IsEmpty() || mAvailableOptionsDisplayText.IsEmpty()) {
        return;
    }

    const auto currentIndex = mAvailableOptionsStrings.IndexOfByKey(mCurrentValueString);
    const auto nextIndex = currentIndex - 1 < 0 ? mAvailableOptionsStrings.Num() - 1 : currentIndex - 1;
    mCurrentValueString = mAvailableOptionsStrings[nextIndex];

    trySetCurrentTextFromStringValue(mCurrentValueString);
    // updates the game user settings to store the vale for this setting
    if (mDataDynamicSetter) {
        mDataDynamicSetter->setValueFromString(mCurrentValueString);
        notifyDataModified(this);
    }
}

void UMKUI_ListDataObjectString::onDataObjectInitialized()
{
    if (!mAvailableOptionsStrings.IsEmpty()) {
        // todo: is there a better way to choose the default values for settings?
        mCurrentValueString = mAvailableOptionsStrings[0];
    }

    // read the values from a config file instead of resetting them every time the game starts up
    if (mDataDynamicGetter && !mDataDynamicGetter->getValueAsString().IsEmpty()) {
        mCurrentValueString = mDataDynamicGetter->getValueAsString();
    }

    
    if (!trySetCurrentTextFromStringValue(mCurrentValueString)) {
        mCurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
    }
}

bool UMKUI_ListDataObjectString::trySetCurrentTextFromStringValue(const FString& stringVal)
{
    const auto index = mAvailableOptionsStrings.IndexOfByKey(stringVal);
    if (index != INDEX_NONE && mAvailableOptionsDisplayText.IsValidIndex(index)) {
        mCurrentDisplayText = mAvailableOptionsDisplayText[index];
        return true;
    }
    return false;
}
