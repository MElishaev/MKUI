// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_OptionsDataRegistry.h"

#include "MKUI_FunctionLibrary.h"
#include "MKUI_GameplayTags.h"
#include "Settings/MKUI_GameUserSettings.h"
#include "Widgets/Options/MKUI_OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectCollection.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectScalar.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectString.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectStringResolution.h"

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

    TArray<UMKUI_ListDataObjectBase*> allChildListItemsRecursive;
    for (auto childListData : (*tab)->getAllChildListData()) {
        if (childListData) {
            allChildListItemsRecursive.Add(childListData);
            if (childListData->hasAnyChildData()) {
                findChildListDataRecursively(childListData, allChildListItemsRecursive);
            }
        }
    }
    
    return allChildListItemsRecursive;
}

void UMKUI_OptionsDataRegistry::findChildListDataRecursively(UMKUI_ListDataObjectBase* parentData,
                                                             TArray<UMKUI_ListDataObjectBase*>& foundChildren) const
{
    if (!parentData || !parentData->hasAnyChildData()) {
        return;
    }

    for (auto subChildListData : parentData->getAllChildListData()) {
        if (subChildListData) {
            foundChildren.Add(subChildListData);
            if (subChildListData->hasAnyChildData()) {
                findChildListDataRecursively(subChildListData, foundChildren);
            }
        }
    }
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

        // overall volume 
        {
            auto overallVolume = NewObject<UMKUI_ListDataObjectScalar>();
            overallVolume->setmDataId("overallVolume");
            overallVolume->setmDataDisplayName(FText::FromString(TEXT("Overall Volume")));
            overallVolume->setmDescriptionRichText(FText::FromString(TEXT("This is a description")));
            overallVolume->setmDisplayValueRange(TRange<float>(0.f, 1.f));
            overallVolume->setmOutputValueRange(TRange<float>(0.f, 2.f));
            overallVolume->setmSliderStepSize(0.01f);
            overallVolume->setDefaultValueFromString(LexToString(1.f)); // this is 50% because the output value range is [0,2]
            overallVolume->setmDisplayNumericType(ECommonNumericType::Percentage);
            overallVolume->setmNumberFormattingOptions(UMKUI_ListDataObjectScalar::noDecimal());

            overallVolume->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(getOverallVolume));
            overallVolume->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(setOverallVolume));

            // this setting has slider so the apply settings will be triggered "manually" on mouse capture end on the slider
            overallVolume->setmbShouldApplySettingImmediately(false);
            
            volumeCategoryCollection->addChildListData(overallVolume);
        }

        // music volume
        {
            auto musicVolume = NewObject<UMKUI_ListDataObjectScalar>();
            musicVolume->setmDataId("musicVolume");
            musicVolume->setmDataDisplayName(FText::FromString(TEXT("Music Volume")));
            musicVolume->setmDescriptionRichText(FText::FromString(TEXT("This is a description")));
            musicVolume->setmDisplayValueRange(TRange<float>(0.f, 1.f));
            musicVolume->setmOutputValueRange(TRange<float>(0.f, 2.f));
            musicVolume->setmSliderStepSize(0.01f);
            musicVolume->setDefaultValueFromString(LexToString(1.f));
            musicVolume->setmDisplayNumericType(ECommonNumericType::Percentage);
            musicVolume->setmNumberFormattingOptions(UMKUI_ListDataObjectScalar::noDecimal());

            musicVolume->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(getMusicVolume));
            musicVolume->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(setMusicVolume));

            // this setting has slider so the apply settings will be triggered "manually" on mouse capture end on the slider
            musicVolume->setmbShouldApplySettingImmediately(false);
            
            volumeCategoryCollection->addChildListData(musicVolume);
        }

        // SFX volume
        {
            auto sfxVolume = NewObject<UMKUI_ListDataObjectScalar>();
            sfxVolume->setmDataId("musicVolume");
            sfxVolume->setmDataDisplayName(FText::FromString(TEXT("Sound Effects Volume")));
            sfxVolume->setmDescriptionRichText(FText::FromString(TEXT("This is a description")));
            sfxVolume->setmDisplayValueRange(TRange<float>(0.f, 1.f));
            sfxVolume->setmOutputValueRange(TRange<float>(0.f, 2.f));
            sfxVolume->setmSliderStepSize(0.01f);
            sfxVolume->setDefaultValueFromString(LexToString(1.f)); // default is 50%
            sfxVolume->setmDisplayNumericType(ECommonNumericType::Percentage);
            sfxVolume->setmNumberFormattingOptions(UMKUI_ListDataObjectScalar::noDecimal());

            sfxVolume->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(getSFXVolume));
            sfxVolume->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(setSFXVolume));

            // this setting has slider so the apply settings will be triggered "manually" on mouse capture end on the slider
            sfxVolume->setmbShouldApplySettingImmediately(false);
            
            volumeCategoryCollection->addChildListData(sfxVolume);
        }
    }

    // sound category
    {
        auto soundCategory = NewObject<UMKUI_ListDataObjectCollection>();
        soundCategory->setmDataId("soundCategory");
        soundCategory->setmDataDisplayName(FText::FromString(TEXT("Sound")));

        audioTabCollection->addChildListData(soundCategory);

        // allow background audio
        {
            auto allowBackgroundAudio = NewObject<UMKUI_ListDataObjectStringBool>();
            allowBackgroundAudio->setmDataId("allowBackgroundAudio");
            allowBackgroundAudio->setmDataDisplayName(FText::FromString(TEXT("Allow Background Audio")));
            allowBackgroundAudio->overrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
            allowBackgroundAudio->overrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
            allowBackgroundAudio->setFalseAsDefaultValue();
            allowBackgroundAudio->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(getAllowBackgroundAudio));
            allowBackgroundAudio->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(setAllowBackgroundAudio));
            allowBackgroundAudio->setmbShouldApplySettingImmediately(true);

            soundCategory->addChildListData(allowBackgroundAudio);
        }

        // use HDR audio
        {
            auto useHDRAudio = NewObject<UMKUI_ListDataObjectStringBool>();
            useHDRAudio->setmDataId("useHDRAudio");
            useHDRAudio->setmDataDisplayName(FText::FromString(TEXT("Use HDR Audio Mode")));
            useHDRAudio->overrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
            useHDRAudio->overrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
            useHDRAudio->setFalseAsDefaultValue();
            useHDRAudio->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(getUseHDRAudioMode));
            useHDRAudio->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(setUseHDRAudioMode));
            useHDRAudio->setmbShouldApplySettingImmediately(true);

            soundCategory->addChildListData(useHDRAudio);
        }
    }
    
    mRegisteredTabCollections.Add(audioTabCollection);
}

void UMKUI_OptionsDataRegistry::initVideoCollectionTab()
{
    const auto videoTabCollection = NewObject<UMKUI_ListDataObjectCollection>();
    videoTabCollection->setmDataId(FName("videoTabCollection"));
    videoTabCollection->setmDataDisplayName(FText::FromString(TEXT("Video")));

    UMKUI_ListDataObjectStringEnum* windowMode = nullptr; // in this scope because we need to use it for edit condition of screen resolution
    
    /****************************** display category **********************************/
    {
        auto displayCategory = NewObject<UMKUI_ListDataObjectCollection>();
        displayCategory->setmDataId("displayCategory");
        displayCategory->setmDataDisplayName(FText::FromString(TEXT("Display")));

        videoTabCollection->addChildListData(displayCategory);

        // created edit conditions
        FOptionsDataEditConditionDescriptor packagedOnlyBuildCondition;
        packagedOnlyBuildCondition.setmEditConditionFunc([]() {
            return !(GIsEditor || GIsPlayInEditorWorld); // returns true only if packaged build  
        });
        packagedOnlyBuildCondition.setmDisabledRichStringReason(TEXT("<Disabled>This condition is modifyable only in packaged builds</>"));
        
        // window mode
        {
            windowMode = NewObject<UMKUI_ListDataObjectStringEnum>();
            windowMode->setmDataId("windowMode");
            windowMode->setmDataDisplayName(FText::FromString(TEXT("Fullscreen Mode")));
            windowMode->setmDescriptionRichText(FText::FromString(TEXT("This is description for full screen option")));
            windowMode->addEnumOption(EWindowMode::Fullscreen, FText::FromString(TEXT("Fullscreen")));
            windowMode->addEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
            windowMode->addEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Window")));
            windowMode->setDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
            windowMode->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(GetFullscreenMode));
            windowMode->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(SetFullscreenMode));
            windowMode->setmbShouldApplySettingImmediately(true);
            windowMode->addEditCondition(packagedOnlyBuildCondition);

            displayCategory->addChildListData(windowMode);
        }

        // screen resolution
        {
            auto screenResolution = NewObject<UMKUI_ListDataObjectStringResolution>();
            screenResolution->setmDataId("screenResolution");
            screenResolution->setmDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
            screenResolution->setmDescriptionRichText(FText::FromString(TEXT("This is description for resolution option")));
            screenResolution->initScreenResolutionOption();
            screenResolution->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(GetScreenResolution));
            screenResolution->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(SetScreenResolution));
            screenResolution->setmbShouldApplySettingImmediately(true);
            screenResolution->addEditCondition(packagedOnlyBuildCondition);

            // add window mode condition
            FOptionsDataEditConditionDescriptor windowModeCondition;
            windowModeCondition.setmEditConditionFunc([windowMode]() {
                // screen resolution is editable if not set to borderless window
                return (windowMode->getCurrentValueAsEnum<EWindowMode::Type>() != EWindowMode::WindowedFullscreen);
            });
            windowModeCondition.setmDisabledRichStringReason(TEXT("\n<Disabled>Screen resolution isn't modifyable when set to borderless window</>"));
            windowModeCondition.setmDisabledForcedStringValue(screenResolution->getmMaxResolutionSupported());
            screenResolution->addEditCondition(windowModeCondition);

            screenResolution->addDataDependency(windowMode);

            displayCategory->addChildListData(screenResolution);
        }
    }

    /****************************** graphics category **********************************/
    {
        auto graphicsCategory = NewObject<UMKUI_ListDataObjectCollection>();
        graphicsCategory->setmDataId("graphicsCategory");
        graphicsCategory->setmDataDisplayName(FText::FromString(TEXT("Graphics")));
        videoTabCollection->addChildListData(graphicsCategory);

        // display gamma
        {
            auto gamma = NewObject<UMKUI_ListDataObjectScalar>();
            gamma->setmDataId("gamma");
            gamma->setmDataDisplayName(FText::FromString(TEXT("Brightness")));
            gamma->setmDescriptionRichText(FText::FromString(TEXT("Controls the brightness of the game.")));
            gamma->setmDisplayValueRange(TRange<float>(0.f, 1.f));
            gamma->setmOutputValueRange(TRange<float>(1.7f, 2.7f)); // UE internal Gamma range
            gamma->setDefaultValueFromString(LexToString(2.2f));
            gamma->setmDisplayNumericType(ECommonNumericType::Percentage);
            gamma->setmNumberFormattingOptions(UMKUI_ListDataObjectScalar::noDecimal());
            gamma->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(getGamma));
            gamma->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(setGamma));
            
            graphicsCategory->addChildListData(gamma);
        }

        // overall quality
        UMKUI_ListDataObjectStringInteger* overallQuality = nullptr;
        {
            overallQuality = NewObject<UMKUI_ListDataObjectStringInteger>();
            overallQuality->setmDataId("overallQuality");
            overallQuality->setmDataDisplayName(FText::FromString(TEXT("Overall Quality")));
            overallQuality->setmDescriptionRichText(FText::FromString(TEXT("Controls the overall quality of the game.")));
            overallQuality->addIntegerOption(0, FText::FromString(TEXT("Low")));
            overallQuality->addIntegerOption(1, FText::FromString(TEXT("Normal")));
            overallQuality->addIntegerOption(2, FText::FromString(TEXT("High")));
            overallQuality->addIntegerOption(3, FText::FromString(TEXT("Epic")));
            overallQuality->addIntegerOption(4, FText::FromString(TEXT("Cinematic")));
            overallQuality->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(GetOverallScalabilityLevel));
            overallQuality->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(SetOverallScalabilityLevel));
            overallQuality->setmbShouldApplySettingImmediately(true);
            
            graphicsCategory->addChildListData(overallQuality);
        }

        // resolution scale
        {
            auto resolutionScale = NewObject<UMKUI_ListDataObjectScalar>();
            resolutionScale->setmDataId("resolutionScale");
            resolutionScale->setmDataDisplayName(FText::FromString(TEXT("3D Resolution")));
            resolutionScale->setmDescriptionRichText(FText::FromString(TEXT("Controls quality of rendered objects in game.")));
            resolutionScale->setmDisplayValueRange(TRange<float>(0.f, 1.f));
            resolutionScale->setmOutputValueRange(TRange<float>(0.f, 1.f));
            resolutionScale->setmDisplayNumericType(ECommonNumericType::Percentage);
            resolutionScale->setmNumberFormattingOptions(UMKUI_ListDataObjectScalar::noDecimal());
            resolutionScale->setmDataDynamicGetter(MAKE_OPTIONS_DATA_ACCESSORS(GetResolutionScaleNormalized));
            resolutionScale->setmDataDynamicSetter(MAKE_OPTIONS_DATA_ACCESSORS(SetResolutionScaleNormalized));
            resolutionScale->setmbShouldApplySettingImmediately(true);

            resolutionScale->addDataDependency(overallQuality);
            
            graphicsCategory->addChildListData(resolutionScale);
        }
    }
    
    mRegisteredTabCollections.Add(videoTabCollection);
}

void UMKUI_OptionsDataRegistry::initControlCollectionTab()
{
    const auto controlsTabCollection = NewObject<UMKUI_ListDataObjectCollection>();
    controlsTabCollection->setmDataId(FName("controlsTabCollection"));
    controlsTabCollection->setmDataDisplayName(FText::FromString(TEXT("Controls")));
    mRegisteredTabCollections.Add(controlsTabCollection);
}
