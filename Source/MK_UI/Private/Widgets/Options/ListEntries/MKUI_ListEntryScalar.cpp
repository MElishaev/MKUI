// MAAKU Studio all rights reserved


#include "Widgets/Options/ListEntries/MKUI_ListEntryScalar.h"

#include "Widgets/Options/DataObjects/MKUI_ListDataObjectScalar.h"
#include "AnalogSlider.h"

void UMKUI_ListEntryScalar::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    mSettingSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::handleSliderValueChanged);
    mSettingSlider->OnMouseCaptureBegin.AddUniqueDynamic(this, &ThisClass::handleSliderMouseCaptureBegin);
    mSettingSlider->OnMouseCaptureEnd.AddUniqueDynamic(this, &ThisClass::handleSliderMouseCaptureEnd); // todo this should trigger writing to file i think
}

void UMKUI_ListEntryScalar::onOwningListDataObjectSet(UMKUI_ListDataObjectBase* listDataObject)
{
    Super::onOwningListDataObjectSet(listDataObject);

    mCachedDataObject = CastChecked<UMKUI_ListDataObjectScalar>(listDataObject);

    mSettingValue->SetNumericType(mCachedDataObject->getmDisplayNumericType());
    mSettingValue->FormattingSpecification = mCachedDataObject->getmNumberFormattingOptions();
    mSettingValue->SetCurrentValue(mCachedDataObject->getCurrentValue());

    mSettingSlider->SetMinValue(mCachedDataObject->getmDisplayValueRange().GetLowerBoundValue());
    mSettingSlider->SetMaxValue(mCachedDataObject->getmDisplayValueRange().GetUpperBoundValue());
    mSettingSlider->SetStepSize(mCachedDataObject->getmSliderStepSize());
    mSettingSlider->SetValue(mCachedDataObject->getCurrentValue());
}

void UMKUI_ListEntryScalar::handleOwningListDataObjectModified(UMKUI_ListDataObjectBase* listDataObject, EOptionsListDataModifiedReason reason)
{
    if (mCachedDataObject) {
        mSettingValue->SetCurrentValue(mCachedDataObject->getCurrentValue());
        mSettingSlider->SetValue(mCachedDataObject->getCurrentValue());
    }
}

void UMKUI_ListEntryScalar::handleSliderValueChanged(float value)
{
    if (mCachedDataObject) {
        mCachedDataObject->setCurrentValueFromSlider(value);
    }
}

void UMKUI_ListEntryScalar::handleSliderMouseCaptureBegin()
{
    selectThisEntryWidget();
}

void UMKUI_ListEntryScalar::handleSliderMouseCaptureEnd()
{
    if (mCachedDataObject) {
        mCachedDataObject->handleSliderMouseCaptureEnd();
    }
}
