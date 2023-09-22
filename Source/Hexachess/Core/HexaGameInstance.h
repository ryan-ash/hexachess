#pragma once

#include <Engine/GameInstance.h>

#include "HexaGameInstance.generated.h"

UENUM(BlueprintType)
enum class EAIType : uint8
{
    Random,
    Copycat,
    MinMax
};

UCLASS()
class HEXACHESS_API UHexaGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    UHexaGameInstance(const FObjectInitializer& ObjectInitializer);

    virtual void Init() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool IsPlayingAgainstAI = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    EAIType AIType = EAIType::Random;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool IsAIPlayingWhite = false;
};
