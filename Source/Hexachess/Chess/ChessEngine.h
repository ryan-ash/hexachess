#include <map>
#include <list>
#include <algorithm>
#include <vector>

#if WITH_EDITOR
#include <CoreMinimal.h>
DEFINE_LOG_CATEGORY_STATIC(LogChessEngine, Log, All);
#endif

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
    Cell(Cell& other): piece(other.piece), piece_color(other.piece_color) {}

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

    bool has_piece_of_same_color(Cell* cell) {
        PieceColor other_color = cell->get_piece_color();
        return piece != PieceType::none
               && piece_color != PieceColor::absent
               && other_color != PieceColor::absent
               && piece_color == other_color;
    }

    bool has_piece_of_opposite_color(Cell* cell) {
        PieceColor other_color = cell->get_piece_color();
        return piece != PieceType::none
               && piece_color != PieceColor::absent
               && other_color != PieceColor::absent
               && piece_color != other_color;
    }

    PieceType get_piece_type() {
        return piece;
    }

    PieceColor get_piece_color() {
        return piece_color;
    }
    
    PieceColor get_opposite_color() {
        switch (piece_color) {
            case PieceColor::white:
                return PieceColor::black;
            case PieceColor::black:
                return PieceColor::white;
        }
        return PieceColor::absent;
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
        auto key = to_position_key(pos);
        auto moves = get_valid_moves(key);
        list<Position> pos_list = {};
        for (const int k : moves) {
            Position p = this->to_position(k);
            pos_list.push_front(p);
        }
        return pos_list;
    }

    bool move_piece(Position& start, Position& goal) {
        return move_piece(board_map, start, goal);
    }

    bool move_piece(map<int, Cell*>& in_board, Position& start, Position& goal) {
        bool is_main_board = &in_board == &board_map;

        #if WITH_EDITOR
        UE_LOG(LogChessEngine, Log, TEXT("%s: Move piece from (%d, %d) to (%d, %d)"), is_main_board ? TEXT("MAIN") : TEXT("INNER"), start.x, start.y, goal.x, goal.y);
        #endif

        int sp = to_position_key(start);
        if (is_valid_position(in_board, sp) && is_valid_position(in_board, goal)) {
            Cell::PieceType pt = in_board[sp]->get_piece_type();
            Cell::PieceColor pc = in_board[sp]->get_piece_color();
            in_board[sp]->remove_piece();
            set_piece(in_board, goal, pt, pc);
        }
        return true;
    }

    bool set_piece(Position& pos, Cell::PieceType pt, Cell::PieceColor pc) {
        return set_piece(board_map, pos, pt, pc);
    }

    bool set_piece(map<int, Cell*>& in_board, Position& pos, Cell::PieceType pt, Cell::PieceColor pc) {
        int key = to_position_key(pos);
        if (!is_valid_position(in_board, key)) {
            return false;
        }
        in_board[key]->set_piece(pt, pc);
        return false;
    }

    bool can_be_captured(Position& pos) {
        return can_be_captured(board_map, pos);
    }

    bool can_be_captured(map<int, Cell*>& in_board, Position& pos) {
        int key = to_position_key(pos);
        return can_be_captured(in_board, key);
    }

    private:

    using TMoveFn = int (*)(const int);

    static const int median = 5;
    static const int max = 10;
    static const int step_x = 1 << 8;
    map<int, Cell*> board_map;
    const vector<int> white_pawn_cell_keys = {256, 513, 770, 1027, 1284, 1539, 1794, 2049, 2304};
    const vector<int> black_pawn_cell_keys = {262, 518, 774, 1030, 1286, 1542, 1798, 2054, 2310};

    list<int> get_valid_moves(int key) {
        return get_valid_moves(board_map, key);
    }

    list<int> get_valid_moves(map<int, Cell*>& in_board, int key, bool skip_filter = false) {
        Cell* cell = in_board[key];
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
        }

        list<int> filtered_list = {};
        auto color_pieces = get_piece_keys(cell->get_piece_color());
        int king_key = -1;
        for (auto piece_key : color_pieces) {
            if (in_board[piece_key]->get_piece_type() == Cell::PieceType::king) {
                king_key = piece_key;
                break;
            }
        }
        if (skip_filter || king_key == -1) {
            return l;
        }
        for (int k : l) {
            auto board_copy = copy_board_map();
            Position start = to_position(key);
            Position goal = to_position(k);

            // to debug:
            // - draw boards in log
            // - figure out which method is broken
            // - fix it

            move_piece(board_copy, start, goal);
            auto final_king_key = king_key;
            if (cell->get_piece_type() == Cell::PieceType::king) {
                final_king_key = k;
            }
            if (!can_be_captured(board_copy, final_king_key)) {
                filtered_list.push_front(k);
            }
        }
        return filtered_list;
    }

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
        return is_valid_position(board_map, key);
    }

    inline bool is_valid_position(map<int, Cell*>& in_board, int key) {
        return in_board.find(key) != in_board.end();
    }

    inline bool is_valid_position(Position& pos) {
        return is_valid_position(board_map, pos);
    }

    inline bool is_valid_position(map<int, Cell*>& in_board, Position& pos) {
        return is_valid_position(in_board, to_position_key(pos));
    }

    void add_pawn_moves(list<int>& l, int key, Cell* cell) {
        TMoveFn fn_move, fn_take_1, fn_take_2;
        switch (cell->get_piece_color()) {
            case Cell::PieceColor::white:
                fn_move = &Board::move_vertically_up;
                fn_take_1 = &Board::move_horizontally_top_left;
                fn_take_2 = &Board::move_horizontally_top_right;
                break;
            case Cell::PieceColor::black:
                fn_move = &Board::move_vertically_down;
                fn_take_1 = &Board::move_horizontally_bottom_left;
                fn_take_2 = &Board::move_horizontally_bottom_right;
                break;
            default:
                return;
        }
        int move = fn_move(key);
        if (is_valid_position(move)) {
            add_if_valid(l, move, cell, false);
            if (!board_map[move]->has_piece() && is_initial_pawn_cell(key, cell)) {
                add_if_valid(l, fn_move(move), cell, false);
            }
        }
        int take = fn_take_1(key);
        add_pawn_take_if_valid(l, take, cell);
        take = fn_take_2(key);
        add_pawn_take_if_valid(l, take, cell);
    }

    void add_pawn_take_if_valid(list<int>& l, int key, Cell* cell) {
        if (is_valid_position(key) && board_map[key]->has_piece_of_opposite_color(cell)) {
            l.push_front(key);
        }
    }

    bool is_initial_pawn_cell(const int key, Cell* cell) {
        const vector<int>* cell_keys;
        switch (cell->get_piece_color()) {
            case Cell::PieceColor::white:
                cell_keys = &white_pawn_cell_keys;
                break;
            case Cell::PieceColor::black:
                cell_keys = &black_pawn_cell_keys;
                break;
            default:
                return false;
        }
        auto arr_end = end(*cell_keys);
        auto k = find(begin(*cell_keys), arr_end, key);
        return k != arr_end;
    }

    void add_bishop_moves(list<int>& l, int key, Cell* cell) {
        TMoveFn fns[6] = { &move_diagonally_top_right
                         , &move_diagonally_top_left
                         , &move_diagonally_bottom_right
                         , &move_diagonally_bottom_left
                         , &move_diagonally_right
                         , &move_diagonally_left
                         };
        add_valid_moves(l, key, fns, 6, cell);
    }

    void add_knight_moves(list<int>& l, int key, Cell* cell) {
        int pos;
        pos = move_vertically_up(move_vertically_up(key));
        add_if_valid(l, move_horizontally_top_right(pos), cell, true);
        add_if_valid(l, move_horizontally_top_left(pos), cell, true);

        pos = move_vertically_down(move_vertically_down(key));
        add_if_valid(l, move_horizontally_bottom_right(pos), cell, true);
        add_if_valid(l, move_horizontally_bottom_left(pos), cell, true);

        pos = move_horizontally_top_right(move_horizontally_top_right(key));
        add_if_valid(l, move_vertically_up(pos), cell, true);
        add_if_valid(l, move_horizontally_bottom_right(pos), cell, true);

        pos = move_horizontally_bottom_right(move_horizontally_bottom_right(key));
        add_if_valid(l, move_vertically_down(pos), cell, true);
        add_if_valid(l, move_horizontally_top_right(pos), cell, true);

        pos = move_horizontally_bottom_left(move_horizontally_bottom_left(key));
        add_if_valid(l, move_vertically_down(pos), cell, true);
        add_if_valid(l, move_horizontally_top_left(pos), cell, true);

        pos = move_horizontally_top_left(move_horizontally_top_left(key));
        add_if_valid(l, move_vertically_up(pos), cell, true);
        add_if_valid(l, move_horizontally_bottom_left(pos), cell, true);
    }

    void add_rook_moves(list<int>& l, int key, Cell* cell) {
        TMoveFn fns[6] = { &move_horizontally_top_right
                         , &move_horizontally_top_left
                         , &move_horizontally_bottom_right
                         , &move_horizontally_bottom_left
                         , &move_vertically_up
                         , &move_vertically_down
                         };
        add_valid_moves(l, key, fns, 6, cell);
    }

    void add_queen_moves(list<int>& l, int key, Cell* cell) {
        add_bishop_moves(l, key, cell);
        add_rook_moves(l, key, cell);
    }

    void add_king_moves(list<int>& l, int key, Cell* cell) {
        add_if_valid(l, move_vertically_up(key), cell, true);
        add_if_valid(l, move_vertically_down(key), cell, true);
        add_if_valid(l, move_horizontally_top_right(key), cell, true);
        add_if_valid(l, move_horizontally_top_left(key), cell, true);
        add_if_valid(l, move_horizontally_bottom_right(key), cell, true);
        add_if_valid(l, move_horizontally_bottom_left(key), cell, true);
        add_if_valid(l, move_diagonally_top_right(key), cell, true);
        add_if_valid(l, move_diagonally_top_left(key), cell, true);
        add_if_valid(l, move_diagonally_bottom_right(key), cell, true);
        add_if_valid(l, move_diagonally_bottom_left(key), cell, true);
    }

    void add_valid_moves(list<int>& l, const int key, TMoveFn fns[], int fns_count, Cell* cell) {
        int current_pos;
        for (int i = 0; i < fns_count; i++) {
            TMoveFn fn = fns[i];
            current_pos = fn(key);
            while (is_valid_position(current_pos)) {
                Cell* c = board_map[current_pos];
                if (c->has_piece()) {
                    if (c->has_piece_of_same_color(cell)) {
                        // cannot take a piece of the same color and cannot move further
                        break;
                    } else {
                        // can take a piece of the opposite color but cannot move further
                        l.push_front(current_pos);
                        break;
                    }
                } else {
                    // empty cell, can continue moving
                    l.push_front(current_pos);
                    current_pos = fn(current_pos);
                }
            }
        }
    }

    inline void add_if_valid(list<int>& l, int key, Cell* cell, bool can_take) {
        if (is_valid_position(key)) {
            Cell* c = board_map[key];
            if (c->has_piece()) {
                if (c->has_piece_of_opposite_color(cell) && can_take) {
                    l.push_front(key);
                }
            } else {
                l.push_front(key);
            }
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

    static int move_diagonally_right(const int key) {
        int x = get_x(key);
        if (x % 2 == 0) {
            if (x == median - 1) {
                return key + step_x*2; // x+2, y
            } else if (x < median) {
                return key + step_x*2 + 1; // x+2, y+1
            } else {
                return key + step_x*2 - 1; // x+2, y-1
            }
        } else {
            if (x < median) {
                return key + step_x*2 + 1; // x+2, y+1
            } else {
                return key + step_x*2 - 1; // x+2, y-1
            }
        }
    }

    static int move_diagonally_left(const int key) {
        int x = get_x(key);
        if (x % 2 == 0) {
            if (x == median + 1) {
                return key - step_x*2; // x-2, y
            } else if (x < median) {
                return key - step_x*2 - 1; // x-2, y-1
            } else {
                return key - step_x*2 + 1; // x-2, y+1
            }
        } else {
            if (x <= median) {
                return key - step_x*2 - 1; // x-2, y-1
            } else {
                return key - step_x*2 + 1; // x-2, y+1
            }
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

    list<int> get_piece_keys(Cell::PieceColor pc) {
        return get_piece_keys(board_map, pc);
    }

    list<int> get_piece_keys(map<int, Cell*>& in_board, Cell::PieceColor pc) {
        list<int> l = {};
        for(const auto& [key, cell] : in_board) {
            if (cell->get_piece_color() == pc) {
                l.push_front(key);
            }
        }
        return l;
    }

    list<int> get_all_piece_move_keys(Cell::PieceColor pc) {
        return get_all_piece_move_keys(board_map, pc);
    }

    list<int> get_all_piece_move_keys(map<int, Cell*>& in_board, Cell::PieceColor pc) {
        list<int> all_moves = {};
        auto all_piece_keys = get_piece_keys(in_board, pc);
        for (int key : all_piece_keys) {
            auto moves = get_valid_moves(in_board, key, true);
            all_moves.insert(all_moves.end(), moves.begin(), moves.end());
        }
        return all_moves;
    }

    bool can_be_captured(const int key) {
        return can_be_captured(board_map, key);
    }

    bool can_be_captured(map<int, Cell*>& in_board, const int key) {
        Cell::PieceColor pc = in_board[key]->get_opposite_color();
        auto all_moves = get_all_piece_move_keys(in_board, pc);
        auto k = find(begin(all_moves), end(all_moves), key);
        return k != end(all_moves);
    }

    map<int, Cell*> copy_board_map() {
        map<int, Cell*> board_map_copy = {};
        for (const auto& [key, cell] : this->board_map) {
            board_map_copy[key] = new Cell(*cell);
        }
        return board_map_copy;
    }
};
