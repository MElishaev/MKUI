// MAAKU Studio all rights reserved


#include "Widgets/Options/MKUI_W_KeyRemapScreen.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "CommonRichTextBlock.h"
#include "ICommonInputModule.h"
#include "CommonUITypes.h"
#include "Framework/Application/IInputProcessor.h"

/**
 * This class will be instantiated when we need to listen and process key pressed by the user in the UI.
 * For example, when the tries to rebind a key, we will need to listen to any key pressed that he want
 * the new binding to be - this class handles it.
 */
class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
    FKeyRemapScreenInputPreprocessor(ECommonInputType inputTypeToListenTo, ULocalPlayer* owningLocalPlayer) :
        mCachedInputTypeToListenTo(inputTypeToListenTo),
        mCachedWeakOwningLocalPlayer(owningLocalPlayer)
    {
    }

    DECLARE_DELEGATE_OneParam(FOnInputPreprocessorKeyPressedDelegate, const FKey& /*pressedKey*/)
    FOnInputPreprocessorKeyPressedDelegate mOnInputPreprocessorKeyPressed;

    DECLARE_DELEGATE_OneParam(FOnInputPreprocessorKeySelectCanceledDelegate, const FString& /*canceledReason*/)
    FOnInputPreprocessorKeySelectCanceledDelegate mOnInputPreprocessorKeySelectCanceled;

protected:
    virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
    {
        // nothing we want to do here
    }

    virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
    {
        processPressedKey(InKeyEvent.GetKey());
        return true;
    }

    virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
    {
        processPressedKey(MouseEvent.GetEffectingButton());
        return true;
    }

    void processPressedKey(const FKey& pressedKey) const
    {
        if (pressedKey == EKeys::Escape) {
            (void)mOnInputPreprocessorKeySelectCanceled.ExecuteIfBound(TEXT("Key remap has been canceled"));
            return;
        }

        switch (mCachedInputTypeToListenTo) {
            case ECommonInputType::MouseAndKeyboard:
                if (pressedKey.IsGamepadKey()) {
                    (void)mOnInputPreprocessorKeySelectCanceled.ExecuteIfBound(
                        TEXT("Detected gamepad key pressed for keyboard inputs - key remap has been canceled"));
                    return;
                }
                break;
            case ECommonInputType::Gamepad:
                if (!pressedKey.IsGamepadKey()) {
                    (void)mOnInputPreprocessorKeySelectCanceled.ExecuteIfBound(
                        TEXT("Detected keyboard key pressed for gamepad inputs - key remap has been canceled"));
                    return;
                }
                break;
        }

        (void)mOnInputPreprocessorKeyPressed.ExecuteIfBound(pressedKey);
    }

private:
    ECommonInputType mCachedInputTypeToListenTo;
    TWeakObjectPtr<ULocalPlayer> mCachedWeakOwningLocalPlayer;
};


void UMKUI_W_KeyRemapScreen::setDesiredInputTypeToFilter(ECommonInputType desiredInputType)
{
    mCachedDesiredInputType = desiredInputType;
}

void UMKUI_W_KeyRemapScreen::NativeOnActivated()
{
    Super::NativeOnActivated();

    mCachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(mCachedDesiredInputType, GetOwningLocalPlayer());
    mCachedInputPreprocessor->mOnInputPreprocessorKeyPressed.BindUObject(this, &ThisClass::handleValidKeyPressed);
    mCachedInputPreprocessor->mOnInputPreprocessorKeySelectCanceled.BindUObject(this, &ThisClass::handleKeySelectionCanceled);

    // -1 sets the priority of this input processors over all the others because in key bindings
    // we want to be the first to catch any key pressed.
    FSlateApplication::Get().RegisterInputPreProcessor(mCachedInputPreprocessor, -1);

    FString inputDeviceName;
    switch (mCachedDesiredInputType) {
        case ECommonInputType::MouseAndKeyboard:
            inputDeviceName = TEXT("Mouse and Keyboard");
            break;
        case ECommonInputType::Gamepad:
            inputDeviceName = TEXT("Gamepad");
            break;
    }

    const FString displayRichMsg = FString::Printf(
        TEXT("<KeyRemapDefault>Press any</> <KeyRemapHighlight>%s</> <KeyRemapDefault>key.</>"),
        *inputDeviceName);

    mDisplayText->SetText(FText::FromString(displayRichMsg));
}

void UMKUI_W_KeyRemapScreen::NativeOnDeactivated()
{
    Super::NativeOnDeactivated();

    if (mCachedInputPreprocessor) {
        FSlateApplication::Get().UnregisterInputPreProcessor(mCachedInputPreprocessor);
        mCachedInputPreprocessor.Reset();
    }
}

void UMKUI_W_KeyRemapScreen::requestDeactivateWidget(TFunction<void()> preDeactivateCallback)
{
    // delay a tick for correct input processing
    FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([preDeactivateCallback, this](float deltaTime) {
        preDeactivateCallback(); // this internally broadcasts that key pressed / canceled
        DeactivateWidget();
        return false; // false to stop this lambda from keep ticking
    }));
    
}

void UMKUI_W_KeyRemapScreen::handleValidKeyPressed(const FKey& pressedKey)
{
    requestDeactivateWidget([pressedKey, this]() {
        (void) mOnKeyRemapScreenKeyPressed.ExecuteIfBound(pressedKey);
    });
}

void UMKUI_W_KeyRemapScreen::handleKeySelectionCanceled(const FString& cancelReason)
{
    requestDeactivateWidget([this, cancelReason]() {
        (void) mOnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(cancelReason);
    });
}
