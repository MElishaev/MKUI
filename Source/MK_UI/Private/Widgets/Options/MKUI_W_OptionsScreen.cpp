// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_W_OptionsScreen.h"

#include "ICommonInputModule.h"
#include "MKUI_DebugHelper.h"
#include "Input/CommonUIInputTypes.h"
#include "Settings/MKUI_GameUserSettings.h"
#include "Subsystems/MKUI_Subsystem.h"
#include "Widgets/Components/MKUI_CommonButtonBase.h"
#include "Widgets/Components/MKUI_CommonListView.h"
#include "Widgets/Components/MKUI_TabListWidgetBase.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectCollection.h"
#include "Widgets/Options/MKUI_OptionsDataRegistry.h"
#include "Widgets/Options/MKUI_OptionsDetailsPanel.h"
#include "Widgets/Options/ListEntries/MKUI_ListEntryBase.h"

void UMKUI_W_OptionsScreen::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    /* here we need custom bound actions to be added to our Options screen bar.
     * apart from the "Back" action that is provided by default with CommonUI, we need also
     * "Reset" action for resetting the options to their default state. This mean we need to
     * create manually the actions and register them to be LISTENED TO BY THIS WIDGET. 
     */

    if (mEnhancedInputResetAction) {
        const FBindUIActionArgs resetActionArgs = FBindUIActionArgs(mEnhancedInputResetAction,
                                                                    true,
                                                                    FSimpleDelegate::CreateUObject(
                                                                        this,
                                                                        &ThisClass::onResetBoundActionTriggered));

        mResetActionHandle = RegisterUIActionBinding(resetActionArgs);
    }

    const FBindUIActionArgs backActionArgs = FBindUIActionArgs(ICommonInputModule::GetSettings().GetEnhancedInputBackAction(),
                                                               true,
                                                               FSimpleDelegate::CreateUObject(
                                                                   this,
                                                                   &ThisClass::onBackBoundActionTriggered));

    RegisterUIActionBinding(backActionArgs);

    mOptionsTabList->OnTabSelected.AddUniqueDynamic(this, &UMKUI_W_OptionsScreen::onOptionsTabSelected);
    mOptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::onListViewItemHovered);  // bind callback for item hovered state
    mOptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::onListViewItemSelected); // bind callback for item hovered state
}

void UMKUI_W_OptionsScreen::NativeOnDeactivated()
{
    Super::NativeOnDeactivated();
    /** on deactivation of the options screen widget, trigger the game to
     * apply the settings and save them into the .ini config file */
    UMKUI_GameUserSettings::getInstance()->ApplySettings(true);
}

void UMKUI_W_OptionsScreen::NativeOnActivated()
{
    /** when options screen is activated, it goes over all the registered tabs in the data registry,
     * and if the data registry not created yet, it also creates it.
     * while going over the registered tab collections in the data registry, it registers each tab
     * into the "Options" tab list.
     */
    Super::NativeOnActivated();

    for (const auto tabCollection : getDataRegistry()->getRegisteredTabCollections()) {
        if (!tabCollection) {
            continue;
        }

        const FName tabId = tabCollection->getmDataId();
        if (mOptionsTabList->GetTabButtonBaseByID(tabId)) {
            continue;
        }

        mOptionsTabList->requestRegisterTabButton(tabId, tabCollection->getmDataDisplayName());
    }
}

UWidget* UMKUI_W_OptionsScreen::NativeGetDesiredFocusTarget() const
{
    if (auto selectedObj = mOptionsList->GetSelectedItem()) {
        if (auto selectedEntryWidget = mOptionsList->GetEntryWidgetFromItem(selectedObj)) {
            return selectedEntryWidget;
        }
    }
    return Super::NativeGetDesiredFocusTarget();
}

UMKUI_OptionsDataRegistry* UMKUI_W_OptionsScreen::getDataRegistry()
{
    if (!mDataRegistry) {
        mDataRegistry = NewObject<UMKUI_OptionsDataRegistry>();
        mDataRegistry->init(GetOwningLocalPlayer());
    }
    checkf(mDataRegistry, TEXT("Data registry of options menu isn't valid - this should never happen here"));
    return mDataRegistry;
}

void UMKUI_W_OptionsScreen::onResetBoundActionTriggered()
{
    // show confirmation screen for the user to make sure he want to reset the settings
    if (mResettableDataArray.IsEmpty()) {
        return;
    }

    auto currentSelectedTabButton = mOptionsTabList->GetTabButtonBaseByID(mOptionsTabList->GetActiveTab());
    auto tabName = CastChecked<UMKUI_CommonButtonBase>(currentSelectedTabButton)->getButtonText().ToString();

    UMKUI_Subsystem::getInstance(this)->pushConfirmScreenToModalStackAsync(
        EConfirmScreenType::YesNo,
        FText::FromString(TEXT("Reset settings to default")),
        FText::FromString(TEXT("Are you sure you want to reset the") + tabName + TEXT(" to default?")),
        [this](EConfirmScreenButtonType buttonType) {
            if (buttonType == EConfirmScreenButtonType::Confirmed) {
                mbIsResettingData = true;
                bool bSuccessfullyResetAll = true;
                for (auto dataToRest : mResettableDataArray) {
                    if (dataToRest) {
                        bSuccessfullyResetAll &= dataToRest->tryResetBackToDefaultValue();
                    }
                }

                if (bSuccessfullyResetAll) {
                    mResettableDataArray.Empty();
                    RemoveActionBinding(mResetActionHandle);
                }
                mbIsResettingData = false;
            }
        });
}

void UMKUI_W_OptionsScreen::onBackBoundActionTriggered()
{
    DeactivateWidget();
}

void UMKUI_W_OptionsScreen::onOptionsTabSelected(FName tabId)
{
    // clear previous details - SetSelectedIndex() below will trigger the update details panel with the first setting in the tab
    mListEntryDetailsPanel->clearDetailsPanelInfo();

    const auto optionsList = getDataRegistry()->getListSourceItemsBySelectedTabId(tabId);
    mOptionsList->SetListItems(optionsList);
    mOptionsList->RequestRefresh();

    // if there are items navigate to the top of the list and highlight (select) the first.
    // (navigation is needed when the list is long and scrollable, we don't want to open the options tab mid list)
    if (mOptionsList->GetNumItems() != 0) {
        mOptionsList->NavigateToIndex(0);
        mOptionsList->SetSelectedIndex(0);
    }

    // store the current resettable data in the resettable array (only data that has default value and currently not default).
    // we do this so that if we have settings that are not defaulted in the CURRENT TAB, then the "Reset" button will appear
    // immediately when TAB opened.
    // inside the loop we also bind a callback for each setting entry to affect dynamically the presence of the 
    // Reset button on the fly if the settings deviate or align with the default manually by the user.
    mResettableDataArray.Empty();
    for (auto optionData : optionsList) {
        if (!optionData)
            continue;

        if (!optionData->onListDataModified.IsBoundToObject(this)) {
            optionData->onListDataModified.AddUObject(this, &ThisClass::onListViewDataModified);
        }

        if (optionData->canResetBackToDefaultValue()) {
            mResettableDataArray.AddUnique(optionData);
        }
    }

    // remove the Reset button if no resettable data when tab was opened, else add it if missing
    if (mResettableDataArray.IsEmpty()) {
        RemoveActionBinding(mResetActionHandle);
    }
    else if (!GetActionBindings().Contains(mResetActionHandle)) {
        AddActionBinding(mResetActionHandle);
    }
}

void UMKUI_W_OptionsScreen::onListViewItemHovered(UObject* hoveredItem, bool bHovered)
{
    if (!hoveredItem) {
        return;
    }
    const auto entryWidget = mOptionsList->GetEntryWidgetFromItem<UMKUI_ListEntryBase>(hoveredItem);
    check(entryWidget);
    entryWidget->nativeOnListEntryWidgetHovered(bHovered);

    if (bHovered) {
        mListEntryDetailsPanel->updateDetailsPanelInfo(CastChecked<UMKUI_ListDataObjectBase>(hoveredItem),
                                                       tryGetEntryWidgetClassName(hoveredItem));
    }
    else {
        if (const auto selectedItem = mOptionsList->GetSelectedItem<UMKUI_ListDataObjectBase>()) {
            mListEntryDetailsPanel->updateDetailsPanelInfo(selectedItem, tryGetEntryWidgetClassName(selectedItem));
        }
    }
}

void UMKUI_W_OptionsScreen::onListViewItemSelected(UObject* selectedItem)
{
    if (!selectedItem) {
        return;
    }

    mListEntryDetailsPanel->updateDetailsPanelInfo(CastChecked<UMKUI_ListDataObjectBase>(selectedItem),
                                                   tryGetEntryWidgetClassName(selectedItem));
}

void UMKUI_W_OptionsScreen::onListViewDataModified(UMKUI_ListDataObjectBase* data, EOptionsListDataModifiedReason modificationReason)
{
    // we don't want to trigger this callback when the modification event is due to resetting all values to default.
    // otherwise what happens is that the mResettableDataArray is modified while the reset callback clears it
    if (!data || mbIsResettingData)
        return;

    /**
     * check if the modification of the current setting triggered the tab to not be in default value state
     * and if it is, add the Reset button to the screen, else, remove it
     */
    if (data->canResetBackToDefaultValue()) {
        mResettableDataArray.AddUnique(data);
        if (!GetActionBindings().Contains(mResetActionHandle)) {
            AddActionBinding(mResetActionHandle);
        }
    }
    else if (mResettableDataArray.Contains(data)) {
        mResettableDataArray.Remove(data);
    }

    if (mResettableDataArray.IsEmpty()) {
        RemoveActionBinding(mResetActionHandle);
    }
}

FString UMKUI_W_OptionsScreen::tryGetEntryWidgetClassName(UObject* owningListItem) const
{
    if (const auto foundEntry = mOptionsList->GetEntryWidgetFromItem(owningListItem)) {
        return foundEntry->GetClass()->GetName();
    }
    return TEXT("Entry widget not valid");
}
