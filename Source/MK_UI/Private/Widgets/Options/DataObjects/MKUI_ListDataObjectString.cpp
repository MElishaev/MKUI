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
    // updates the game user settings to store the value for this setting
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
    // updates the game user settings to store the value for this setting
    if (mDataDynamicSetter) {
        mDataDynamicSetter->setValueFromString(mCurrentValueString);
        notifyDataModified(this);
    }
}

void UMKUI_ListDataObjectString::onRotatorInitiatedValueChange(const FText& newSelectedText)
{
    const int32 index = mAvailableOptionsDisplayText.IndexOfByPredicate([newSelectedText](const FText& availableText) {
        return availableText.EqualTo(newSelectedText);
    });

    if (index != INDEX_NONE && mAvailableOptionsStrings.IsValidIndex(index)) {
        mCurrentDisplayText = newSelectedText;
        mCurrentValueString = mAvailableOptionsStrings[index];

        if (mDataDynamicSetter) {
            mDataDynamicSetter->setValueFromString(mCurrentValueString);
            notifyDataModified(this);
        }
    }
}

void UMKUI_ListDataObjectString::onDataObjectInitialized()
{
    if (hasDefaultValue()) {
        mCurrentValueString = getDefaultValueAsString();
    }

    // read the values from a config file instead of resetting them every time the game starts up
    if (mDataDynamicGetter && !mDataDynamicGetter->getValueAsString().IsEmpty()) {
        mCurrentValueString = mDataDynamicGetter->getValueAsString();
    }
    
    if (!trySetCurrentTextFromStringValue(mCurrentValueString)) {
        mCurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
    }
}

bool UMKUI_ListDataObjectString::canResetBackToDefaultValue() const
{
    return hasDefaultValue() && (mCurrentValueString != getDefaultValueAsString());
}

bool UMKUI_ListDataObjectString::tryResetBackToDefaultValue()
{
    if (canResetBackToDefaultValue()) {
        mCurrentValueString = getDefaultValueAsString();
        trySetCurrentTextFromStringValue(mCurrentValueString);
        if (mDataDynamicSetter) {
            mDataDynamicSetter->setValueFromString(mCurrentValueString);
            notifyDataModified(this, EOptionsListDataModifiedReason::ResetToDefaults);
            return true;
        }
    }
    return false;
}

bool UMKUI_ListDataObjectString::canSetToForcedStringValue(const FString& value) const
{
    return mCurrentValueString != value;
}

void UMKUI_ListDataObjectString::onSetToForcedStringValue(const FString& value)
{
    mCurrentValueString = value;
    trySetCurrentTextFromStringValue(mCurrentValueString);
    if (mDataDynamicSetter) {
        mDataDynamicSetter->setValueFromString(mCurrentValueString);
        notifyDataModified(this, EOptionsListDataModifiedReason::DependencyModified);
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

/********************************************** STRING BOOL CLASS ***************************************************************/

void UMKUI_ListDataObjectStringBool::overrideTrueDisplayText(const FText& newDisplayText)
{
    if (!mAvailableOptionsStrings.Contains(mTrueString)) {
        addOptionValue(mTrueString, newDisplayText);
    }
}

void UMKUI_ListDataObjectStringBool::overrideFalseDisplayText(const FText& newDisplayText)
{
    if (!mAvailableOptionsStrings.Contains(mFalseString)) {
        addOptionValue(mFalseString, newDisplayText);
    }
}

void UMKUI_ListDataObjectStringBool::setTrueAsDefaultValue()
{
    setDefaultValueFromString(mTrueString);
}

void UMKUI_ListDataObjectStringBool::setFalseAsDefaultValue()
{
    setDefaultValueFromString(mFalseString);
}

void UMKUI_ListDataObjectStringBool::onDataObjectInitialized()
{
    // this should be called before super because super depends on this when trying to set display text from string value
    tryInitBoolValues();

    Super::onDataObjectInitialized();
}

void UMKUI_ListDataObjectStringBool::tryInitBoolValues()
{
    if (!mAvailableOptionsStrings.Contains(mTrueString)) {
        addOptionValue(mTrueString, FText::FromString(TEXT("ON")));
    }

    if (!mAvailableOptionsStrings.Contains(mFalseString)) {
        addOptionValue(mFalseString, FText::FromString(TEXT("OFF")));
    }
}


/********************************************** STRING INTEGER CLASS ***************************************************************/

void UMKUI_ListDataObjectStringInteger::addIntegerOption(int32 val, const FText& displayText)
{
    addOptionValue(LexToString(val), displayText);
}

void UMKUI_ListDataObjectStringInteger::onDataObjectInitialized()
{
    Super::onDataObjectInitialized();

    if (!trySetCurrentTextFromStringValue(mCurrentValueString)) {
        mCurrentDisplayText = FText::FromString(TEXT("Custom"));
    }
}

void UMKUI_ListDataObjectStringInteger::handleDependencyDataModified(UMKUI_ListDataObjectBase* modifiedDependency,
                                                                     EOptionsListDataModifiedReason reason)
{
    if (mDataDynamicGetter) {
        if (mCurrentValueString == mDataDynamicGetter->getValueAsString()) {
            return;
        }
        // if dependency change (e.g. "overall quality" change to EPIC) then this dependent data object should
        // change to the same value. NOTE:::: it seems like this will work only for specifically the overall
        // quality setting because this UE setting internally encapsulates and modifies all associated values to it.
        // this means that using here below the getter will give us the correct value because it was changed by
        // changing the overall quality, but for other dependencies, this needs to be manually implemented.
        // like in the screen resolution and window mode.
        mCurrentValueString = mDataDynamicGetter->getValueAsString();
        if (!trySetCurrentTextFromStringValue(mCurrentValueString)) {
            mCurrentDisplayText = FText::FromString(TEXT("Custom"));
        }

        notifyDataModified(this, reason);
    }
    Super::handleDependencyDataModified(modifiedDependency, reason); // call super for broadcasting the change to the UI entry widget
}
