// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MKUI_OptionsDataRegistry.generated.h"

class UMKUI_ListDataObjectCollection;
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
    
private:
    void initGameplayCollectionTab();
    void initAudioCollectionTab();
    void initVideoCollectionTab();
    void initControlCollectionTab();

    UPROPERTY(Transient)
    TArray<UMKUI_ListDataObjectCollection*> mRegisteredTabCollections; // todo: why not map?
};
