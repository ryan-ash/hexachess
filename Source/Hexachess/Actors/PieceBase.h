#pragma once

#include <CoreMinimal.h>
#include <GeometryCollection/GeometryCollectionActor.h>

// it's weird this is required...
#include "../Types/PieceType.h"

#include "PieceBase.generated.h"


UCLASS()
class APieceBase : public AGeometryCollectionActor
{
    GENERATED_BODY()

    APieceBase();

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    EPieceType Type = EPieceType::Pawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 ColorID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 GridX = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 GridY = 0;

    UFUNCTION(BlueprintCallable)
    virtual void Move(int32 NewGridX, int32 NewGridY) {}

    UFUNCTION(BlueprintCallable)
    virtual void Kill() {}

    UFUNCTION(BlueprintCallable)
    virtual void SetColor(int32 NewColorID) {}

};
