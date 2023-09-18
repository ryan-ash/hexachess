#include <map>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

struct Position {

    Position() {}
    Position(int x, int y): x(x), y(y) {}

    int x, y;
};

class Cell {
    public:
    enum PieceType {
        none, pawn, knight, bishop, rook, queen, king
    };

    enum PieceColor {
        absent, white, black
    };

    Cell() {}
    Cell(PieceType pt, PieceColor pc): piece(pt), piece_color(pc) {}

    void set_piece(PieceType pt, PieceColor pc) {
        piece = pt;
        piece_color = pc;
    }

    void remove_piece() {
        piece = PieceType::none;
        piece_color = PieceColor::absent;
    }

    bool has_piece() {
        return piece != PieceType::none;
    }

    bool has_white_piece() {
        return piece != PieceType::none && piece_color == PieceColor::white;
    }

    bool has_black_piece() {
        return piece != PieceType::none && piece_color == PieceColor::black;
    }

    PieceType get_piece_type() {
        return piece;
    }

    PieceColor get_piece_color() {
        return piece_color;
    }
    
    private:
    PieceType piece = PieceType::none;
    PieceColor piece_color = PieceColor::absent;
};

class Board {
    public:

    Board() {
        for (int x = 0; x <= max; x++) {
            int y_max = median + x;
            if (y_max > max) {
                y_max = max - y_max % max;
            }
            for (int y = 0; y <= y_max; y++) {
                int pos = to_position_key(x, y);
                Cell* cell = new Cell();
                board_map[pos] = cell;
            }
        }
    }

    bool is_valid_position(int x, int y) {
        int pos = to_position_key(x, y);
        return is_valid_position(pos);
    }

    list<Position> get_valid_moves(Position& pos) {
        int key = to_position_key(pos);
        Cell* cell = board_map[key];
        list<int> l = {};
        switch (cell->get_piece_type()) {
            case Cell::PieceType::none:
                break;
            case Cell::PieceType::pawn:
                add_pawn_moves(l, key, cell);
                break;
            case Cell::PieceType::bishop:
                add_bishop_moves(l, key, cell);
                break;
            case Cell::PieceType::knight:
                add_knight_moves(l, key, cell);
                break;
            case Cell::PieceType::rook:
                add_rook_moves(l, key, cell);
                break;
            case Cell::PieceType::queen:
                add_queen_moves(l, key, cell);
                break;
            case Cell::PieceType::king:
                add_king_moves(l, key, cell);
                break;
        };
        list<Position> ret_l = {};
        for_each(l.begin(), l.end(), [&ret_l = ret_l, this](int k) {
            Position p = this->to_position(k);
            ret_l.push_front(p);
        });
        return ret_l;
    }

    bool move_piece(Position& start, Position& goal) {
        int sp = to_position_key(start);
        if (is_valid_position(sp) && is_valid_position(goal)) {

            Cell::PieceType pt = board_map[sp]->get_piece_type();
            Cell::PieceColor pc = board_map[sp]->get_piece_color();
            board_map[sp]->remove_piece();
            set_piece(goal, pt, pc);
        }
        return true;
    }

    void set_piece(Position& pos, Cell::PieceType pt, Cell::PieceColor pc) {
        int key = to_position_key(pos);
        board_map[key]->set_piece(pt, pc);
    }

    private:

    using TMoveFn = int (*)(const int);

    static const int median = 5;
    static const int max = 10;
    static const int step_x = 1 << 8;
    map<int, Cell*> board_map;
    const int white_pawn_cell_keys[9] = {256, 513, 770, 1027, 1284, 1541, 1796, 2051, 2306};
    const int black_pawn_cell_keys[9] = {262, 518, 774, 1030, 1286, 1542, 1798, 2054, 2310};

    inline int to_position_key(int x, int y) {
        return (x << 8) + y;
    }

    inline int to_position_key(Position pos) {
        return to_position_key(pos.x, pos.y);
    }

    Position to_position(int key) {
        Position pos = Position{get_x(key), get_y(key)};
        return pos;
    }

    inline bool is_valid_position(int key) {
        return board_map.find(key) != board_map.end();
    }

    inline bool is_valid_position(Position& pos) {
        return is_valid_position(to_position_key(pos));
    }

    void add_pawn_moves(list<int>& l, int key, Cell* cell) {
        TMoveFn fn;
        switch (cell->get_piece_color()) {
            case Cell::PieceColor::white:
                fn = &Board::move_vertically_up;
                break;
            case Cell::PieceColor::black:
                fn = &Board::move_vertically_down;
                break;
            default:
                fn = &Board::ping;
                break;
        }
        int move = fn(key);
        add_if_valid(l, move);
        if (is_initial_pawn_cell(key, cell)) {
            add_if_valid(l, fn(move));
        }
    }

    bool is_initial_pawn_cell(const int key, Cell* cell) {
        const int* cell_keys;
        switch (cell->get_piece_color()) {
            case Cell::PieceColor::white:
                cell_keys = white_pawn_cell_keys;
                break;
            case Cell::PieceColor::black:
                cell_keys = black_pawn_cell_keys;
                break;
        }
        auto arr_end = end(white_pawn_cell_keys);
        const int *k = find(begin(white_pawn_cell_keys), arr_end, key);
        return k != arr_end;
    }

    void add_bishop_moves(list<int>& l, int key, Cell* cell) {
        TMoveFn fns[4] = { &move_diagonally_top_right
                         , &move_diagonally_top_left
                         , &move_diagonally_bottom_right
                         , &move_diagonally_bottom_left
                         };
        add_valid_moves(l, key, fns, 4);
    }

    void add_knight_moves(list<int>& l, int key, Cell* cell) {
        int pos;
        pos = move_vertically_up(move_vertically_up(key));
        add_if_valid(l, move_horizontally_top_right(pos));
        add_if_valid(l, move_horizontally_top_left(pos));

        pos = move_vertically_down(move_vertically_down(key));
        add_if_valid(l, move_horizontally_bottom_right(pos));
        add_if_valid(l, move_horizontally_bottom_left(pos));

        pos = move_horizontally_top_right(move_horizontally_top_right(key));
        add_if_valid(l, move_vertically_up(pos));
        add_if_valid(l, move_horizontally_bottom_right(pos));

        pos = move_horizontally_bottom_right(move_horizontally_bottom_right(key));
        add_if_valid(l, move_vertically_down(pos));
        add_if_valid(l, move_horizontally_top_right(pos));

        pos = move_horizontally_bottom_left(move_horizontally_bottom_left(key));
        add_if_valid(l, move_vertically_down(pos));
        add_if_valid(l, move_horizontally_top_left(pos));

        pos = move_horizontally_top_left(move_horizontally_top_left(key));
        add_if_valid(l, move_vertically_up(pos));
        add_if_valid(l, move_horizontally_bottom_left(pos));
    }

    void add_rook_moves(list<int>& l, int key, Cell* cell) {
        TMoveFn fns[6] = { &move_horizontally_top_right
                         , &move_horizontally_top_left
                         , &move_horizontally_bottom_right
                         , &move_horizontally_bottom_left
                         , &move_vertically_up
                         , &move_vertically_down
                         };
        add_valid_moves(l, key, fns, 6);
    }

    void add_queen_moves(list<int>& l, int key, Cell* cell) {
        add_bishop_moves(l, key, cell);
        add_rook_moves(l, key, cell);
    }

    void add_king_moves(list<int>& l, int key, Cell* cell) {
        add_if_valid(l, move_vertically_up(key));
        add_if_valid(l, move_vertically_down(key));
        add_if_valid(l, move_horizontally_top_right(key));
        add_if_valid(l, move_horizontally_top_left(key));
        add_if_valid(l, move_horizontally_bottom_right(key));
        add_if_valid(l, move_horizontally_bottom_left(key));
        add_if_valid(l, move_diagonally_top_right(key));
        add_if_valid(l, move_diagonally_top_left(key));
        add_if_valid(l, move_diagonally_bottom_right(key));
        add_if_valid(l, move_diagonally_bottom_left(key));
    }

    void add_valid_moves(list<int>& l, const int key, TMoveFn fns[], int fns_count) {
        int current_pos;
        for (int i = 0; i < fns_count; i++) {
            TMoveFn fn = fns[i];
            current_pos = fn(key);
            while (is_valid_position(current_pos)) {
                l.push_front(current_pos);
                current_pos = fn(current_pos);
            }
        }
    }


    inline void add_if_valid(list<int>& l, int key) {
        if (is_valid_position(key)) {
            l.push_front(key);
        }
    }

    static inline int move_vertically_up(const int key) {
        return key + 1; // x, y+1
    }

    static inline int move_vertically_down(const int key) {
        return key - 1; // x, y-1
    }

    static int move_horizontally_top_right(const int key) {
        if (get_x(key) < median) {
            return key + step_x + 1; // x+1, y+1
        } else {
            return key + step_x; // x+1, y
        }
    }

    static int move_horizontally_top_left(const int key) {
        if (get_x(key) > median) {
            return key - step_x + 1; // x-1, y+1
        } else {
            return key - step_x; // x-1, y
        }
    }

    static int move_horizontally_bottom_right(const int key) {
        if (get_x(key) < median) {
            return key + step_x; // x+1, y
        } else {
            return key + step_x - 1; // x+1, y-1
        }
    }

    static int move_horizontally_bottom_left(const int key) {
        if (get_x(key) > median) {
            return key - step_x; // x-1, y
        } else {
            return key - step_x - 1; // x-1, y-1
        }
    }

    static int move_diagonally_top_right(const int key) {
        if (get_x(key) < median) {
            return key + step_x + 2; // x+1, y+2
        } else {
            return key + step_x + 1; // x+1, y+1
        }
    }

    static int move_diagonally_top_left(const int key) {
        if (get_x(key) > median) {
            return key - step_x + 2; // x-1, y+2
        } else {
            return key - step_x + 1; // x-1, y+1
        }
    }

    static int move_diagonally_bottom_right(const int key) {
        if (get_x(key) < median) {
            return key + step_x - 1; // x+1, y-1
        } else {
            return key + step_x - 2; // x+1, y-2
        }
    }

    static int move_diagonally_bottom_left(const int key) {
        if (get_x(key) > median) {
            return key - step_x - 1; // x-1, y-1
        } else {
            return key - step_x - 2; // x-1, y-2
        }
    }

    static int ping(const int key) {
        return key;
    }

    static inline int get_x(const int key) {
        return key >> 8;
    }

    static inline int get_y(const int key) {
        return key & 0xFF;
    }
};

void print_moves(Board* b, int x, int y, Cell::PieceType pt, Cell::PieceColor pc, string piece_name) {
    Position p = Position{x, y};
    b->set_piece(p, pt, pc);
    list<Position> lp = b->get_valid_moves(p);
    cout << piece_name << " x: " << p.x << ", y: " << p.y << endl;
    cout << "Valid moves: " << endl;
    for_each(lp.begin(), lp.end(), [](Position& p) {
        cout << "x: " << p.x << ", y: " << p.y << endl;
    });
}

// int main() {
//     cout << "Start test" << endl;

//     Board* b = new Board();

//     print_moves(b, 0, 0, Cell::PieceType::knight, Cell::PieceColor::white, "Knight");
//     print_moves(b, 1, 0, Cell::PieceType::pawn, Cell::PieceColor::white, "Pawn");

//     cout << "End test" << endl;
//     return 0;
// }