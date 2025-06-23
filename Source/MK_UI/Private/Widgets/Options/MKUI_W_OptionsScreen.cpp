// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_W_OptionsScreen.h"

#include "ICommonInputModule.h"
#include "MKUI_DebugHelper.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/Components/MKUI_CommonListView.h"
#include "Widgets/Components/MKUI_TabListWidgetBase.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectCollection.h"
#include "Widgets/Options/MKUI_OptionsDataRegistry.h"

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
    MKUI_Debug::print(TEXT("reset button clicked"));
}

void UMKUI_W_OptionsScreen::onBackBoundActionTriggered()
{
    DeactivateWidget();
}

void UMKUI_W_OptionsScreen::onOptionsTabSelected(FName tabId)
{
    const auto optionsList = getDataRegistry()->getListSourceItemsBySelectedTabId(tabId);
    mOptionsList->SetListItems(optionsList);
    mOptionsList->RequestRefresh();

    // if there are items navigate to the top of the list and highlight (select) the first.
    // (navigation is needed when the list is long and scrollable, we don't want to open the options tab mid list)
    if (mOptionsList->GetNumItems() != 0) {
        mOptionsList->NavigateToIndex(0);
        mOptionsList->SetSelectedIndex(0);
    }
}
