// MAAKU Studio all rights reserved


#include "MKUI_FunctionLibrary.h"
#include "Settings/MKUI_DeveloperSettings.h"

TSoftClassPtr<UMKUI_W_ActivatableBase> UMKUI_FunctionLibrary::getSoftWidgetClassByTag(UPARAM(meta=(Category="MKUI.widget")) FGameplayTag widgetTag)
{
    const auto devSettings = GetDefault<UMKUI_DeveloperSettings>();

    checkf(devSettings->mWidgetMap.Contains(widgetTag), TEXT("Couldn't find the corresponding widget %s"), *(widgetTag.ToString()));

    return devSettings->mWidgetMap.FindRef(widgetTag);
}

TSoftObjectPtr<UTexture2D> UMKUI_FunctionLibrary::getOptionsSoftImageByTag(UPARAM(meta=(Categories="MKUI.image")) FGameplayTag imgTag)
{
    const auto devSettings = GetDefault<UMKUI_DeveloperSettings>();
    checkf(devSettings->mWidgetMap.Contains(imgTag), TEXT("Couldn't find the corresponding image %s"), *(imgTag.ToString()));
    return devSettings->mOptionsScreenSoftImageMap.FindRef(imgTag);
}
