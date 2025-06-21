// MAAKU Studio all rights reserved


#include "Widgets/MKUI_W_ConfirmScreen.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/MKUI_CommonButtonBase.h"
#include "ICommonInputModule.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::createOkScreenInfo(const FText& title, const FText& msg)
{
    auto infoObj = NewObject<UConfirmScreenInfoObject>();
    infoObj->mScreenTitle = title;
    infoObj->mScreenMessage = msg;

    FConfirmScreenButtonInfo okButtonInfo;
    okButtonInfo.mConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
    okButtonInfo.mButtonText = FText::FromString(TEXT("Confirm"));

    infoObj->mAvailableButtons.Add(okButtonInfo);

    return infoObj;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::createYesNoScreenInfo(const FText& title, const FText& msg)
{
    auto infoObj = NewObject<UConfirmScreenInfoObject>();
    infoObj->mScreenTitle = title;
    infoObj->mScreenMessage = msg;

    FConfirmScreenButtonInfo yesButtonInfo;
    yesButtonInfo.mConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    yesButtonInfo.mButtonText = FText::FromString(TEXT("Yes"));
    infoObj->mAvailableButtons.Add(yesButtonInfo);

    FConfirmScreenButtonInfo noButtonInfo;
    noButtonInfo.mConfirmScreenButtonType = EConfirmScreenButtonType::Canceled;
    noButtonInfo.mButtonText = FText::FromString(TEXT("No"));
    infoObj->mAvailableButtons.Add(noButtonInfo);

    return infoObj;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::createOkCancelScreenInfo(const FText& title, const FText& msg)
{
    auto infoObj = NewObject<UConfirmScreenInfoObject>();
    infoObj->mScreenTitle = title;
    infoObj->mScreenMessage = msg;

    FConfirmScreenButtonInfo okButtonInfo;
    okButtonInfo.mConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    okButtonInfo.mButtonText = FText::FromString(TEXT("Ok"));
    infoObj->mAvailableButtons.Add(okButtonInfo);

    FConfirmScreenButtonInfo cancelButtonInfo;
    cancelButtonInfo.mConfirmScreenButtonType = EConfirmScreenButtonType::Canceled;
    cancelButtonInfo.mButtonText = FText::FromString(TEXT("Cancel"));
    infoObj->mAvailableButtons.Add(cancelButtonInfo);

    return infoObj;
}

void UMKUI_W_ConfirmScreen::initConfirmScreen(UConfirmScreenInfoObject* infoObject,
                                              TFunction<void(EConfirmScreenButtonType)> clickedButtonCallback)
{
    check(infoObject && mTitle && mMessage && mButtons);

    mTitle->SetText(infoObject->mScreenTitle);
    mMessage->SetText(infoObject->mScreenMessage);

    // checking if contains old buttons created previously
    if (mButtons->GetNumEntries() != 0) {
        auto resetButtonWidgetCallback = [](UMKUI_CommonButtonBase& existingButton) {
            existingButton.OnClicked().Clear(); // clear all bound functions to this delegate  
        };
        // reset the buttons' dynamic entry box by calling the callback on every button before clearing it
        mButtons->Reset<UMKUI_CommonButtonBase>(resetButtonWidgetCallback);
    }

    // if we try to init confirm screen but no buttons were passed to populate the confirm screen - shouldn't happen
    check(!infoObject->mAvailableButtons.IsEmpty());

    // allocates the buttons for this confirm screen
    for (const auto& availableButtonInfo : infoObject->mAvailableButtons) {

        /* get the info about the click/back actions of common UI input we configured in the project setting */
        UInputAction* inputAction = nullptr;
        switch (availableButtonInfo.mConfirmScreenButtonType) {
            case EConfirmScreenButtonType::Confirmed:
                // commented this out because if the button type is "confirm" (say "Enter"), but the widget focuses on the "cancel" button
                // we dont want the action of pressing the "confirm" button to exit the game if we are
                // in confirmation screen to exit the game, the "no" is highlighted and we pressed enter..
                // inputAction = ICommonInputModule::GetSettings().GetEnhancedInputClickAction();
                break;
            case EConfirmScreenButtonType::Canceled:
            case EConfirmScreenButtonType::Closed:
                inputAction = ICommonInputModule::GetSettings().GetEnhancedInputBackAction();
                break;
            case EConfirmScreenButtonType::Unknown:
                break;
        }
        check(inputAction);

        auto addedButton = mButtons->CreateEntry<UMKUI_CommonButtonBase>(); // creates entry into the dynamic box and returns a ptr to it
        addedButton->setButtonText(availableButtonInfo.mButtonText);
        addedButton->SetTriggeringEnhancedInputAction(inputAction); // set button action based on the passed type
        
        // bind a function to the on clicked of the newly created button - the function is passed by the caller for
        // the creation of this confirm screen, hence the caller will handle the result of the interaction with this
        // confirm screen based on the type of the clicked button
        addedButton->OnClicked().AddLambda([clickedButtonCallback, availableButtonInfo, this]() {
            clickedButtonCallback(availableButtonInfo.mConfirmScreenButtonType);
            DeactivateWidget(); // after called the clicked callback this confirm screen can be closed
        });
    }

    if (mButtons->GetNumEntries() != 0) {
        // hopefully set focus on the "no/cancel" button preventing a mistaken double keyboard click or something
        // (also, this will focus the navigation on the new confirm screen for gamepad users)
        mButtons->GetAllEntries().Last()->SetFocus();
    }
}
