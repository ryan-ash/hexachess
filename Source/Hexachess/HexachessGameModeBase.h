// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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

	// session boilerplate for later

	UFUNCTION(BlueprintCallable)
	virtual void StartGame();

	UFUNCTION(BlueprintCallable)
	virtual void EndGame() {}

	UFUNCTION(BlueprintCallable)
	virtual void RestartGame() {}

	UFUNCTION(BlueprintCallable)
	virtual void PauseGame() {}

	UFUNCTION(BlueprintCallable)
	virtual void ResumeGame() {}

	// game logic

	UFUNCTION(BlueprintCallable)
	virtual void CreatePieces() {}

	UFUNCTION(BlueprintCallable)
	virtual void CreateLogicalBoard();

private:

	Board* ActiveBoard = nullptr;
};


// things to decide:
// - Cell -> do we need it as Actor? or just a struct?
// - notation -> later
// - mapping visual board to logical:
// --- visual board is generated
// --- we store its details in a struct




// cell: something that can be occupied by a piece
// - position
// - color id?..
// - piece pointer?
// - actor (component) pointer
// piece: a base class for all pieces
// - team id
// - alive / dead
// - piece type?..
// - actor (component) pointer
// board: a collection of cells and pieces




// HexachessGameMode -> BP_HexachessGameMode
// - assigns teams
// - creates pieces
// - creates logical board
// - starts game

// AHexGrid -> BP_HexGrid
// - runs grid generation
// - initializes cells

// Controller
// - presses on Piece -> Piece::GetPossibleMoves
// - tries to move Piece -> Piece::IsValidMove
// - moves Piece -> Piece::Move
// -> returns move result meta struct: capture, check, checkmate, promotion (type), stalemate

