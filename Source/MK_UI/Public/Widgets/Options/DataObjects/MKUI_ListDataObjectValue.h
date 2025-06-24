// MAAKU Studio all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "MKUI_ListDataObjectBase.h"
#include "MKUI_ListDataObjectValue.generated.h"

class MKUI_FOptionsDataInteractionHelper;
/**
 * This class represents the value of a setting in the option menu.
 * This class will act as a base class for different values possible in an options menu.
 */
UCLASS(Abstract)
class MK_UI_API UMKUI_ListDataObjectValue : public UMKUI_ListDataObjectBase
{
    GENERATED_BODY()

public:
    void setmDataDynamicGetter(const TSharedPtr<MKUI_FOptionsDataInteractionHelper>& dynamicGetter) { mDataDynamicGetter = dynamicGetter; }
    void setmDataDynamicSetter(const TSharedPtr<MKUI_FOptionsDataInteractionHelper>& dynamicSetter) { mDataDynamicSetter = dynamicSetter; }
    
protected:
    /* Below members store the path to the getters and setters for the value of
     * the settings option that the child classes of this class represents */
    TSharedPtr<MKUI_FOptionsDataInteractionHelper> mDataDynamicGetter;
    TSharedPtr<MKUI_FOptionsDataInteractionHelper> mDataDynamicSetter;

};
