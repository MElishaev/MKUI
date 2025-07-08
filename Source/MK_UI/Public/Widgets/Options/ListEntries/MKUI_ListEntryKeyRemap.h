// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListEntryBase.h"
#include "MKUI_ListEntryKeyRemap.generated.h"

class UMKUI_CommonButtonBase;
/**
 * This class represents the entry widget of the key bindings entries in the options menu
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_ListEntryKeyRemap : public UMKUI_ListEntryBase
{
    GENERATED_BODY()

private:
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
    UMKUI_CommonButtonBase* mRemapKey;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
    UMKUI_CommonButtonBase* mResetKeyMapping;
};
