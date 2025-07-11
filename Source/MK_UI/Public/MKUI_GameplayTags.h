// MAAKU Studio all rights reserved

#pragma once

#include "NativeGameplayTags.h"

namespace MKUI_GameplayTags
{
    // MK_UI_API macro will give us access to the gameplay tags defined here in other modules of the project
    // They will also be shown in the project settings under gameplay tags
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widgetStack_modal);
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widgetStack_gameMenu);
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widgetStack_gameHUD);
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widgetStack_frontend);

    // UI Widgets
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widget_pressAnyKeyScreen);
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widget_mainMenuScreen);
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widget_confirmScreen);
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widget_storyScreen);
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widget_optionsScreen);
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_widget_keyRemapScreen);

    // Options image
    MK_UI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MKUI_image_testImage);
}
