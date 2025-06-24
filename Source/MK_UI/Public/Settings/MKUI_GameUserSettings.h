// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "MKUI_GameUserSettings.generated.h"

/**
 * This class stores and loads and applies the game user settings
 */
UCLASS()
class MK_UI_API UMKUI_GameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    static UMKUI_GameUserSettings* getInstance();

    /******** Gameplay collection tab *********/
    UFUNCTION()
    FString getmCurrentGameDifficulty() const { return mCurrentGameDifficulty; }
    UFUNCTION()
    void setmCurrentGameDifficulty(const FString& gameDifficulty) { mCurrentGameDifficulty = gameDifficulty; }
    
private:
    UPROPERTY(Config)
    FString mCurrentGameDifficulty;
};
