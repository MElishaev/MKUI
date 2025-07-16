#pragma once

#include "MKUIEnumTypes.generated.h"

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
    Ok,
    YesNo,
    OkCancel,
    Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
    Confirmed,
    Canceled,
    Closed,
    Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifiedReason : uint8
{
    DirectlyModified,
    DependencyModified,
    ResetToDefaults,
    Unknown UMETA(Hidden)
};
