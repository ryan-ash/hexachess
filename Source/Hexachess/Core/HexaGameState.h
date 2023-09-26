#pragma once

#include <map>

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "Hexachess/Chess/ChessEngine.h"
#include "Hexachess/Types/PieceInfo.h"
#include "Hexachess/Types/AIType.h"

#include "HexaGameState.generated.h"


class Board;

using namespace std;


struct MoveResult
{
	MoveResult() = default;
	MoveResult(int32 InFromKey, int32 InToKey, int32 InScore)
		: FromKey(InFromKey)
		, ToKey(InToKey)
		, Score(InScore)
	{}

	int32 FromKey = -1;
	int32 ToKey = -1;
	int32 Score = -1;
};

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

    // minmax algorithm
    // - we need to get all the possible moves for the AI
    // - for each move, we need to get all the possible moves for the player
    // - repeat the recursion until the depth limit is reached
    // - evaluate the board state for all bottom nodes (it's recursion exit point)
    // - keep going up taking other min or max values among the siblings' values
    // - last step should give you the best move; return it
	MoveResult MiniMax(map<int32, Cell*>& in_board, int32 Depth, bool IsWhitePlayer, int32 Alpha, int32 Beta);

	Board* ActiveBoard = nullptr;
};
