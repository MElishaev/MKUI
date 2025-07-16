// MAAKU Studio all rights reserved


#include "AsyncActions/MKUI_AsyncAction_PushConfirmScreen.h"
#include "Engine/Engine.h"
#include "Subsystems/MKUI_Subsystem.h"

UMKUI_AsyncAction_PushConfirmScreen* UMKUI_AsyncAction_PushConfirmScreen::pushConfirmScreen(const UObject* wco,
                                                                                            EConfirmScreenType screenType,
                                                                                            FText screenTitle,
                                                                                            FText screenMsg)
{
    if (GEngine) {
        if (auto world = GEngine->GetWorldFromContextObject(wco, EGetWorldErrorMode::LogAndReturnNull)) {
            auto createdAction = NewObject<UMKUI_AsyncAction_PushConfirmScreen>();
            createdAction->mCachedOwningWorld = world;
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
    UMKUI_Subsystem::getInstance(mCachedOwningWorld.Get())->pushConfirmScreenToModalStackAsync(
        mCachedScreenType,
        mCachedScreenTitle,
        mCachedScreenMsg,
        [this](EConfirmScreenButtonType clickedButtonType) {
            onButtonClicked.Broadcast(clickedButtonType);
            SetReadyToDestroy();
        });
}
