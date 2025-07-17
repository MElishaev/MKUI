// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MKUI_AsyncAction_PushConfirmScreen.generated.h"

enum class EConfirmScreenButtonType : uint8;
enum class EConfirmScreenType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmButtonClicked, EConfirmScreenButtonType, buttonType);

/**
 * @see MKUI_AsyncAction_PushSoftWidget for explanation of async actions 
 */
UCLASS()
class MK_UI_API UMKUI_AsyncAction_PushConfirmScreen : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable,
        meta=(WorldContext="wco", HidePin="wco", BlueprintInternalUseOnly="true", DisplayName="Show Confirmation Screen"))
    static UMKUI_AsyncAction_PushConfirmScreen* pushConfirmScreen(const UObject* wco,
                                                                  EConfirmScreenType screenType,
                                                                  FText screenTitle,
                                                                  FText screenMsg);

    virtual void Activate() override;

    UPROPERTY(BlueprintAssignable)
    FOnConfirmButtonClicked onButtonClicked;
private:
    TWeakObjectPtr<UWorld> mCachedOwningWorld;
    EConfirmScreenType mCachedScreenType;
    FText mCachedScreenTitle;
    FText mCachedScreenMsg;
};
