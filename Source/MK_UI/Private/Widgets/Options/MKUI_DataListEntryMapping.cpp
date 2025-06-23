// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_DataListEntryMapping.h"

#include "Widgets/Options/DataObjects/MKUI_ListDataObjectBase.h"

TSubclassOf<UMKUI_ListEntryBase> UMKUI_DataListEntryMapping::findEntryByDataObject(UMKUI_ListDataObjectBase* dataObj) const
{
    check(dataObj);

    // run up the inheritance hierarchy of the input obj and find the associated list entry
    // that matches the input data object class or one of its parents
    for (UClass* dataObjectClass = dataObj->GetClass(); dataObjectClass; dataObjectClass = dataObjectClass->GetSuperClass()) {
        if (auto convertedDataObjectClass = TSubclassOf<UMKUI_ListDataObjectBase>(dataObjectClass)) {
            if (mClassToEntryMapping.Contains(convertedDataObjectClass)) {
                return mClassToEntryMapping.FindRef(convertedDataObjectClass);
            }
        }
    }

    return {};
}
