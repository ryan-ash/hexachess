/*
#include "ChessEngine.h"

void print_moves(Board* b, int32 x, int32 y, Cell::PieceType pt, Cell::PieceColor pc, string piece_name) {
    Position p = Position{x, y};
    b->set_piece(p, pt, pc);
    list<Position> lp = b->get_valid_moves(p);
    cout << piece_name << " x: " << p.x << ", y: " << p.y << endl;
    cout << "Valid moves: " << endl;
    for_each(lp.begin(), lp.end(), [](Position& p) {
        cout << "x: " << p.x << ", y: " << p.y << endl;
    });
}

int32 main() {
    cout << "Start test" << endl;

    Board* b = new Board();

    print_moves(b, 0, 0, Cell::PieceType::knight, Cell::PieceColor::white, "White Knight");
    print_moves(b, 2, 2, Cell::PieceType::pawn, Cell::PieceColor::black, "Black Pawn");
    print_moves(b, 3, 2, Cell::PieceType::pawn, Cell::PieceColor::white, "White Pawn");
    print_moves(b, 5, 6, Cell::PieceType::pawn, Cell::PieceColor::black, "Black Pawn");
    print_moves(b, 4, 4, Cell::PieceType::bishop, Cell::PieceColor::white, "White Bishop");

    cout << "End test" << endl;
    return 0;
}
*/