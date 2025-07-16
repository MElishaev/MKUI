// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUITypes/MKUIEnumTypes.h"
#include "MKUITypes/MKUI_StructTypes.h"
#include "UObject/Object.h"
#include "MKUI_ListDataObjectBase.generated.h"

class UTexture2D;

/**
 * This class is a container for everything needed to be able to display the data for the
 * setting entry in the options menu.
 * This class instantiated in the OptionsDataRegistry in each of the options tab initializations. 
 */
UCLASS(Abstract)
class MK_UI_API UMKUI_ListDataObjectBase : public UObject
{
    GENERATED_BODY()

public:
    // todo: could this be done with MVVM instead of delegate bindings?
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModified, UMKUI_ListDataObjectBase*, EOptionsListDataModifiedReason);
    FOnListDataModified onListDataModified; // delegate that broadcast when this data object's data modified
    FOnListDataModified onDependencyDataModified; // delegate that broadcast when a dependency of this data object's data is modified

    // dispatches the call for specific initialization done by child class
    void initDataObject();

    // empty in the base class. Child class should override it and return all child data a tab has
    virtual TArray<UMKUI_ListDataObjectBase*> getAllChildListData() const { return {}; }
    virtual bool hasAnyChildData() const { return false; }

    FORCEINLINE FName getmDataId() const { return mDataId; }
    FORCEINLINE void setmDataId(FName id) { mDataId = id; }

    FORCEINLINE FText getmDataDisplayName() const { return mDataDisplayText; }
    FORCEINLINE void setmDataDisplayName(const FText& name) { mDataDisplayText = name; }

    FORCEINLINE FText getmDescriptionRichText() const { return mDescriptionRichText; }
    FORCEINLINE void setmDescriptionRichText(const FText& desc) { mDescriptionRichText = desc; }

    FORCEINLINE FText getmDisabledRichText() const { return mDisabledRichText; }
    FORCEINLINE void setmDisabledRichText(const FText& disabledText) { mDisabledRichText = disabledText; }

    FORCEINLINE TSoftObjectPtr<UTexture2D> getmSoftDescriptionImage() const { return mSoftDescriptionImage; }
    FORCEINLINE void setmSoftDescriptionImage(const TSoftObjectPtr<UTexture2D>& softTexPtr) { mSoftDescriptionImage = softTexPtr; }

    FORCEINLINE UMKUI_ListDataObjectBase* getmParentData() const { return mParentData; }
    FORCEINLINE void setmParentData(UMKUI_ListDataObjectBase* parentData) { mParentData = parentData; }

    FORCEINLINE void setmbShouldApplySettingImmediately(bool val) { mbShouldApplySettingImmediately = val; }

    /**
     * These methods related to the ability to reset options back to default.
     * Child classes should provide implementation for these.
     */
    // returns true if default value was assigned using the TOptional<> object
    virtual bool hasDefaultValue() const { return false; }
    // returns true if the setting has default value setup and the current value is not the default, otherwise false
    virtual bool canResetBackToDefaultValue() const { return false; }
    // if canResetBackToDefaultValue() returns true - this method will set the value to default, update the UI, and notify to store in config file
    virtual bool tryResetBackToDefaultValue() { return false; }

    void addEditCondition(const FOptionsDataEditConditionDescriptor& editCondition);
    bool isDataEditable();
    
    // used to add other data object that this object should depend on such that if the dependency changes it should affect this data object
    void addDataDependency(UMKUI_ListDataObjectBase* dataDependency);
protected:
    // empty in base class. should be overriden by child classes for initialization
    virtual void onDataObjectInitialized() {};

    // should be called when the data in the child class was changed
    virtual void notifyDataModified(UMKUI_ListDataObjectBase* data,
                                    EOptionsListDataModifiedReason reason = EOptionsListDataModifiedReason::DirectlyModified);

    // parent class returns false by default - child classes should override these function
    virtual bool canSetToForcedStringValue(const FString& value) const { return false; };
    virtual void onSetToForcedStringValue(const FString& value) {};

    // logic that handles the effect of dependency data modified - overriden by child classes
    virtual void handleDependencyDataModified(UMKUI_ListDataObjectBase* modifiedDependency,
                                              EOptionsListDataModifiedReason reason);
    
    void applySettingsManually();

private:
    FName mDataId;
    FText mDataDisplayText; // todo: is this needed? why i have this and also in the string representing subclass also a displayText?? 
    FText mDescriptionRichText;
    FText mDisabledRichText; // displayed when a specific setting is disabled for example
    TSoftObjectPtr<UTexture2D> mSoftDescriptionImage;

    // if true, all the setting values located in UMKUI_GameUserSettings written immediately to config file.
    // should be careful with enabling it for values that can change every frame, like sliders
    bool mbShouldApplySettingImmediately = false; 

    UPROPERTY(Transient)
    UMKUI_ListDataObjectBase* mParentData;
    
    UPROPERTY(Transient)
    TArray<FOptionsDataEditConditionDescriptor> mEditConditions; // stores the conditions that needs to be met to edit this entry option
};
