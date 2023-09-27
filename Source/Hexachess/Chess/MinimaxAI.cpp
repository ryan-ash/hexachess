#include "MinimaxAI.h"

#include "Core/HexaGameState.h"
#include "Chess/ChessEngine.h"


void UMinimaxAIComponent::StartCalculatingMove(Board* ActiveBoard, bool IsWhiteAI)
{
    const auto CompleteCallback = [this](TArray<FIntPoint>& Result)
    {
        const auto* GameState = GetWorld()->GetGameState<AHexaGameState>();
        GameState->OnAIFinishedCalculatingMove.Broadcast(Result[0], Result[1]);
    };

    AsyncTask(ENamedThreads::GameThread, [this, ActiveBoard, IsWhiteAI, CompleteCallback]
    {
        TArray<FIntPoint> Result;

        map<int32, Cell*> board = ActiveBoard->board_map;
        MoveResult ai_result = MiniMax(ActiveBoard, board, 3, IsWhiteAI, -9000.f, 9000.f);

        Position FromPosition = ActiveBoard->to_position(ai_result.FromKey);
        Position ToPosition = ActiveBoard->to_position(ai_result.ToKey);

        Result.Add(FIntPoint{FromPosition.x, FromPosition.y});
        Result.Add(FIntPoint{ToPosition.x, ToPosition.y});

        CompleteCallback(Result);
    });
}

MoveResult UMinimaxAIComponent::MiniMax(Board* ActiveBoard, map<int32, Cell*>& in_board_map, int32 Depth, bool IsWhitePlayer, int32 Alpha, int32 Beta)
{
    if (Depth == 0)
    {
        return MoveResult(0, 0, ActiveBoard->evaluate(in_board_map));
    }

    MoveResult Result;
    if (IsWhitePlayer)
    {
        int32 MaxEval = -9000;
        list<int32> PieceKeys = ActiveBoard->get_piece_keys(in_board_map, Cell::PieceColor::white);
        for (int32 piece : PieceKeys) {
            list<int32> MoveKeys = ActiveBoard->get_valid_moves(in_board_map, piece);
            for (int32 move : MoveKeys) {
                auto board_copy = ActiveBoard->copy_board_map(in_board_map);
                Position start = ActiveBoard->to_position(piece);
                Position goal = ActiveBoard->to_position(move);
                ActiveBoard->move_piece(board_copy, start, goal);
                MoveResult child_result = MiniMax(ActiveBoard, board_copy, Depth - 1, false, Alpha, Beta);

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
        list<int32> PieceKeys = ActiveBoard->get_piece_keys(in_board_map, Cell::PieceColor::black);
        for (int32 piece : PieceKeys) {
            list<int32> MoveKeys = ActiveBoard->get_valid_moves(in_board_map, piece);
            for (int32 move : MoveKeys) {
                auto board_copy = ActiveBoard->copy_board_map(in_board_map);
                Position start = ActiveBoard->to_position(piece);
                Position goal = ActiveBoard->to_position(move);
                ActiveBoard->move_piece(board_copy, start, goal);
                MoveResult child_result = MiniMax(ActiveBoard, board_copy, Depth - 1, true, Alpha, Beta);

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
