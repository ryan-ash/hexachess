#pragma once

#include "CoreMinimal.h"

#include "AIType.generated.h"

UENUM(BlueprintType)
enum class EAIType : uint8
{
    Random,
    Copycat,
    MinMax
};
