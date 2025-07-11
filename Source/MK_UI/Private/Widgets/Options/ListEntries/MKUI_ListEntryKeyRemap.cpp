// MAAKU Studio all rights reserved


#include "Widgets/Options/ListEntries/MKUI_ListEntryKeyRemap.h"

#include "MKUI_FunctionLibrary.h"
#include "MKUI_GameplayTags.h"
#include "Subsystems/MKUI_Subsystem.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Components/MKUI_CommonButtonBase.h"
#include "Widgets/Components/MKUI_CommonListView.h"
#include "Widgets/Options/MKUI_W_KeyRemapScreen.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectKeyRemap.h"

void UMKUI_ListEntryKeyRemap::onOwningListDataObjectSet(UMKUI_ListDataObjectBase* listDataObject)
{
    Super::onOwningListDataObjectSet(listDataObject);

    mCachedOwningKeyRemapDataObject = CastChecked<UMKUI_ListDataObjectKeyRemap>(listDataObject);
    mRemapKey->setButtonImg(mCachedOwningKeyRemapDataObject->getIconFromCurrentKey());
}

void UMKUI_ListEntryKeyRemap::handleOwningListDataObjectModified(UMKUI_ListDataObjectBase* listDataObject,
                                                                 EOptionsListDataModifiedReason reason)
{
    if (mCachedOwningKeyRemapDataObject) {
        mRemapKey->setButtonImg(mCachedOwningKeyRemapDataObject->getIconFromCurrentKey());
    }
}

void UMKUI_ListEntryKeyRemap::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    mRemapKey->OnClicked().AddUObject(this, &ThisClass::handleKeyRemapButtonClicked);
    mResetKeyMapping->OnClicked().AddUObject(this, &UMKUI_ListEntryKeyRemap::handleResetKeyBindingButtonClicked);
}

void UMKUI_ListEntryKeyRemap::handleKeyRemapButtonClicked()
{
    selectThisEntryWidget();

    UMKUI_Subsystem::getInstance(this)->pushSoftWidgetToStackAsync(
        MKUI_GameplayTags::MKUI_widgetStack_modal,
        UMKUI_FunctionLibrary::getSoftWidgetClassByTag(MKUI_GameplayTags::MKUI_widget_keyRemapScreen),
        [this](EAsyncPushWidgetState pushState, UMKUI_W_ActivatableBase* pushedwidget) {
            if (pushState == EAsyncPushWidgetState::OnCreatedBeforePush) {
                const auto createdKeyRemapScreen = CastChecked<UMKUI_W_KeyRemapScreen>(pushedwidget);
                createdKeyRemapScreen->mOnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::handleKeyToRemapPressed);
                createdKeyRemapScreen->mOnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::handleKeyRemapCanceled);

                if (mCachedOwningKeyRemapDataObject) {
                    createdKeyRemapScreen->setDesiredInputTypeToFilter(mCachedOwningKeyRemapDataObject->getDesiredInputType());
                }
            }
        }
        );
}

void UMKUI_ListEntryKeyRemap::handleResetKeyBindingButtonClicked()
{
    selectThisEntryWidget();

    if (!mCachedOwningKeyRemapDataObject) {
        return;
    }

    // if current key is already the default key, display OK screen that says this is already the default
    if (!mCachedOwningKeyRemapDataObject->canResetBackToDefaultValue()) {
        UMKUI_Subsystem::getInstance(this)->pushConfirmScreenToModalStackAsync(
            EConfirmScreenType::Ok,
            FText::FromString(TEXT("Reset key mapping")),
            FText::FromString(TEXT("This key binding is already set to default")),
            [](EConfirmScreenButtonType clickedButton) {
            }
            );
        return;
    }

    // else reset the binding back to default
    UMKUI_Subsystem::getInstance(this)->pushConfirmScreenToModalStackAsync(
        EConfirmScreenType::YesNo,
        FText::FromString(TEXT("Reset key mapping")),
        FText::FromString(
            TEXT("Are you sure you want to reset key binding for ") + mCachedOwningKeyRemapDataObject->getmDataDisplayName().ToString() +
            TEXT("?")),
        [this](EConfirmScreenButtonType clickedButton) {
            if (clickedButton == EConfirmScreenButtonType::Confirmed) {
                const FKey& defaultKey = mCachedOwningKeyRemapDataObject->getOwningKeyMapping()->GetDefaultKey();
                if (!mCachedOwningKeyRemapDataObject->tryBindNewInputKey(defaultKey)) {
                    showBindConfirmationScreen(defaultKey);
                }
            }
        }
        );
}

void UMKUI_ListEntryKeyRemap::handleKeyToRemapPressed(const FKey& pressedKey)
{
    if (mCachedOwningKeyRemapDataObject) {
        if (!mCachedOwningKeyRemapDataObject->tryBindNewInputKey(pressedKey)) {
            // if we got here, the key is already bound - ask user for confirmation to unbind the previous action of the key
            showBindConfirmationScreen(pressedKey);            
        }
    }
}

void UMKUI_ListEntryKeyRemap::handleKeyRemapCanceled(const FString& cancelReason)
{
    UMKUI_Subsystem::getInstance(this)->pushConfirmScreenToModalStackAsync(
        EConfirmScreenType::Ok,
        FText::FromString(TEXT("Key Remap")),
        FText::FromString(cancelReason),
        [](EConfirmScreenButtonType clickedButton) {}
        );
}

UMKUI_ListDataObjectKeyRemap* UMKUI_ListEntryKeyRemap::findDataObjectKeyRemapByKey(const FKey& key) const
{
    // this consists entries like collection entries and key remap entries
    const auto controlTabListView = CastChecked<UMKUI_CommonListView>(GetOwningListView());
    auto allListItems = controlTabListView->GetListItems();

    for (const auto listItem : allListItems) {
        if (listItem->IsA(UMKUI_ListDataObjectKeyRemap::StaticClass())) {
            const auto listDataObjectKeyRemap = CastChecked<UMKUI_ListDataObjectKeyRemap>(listItem);
            if (listDataObjectKeyRemap->getOwningKeyMapping()->GetCurrentKey() == key) {
                return listDataObjectKeyRemap;
            }
        }
    }
    return nullptr;
}

void UMKUI_ListEntryKeyRemap::showBindConfirmationScreen(const FKey& keyToBind)
{
    const auto boundDataObjectOfPressedKey = findDataObjectKeyRemapByKey(keyToBind);

    // this shouldn't be nullptr because this function is called when we know that the key is bound to another action
    check(boundDataObjectOfPressedKey);

    const FText boundAction = boundDataObjectOfPressedKey->getOwningKeyMapping()->GetDisplayName();

    UMKUI_Subsystem::getInstance(this)->pushConfirmScreenToModalStackAsync(
        EConfirmScreenType::OkCancel,
        FText::FromString(TEXT("Rebind existing binding")),
        FText::FromString(FString::Printf(
            TEXT(
                "Confirming will unbind %s from %s. Would you like to continue?"),
            *(keyToBind.GetDisplayName().ToString()),
            *(boundAction.ToString()))),
        [this, keyToBind, boundDataObjectOfPressedKey](EConfirmScreenButtonType clickedButton) {
            if (clickedButton == EConfirmScreenButtonType::Confirmed) {
                boundDataObjectOfPressedKey->unbindInputKey();
                mCachedOwningKeyRemapDataObject->bindNewInputKey(keyToBind);
            }
        }
        );
}
