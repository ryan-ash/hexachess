// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Types/PieceInfo.h"

#include "HexachessGameModeBase.generated.h"


class Board;

/**
 * 
 */
UCLASS()
class HEXACHESS_API AHexachessGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AHexachessGameModeBase();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// session boilerplate for later

	UFUNCTION(BlueprintCallable)
	virtual void StartGame();

	UFUNCTION(BlueprintCallable)
	virtual void EndGame();

	UFUNCTION(BlueprintCallable)
	virtual void RestartGame() {}

	UFUNCTION(BlueprintCallable)
	virtual void PauseGame() {}

	UFUNCTION(BlueprintCallable)
	virtual void ResumeGame() {}

	// game logic

	UFUNCTION(BlueprintCallable)
	virtual void CreateLogicalBoard();

	UFUNCTION(BlueprintCallable)
	virtual void RegisterPiece(FPieceInfo PieceInfo);

	UFUNCTION(BlueprintCallable)
	virtual TArray<FIntPoint> GetMovesForCell(FIntPoint InPosition);

	UFUNCTION(BlueprintCallable)
	virtual void MovePiece(FIntPoint From, FIntPoint To);

	UFUNCTION(BlueprintCallable)
	virtual bool IsCellUnderAttack(FIntPoint InPosition);

private:

	Board* ActiveBoard = nullptr;
};

