// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_OptionsDataRegistry.h"

#include "Widgets/Options/MKUI_ListDataObjectCollection.h"

void UMKUI_OptionsDataRegistry::init(ULocalPlayer* owningLocalPlayer)
{
    initGameplayCollectionTab();
    initAudioCollectionTab();
    initVideoCollectionTab();
    initControlCollectionTab();
}

void UMKUI_OptionsDataRegistry::initGameplayCollectionTab()
{
    // construct the data objects collection for the gameplay settings
    const auto gameplayTabCollection = NewObject<UMKUI_ListDataObjectCollection>();
    gameplayTabCollection->setmDataId(FName("gameplayTabCollection"));
    gameplayTabCollection->setmDataDisplayName(FText::FromString(TEXT("Gameplay")));
    mRegisteredTabCollections.Add(gameplayTabCollection);
}

void UMKUI_OptionsDataRegistry::initAudioCollectionTab()
{
    const auto audioTabCollection = NewObject<UMKUI_ListDataObjectCollection>();
    audioTabCollection->setmDataId(FName("audioTabCollection"));
    audioTabCollection->setmDataDisplayName(FText::FromString(TEXT("Audio")));
    mRegisteredTabCollections.Add(audioTabCollection);
}

void UMKUI_OptionsDataRegistry::initVideoCollectionTab()
{
    const auto videoTabCollection = NewObject<UMKUI_ListDataObjectCollection>();
    videoTabCollection->setmDataId(FName("videoTabCollection"));
    videoTabCollection->setmDataDisplayName(FText::FromString(TEXT("Video")));
    mRegisteredTabCollections.Add(videoTabCollection);
}

void UMKUI_OptionsDataRegistry::initControlCollectionTab()
{
    const auto controlsTabCollection = NewObject<UMKUI_ListDataObjectCollection>();
    controlsTabCollection->setmDataId(FName("controlsTabCollection"));
    controlsTabCollection->setmDataDisplayName(FText::FromString(TEXT("Controls")));
    mRegisteredTabCollections.Add(controlsTabCollection);
}
