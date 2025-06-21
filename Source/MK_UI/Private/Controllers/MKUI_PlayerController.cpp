// MAAKU Studio all rights reserved


#include "Controllers/MKUI_PlayerController.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void AMKUI_PlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // get the main menu camera and posses it
    // TODO: maybe create some config file mapping named static variables to tags?
    TArray<AActor*> cameras;
    UGameplayStatics::GetAllActorsOfClassWithTag(this,  ACameraActor::StaticClass(), FName("default"), cameras);
    if (!cameras.IsEmpty()) {
        SetViewTarget(cameras[0]);
    }
}
