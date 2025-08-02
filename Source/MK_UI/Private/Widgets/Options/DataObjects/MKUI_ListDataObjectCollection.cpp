// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectCollection.h"

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

void UMKUI_ListDataObjectCollection::sortByName()
{
    mChildListDataArray.Sort([](UMKUI_ListDataObjectBase& a, UMKUI_ListDataObjectBase& b) {
        return a.getmDataId().Compare(b.getmDataId()) < 0;
    });
}
