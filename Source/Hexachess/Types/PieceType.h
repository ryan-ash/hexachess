#pragma once

#include <CoreMinimal.h>

#include "PieceType.generated.h"

UENUM(BlueprintType)
enum class EPieceType : uint8
{
    King,
    Queen,
    Bishop,
    Knight,
    Rook,
    Pawn
};
