#pragma once

#include <CoreMinimal.h>

#include "CellInfo.generated.h"


USTRUCT()
struct FCellInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 GridX = 0;

    UPROPERTY(EditAnywhere)
    int32 GridY = 0;

    UPROPERTY(EditAnywhere)
    int32 ColorID = 0;

    UPROPERTY(EditAnywhere)
    UMaterialInstanceDynamic* Material = nullptr;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh = nullptr;
};