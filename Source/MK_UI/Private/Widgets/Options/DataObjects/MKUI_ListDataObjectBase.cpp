// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectBase.h"

void UMKUI_ListDataObjectBase::initDataObject()
{
    onDataObjectInitialized();
}

void UMKUI_ListDataObjectBase::notifyDataModified(UMKUI_ListDataObjectBase* data, EOptionsListDataModifiedReason reason)
{
    onListDataModified.Broadcast(data, reason);
}
