// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MKUI_LoadingScreenObserver.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UMKUI_LoadingScreenObserver : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class MK_UI_API IMKUI_LoadingScreenObserver
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintNativeEvent)
    void handleLoadingScreenActivated();

    UFUNCTION(BlueprintNativeEvent)
    void handleLoadingScreenDeactivated();    
};
