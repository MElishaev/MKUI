// MAAKU Studio all rights reserved


#include "MKUI_FunctionLibrary.h"
#include "Settings/MKUI_DeveloperSettings.h"

TSoftClassPtr<UMKUI_W_ActivatableBase> UMKUI_FunctionLibrary::getSoftWidgetClassByTag(UPARAM(meta = (Category = "MKUI.widget"))
                                                                                          FGameplayTag widgetTag)
{
    const auto devSettings = GetDefault<UMKUI_DeveloperSettings>();

    checkf(devSettings->mWidgetMap.Contains(widgetTag), TEXT("Couldn't find the corresponding widget %s"), *(widgetTag.ToString()));

    return devSettings->mWidgetMap.FindRef(widgetTag);
}

TSoftObjectPtr<UTexture2D> UMKUI_FunctionLibrary::getOptionsSoftImageByTag(UPARAM(meta = (Categories = "MKUI.image")) FGameplayTag imgTag)
{
    const UMKUI_DeveloperSettings* devSettings = GetDefault<UMKUI_DeveloperSettings>();
    const TSoftObjectPtr<UTexture2D>* image = devSettings->mOptionsScreenSoftImageMap.Find(imgTag);
    if (!image) {
        UE_LOG(LogTemp, Warning, TEXT("MK_UI could not find an optional Options image mapped to %s."), *imgTag.ToString());
        return {};
    }

    return *image;
}
