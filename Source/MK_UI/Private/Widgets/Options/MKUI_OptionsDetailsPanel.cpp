// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_OptionsDetailsPanel.h"
#include "CommonTextBlock.h"
#include "CommonRichTextBlock.h"
#include "CommonLazyImage.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectBase.h"

void UMKUI_OptionsDetailsPanel::updateDetailsPanelInfo(UMKUI_ListDataObjectBase* dataObj, const FString& className)
{
    if (!dataObj) {
        return;
    }
    
    if (!dataObj->getmSoftDescriptionImage().IsNull()) {
        mDescriptionImage->SetBrushFromLazyTexture(dataObj->getmSoftDescriptionImage());
        mDescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    else {
        mDescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
    }
    mDescription->SetText(dataObj->getmDescriptionRichText());
    mTitle->SetText(dataObj->getmDataDisplayName());
    mDisabledReason->SetText(dataObj->isDataEditable() ? FText::GetEmpty() : dataObj->getmDisabledRichText());

    const FString dynamicDetails = FString::Printf(
        TEXT("Data Object Class: <Bold>%s</>\n"
             "Entry widget class: <Bold>%s</>"),
             *(dataObj->GetClass()->GetName()), *className
        );
    mDynamicDetails->SetText(FText::FromString(dynamicDetails));        
}

void UMKUI_OptionsDetailsPanel::clearDetailsPanelInfo()
{
    mDescription->SetText(FText::GetEmpty());
    mTitle->SetText(FText::GetEmpty());
    mDescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
    mDisabledReason->SetText(FText::GetEmpty());
    mDynamicDetails->SetText(FText::GetEmpty());
}

void UMKUI_OptionsDetailsPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    clearDetailsPanelInfo();
}
