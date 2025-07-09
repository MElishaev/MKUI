// MAAKU Studio all rights reserved


#include "Widgets/Options/ListEntries/MKUI_ListEntryKeyRemap.h"

#include "Widgets/Components/MKUI_CommonButtonBase.h"
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
