// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "MKUI_GameUserSettings.generated.h"

/**
 * This class stores and loads and applies the game user settings
 */
UCLASS(BlueprintType)
class MK_UI_API UMKUI_GameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:

    UMKUI_GameUserSettings();    
    
    static UMKUI_GameUserSettings* getInstance();

    /******** Gameplay collection tab *********/
    UFUNCTION()
    FString getmCurrentGameDifficulty() const { return mCurrentGameDifficulty; }
    UFUNCTION()
    void setmCurrentGameDifficulty(const FString& gameDifficulty) { mCurrentGameDifficulty = gameDifficulty; }
    /******** Gameplay collection tab *********/

    /******** Audio collection tab *********/
    UFUNCTION(BlueprintCallable, Category="MKUI Settings")
    float getOverallVolume() const { return mOverallVolume; }

    UFUNCTION(BlueprintCallable, Category="MKUI Settings")
    void setOverallVolume(float vol);

    UFUNCTION(BlueprintCallable, Category="MKUI Settings")
    float getMusicVolume() const { return mMusicVolume; }

    UFUNCTION(BlueprintCallable, Category="MKUI Settings")
    void setMusicVolume(float vol);

    UFUNCTION(BlueprintCallable, Category="MKUI Settings")
    float getSFXVolume() const { return mSFXVolume; }

    UFUNCTION(BlueprintCallable, Category="MKUI Settings")
    void setSFXVolume(float vol);

    UFUNCTION()
    bool getAllowBackgroundAudio() const { return mbAllowBackgroundAudio; }
    UFUNCTION()
    void setAllowBackgroundAudio(bool bIsAllowed);

    UFUNCTION()
    bool getUseHDRAudioMode() const { return mbUseHDRAudioMode; }
    UFUNCTION()
    void setUseHDRAudioMode(bool bIsAllowed);
    /******** Audio collection tab *********/

    /******** Video collection tab *********/
    UFUNCTION()
    float getGamma() const;
    UFUNCTION()
    void setGamma(float val);
    /******** Video collection tab *********/

private:
    /******** Gameplay collection tab *********/
    // the config property makes this one of the game settings that is stored in the .ini
    // file and applied when the virtual function of  ApplySettings() is called
    UPROPERTY(Config)
    FString mCurrentGameDifficulty;
    /******** Gameplay collection tab *********/

    /******** Audio collection tab *********/
    UPROPERTY(Config)
    float mOverallVolume;
    UPROPERTY(Config)
    float mMusicVolume;
    UPROPERTY(Config)
    float mSFXVolume;
    UPROPERTY(Config)
    bool mbAllowBackgroundAudio;
    UPROPERTY(Config)
    bool mbUseHDRAudioMode;
    /******** Audio collection tab *********/

    /******** Video collection tab *********/
    UPROPERTY(Config)
    float mGamma;
    /******** Video collection tab *********/

};
