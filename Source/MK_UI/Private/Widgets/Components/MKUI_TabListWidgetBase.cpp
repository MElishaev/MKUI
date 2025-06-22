// MAAKU Studio all rights reserved


#include "Widgets/Components/MKUI_TabListWidgetBase.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Components/MKUI_CommonButtonBase.h"

void UMKUI_TabListWidgetBase::requestRegisterTabButton(const FName tabId, const FText& tabDisplayName)
{
    RegisterTab(tabId, mTabButtonEntryWidgetClass, nullptr); // we have no content widget for that tab
    if (const auto button = Cast<UMKUI_CommonButtonBase>(GetTabButtonBaseByID(tabId))) {
        button->setButtonText(tabDisplayName);
    }
}

#if WITH_EDITOR	
void UMKUI_TabListWidgetBase::ValidateCompiledDefaults(IWidgetCompilerLog& compileLog) const
{
    Super::ValidateCompiledDefaults(compileLog);
    if (!mTabButtonEntryWidgetClass) {
        compileLog.Error(FText::FromString(TEXT("The variable mTabButtonEntryWidgetClass has no valid entry specified. ") +
            GetClass()->GetName() + TEXT(" needs a valid entry widget class to function properly")));
    }
}
#endif
