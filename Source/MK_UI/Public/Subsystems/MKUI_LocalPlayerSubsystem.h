// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MKUITypes/MKUISubsystemTypes.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MKUI_LocalPlayerSubsystem.generated.h"

enum class EConfirmScreenButtonType : uint8;
enum class EConfirmScreenType : uint8;
class APlayerController;
class UInputMappingContext;
class UMKUI_W_ActivatableBase;
class UMKUI_W_PrimaryLayout;

/**
 * Owns the MK_UI root layout and stack operations for one local player.
 *
 * The subsystem creates the configured primary layout when its local player
 * receives a Player Controller. This keeps UI lifetime independent of GameMode
 * and of any particular Player Controller subclass.
 */
UCLASS()
class MK_UI_API UMKUI_LocalPlayerSubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    /** Resolves the subsystem for a widget, Player Controller, Local Player, or world context. */
    static UMKUI_LocalPlayerSubsystem* getInstance(const UObject* worldContextObject);

    virtual void Initialize(FSubsystemCollectionBase& collection) override;
    virtual void Deinitialize() override;
    virtual void PlayerControllerChanged(APlayerController* newPlayerController) override;

    UFUNCTION(BlueprintPure, Category = "MKUI")
    UMKUI_W_PrimaryLayout* getPrimaryLayout() const { return mPrimaryLayout; }

    /** Adopts a layout created by the deprecated controller-driven integration path. */
    void registerPrimaryLayoutWidget(UMKUI_W_PrimaryLayout* widget);

    UFUNCTION(BlueprintCallable, Category = "MKUI")
    void removeAllWidgetsFromStack(UPARAM(meta=(Categories="MKUI.widgetStack")) const FGameplayTag widgetStackTag);

    void pushSoftWidgetToStackAsync(
        const FGameplayTag& widgetStackTag,
        TSoftClassPtr<UMKUI_W_ActivatableBase> widgetClass,
        TFunction<void(EAsyncPushWidgetState, UMKUI_W_ActivatableBase*)> asyncPushStateCallback);

    void pushConfirmScreenToModalStackAsync(
        EConfirmScreenType screenType,
        const FText& screenTitle,
        const FText& screenMsg,
        TFunction<void(EConfirmScreenButtonType)> buttonClickedCallback);

private:
    void createPrimaryLayout(APlayerController* owningPlayerController);
    void applyGenericInputMapping();
    void removeGenericInputMapping();

    /** Primary Common UI root owned by this local player. */
    UPROPERTY(Transient)
    TObjectPtr<UMKUI_W_PrimaryLayout> mPrimaryLayout;

    /** Generic UI mapping currently registered with Enhanced Input. */
    UPROPERTY(Transient)
    TObjectPtr<UInputMappingContext> mAppliedInputMappingContext;
};
