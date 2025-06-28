// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_OptionsDataRegistry.h"

#include "MKUI_FunctionLibrary.h"
#include "MKUI_GameplayTags.h"
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
    
    // TODO: best case will be to read and populate this data from some file that can be changed without recompiling the project
    
    // in this section allocate all the different settings for this tab
    {
        const auto gameDifficulty = NewObject<UMKUI_ListDataObjectString>();
        gameDifficulty->setmDataId("gameDifficulty");
        gameDifficulty->setmDataDisplayName(FText::FromString(TEXT("Difficulty")));
        gameDifficulty->setmDescriptionRichText(FText::FromString(TEXT("This lets you control how challenging you want your experience to be")));
        gameDifficulty->addOptionValue(TEXT("Easy"), FText::FromString("Easy"));
        gameDifficulty->addOptionValue(TEXT("Normal"), FText::FromString("Normal"));
        gameDifficulty->addOptionValue(TEXT("Hard"), FText::FromString("Hard"));
        gameDifficulty->setDefaultValueFromString(TEXT("Normal"));

        // set path to the setter and getters
        gameDifficulty->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(getmCurrentGameDifficulty));
        gameDifficulty->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(setmCurrentGameDifficulty));

        gameplayTabCollection->addChildListData(gameDifficulty);
    }

    // test item for testing functionality when multiple values in the options tab
    {
        const auto testItem = NewObject<UMKUI_ListDataObjectString>();
        testItem->setmDataId("testItem");
        testItem->setmDataDisplayName(FText::FromString(TEXT("Test Item")));
        testItem->setmSoftDescriptionImage(UMKUI_FunctionLibrary::getOptionsSoftImageByTag(MKUI_GameplayTags::MKUI_image_testImage));
        gameplayTabCollection->addChildListData(testItem);
    }
    
    mRegisteredTabCollections.Add(gameplayTabCollection);
}

void UMKUI_OptionsDataRegistry::initAudioCollectionTab()
{
    const auto audioTabCollection = NewObject<UMKUI_ListDataObjectCollection>();
    audioTabCollection->setmDataId(FName("audioTabCollection"));
    audioTabCollection->setmDataDisplayName(FText::FromString(TEXT("Audio")));

    // volume category - collection of options related to the volume options
    {
        auto volumeCategoryCollection = NewObject<UMKUI_ListDataObjectCollection>();
        volumeCategoryCollection->setmDataId(FName("volumeCategoryCollection"));
        volumeCategoryCollection->setmDataDisplayName(FText::FromString(TEXT("Volume")));

        audioTabCollection->addChildListData(volumeCategoryCollection);

        // test item for category
        {
            auto testItem = NewObject<UMKUI_ListDataObjectString>();
            testItem->setmDataId("testItem");
            testItem->setmDataDisplayName(FText::FromString(TEXT("Test Item")));
            volumeCategoryCollection->addChildListData(testItem);
        }
    }

    // sound category
    {
        
    }
    
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
