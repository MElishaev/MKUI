// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListEntryBase.h"
#include "MKUI_ListEntryString.generated.h"

class UMKUI_CommonRotator;
class UMKUI_CommonButtonBase;
/**
 * This represents a list entry of string types
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_ListEntryString : public UMKUI_ListEntryBase
{
    GENERATED_BODY()

private:
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
    UMKUI_CommonButtonBase* mButtonPrev;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
    UMKUI_CommonRotator* mOptionsRotator; // for gamepad - gives the possibility to rotate between the options in a specific setting

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
    UMKUI_CommonButtonBase* mButtonNext;
};
