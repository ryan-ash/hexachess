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

void AHexachessGameModeBase::RegisterPiece(FPieceInfo PieceInfo)
{
    const auto PieceType = [&]()
    {
        switch (PieceInfo.Type)
        {
        case EPieceType::Pawn:
            return Cell::PieceType::pawn;
        case EPieceType::Knight:
            return Cell::PieceType::knight;
        case EPieceType::Bishop:
            return Cell::PieceType::bishop;
        case EPieceType::Rook:
            return Cell::PieceType::rook;
        case EPieceType::Queen:
            return Cell::PieceType::queen;
        case EPieceType::King:
            return Cell::PieceType::king;
        default:
            return Cell::PieceType::pawn;
        }
    }();

    // crashes here:
    Position PiecePosition = Position{PieceInfo.X, PieceInfo.Y};
    ActiveBoard->set_piece(PiecePosition, PieceType, PieceInfo.TeamID == 0 ? Cell::PieceColor::white : Cell::PieceColor::black);
}

TArray<FIntPoint> AHexachessGameModeBase::GetMovesForCell(FIntPoint InPosition)
{
    TArray<FIntPoint> Result;

    Position PiecePosition = Position{InPosition.X, InPosition.Y};
    list<Position> Moves = ActiveBoard->get_valid_moves(PiecePosition);

    for (const auto& Move : Moves)
    {
        Result.Add(FIntPoint{Move.x, Move.y});
    }

    return Result;
}

void AHexachessGameModeBase::MovePiece(FIntPoint From, FIntPoint To)
{
    Position FromPosition = Position{From.X, From.Y};
    Position ToPosition = Position{To.X, To.Y};

    ActiveBoard->move_piece(FromPosition, ToPosition);
}
