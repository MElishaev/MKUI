// MAAKU Studio all rights reserved


#include "Widgets/Components/MKUI_CommonRotator.h"
#include "CommonTextBlock.h"


void UMKUI_CommonRotator::setSelectedOptionByText(const FText& optionText)
{
    const auto index = TextLabels.IndexOfByPredicate([optionText](const FText& t) {
        return t.EqualTo(optionText);
    });

    if (index != INDEX_NONE) {
        SetSelectedItem(index);
    }
    else {
        // if we are here it is probably because the optionText is the INVALID option.
        // in this case we just set the text of the rotator to INVALID.
        MyText->SetText(optionText);
    }
}
