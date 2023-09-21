// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "Hexachess/Types/PieceInfo.h"

#include "HexaGameState.generated.h"


class Board;

/**
 * 
 */
UCLASS()
class HEXACHESS_API AHexaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AHexaGameState();

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

	UFUNCTION(BlueprintCallable )
	virtual void MovePiece(FIntPoint From, FIntPoint To);

	UFUNCTION(BlueprintCallable)
	virtual bool IsCellUnderAttack(FIntPoint InPosition);

	UFUNCTION(BlueprintCallable)
	virtual bool AreThereValidMovesForPlayer(bool IsWhitePlayer);

	UFUNCTION(BlueprintCallable)
	virtual TArray<FIntPoint> GetValidMovesForPlayer(bool IsWhitePlayer);

private:
	
	Board* ActiveBoard = nullptr;
};

