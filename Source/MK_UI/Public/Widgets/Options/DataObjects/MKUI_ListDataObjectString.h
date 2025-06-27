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

    FORCEINLINE const TArray<FText>& getmAvailableOptionsDisplayText() const { return mAvailableOptionsDisplayText; }
    FORCEINLINE FText getmCurrentDisplayText() const { return mCurrentDisplayText; }
protected:
    virtual void onDataObjectInitialized() override;
    virtual bool canResetBackToDefaultValue() const override;
    virtual bool tryResetBackToDefaultValue() override;

    bool trySetCurrentTextFromStringValue(const FString& stringVal);
    
    FString mCurrentValueString; // current string value of the chosen option
    FText mCurrentDisplayText; // the display text of the current chosen option value

    /* on array below we assume that the string value at index N of string array
     * represents the display text at index N of display names array*/
    TArray<FString> mAvailableOptionsStrings; // holds the string values of available options - used to store saved settings to config file 
    TArray<FText> mAvailableOptionsDisplayText; // the display texts of the option values 

};
