// MAAKU Studio all rights reserved


#include "Widgets/Components/MKUI_CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Subsystems/MKUI_Subsystem.h"

void UMKUI_CommonButtonBase::setButtonText(const FText& text)
{
    if (mCommonButtonText && !text.IsEmpty()) {
        mCommonButtonText->SetText(mbUseUpperCaseForButtonText ? text.ToUpper() : text);
    }
}

FText UMKUI_CommonButtonBase::getButtonText() const
{
    if (mCommonButtonText) {
        return mCommonButtonText->GetText();
    }
    return FText();
}

void UMKUI_CommonButtonBase::NativePreConstruct()
{
    Super::NativePreConstruct();

    setButtonText(mButtonDisplayText);
}

void UMKUI_CommonButtonBase::NativeOnCurrentTextStyleChanged()
{
    /* in this method we tell the button class on what common text block he should apply
     * the new text style */
    Super::NativeOnCurrentTextStyleChanged();

    if (mCommonButtonText && GetCurrentTextStyleClass()) {
        mCommonButtonText->SetStyle(GetCurrentTextStyleClass());
    }
}

void UMKUI_CommonButtonBase::NativeOnHovered()
{
    Super::NativeOnHovered();

    if (!mButtonDescription.IsEmpty()) {
        UMKUI_Subsystem::getInstance(this)->onButtonDescUpdated.Broadcast(this, mButtonDescription);
    }
}

void UMKUI_CommonButtonBase::NativeOnUnhovered()
{
    Super::NativeOnUnhovered();
    UMKUI_Subsystem::getInstance(this)->onButtonDescUpdated.Broadcast(this, FText::GetEmpty());
}
