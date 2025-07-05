// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListDataObjectValue.h"
#include "MKUI_ListDataObjectString.generated.h"

/**
 * This class represents the data we see of ONE of the setting options that are represented by string.
 * With this class we can also store the current settings the user chosen and load back when needed.
 */
UCLASS()
class MK_UI_API UMKUI_ListDataObjectString : public UMKUI_ListDataObjectValue
{
    GENERATED_BODY()
public:
    void addOptionValue(const FString& stringVal, const FText& displayText);

    /* Methods to control the advancement of the rotator between options */
    void advanceToNextOption();
    void advanceToPrevOption();

    
    void onRotatorInitiatedValueChange(const FText& newSelectedText);

    FORCEINLINE const TArray<FText>& getmAvailableOptionsDisplayText() const { return mAvailableOptionsDisplayText; }
    FORCEINLINE FText getmCurrentDisplayText() const { return mCurrentDisplayText; }
protected:
    virtual void onDataObjectInitialized() override;
    virtual bool canResetBackToDefaultValue() const override;
    virtual bool tryResetBackToDefaultValue() override;
    virtual bool canSetToForcedStringValue(const FString& value) const override;
    virtual void onSetToForcedStringValue(const FString& value) override;

    bool trySetCurrentTextFromStringValue(const FString& stringVal);
    
    FString mCurrentValueString; // current string value of the chosen option
    FText mCurrentDisplayText; // the display text of the current chosen option value

    /* on array below we assume that the string value at index N of string array
     * represents the display text at index N of display names array*/
    TArray<FString> mAvailableOptionsStrings; // holds the string values of available options - used to store saved settings to config file 
    TArray<FText> mAvailableOptionsDisplayText; // the display texts of the option values 

};


UCLASS()
class MK_UI_API UMKUI_ListDataObjectStringBool : public UMKUI_ListDataObjectString
{
    GENERATED_BODY()
public:
    /** use this when you want the "true/false" value to be displayed in other way than "ON" - like "Allow","Yes","Enable" etc.
     * The override is called before this onDataObjectInitialized() is called, which is done after this data object is added
     * to a specific collection using UMKUI_ListDataObjectCollection::addListDataObject() which triggers the initDataObject=>onDataObjectInitialized
     */
    void overrideTrueDisplayText(const FText& newDisplayText);
    void overrideFalseDisplayText(const FText& newDisplayText);

    void setTrueAsDefaultValue();
    void setFalseAsDefaultValue();
    
protected:
    virtual void onDataObjectInitialized() override;
    
private:
    // this method will try to initialized the text options if not already overriden by the override methods before adding this instance to UMKUI_ListDataObjectCollection 
    void tryInitBoolValues();
    
    const FString mTrueString = TEXT("true");
    const FString mFalseString = TEXT("false");    
};


UCLASS()
class MK_UI_API UMKUI_ListDataObjectStringEnum : public UMKUI_ListDataObjectString
{
    GENERATED_BODY()
public:

    template<typename EnumType>
    void addEnumOption(EnumType enumOption, const FText& optionDisplayText)
    {
        const UEnum* enumOptionClass = StaticEnum<EnumType>();
        const FString convertedEnum = enumOptionClass->GetNameStringByValue(enumOption);
        addOptionValue(convertedEnum, optionDisplayText);
    }

    template<typename EnumType>
    EnumType getCurrentValueAsEnum() const
    {
        const UEnum* enumOptionClass = StaticEnum<EnumType>();
        return static_cast<EnumType>(enumOptionClass->GetValueByNameString(mCurrentValueString));        
    }

    template<typename EnumType>
    void setDefaultValueFromEnumOption(EnumType enumOption)
    {
        const UEnum* enumOptionClass = StaticEnum<EnumType>();
        const FString convertedEnum = enumOptionClass->GetNameStringByValue(enumOption);
        setDefaultValueFromString(convertedEnum);                
    }
};