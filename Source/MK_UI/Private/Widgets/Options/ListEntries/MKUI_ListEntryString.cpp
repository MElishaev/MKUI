// MAAKU Studio all rights reserved


#include "Widgets/Options/ListEntries/MKUI_ListEntryString.h"

#include "CommonInputSubsystem.h"
#include "Widgets/Components/MKUI_CommonButtonBase.h"
#include "Widgets/Components/MKUI_CommonRotator.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectString.h"

void UMKUI_ListEntryString::onOwningListDataObjectSet(UMKUI_ListDataObjectBase* listDataObject)
{
    Super::onOwningListDataObjectSet(listDataObject);

    mOwningListDataObject = CastChecked<UMKUI_ListDataObjectString>(listDataObject);

    mOptionsRotator->PopulateTextLabels(mOwningListDataObject->getmAvailableOptionsDisplayText());
    mOptionsRotator->setSelectedOptionByText(mOwningListDataObject->getmCurrentDisplayText());
}

void UMKUI_ListEntryString::handleOwningListDataObjectModified(UMKUI_ListDataObjectBase* listDataObject, EOptionsListDataModifiedReason reason)
{
    if (mOwningListDataObject) {
        mOptionsRotator->setSelectedOptionByText(mOwningListDataObject->getmCurrentDisplayText());
    }
}

void UMKUI_ListEntryString::onToggleEditableState(bool bIsEditable)
{
    Super::onToggleEditableState(bIsEditable);

    mButtonNext->SetIsEnabled(bIsEditable);
    mButtonPrev->SetIsEnabled(bIsEditable);
    mOptionsRotator->SetIsEnabled(bIsEditable);
}

void UMKUI_ListEntryString::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    mButtonNext->OnClicked().AddUObject(this, &ThisClass::onButtonNextClicked);
    mButtonPrev->OnClicked().AddUObject(this, &ThisClass::onButtonPrevClicked);
    mOptionsRotator->OnClicked().AddUObject(this, &ThisClass::onRotatorClicked);
    mOptionsRotator->OnRotatedEvent.AddUObject(this, &ThisClass::onRotatorValueChanged);
}

void UMKUI_ListEntryString::onButtonNextClicked()
{
    if (mOwningListDataObject) {
        mOwningListDataObject->advanceToNextOption();
    }
    selectThisEntryWidget();
}

void UMKUI_ListEntryString::onButtonPrevClicked()
{
    if (mOwningListDataObject) {
        mOwningListDataObject->advanceToPrevOption();
    }
    selectThisEntryWidget();
}

void UMKUI_ListEntryString::onRotatorClicked()
{
    selectThisEntryWidget();
}

void UMKUI_ListEntryString::onRotatorValueChanged(int32 value, bool bUserInitiated)
{
    if (!mOwningListDataObject) {
        return;
    }

    auto commonInputSubsystem = GetInputSubsystem();
    if (!commonInputSubsystem || !bUserInitiated) {
        return;
    }

    if (commonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad) {
        mOwningListDataObject->onRotatorInitiatedValueChange(mOptionsRotator->GetSelectedText());
    }
}
