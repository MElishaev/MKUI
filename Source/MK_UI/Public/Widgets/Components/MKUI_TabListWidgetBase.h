// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "MKUI_TabListWidgetBase.generated.h"

class UMKUI_CommonButtonBase;
/**
 * This class represents a list of buttons to represent some buttons groups.
 * For example, this can represent tab buttons in a "Settings" menu.
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_TabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
    void requestRegisterTabButton(const FName tabId, const FText& tabDisplayName);
    
private:
#if WITH_EDITOR
    // this validates some stuff we want (based on our implementation) and throws compile log error in case invalid.
    // this is similar to what you see in a BP for example, where some variable remain nullptr or something and the BP
    // throws for you an error to specify this.
    virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& compileLog) const override;
#endif
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MKUI Tab List Settings", meta=(AllowPrivateAccess="true", ClampMin="1", ClampMax="10"))
    int32 mDebugPreviewTabCount = 3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MKUI Tab List Settings", meta=(AllowPrivateAccess="true"))
    TSubclassOf<UMKUI_CommonButtonBase> mTabButtonEntryWidgetClass; // what type of buttons can be placed under this tab list
};
