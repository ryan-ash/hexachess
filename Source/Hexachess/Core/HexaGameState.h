#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "HexaGameState.generated.h"


/**
 * 
 */
UCLASS()
class HEXACHESS_API AHexaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	// session boilerplate for later

	UFUNCTION(BlueprintCallable)
	virtual void RestartGame() {}

	UFUNCTION(BlueprintCallable)
	virtual void PauseGame() {}

	UFUNCTION(BlueprintCallable)
	virtual void ResumeGame() {}
};
