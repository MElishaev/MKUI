// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MKUI_Subsystem.generated.h"

enum class EConfirmScreenButtonType : uint8;
enum class EConfirmScreenType : uint8;
class UMKUI_CommonButtonBase;
class UMKUI_W_ActivatableBase;
struct FGameplayTag;
class UMKUI_W_PrimaryLayout;

enum class EAsyncPushWidgetState : uint8
{
    OnCreatedBeforePush,
    AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionUpdated, UMKUI_CommonButtonBase*, button, FText, description);

/**
 * This subsystem act as an easy access to the UI and widgets system in the game.
 * (not sure why this can't be in HUD - though one reason can be that HUD must be
 * accessed through PlayerController which could be small annoyance, adding 1 more
 * indirection)
 */
UCLASS()
class MK_UI_API UMKUI_Subsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // method for easy access through cpp
    static UMKUI_Subsystem* getInstance(const UObject* worldContextObject);

    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

    UFUNCTION(BlueprintCallable, Category="MKUI")
    void registerPrimaryLayoutWidget(UMKUI_W_PrimaryLayout* widget);

    /**
     * This method pushed the soft pointer of a widget to the stack specified by widget tag and calls the
     * callback before and after the widget is pushed to the stack (this loads the widget from disk if not loaded)
     * @param widgetTag widget tag to which we want to add the loaded widget
     * @param widgetClass soft pointer to the widget we want to load
     * @param asyncPushStateCallback callbacks for before and after widget is pushed to the stack -
     *                               the state is specified by EAsyncPushWidgetState input param 
     */

    void pushSoftWidgetToStackAsync(const FGameplayTag& widgetTag,
                                    TSoftClassPtr<UMKUI_W_ActivatableBase> widgetClass,
                                    TFunction<void(EAsyncPushWidgetState, UMKUI_W_ActivatableBase*)> asyncPushStateCallback);

    // Modal is the term for popup screens that take control over the background
    void pushConfirmScreenToModalStackAsync(EConfirmScreenType screenType,
                                            const FText& screenTitle,
                                            const FText& screenMsg,
                                            TFunction<void(EConfirmScreenButtonType)> buttonClickedCallback);

public:
    UPROPERTY(BlueprintAssignable)
    FOnButtonDescriptionUpdated onButtonDescUpdated;

private:
    UPROPERTY(Transient)
    UMKUI_W_PrimaryLayout* mPrimaryLayout; // primary layout that holds all the widgets
};
