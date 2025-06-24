// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_OptionsDataRegistry.h"

#include "Settings/MKUI_GameUserSettings.h"
#include "Widgets/Options/MKUI_OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectCollection.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectString.h"

#define MAKE_OPTIONS_DATA_ACCESSORS(accessorFuncName) \
    MakeShared<MKUI_FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UMKUI_GameUserSettings, accessorFuncName))

void UMKUI_OptionsDataRegistry::init(ULocalPlayer* owningLocalPlayer)
{
    initGameplayCollectionTab();
    initAudioCollectionTab();
    initVideoCollectionTab();
    initControlCollectionTab();
}

TArray<UMKUI_ListDataObjectBase*> UMKUI_OptionsDataRegistry::getListSourceItemsBySelectedTabId(const FName tabId) const
{
    auto tab = mRegisteredTabCollections.FindByPredicate([tabId](const UMKUI_ListDataObjectCollection* v) {
        return v->getmDataId() == tabId;
    });

    checkf(tab, TEXT("No valid tab found with ID %s"), *tabId.ToString());

    return (*tab)->getAllChildListData();
}

void UMKUI_OptionsDataRegistry::initGameplayCollectionTab()
{
    // construct the data objects collection for the gameplay settings
    const auto gameplayTabCollection = NewObject<UMKUI_ListDataObjectCollection>();
    gameplayTabCollection->setmDataId(FName("gameplayTabCollection"));
    gameplayTabCollection->setmDataDisplayName(FText::FromString(TEXT("Gameplay")));

    // todo: im not sure i like this approach but we will see as the course progresses...
    //  i don't like about it that the options are hardcoded here in code - will see if there are option to disable or hide in widget.
    //  im also not sure that all this overhead of inheritances is needed here.. why not use enums for settings of "texts", numbers
    //  for slider and bool for checkboxes, why all these classes are needed? this maybe would look "uglier" but could be much simpler IMO.    

    // in this section allocate all the different settings for this tab
    const auto gameDifficulty = NewObject<UMKUI_ListDataObjectString>();
    gameDifficulty->setmDataId("gameDifficulty");
    gameDifficulty->setmDataDisplayName(FText::FromString(TEXT("Difficulty")));
    gameDifficulty->addOptionValue("Easy", FText::FromString("Easy"));
    gameDifficulty->addOptionValue("Normal", FText::FromString("Normal"));
    gameDifficulty->addOptionValue("Hard", FText::FromString("Hard"));

    // set path to the setter and getters
    gameDifficulty->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(getmCurrentGameDifficulty));
    gameDifficulty->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(setmCurrentGameDifficulty));

    gameplayTabCollection->addChildListData(gameDifficulty);

    const auto testItem = NewObject<UMKUI_ListDataObjectString>();
    testItem->setmDataId("testItem");
    testItem->setmDataDisplayName(FText::FromString(TEXT("Test Item")));
    gameplayTabCollection->addChildListData(testItem);
    
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
