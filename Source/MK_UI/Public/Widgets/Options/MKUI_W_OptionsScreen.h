// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MKUI_W_ActivatableBase.h"
#include "MKUI_W_OptionsScreen.generated.h"

enum class EOptionsListDataModifiedReason : uint8;
class UMKUI_ListDataObjectBase;
class UMKUI_OptionsDetailsPanel;
class UMKUI_CommonListView;
class UMKUI_TabListWidgetBase;
class UMKUI_OptionsDataRegistry;
class UInputAction;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_W_OptionsScreen : public UMKUI_W_ActivatableBase
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    virtual void NativeOnDeactivated() override;

    virtual void NativeOnActivated() override;
    
private:
    UMKUI_OptionsDataRegistry* getDataRegistry();
    
    void onResetBoundActionTriggered();
    void onBackBoundActionTriggered();

    UFUNCTION()
    void onOptionsTabSelected(FName tabId);

    void onListViewItemHovered(UObject* hoveredItem, bool bHovered);
    void onListViewItemSelected(UObject* selectedItem);
    void onListViewDataModified(UMKUI_ListDataObjectBase* data, EOptionsListDataModifiedReason modificationReason);

    // helper method tries to return the input object class name or "invalid" if couldn't
    FString tryGetEntryWidgetClassName(UObject* owningListItem) const;
    
    /**** bound widgets ****/
    UPROPERTY(meta=(BindWidget))
    UMKUI_TabListWidgetBase* mOptionsTabList;

    UPROPERTY(meta=(BindWidget))
    UMKUI_CommonListView* mOptionsList; // holds the various settings of the currently selected options tab

    UPROPERTY(meta=(BindWidget))
    UMKUI_OptionsDetailsPanel* mListEntryDetailsPanel; // details panel that show info about the selected option
    /**** bound widgets ****/

    
    /** This member holds the Data for the various settings tab in the options/settings menu.
     * this member shouldn't be directly accessed even in this class - every interaction with it should be through getDataRegistry()
     * because this member can be deallocated and reallocated through the runtime based on need. */
    UPROPERTY(Transient)
    UMKUI_OptionsDataRegistry* mDataRegistry;
    
    // taking inspiration from how commonUI does it in the CommonUIInputData class, we will add custom bound actions
    UPROPERTY(EditDefaultsOnly, Category = "MKUI Options Screen", meta = (EditCondition = "CommonInput.CommonInputSettings.IsEnhancedInputSupportEnabled", EditConditionHides))
    TObjectPtr<UInputAction> mEnhancedInputResetAction; // reset settings input actions

    FUIActionBindingHandle mResetActionHandle; // handle for removal of the action button from the BoundActionsBar

    UPROPERTY(Transient)
    TArray<UMKUI_ListDataObjectBase*> mResettableDataArray;

    bool mbIsResettingData = false;
};
