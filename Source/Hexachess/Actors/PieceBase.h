#pragma once

#include <CoreMinimal.h>

// it's weird this is required...
#include "../Types/PieceType.h"

#include "PieceBase.generated.h"


UCLASS()
class APieceBase : public AActor
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    EPieceType Type = EPieceType::Pawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 ColorID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 GridX = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    int32 GridY = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    UStaticMeshComponent* Mesh = nullptr;

    UFUNCTION(BlueprintCallable)
    virtual void Move(int32 NewGridX, int32 NewGridY) {}

    UFUNCTION(BlueprintCallable)
    virtual void Kill() {}

    UFUNCTION(BlueprintCallable)
    virtual void SetColor(int32 NewColorID) {}

};
