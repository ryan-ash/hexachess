#include "HexachessGameModeBase.h"


AHexachessGameModeBase::AHexachessGameModeBase() : Super()
{

}

void AHexachessGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    StartGame();
}

void AHexachessGameModeBase::StartGame()
{
    CreatePieces();
    CreateLogicalBoard();
}

void AHexachessGameModeBase::CreateLogicalBoard()
{
    // logical board creation code can be placed here...
}
