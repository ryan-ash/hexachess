#pragma once

#include <CoreMinimal.h>

#include "Piece.h"

class Cell
{

public:
    Cell() {}
    Cell(Piece* p): piece(p) {}

    bool HasPiece() {
        return piece != nullptr;
    }

private:

    Piece* piece = nullptr;
};
