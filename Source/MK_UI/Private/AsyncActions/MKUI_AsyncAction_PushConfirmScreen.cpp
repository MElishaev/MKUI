// MAAKU Studio all rights reserved


#include "AsyncActions/MKUI_AsyncAction_PushConfirmScreen.h"
#include "Engine/Engine.h"
#include "Subsystems/MKUI_LocalPlayerSubsystem.h"

UMKUI_AsyncAction_PushConfirmScreen* UMKUI_AsyncAction_PushConfirmScreen::pushConfirmScreen(const UObject* wco,
                                                                                            EConfirmScreenType screenType,
                                                                                            FText screenTitle,
                                                                                            FText screenMsg)
{
    if (GEngine) {
        if (auto world = GEngine->GetWorldFromContextObject(wco, EGetWorldErrorMode::LogAndReturnNull)) {
            auto createdAction = NewObject<UMKUI_AsyncAction_PushConfirmScreen>();
            createdAction->mCachedUISubsystem = UMKUI_LocalPlayerSubsystem::getInstance(wco);
            createdAction->mCachedScreenMsg = screenMsg;
            createdAction->mCachedScreenTitle = screenTitle;
            createdAction->mCachedScreenType = screenType;

            createdAction->RegisterWithGameInstance(world);

            return createdAction;
        }
    }
    return nullptr;
}

void UMKUI_AsyncAction_PushConfirmScreen::Activate()
{
    if (UMKUI_LocalPlayerSubsystem* uiSubsystem = mCachedUISubsystem.Get()) {
        uiSubsystem->pushConfirmScreenToModalStackAsync(
            mCachedScreenType,
            mCachedScreenTitle,
            mCachedScreenMsg,
            [this](EConfirmScreenButtonType clickedButtonType) {
                onButtonClicked.Broadcast(clickedButtonType);
                SetReadyToDestroy();
            });
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("MK_UI could not resolve the owning local player's UI subsystem."));
        SetReadyToDestroy();
    }
}
