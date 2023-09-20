#include "HexaGameState.h"
#include "Hexachess/Chess/ChessEngine.h"


AHexaGameState::AHexaGameState() : Super()
{

}

void AHexaGameState::BeginPlay()
{
    Super::BeginPlay();

    StartGame();
}

void AHexaGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    EndGame();
}

void AHexaGameState::StartGame()
{
    CreateLogicalBoard();
}

void AHexaGameState::EndGame()
{
    if (ActiveBoard != nullptr)
    {
        delete ActiveBoard;
        ActiveBoard = nullptr;
    }
}

void AHexaGameState::CreateLogicalBoard()
{
    ActiveBoard = new Board();
}

void AHexaGameState::RegisterPiece(FPieceInfo PieceInfo)
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

TArray<FIntPoint> AHexaGameState::GetMovesForCell(FIntPoint InPosition)
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

void AHexaGameState::MovePiece(FIntPoint From, FIntPoint To)
{
    Position FromPosition = Position{From.X, From.Y};
    Position ToPosition = Position{To.X, To.Y};

    ActiveBoard->move_piece(FromPosition, ToPosition);
}

bool AHexaGameState::IsCellUnderAttack(FIntPoint InPosition)
{
    Position PiecePosition = Position{InPosition.X, InPosition.Y};
    return ActiveBoard->can_be_captured(PiecePosition);
}