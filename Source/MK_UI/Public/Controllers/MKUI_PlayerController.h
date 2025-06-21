// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MKUI_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MK_UI_API AMKUI_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

    virtual void OnPossess(APawn* InPawn) override;
};
