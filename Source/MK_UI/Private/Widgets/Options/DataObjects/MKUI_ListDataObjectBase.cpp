// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectBase.h"
#include "Settings/MKUI_GameUserSettings.h"

void UMKUI_ListDataObjectBase::initDataObject()
{
    onDataObjectInitialized();
}

void UMKUI_ListDataObjectBase::addEditCondition(const FOptionsDataEditConditionDescriptor& editCondition)
{
    mEditConditions.Add(editCondition);
}

bool UMKUI_ListDataObjectBase::isDataEditable()
{
    if (mEditConditions.IsEmpty()) {
        return true;
    }

    /* goes over edit conditions and if find not met conditions, store the reason to display to the user and
     * if has forced value to be applied, tries to apply the forced value */
    bool bIsEditable = true;
    FString cachedDisabledReason;
    for (const auto& c : mEditConditions) {
        if (c.isEditConditionMet()) {
            continue;
        }
        bIsEditable = false;
        cachedDisabledReason.Append(c.getmDisabledRichStringReason());
        setmDisabledRichText(FText::FromString(cachedDisabledReason));
        if (c.hasForcedStringValueSet()) {
            const FString& forcedVal = c.getmDisabledForcedStringValue();
            if (canSetToForcedStringValue(forcedVal)) {
                onSetToForcedStringValue(forcedVal);
            }
        }
    }

    return bIsEditable;
}

void UMKUI_ListDataObjectBase::addDataDependency(UMKUI_ListDataObjectBase* dataDependency)
{
    if (dataDependency && !dataDependency->onListDataModified.IsBoundToObject(this)) {
        dataDependency->onListDataModified.AddUObject(this, &ThisClass::handleDependencyDataModified);
    }
}

void UMKUI_ListDataObjectBase::notifyDataModified(UMKUI_ListDataObjectBase* data, EOptionsListDataModifiedReason reason)
{
    onListDataModified.Broadcast(data, reason);

    // todo: should this be here? this make the function no exactly what it is called
    if (mbShouldApplySettingImmediately) {
        UMKUI_GameUserSettings::getInstance()->ApplySettings(true);
    }
}

void UMKUI_ListDataObjectBase::handleDependencyDataModified(UMKUI_ListDataObjectBase* modifiedDependency,
                                                            EOptionsListDataModifiedReason reason)
{
    onDependencyDataModified.Broadcast(modifiedDependency, reason);
}

void UMKUI_ListDataObjectBase::applySettingsManually()
{
    UMKUI_GameUserSettings::getInstance()->ApplySettings(true);
}
