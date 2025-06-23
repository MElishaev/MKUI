// MAAKU Studio all rights reserved


#include "Widgets/Components/MKUI_CommonListView.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/MKUI_DataListEntryMapping.h"
#include "Widgets/Options/ListEntries/MKUI_ListEntryBase.h"
#include "Widgets/Options/DataObjects/MKUI_ListDataObjectBase.h"

UUserWidget& UMKUI_CommonListView::OnGenerateEntryWidgetInternal(UObject* Item,
                                                                 TSubclassOf<UUserWidget> DesiredEntryClass,
                                                                 const TSharedRef<STableViewBase>& OwnerTable)
{
    if (IsDesignTime()) {
        // in design time let the common list do its own thing because we don't care
        return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
    }

    if (auto foundEntryClass = mDataToEntryMapping->findEntryByDataObject(CastChecked<UMKUI_ListDataObjectBase>(Item))) {
        return GenerateTypedEntry<UMKUI_ListEntryBase>(foundEntryClass, OwnerTable);
    }

    return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

#if WITH_EDITOR
void UMKUI_CommonListView::ValidateCompiledDefaults(IWidgetCompilerLog& compileLog) const
{
    // todo - is there a way that this will popup when i try to play and not only when i try to compile from inside the BP?
    Super::ValidateCompiledDefaults(compileLog);
    if (!mDataToEntryMapping) {
        compileLog.Error(FText::FromString(TEXT("mDataToEntryMapping is not provided")));
    }
}
#endif
