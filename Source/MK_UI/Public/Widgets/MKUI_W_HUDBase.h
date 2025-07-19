// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_W_ActivatableBase.h"
#include "MKUI_W_HUDBase.generated.h"

/**
 * This is a base class representing the player HUD in game
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_W_HUDBase : public UMKUI_W_ActivatableBase
{
    GENERATED_BODY()


};
