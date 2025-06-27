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
public:

    UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="onListEntryWidgetHovered"))
    void BP_onListEntryWidgetHovered(bool bHovered, bool bEntryWidgetStillSelected);
    void nativeOnListEntryWidgetHovered(bool bHovered);
protected:
    virtual void NativeOnListItemObjectSet(UObject* listItemObject) override;
    virtual void NativeOnEntryReleased() override;

    /**
     * Child blueprint should override this function for gamepad to interact properly with the options.
     * Child should return the exact widget that it wants the gamepad to control and interact with
     */
    UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Get Widget to Focus For Gamepad"))
    UWidget* BP_getWidgetToFocusForGamepad() const; 
    
    /**
     * Here we use this method to fix the focus target when using gamepad. This will fix the issues
     * where when the focus is on this list entry, moving the analog stick left/right or trying to change
     * the option value won't work because the focus is on the entry and not on the value of the option.
     */
    virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

    // Child class should override this method to implement initialization. Super call is expected.
    // This is called when the data object that will be represented by this entry widget was set.
    virtual void onOwningListDataObjectSet(UMKUI_ListDataObjectBase* listDataObject);

    // Child class should override this method to implement the modification of what that widget displays
    virtual void onOwningListDataObjectModified(UMKUI_ListDataObjectBase* listDataObject, EOptionsListDataModifiedReason reason);

    /**
     * This method comes to implement the selection of this widget no matter what was pressed on on this widget,
     * the text, the buttons, the rotator... I don't know why this matters because the selection is only visual
     * when you use mouse.
     */
    void selectThisEntryWidget();
private:
    /******** bound widgets **********/
    UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
    UCommonTextBlock* mSettingDisplayName;
    /******** bound widgets **********/
};
