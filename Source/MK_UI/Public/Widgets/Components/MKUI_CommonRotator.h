// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "MKUI_CommonRotator.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_CommonRotator : public UCommonRotator
{
	GENERATED_BODY()

public:
    void setSelectedOptionByText(const FText& optionText);
	
};
