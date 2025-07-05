// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListDataObjectString.h"
#include "MKUI_ListDataObjectStringResolution.generated.h"

/**
 * 
 */
UCLASS()
class MK_UI_API UMKUI_ListDataObjectStringResolution : public UMKUI_ListDataObjectString
{
    GENERATED_BODY()

public:
    void initScreenResolutionOption();

protected:
    virtual void onDataObjectInitialized() override;

private:
    FString resolutionToStringValue(const FIntPoint& res) const;
    FText resolutionToDisplayText(const FIntPoint& res) const;

    FString mMaxResolutionSupported;
};
