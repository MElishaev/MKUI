// MAAKU Studio all rights reserved


#include "Controllers/MKUI_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/MKUI_DeveloperSettings.h"
#include "Settings/MKUI_GameUserSettings.h"

void AMKUI_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    const UMKUI_DeveloperSettings* mkuiDeveloperSettings = GetDefault<UMKUI_DeveloperSettings>();
    check(mkuiDeveloperSettings);

    if (!mkuiDeveloperSettings->mInputMappingContext.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Missing InputMappingContext. Check 'LastDepth' UI settings."));
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    check(subsystem);

    subsystem->AddMappingContext(mkuiDeveloperSettings->mInputMappingContext.LoadSynchronous(), 0);
}

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

    // run benchmark on startup if not already done to determine the default starting settings
    auto settings = UMKUI_GameUserSettings::getInstance();
    if (settings->GetLastCPUBenchmarkResult() == -1.f || settings->GetLastGPUBenchmarkResult() == -1.f) {
        settings->RunHardwareBenchmark();
        settings->ApplyHardwareBenchmarkResults();
    }
}
