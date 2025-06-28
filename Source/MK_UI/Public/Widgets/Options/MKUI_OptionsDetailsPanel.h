// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MKUI_OptionsDetailsPanel.generated.h"

class UMKUI_ListDataObjectBase;
class UCommonTextBlock;
class UCommonLazyImage;
class UCommonRichTextBlock;
/**
 * This base class (for deriving as BP) represent the panel for showing various options' details.
 * may contain images, text, maybe short GIFs.
 * A simple UserWidget as a parent class was chosen because this widget is for displaying
 * details only - no user interaction is needed with this panel, hence we don't need the
 * functionalities of CommonUI regarding user input.
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class MK_UI_API UMKUI_OptionsDetailsPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    void updateDetailsPanelInfo(UMKUI_ListDataObjectBase* dataObj, const FString& className = "");

    // clears the info in the details panel
    void clearDetailsPanelInfo();

protected:
    virtual void NativeOnInitialized() override;
private:
    /************ Bound widgets *****************/
    UPROPERTY(meta=(BindWidget, AllowPrivateAccess="true"))
    UCommonTextBlock* mTitle; // title of a specific setting details - i think this should be as the name of the setting

    UPROPERTY(meta=(BindWidget, AllowPrivateAccess="true"))
    UCommonLazyImage* mDescriptionImage; // image for options that image is needed (maybe video options or HUD related examples)

    UPROPERTY(meta=(BindWidget, AllowPrivateAccess="true"))
    UCommonRichTextBlock* mDescription; // description for the setting - what generally this settings control

    UPROPERTY(meta=(BindWidget, AllowPrivateAccess="true"))
    UCommonRichTextBlock* mDynamicDetails; // details for the setting - like what each option means

    UPROPERTY(meta=(BindWidget, AllowPrivateAccess="true"))
    UCommonRichTextBlock* mDisabledReason;
    /************ Bound widgets *****************/

};
