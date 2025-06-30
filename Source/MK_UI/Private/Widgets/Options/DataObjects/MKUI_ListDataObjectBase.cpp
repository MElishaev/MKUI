// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectBase.h"
#include "Settings/MKUI_GameUserSettings.h"

void UMKUI_ListDataObjectBase::initDataObject()
{
    onDataObjectInitialized();
}

void UMKUI_ListDataObjectBase::notifyDataModified(UMKUI_ListDataObjectBase* data, EOptionsListDataModifiedReason reason)
{
    onListDataModified.Broadcast(data, reason);

    // todo: should this be here? this make the function no exactly what it is called
    if (mbShouldApplySettingImmediately) {
        UMKUI_GameUserSettings::getInstance()->ApplySettings(true);
    }
}

void UMKUI_ListDataObjectBase::applySettingsManually()
{
    UMKUI_GameUserSettings::getInstance()->ApplySettings(true);
}
