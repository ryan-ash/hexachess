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

UENUM(BlueprintType)
enum class EAIDifficulty : uint8
{
    Easy,
    Medium,
    Hard
};