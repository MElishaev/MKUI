// MAAKU Studio all rights reserved


#include "Widgets/Options/DataObjects/MKUI_ListDataObjectStringResolution.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Settings/MKUI_GameUserSettings.h"
#include "Widgets/Options/MKUI_OptionsDataInteractionHelper.h"

void UMKUI_ListDataObjectStringResolution::initScreenResolutionOption()
{
    TArray<FIntPoint> supportedResolutions;
    UKismetSystemLibrary::GetSupportedFullscreenResolutions(supportedResolutions);

    // sort to display by ascending order
    supportedResolutions.Sort([](const FIntPoint& p1, const FIntPoint& p2) {
        return p1.SizeSquared() < p2.SizeSquared();
    });
    
    for (const auto& res : supportedResolutions) {
        addOptionValue(resolutionToStringValue(res), resolutionToDisplayText(res));
    }
    
    mMaxResolutionSupported = resolutionToStringValue(supportedResolutions.Last());
    setDefaultValueFromString(mMaxResolutionSupported);
}

void UMKUI_ListDataObjectStringResolution::onDataObjectInitialized()
{
    Super::onDataObjectInitialized();

    // in case of playing in editor, trySetCurrentTextFromStringValue won't work because it is easy
    // to be in a weird natively unsupported resolution.. because it depends on how you resized
    // your editor screen
    if (!trySetCurrentTextFromStringValue(mCurrentValueString)) {
        mCurrentDisplayText = resolutionToDisplayText(UMKUI_GameUserSettings::getInstance()->GetScreenResolution());
    }
}

FString UMKUI_ListDataObjectStringResolution::resolutionToStringValue(const FIntPoint& res) const
{
    // the expected string value for resolution for a dynamic setter (API method that sets the resolution)
    // is in the format of "(X=9999, Y=9999)"
    return FString::Printf(TEXT("(X=%i, Y=%i)"), res.X, res.Y);
}

FText UMKUI_ListDataObjectStringResolution::resolutionToDisplayText(const FIntPoint& res) const
{
    const FString displayString = FString::Printf(TEXT("%i x %i"), res.X, res.Y);
    return FText::FromString(displayString);
}
