// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_W_OptionsScreen.h"

#include "ICommonInputModule.h"
#include "MKUI_DebugHelper.h"
#include "Input/CommonUIInputTypes.h"

void UMKUI_W_OptionsScreen::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    /* here we need custom bound actions to be added to our Options screen bar.
     * apart from the "Back" action that is provided by default with CommonUI, we need also
     * "Reset" action for resetting the options to their default state. This mean we need to
     * create manually the actions and register them to be LISTENED TO BY THIS WIDGET. 
     */
    
    if (mEnhancedInputResetAction) {
        const FBindUIActionArgs resetActionArgs = FBindUIActionArgs(mEnhancedInputResetAction,
                                                                    true,
                                                                    FSimpleDelegate::CreateUObject(
                                                                        this,
                                                                        &ThisClass::onResetBoundActionTriggered));

        mResetActionHandle = RegisterUIActionBinding(resetActionArgs);
    }

    const FBindUIActionArgs backActionArgs = FBindUIActionArgs(ICommonInputModule::GetSettings().GetEnhancedInputBackAction(),
                                                               true,
                                                               FSimpleDelegate::CreateUObject(
                                                                   this,
                                                                   &ThisClass::onBackBoundActionTriggered));

    RegisterUIActionBinding(backActionArgs);
}

void UMKUI_W_OptionsScreen::onResetBoundActionTriggered()
{
    // show confirmation screen for the user to make sure he want to reset the settings
    MKUI_Debug::print(TEXT("reset button clicked"));
}

void UMKUI_W_OptionsScreen::onBackBoundActionTriggered()
{
    DeactivateWidget();
}
