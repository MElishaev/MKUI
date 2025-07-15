// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListEntryBase.h"
#include "MKUI_ListEntryKeyRemap.generated.h"

class UMKUI_ListDataObjectKeyRemap;
class UMKUI_CommonButtonBase;
/**
 * This class represents the entry widget of the key bindings entries in the options menu
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_ListEntryKeyRemap : public UMKUI_ListEntryBase
{
    GENERATED_BODY()

protected:
    virtual void onOwningListDataObjectSet(UMKUI_ListDataObjectBase* listDataObject) override;

    virtual void handleOwningListDataObjectModified(UMKUI_ListDataObjectBase* listDataObject, EOptionsListDataModifiedReason reason) override;

    virtual void NativeOnInitialized() override;

    
private:
    void handleKeyRemapButtonClicked();
    void handleResetKeyBindingButtonClicked();

    void handleKeyToRemapPressed(const FKey& pressedKey);
    void handleKeyRemapCanceled(const FString& cancelReason);

    // returns key remap data object that represents the input key, if not found returns nullptr
    UMKUI_ListDataObjectKeyRemap* findDataObjectKeyRemapByKey(const FKey& key) const;

    void showBindConfirmationScreen(const FKey& keyToBind);
    
    /******** bound widgets **********/
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"), Category="MKUI")
    UMKUI_CommonButtonBase* mRemapKey;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"), Category="MKUI")
    UMKUI_CommonButtonBase* mResetKeyMapping;
    /******** bound widgets **********/
    
    UPROPERTY(Transient)
    UMKUI_ListDataObjectKeyRemap* mCachedOwningKeyRemapDataObject;
};
