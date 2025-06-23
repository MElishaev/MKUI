// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "MKUI_ListEntryBase.generated.h"

enum class EOptionsListDataModifiedReason : uint8;
class UMKUI_ListDataObjectBase;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_ListEntryBase : public UCommonUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

protected:
    virtual void NativeOnListItemObjectSet(UObject* listItemObject) override;

    // Child class should override this method to implement initialization. Super call is expected.
    // This is called when the data object that will be represented by this entry widget was set.
    virtual void onOwningListDataObjectSet(UMKUI_ListDataObjectBase* listDataObject);

    // Child class should override this method to implement the modification of what that widget displays
    virtual void onOwningListDataObjectModified(UMKUI_ListDataObjectBase* listDataObject, EOptionsListDataModifiedReason reason);
private:
    /******** bound widgets **********/
    UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
    UCommonTextBlock* mSettingDisplayName;
    /******** bound widgets **********/
};
