// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MKUI_DataListEntryMapping.generated.h"

class UMKUI_ListEntryBase;
class UMKUI_ListDataObjectBase;
/**
 * 
 */
UCLASS()
class MK_UI_API UMKUI_DataListEntryMapping : public UDataAsset
{
    GENERATED_BODY()

public:
    TSubclassOf<UMKUI_ListEntryBase> findEntryByDataObject(UMKUI_ListDataObjectBase* dataObj) const;
    
private:
    /**
     * This member maps subclasses of ListDataObjects to their appropriate ListEntries (e.g. string/bool/integer etc.)
     */
    UPROPERTY(EditDefaultsOnly)
    TMap<TSubclassOf<UMKUI_ListDataObjectBase>, TSubclassOf<UMKUI_ListEntryBase>> mClassToEntryMapping;

};
