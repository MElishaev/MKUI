// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUITypes/MKUIEnumTypes.h"
#include "UObject/Object.h"
#include "MKUI_ListDataObjectBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MK_UI_API UMKUI_ListDataObjectBase : public UObject
{
    GENERATED_BODY()

public:
    // todo: could this be done with MVVM instead of delegate bindings?
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModified, UMKUI_ListDataObjectBase*, EOptionsListDataModifiedReason);
    FOnListDataModified onListDataModified;

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
    FORCEINLINE void setmSoftDescriptionImage(TSoftObjectPtr<UTexture2D> softTexPtr) { mSoftDescriptionImage = softTexPtr; }

    FORCEINLINE UMKUI_ListDataObjectBase* getmParentData() const { return mParentData; }
    FORCEINLINE void setmParentData(UMKUI_ListDataObjectBase* parentData) { mParentData = parentData; }

protected:
    // empty in base class. should be overriden by child classes for initialization
    virtual void onDataObjectInitialized() {};

    // should be called when the data in the child class was changed
    virtual void notifyDataModified(UMKUI_ListDataObjectBase* data,
                                    EOptionsListDataModifiedReason reason = EOptionsListDataModifiedReason::DirectlyModified);

private:
    FName mDataId;
    FText mDataDisplayText; // todo: is this needed? why i have this and also in the string representing subclass also a displayText?? 
    FText mDescriptionRichText;
    FText mDisabledRichText; // displayed when a specific setting is disabled for example
    TSoftObjectPtr<UTexture2D> mSoftDescriptionImage;

    UPROPERTY(Transient)
    UMKUI_ListDataObjectBase* mParentData;
};
