#include "HexaGameState.h"


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

bool AHexaGameState::AreThereValidMovesForPlayer(bool IsWhitePlayer)
{
    return ActiveBoard->are_there_valid_moves(IsWhitePlayer ? Cell::PieceColor::white : Cell::PieceColor::black);
}

TArray<FIntPoint> AHexaGameState::GetValidMovesForPlayer(bool IsWhitePlayer)
{
    TArray<FIntPoint> Result;

    list<int32> MoveKeys = ActiveBoard->get_all_piece_move_keys(IsWhitePlayer ? Cell::PieceColor::white : Cell::PieceColor::black);
    for (const auto& Move : MoveKeys)
    {
        Position MovePosition = ActiveBoard->to_position(Move);
        Result.Add(FIntPoint{MovePosition.x, MovePosition.y});
    }

    return Result;
}

TArray<FIntPoint> AHexaGameState::MakeAIMove(bool IsWhiteAI, EAIType AIType)
{
    TArray<FIntPoint> Result;

    // this is a very naive implementation, but it should work for now
    switch(AIType)
    {
        case EAIType::Random:
            Result = CalculateRandomAIMove(IsWhiteAI);
            break;
        case EAIType::Copycat:
            Result = CalculateCopycatAIMove(IsWhiteAI);
            break;
        case EAIType::MinMax:
            Result = CalculateMinMaxAIMove(IsWhiteAI);
            break;
    }

    return Result;
}

TArray<FIntPoint> AHexaGameState::CalculateRandomAIMove(bool IsWhiteAI)
{
    TArray<FIntPoint> Result;

    list<int32> PieceKeys = ActiveBoard->get_piece_keys(IsWhiteAI ? Cell::PieceColor::white : Cell::PieceColor::black);
    list<int32> PieceKeysWithValidMoves;

    bool foundValidMove = false;
    while (!foundValidMove) {
        int32 RandomIndex = FMath::RandRange(0, PieceKeys.size() - 1);
        int32 RandomPieceKey = *std::next(PieceKeys.begin(), RandomIndex);
        auto PieceMoves = ActiveBoard->get_valid_moves(RandomPieceKey);
        if (PieceMoves.size() > 0)
        {
            foundValidMove = true;
            int32 RandomMoveIndex = FMath::RandRange(0, PieceMoves.size() - 1);
            int32 RandomMoveKey = *std::next(PieceMoves.begin(), RandomMoveIndex);
            Position FromPosition = ActiveBoard->to_position(RandomPieceKey);
            Position ToPosition = ActiveBoard->to_position(RandomMoveKey);

            // ActiveBoard->move_piece(FromPosition, ToPosition);

            Result.Add(FIntPoint{FromPosition.x, FromPosition.y});
            Result.Add(FIntPoint{ToPosition.x, ToPosition.y});
        }
    }

    return Result;
}

TArray<FIntPoint> AHexaGameState::CalculateCopycatAIMove(bool IsWhiteAI)
{
    TArray<FIntPoint> Result;
    return Result;
}

TArray<FIntPoint> AHexaGameState::CalculateMinMaxAIMove(bool IsWhiteAI)
{
    TArray<FIntPoint> Result;

    map<int32, Cell*> board = ActiveBoard->board_map;
    MoveResult ai_result = MiniMax(board, 3, IsWhiteAI, -9000.f, 9000.f);

    Position FromPosition = ActiveBoard->to_position(ai_result.FromKey);
    Position ToPosition = ActiveBoard->to_position(ai_result.ToKey);

    Result.Add(FIntPoint{FromPosition.x, FromPosition.y});
    Result.Add(FIntPoint{ToPosition.x, ToPosition.y});

    return Result;
}

MoveResult AHexaGameState::MiniMax(map<int32, Cell*>& in_board, int32 Depth, bool IsWhitePlayer, int32 Alpha, int32 Beta)
{
    if (Depth == 0)
    {
        return MoveResult(0, 0, ActiveBoard->evaluate(in_board));
    }

    MoveResult Result;
    if (IsWhitePlayer)
    {
        int32 MaxEval = -9000;
        list<int32> PieceKeys = ActiveBoard->get_piece_keys(in_board, Cell::PieceColor::white);
        for (int32 piece : PieceKeys) {
            list<int32> MoveKeys = ActiveBoard->get_valid_moves(in_board, piece);
            for (int32 move : MoveKeys) {
                auto board_copy = ActiveBoard->copy_board_map(in_board);
                Position start = ActiveBoard->to_position(piece);
                Position goal = ActiveBoard->to_position(move);
                ActiveBoard->move_piece(board_copy, start, goal);
                MoveResult child_result = MiniMax(board_copy, Depth - 1, false, Alpha, Beta);

                ActiveBoard->clear_board_map(board_copy);
                if (child_result.Score > MaxEval)
                {
                    Result.FromKey = piece;
                    Result.ToKey = move;
                }
                MaxEval = FMath::Max(MaxEval, child_result.Score);

                // pruning
                Alpha = FMath::Max(Alpha, child_result.Score);
                if (Beta <= Alpha)
                {
                    break;
                }
            }
        }
        Result.Score = MaxEval;
    }
    else
    {
        int32 MinEval = 9000;
        list<int32> PieceKeys = ActiveBoard->get_piece_keys(in_board, Cell::PieceColor::black);
        for (int32 piece : PieceKeys) {
            list<int32> MoveKeys = ActiveBoard->get_valid_moves(in_board, piece);
            for (int32 move : MoveKeys) {
                auto board_copy = ActiveBoard->copy_board_map(in_board);
                Position start = ActiveBoard->to_position(piece);
                Position goal = ActiveBoard->to_position(move);
                ActiveBoard->move_piece(board_copy, start, goal);
                MoveResult child_result = MiniMax(board_copy, Depth - 1, true, Alpha, Beta);

                ActiveBoard->clear_board_map(board_copy);
                if (child_result.Score < MinEval)
                {
                    Result.FromKey = piece;
                    Result.ToKey = move;
                }
                MinEval = FMath::Min(MinEval, child_result.Score);

                // pruning
                Beta = FMath::Min(Beta, child_result.Score);
                if (Beta <= Alpha)
                {
                    break;
                }
            }
        }
        Result.Score = MinEval;
    }

    return Result;
}
