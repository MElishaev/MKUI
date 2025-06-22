// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MKUI_ListDataObjectBase.generated.h"

#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
    FORCEINLINE DataType get##PropertyName() const { return PropertyName; } \
    void set##PropertyName(DataType data) { PropertyName = data; }

/**
 * 
 */
UCLASS()
class MK_UI_API UMKUI_ListDataObjectBase : public UObject
{
    GENERATED_BODY()
public:
    void initDataObject();
    
    // empty in the base class. Child class should override it and return all child data a tab has
    virtual TArray<UMKUI_ListDataObjectBase*> getAllChildListData() const { return {}; }
    virtual bool hasAnyChildData() const { return false; }
    
    LIST_DATA_ACCESSOR(FName, mDataId);
    LIST_DATA_ACCESSOR(FText, mDataDisplayName);
    LIST_DATA_ACCESSOR(FText, mDescriptionRichText);
    LIST_DATA_ACCESSOR(FText, mDisabledRichText);
    LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, mSoftDescriptionImage);
    LIST_DATA_ACCESSOR(UMKUI_ListDataObjectBase*, mParentData);

protected:

    // empty in base class. should be overriden by child classes for initialization
    virtual void onDataObjectInitialized() {};
    
private:
    FName mDataId;
    FText mDataDisplayName;
    FText mDescriptionRichText;
    FText mDisabledRichText; // displayed when a specific setting is disabled for example
    TSoftObjectPtr<UTexture2D> mSoftDescriptionImage;

    UPROPERTY(Transient)
    UMKUI_ListDataObjectBase* mParentData;
};
