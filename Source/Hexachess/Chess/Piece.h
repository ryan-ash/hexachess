#pragma once

#include <CoreMinimal.h>

class Point
{

}

class Piece
{
    Piece() {}
    Piece(Point* p): position(p) {}

    AActor* position = nullptr;

    TArray<Point*> GetPossibleMoves(Point* p) {}
    bool IsValidMove(Point* p) {}
};