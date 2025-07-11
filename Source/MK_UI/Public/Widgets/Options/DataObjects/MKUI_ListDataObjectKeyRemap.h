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

    // this method tries to bind new key to this data object mapping. if key not already bound, the binding will
    // go through successfully and return true, otherwise return false
    bool tryBindNewInputKey(const FKey& newKey);
    
    // this method will bind the newKey to this data object - no matter if the key already bound to another action.
    // use tryBindNewInputKey if previous bindings matter to you
    void bindNewInputKey(const FKey& newKey);

    void unbindInputKey();
    
    FSlateBrush getIconFromCurrentKey() const;

    // get the key mapping that this data object represents 
    FPlayerKeyMapping* getOwningKeyMapping() const;
    
    virtual bool hasDefaultValue() const override;
    virtual bool canResetBackToDefaultValue() const override;
    // this should be called only through the reset button of the controls tab (but not through the individual binding reset button)
    virtual bool tryResetBackToDefaultValue() override;
    
    FORCEINLINE ECommonInputType getDesiredInputType() const { return mCachedDesiredInputKeyType; }
private:

    /**
     * checks if key already bound and returns true if it is
     */
    bool isKeyAlreadyBound(const FKey& keyToCheck) const;
    
    UPROPERTY(Transient)
    UEnhancedInputUserSettings* mCachedUserSettings;

    UPROPERTY(Transient)
    UEnhancedPlayerMappableKeyProfile* mCachedMappableKeyProfile;

    ECommonInputType mCachedDesiredInputKeyType;
    FName mCachedMappingName;
    EPlayerMappableKeySlot mCachedMappableKeySlot;
};
