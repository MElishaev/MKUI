// MAAKU Studio all rights reserved


#include "Widgets/Options/ListEntries/MKUI_ListEntryKeyRemap.h"

#include "MKUI_FunctionLibrary.h"
#include "MKUI_GameplayTags.h"
#include "Subsystems/MKUI_Subsystem.h"
#include "Widgets/Components/MKUI_CommonButtonBase.h"
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
    mResetKeyMapping->OnClicked().AddUObject(this, &UMKUI_ListEntryKeyRemap::handleKeyRemapButtonClicked);
}

void UMKUI_ListEntryKeyRemap::handleKeyRemapButtonClicked()
{
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
}

void UMKUI_ListEntryKeyRemap::handleKeyToRemapPressed(const FKey& pressedKey)
{
    if (mCachedOwningKeyRemapDataObject) {
        mCachedOwningKeyRemapDataObject->bindNewInputKey(pressedKey);
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
