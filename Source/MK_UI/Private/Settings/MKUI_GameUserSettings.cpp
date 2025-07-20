// MAAKU Studio all rights reserved


#include "Settings/MKUI_GameUserSettings.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/App.h"
#include "Settings/MKUI_DeveloperSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"


UMKUI_GameUserSettings::UMKUI_GameUserSettings()
{

}

UMKUI_GameUserSettings* UMKUI_GameUserSettings::getInstance()
{
    if (GEngine) {
        return CastChecked<UMKUI_GameUserSettings>(GEngine->GetGameUserSettings());
    }
    return nullptr;
}

void UMKUI_GameUserSettings::setOverallVolume(float vol)
{
    // todo : maybe this of creating external sound system manager or something and access it here to update stuff
    UWorld* audioWorld = nullptr;
    const auto* mkuiDevSettings = GetDefault<UMKUI_DeveloperSettings>();

    if (GEngine) {
        audioWorld = GEngine->GetCurrentPlayWorld();
    }

    if (!audioWorld || !mkuiDevSettings) {
        UE_LOG(LogTemp, Error, TEXT("%hs: Couldn't retrieve world or developer settings"), __FUNCTION__)
        return;
    }

    USoundClass* masterSoundClass = nullptr;
    if (UObject* loadedObject = mkuiDevSettings->mMasterSoundClass.TryLoad()) {
        masterSoundClass = CastChecked<USoundClass>(loadedObject);
    }

    USoundMix* defaultSoundMix = nullptr;
    if (UObject* LoadedObject = mkuiDevSettings->mDefaultSoundMix.TryLoad()) {
        defaultSoundMix = CastChecked<USoundMix>(LoadedObject);
    }

    mOverallVolume = vol;

    UGameplayStatics::SetSoundMixClassOverride(
        audioWorld,
        defaultSoundMix,
        masterSoundClass,
        mOverallVolume,
        1.f,
        0.2f
        );

    UGameplayStatics::PushSoundMixModifier(audioWorld, defaultSoundMix);
}

void UMKUI_GameUserSettings::setMusicVolume(float vol)
{
    UWorld* audioWorld = nullptr;
    const auto* mkuiDevSettings = GetDefault<UMKUI_DeveloperSettings>();

    if (GEngine) {
        audioWorld = GEngine->GetCurrentPlayWorld();
    }

    if (!audioWorld || !mkuiDevSettings) {
        UE_LOG(LogTemp, Error, TEXT("%hs: Couldn't retrieve world or developer settings"), __FUNCTION__)
        return;
    }

    USoundClass* masterSoundClass = nullptr;
    if (UObject* loadedObject = mkuiDevSettings->mMusicSoundClass.TryLoad()) {
        masterSoundClass = CastChecked<USoundClass>(loadedObject);
    }

    USoundMix* defaultSoundMix = nullptr;
    if (UObject* LoadedObject = mkuiDevSettings->mDefaultSoundMix.TryLoad()) {
        defaultSoundMix = CastChecked<USoundMix>(LoadedObject);
    }

    mMusicVolume = vol;

    UGameplayStatics::SetSoundMixClassOverride(
        audioWorld,
        defaultSoundMix,
        masterSoundClass,
        mMusicVolume,
        1.f,
        0.2f
        );

    UGameplayStatics::PushSoundMixModifier(audioWorld, defaultSoundMix);
}

void UMKUI_GameUserSettings::setSFXVolume(float vol)
{
    UWorld* audioWorld = nullptr;
    const auto* mkuiDevSettings = GetDefault<UMKUI_DeveloperSettings>();

    if (GEngine) {
        audioWorld = GEngine->GetCurrentPlayWorld();
    }

    if (!audioWorld || !mkuiDevSettings) {
        UE_LOG(LogTemp, Error, TEXT("%hs: Couldn't retrieve world or developer settings"), __FUNCTION__)
        return;
    }

    USoundClass* masterSoundClass = nullptr;
    if (UObject* loadedObject = mkuiDevSettings->mSoundFXSoundClass.TryLoad()) {
        masterSoundClass = CastChecked<USoundClass>(loadedObject);
    }

    USoundMix* defaultSoundMix = nullptr;
    if (UObject* LoadedObject = mkuiDevSettings->mDefaultSoundMix.TryLoad()) {
        defaultSoundMix = CastChecked<USoundMix>(LoadedObject);
    }

    mSFXVolume = vol;

    UGameplayStatics::SetSoundMixClassOverride(
        audioWorld,
        defaultSoundMix,
        masterSoundClass,
        mSFXVolume,
        1.f,
        0.2f
        );

    UGameplayStatics::PushSoundMixModifier(audioWorld, defaultSoundMix);
}

void UMKUI_GameUserSettings::setAllowBackgroundAudio(bool bIsAllowed)
{
    mbAllowBackgroundAudio = bIsAllowed;

    const float backgroundVolume = mbAllowBackgroundAudio ? 1.f : 0.f;

    FApp::SetUnfocusedVolumeMultiplier(backgroundVolume);
}

void UMKUI_GameUserSettings::setUseHDRAudioMode(bool bIsAllowed)
{
    mbUseHDRAudioMode = bIsAllowed;
    // todo: the actual logic to control the volume in game goes here
}

float UMKUI_GameUserSettings::getGamma() const
{
    if (GEngine) {
        return GEngine->GetDisplayGamma();
    }
    return 0.f;
}

void UMKUI_GameUserSettings::setGamma(float val)
{
    if (GEngine) {
        GEngine->DisplayGamma = val;
    }
}
