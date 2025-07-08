// MAAKU Studio all rights reserved


#include "Widgets/Options/ListEntries/MKUI_ListEntryBase.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectBase.h"

void UMKUI_ListEntryBase::nativeOnListEntryWidgetHovered(bool bHovered)
{
    BP_onListEntryWidgetHovered(bHovered, GetListItem() ? IsListItemSelected() : false);
}

void UMKUI_ListEntryBase::NativeOnListItemObjectSet(UObject* listItemObject)
{
    IUserObjectListEntry::NativeOnListItemObjectSet(listItemObject);
    onOwningListDataObjectSet(CastChecked<UMKUI_ListDataObjectBase>(listItemObject));
}

void UMKUI_ListEntryBase::NativeOnEntryReleased()
{
    IUserObjectListEntry::NativeOnEntryReleased();

    // for fixing issue that you see 2 highlighted entries when you select any entry but the first on a tab,
    // then switch a tab, but when you get back to the previous tab, by default the first entry is selected
    // but the previous selection wasn't cleared, so you will see the first and the previously selected entries being selected
    nativeOnListEntryWidgetHovered(false);
}

FReply UMKUI_ListEntryBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    auto commonInputSubsystem = GetInputSubsystem();
    if (commonInputSubsystem && (commonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)) {
        if (auto widgetToFocus = BP_getWidgetToFocusForGamepad()) {
            if (auto slateWidgetToFocus = widgetToFocus->GetCachedWidget()) {
                return FReply::Handled().SetUserFocus(slateWidgetToFocus.ToSharedRef());
            }
        }
    }
    return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UMKUI_ListEntryBase::onOwningListDataObjectSet(UMKUI_ListDataObjectBase* listDataObject)
{
    // we should check if this valid because this is an optional widget binding
    if (mSettingDisplayName) {
        mSettingDisplayName->SetText(listDataObject->getmDataDisplayName());
    }

    if (!listDataObject->onListDataModified.IsBoundToObject(this)) {
        listDataObject->onListDataModified.AddUObject(this, &ThisClass::handleOwningListDataObjectModified);
    }

    if (!listDataObject->onDependencyDataModified.IsBoundToObject(this)) {
        listDataObject->onDependencyDataModified.AddUObject(this, &ThisClass::handleDependencyDataModified);
    }
    
    onToggleEditableState(listDataObject->isDataEditable());

    mCachedOwningDataObject = listDataObject;
}

void UMKUI_ListEntryBase::handleOwningListDataObjectModified(UMKUI_ListDataObjectBase* listDataObject, EOptionsListDataModifiedReason reason)
{
    // empty in base class
}

void UMKUI_ListEntryBase::onToggleEditableState(bool bIsEditable)
{
    if (mSettingDisplayName) {
        mSettingDisplayName->SetIsEnabled(bIsEditable);
    }
}

void UMKUI_ListEntryBase::handleDependencyDataModified(UMKUI_ListDataObjectBase* modifiedDependency, EOptionsListDataModifiedReason reason)
{
    if (mCachedOwningDataObject) {
        onToggleEditableState(mCachedOwningDataObject->isDataEditable());
    }
}

void UMKUI_ListEntryBase::selectThisEntryWidget()
{
    CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}
