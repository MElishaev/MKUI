// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MKUI_OptionsDataRegistry.generated.h"

class UMKUI_ListDataObjectBase;
class UMKUI_ListDataObjectCollection;
class ULocalPlayer;

/**
 * 
 */
UCLASS()
class MK_UI_API UMKUI_OptionsDataRegistry : public UObject
{
    GENERATED_BODY()

public:
    // should be called right after allocation of this object
    void init(ULocalPlayer* owningLocalPlayer);

    const TArray<UMKUI_ListDataObjectCollection*>& getRegisteredTabCollections() const { return mRegisteredTabCollections; } 
    
    TArray<UMKUI_ListDataObjectBase*> getListSourceItemsBySelectedTabId(const FName tabId) const;
    
private:
    void findChildListDataRecursively(UMKUI_ListDataObjectBase* parentData, TArray<UMKUI_ListDataObjectBase*>& foundChildren) const;
    
    void initGameplayCollectionTab();
    void initAudioCollectionTab();
    void initVideoCollectionTab();
    void initControlCollectionTab(ULocalPlayer* owningLocalPlayer);
    
    UPROPERTY(Transient)
    TArray<UMKUI_ListDataObjectCollection*> mRegisteredTabCollections; // todo: why not map?
};
