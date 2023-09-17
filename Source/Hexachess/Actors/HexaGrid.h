#pragma once

#include <CoreMinimal.h>

#include "HexaGrid.generated.h"

UCLASS()
class AHexaGrid : public AActor
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 Width = 22;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 Height = 22;

    UFUNCTION(BlueprintCallable)
    virtual void GenerateGrid() {}

};
