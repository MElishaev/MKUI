// MAAKU Studio all rights reserved


#include "Widgets/Options/ListEntries/MKUI_ListEntryBase.h"

#include "CommonTextBlock.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectBase.h"

void UMKUI_ListEntryBase::NativeOnListItemObjectSet(UObject* listItemObject)
{
    IUserObjectListEntry::NativeOnListItemObjectSet(listItemObject);

    onOwningListDataObjectSet(CastChecked<UMKUI_ListDataObjectBase>(listItemObject));
}

void UMKUI_ListEntryBase::onOwningListDataObjectSet(UMKUI_ListDataObjectBase* listDataObject)
{
    // we should check if this valid because this is an optional widget binding
    if (mSettingDisplayName) {
        mSettingDisplayName->SetText(listDataObject->getmDataDisplayName());
    }

    if (!listDataObject->onListDataModified.IsBoundToObject(this)) {
        listDataObject->onListDataModified.AddUObject(this, &ThisClass::onOwningListDataObjectModified);
    }
}

void UMKUI_ListEntryBase::onOwningListDataObjectModified(UMKUI_ListDataObjectBase* listDataObject, EOptionsListDataModifiedReason reason)
{
    // empty in base class
}
