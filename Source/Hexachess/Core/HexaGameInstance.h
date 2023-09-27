#pragma once

#include <Engine/GameInstance.h>

#include "Types/AIType.h"

#include "HexaGameInstance.generated.h"


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
