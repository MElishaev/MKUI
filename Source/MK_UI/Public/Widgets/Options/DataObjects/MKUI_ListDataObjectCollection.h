// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListDataObjectBase.h"
#include "MKUI_ListDataObjectCollection.generated.h"

/**
 * 
 */
UCLASS()
class MK_UI_API UMKUI_ListDataObjectCollection : public UMKUI_ListDataObjectBase
{
    GENERATED_BODY()

public:
    void addChildListData(UMKUI_ListDataObjectBase* dataToAdd);
    
    virtual TArray<UMKUI_ListDataObjectBase*> getAllChildListData() const override;

    virtual bool hasAnyChildData() const override;

    void sortByName();
    
private:
    UPROPERTY(Transient)
    TArray<UMKUI_ListDataObjectBase*> mChildListDataArray;
};
