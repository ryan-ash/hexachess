#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "Chess/MinimaxAI.h"
#include "Types/PieceInfo.h"
#include "Types/AIType.h"

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

	UPROPERTY(BlueprintReadWrite)
	UMinimaxAIComponent* MinimaxAIComponent;

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

	// ai logic

	/*
	 * Returns the moves the AI wants to make. Doesn't actually updates the board.
	 */
	UFUNCTION(BlueprintCallable)
	virtual TArray<FIntPoint> MakeAIMove(bool IsWhiteAI, EAIType AIType);


	// TODO: fix this flow!
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIFinishedCalculatingMove, FIntPoint, From, FIntPoint, To);

	UPROPERTY(BlueprintAssignable)
	FOnAIFinishedCalculatingMove OnAIFinishedCalculatingMove;

private:

	TArray<FIntPoint> CalculateRandomAIMove(bool IsWhiteAI);
	TArray<FIntPoint> CalculateCopycatAIMove(bool IsWhiteAI);
	TArray<FIntPoint> CalculateMinMaxAIMove(bool IsWhiteAI);

	Board* ActiveBoard = nullptr;
};
