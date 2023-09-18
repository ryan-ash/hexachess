#pragma once

#include <CoreMinimal.h>

#include "PieceType.h"

#include "PieceInfo.generated.h"


USTRUCT(BlueprintType)
struct FPieceInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 X = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Y = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TeamID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPieceType Type = EPieceType::Pawn;
};
