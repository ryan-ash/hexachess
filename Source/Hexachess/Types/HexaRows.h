#pragma once

#include <CoreMinimal.h>

#include "HexaRows.generated.h"


USTRUCT(BlueprintType)
struct FTileRow
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TArray<UStaticMeshComponent*> Tiles;
};

USTRUCT(BlueprintType)
struct FBoard
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TArray<FTileRow> Rows;
};
