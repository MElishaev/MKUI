// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListDataObjectBase.h"
#include "MKUI_ListDataObjectKeyRemap.generated.h"

struct FPlayerKeyMapping;
enum class EPlayerMappableKeySlot : uint8;
enum class ECommonInputType : uint8;
class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputUserSettings;

/**
 * This class represents the data needed for key remapping in the game settings 
 */
UCLASS()
class MK_UI_API UMKUI_ListDataObjectKeyRemap : public UMKUI_ListDataObjectBase
{
    GENERATED_BODY()

public:
    void initKeyRemapData(UEnhancedInputUserSettings* inputUserSettings,
                          UEnhancedPlayerMappableKeyProfile* mappableKeyProfile,
                          ECommonInputType inputKeyType,
                          const FPlayerKeyMapping& keyMapping);

    void bindNewInputKey(const FKey& newKey);
    
    FSlateBrush getIconFromCurrentKey() const;

    FORCEINLINE ECommonInputType getDesiredInputType() const { return mCachedDesiredInputKeyType; }
    
private:
    FPlayerKeyMapping* getOwningKeyMapping() const;
    
    UPROPERTY(Transient)
    UEnhancedInputUserSettings* mCachedUserSettings;

    UPROPERTY(Transient)
    UEnhancedPlayerMappableKeyProfile* mCachedMappableKeyProfile;

    ECommonInputType mCachedDesiredInputKeyType;
    FName mCachedMappingName;
    EPlayerMappableKeySlot mCachedMappableKeySlot;
};
