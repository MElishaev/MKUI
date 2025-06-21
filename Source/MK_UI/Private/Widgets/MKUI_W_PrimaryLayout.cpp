// MAAKU Studio all rights reserved


#include "Widgets/MKUI_W_PrimaryLayout.h"

UCommonActivatableWidgetContainerBase* UMKUI_W_PrimaryLayout::findWidgetStackByTag(const FGameplayTag& tag) const
{
    checkf(mRegisteredWidgetStackMap.Contains(tag), TEXT("Can't find the widget stack by tag %s"), *tag.ToString());

    return mRegisteredWidgetStackMap.FindRef(tag);
}

void UMKUI_W_PrimaryLayout::registerWidgetStack(UPARAM(meta=(Categories="MKUI.widgetStack")) const FGameplayTag tag, UCommonActivatableWidgetContainerBase* stack)
{
    if (!IsDesignTime()) {
        if (!mRegisteredWidgetStackMap.Contains(tag)) {
            mRegisteredWidgetStackMap.Add(tag, stack);
        }
    }
}
