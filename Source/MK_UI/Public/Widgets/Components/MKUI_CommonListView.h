// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "MKUI_CommonListView.generated.h"

class UMKUI_DataListEntryMapping;
/**
 * This class is a container (list) for the various settings that a tab in the
 * "Options" menu will contain 
 */
UCLASS()
class MK_UI_API UMKUI_CommonListView : public UCommonListView
{
    GENERATED_BODY()

protected:
    virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item,
                                                       TSubclassOf<UUserWidget> DesiredEntryClass,
                                                       const TSharedRef<STableViewBase>& OwnerTable) override;

private:
#if WITH_EDITOR
    virtual void ValidateCompiledDefaults(IWidgetCompilerLog& compileLog) const override;
#endif

    UPROPERTY(EditAnywhere, Category="MKUI Settings")
    UMKUI_DataListEntryMapping* mDataToEntryMapping;
};
