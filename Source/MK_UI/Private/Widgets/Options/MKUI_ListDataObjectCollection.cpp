// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_ListDataObjectCollection.h"

void UMKUI_ListDataObjectCollection::addChildListData(UMKUI_ListDataObjectBase* dataToAdd)
{
    dataToAdd->initDataObject();
    dataToAdd->setmParentData(this);
    mChildListDataArray.Add(dataToAdd); 
}

TArray<UMKUI_ListDataObjectBase*> UMKUI_ListDataObjectCollection::getAllChildListData() const
{
    return mChildListDataArray;
}

bool UMKUI_ListDataObjectCollection::hasAnyChildData() const
{
    return !mChildListDataArray.IsEmpty();
}
