#include "HexachessGameModeBase.h"

#include "Chess/ChessEngine.h"


AHexachessGameModeBase::AHexachessGameModeBase() : Super()
{

}

void AHexachessGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    StartGame();
}

void AHexachessGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    EndGame();
}

void AHexachessGameModeBase::StartGame()
{
    CreatePieces();
    CreateLogicalBoard();
}

void AHexachessGameModeBase::EndGame()
{
    if (ActiveBoard != nullptr)
    {
        delete ActiveBoard;
        ActiveBoard = nullptr;
    }
}

void AHexachessGameModeBase::CreateLogicalBoard()
{
    ActiveBoard = new Board();

    //     print_moves(b, 0, 0, Cell::PieceType::knight, Cell::PieceColor::white, "Knight");
    //     print_moves(b, 1, 0, Cell::PieceType::pawn, Cell::PieceColor::white, "Pawn");

    // void print_moves(Board* b, int x, int y, Cell::PieceType pt, Cell::PieceColor pc, string piece_name) {

        // b->set_piece(p, pt, pc);
        // list<Position> lp = b->get_valid_moves(p);

}

// create methods for:
// - creating pieces
// - checking possible moves
// - moving pieces