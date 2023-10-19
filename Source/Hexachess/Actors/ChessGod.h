#pragma once

#include "CoreMinimal.h"

#include "Chess/MinimaxAI.h"
#include "Types/PieceInfo.h"
#include "Types/AIType.h"

#include "ChessGod.generated.h"

class Board;


UCLASS(Blueprintable, BlueprintType)
class HEXACHESS_API AChessGod : public AActor
{
    GENERATED_BODY()

    AChessGod(const FObjectInitializer& ObjectInitializer);

public:

	UPROPERTY(BlueprintReadWrite)
	UMinimaxAIComponent* MinimaxAIComponent;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // main flow

	UFUNCTION(BlueprintCallable)
	virtual void StartGame();

	UFUNCTION(BlueprintCallable)
	virtual void EndGame();

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
	virtual TArray<FIntPoint> MakeAIMove(bool IsWhiteAI, EAIType AIType, EAIDifficulty AIDifficulty);


	// TODO: fix this flow!
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIFinishedCalculatingMove, FIntPoint, From, FIntPoint, To);

	UPROPERTY(BlueprintAssignable)
	FOnAIFinishedCalculatingMove OnAIFinishedCalculatingMove;

private:

	TArray<FIntPoint> CalculateRandomAIMove(bool IsWhiteAI);
	TArray<FIntPoint> CalculateCopycatAIMove(bool IsWhiteAI);
	TArray<FIntPoint> CalculateMinMaxAIMove(bool IsWhiteAI, EAIDifficulty AIDifficulty);

	Board* ActiveBoard = nullptr;
};