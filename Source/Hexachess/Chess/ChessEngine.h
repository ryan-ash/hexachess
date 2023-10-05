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
    Position(int32 x, int32 y): x(x), y(y) {}

    int32 x, y;
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

    map<Cell::PieceType, int32> piece_values = {
        {Cell::PieceType::pawn, 1},
        {Cell::PieceType::knight, 3},
        {Cell::PieceType::bishop, 3},
        {Cell::PieceType::rook, 5},
        {Cell::PieceType::queen, 9},
        {Cell::PieceType::king, 100}
    };

    Board() {
        for (int32 x = 0; x <= max; x++) {
            int32 y_max = median + x;
            if (y_max > max) {
                y_max = max - y_max % max;
            }
            for (int32 y = 0; y <= y_max; y++) {
                int32 pos = to_position_key(x, y);
                Cell* cell = new Cell();
                board_map[pos] = cell;
            }
        }
    }

    bool is_valid_position(int32 x, int32 y) {
        int32 pos = to_position_key(x, y);
        return is_valid_position(pos);
    }

    list<Position> get_valid_moves(Position& pos) {
        auto key = to_position_key(pos);
        auto moves = get_valid_moves(key);
        list<Position> pos_list = {};
        for (const int32 k : moves) {
            Position p = this->to_position(k);
            pos_list.push_front(p);
        }
        return pos_list;
    }

    list<int32> get_valid_moves(int32 key) {
        return get_valid_moves(board_map, key);
    }

    list<int32> get_valid_moves(map<int32, Cell*>& in_board, int32 key, bool skip_filter = false) {
        Cell* cell = in_board[key];
        list<int32> l = {};
        switch (cell->get_piece_type()) {
            case Cell::PieceType::none:
                break;
            case Cell::PieceType::pawn:
                add_pawn_moves(in_board, l, key, cell);
                break;
            case Cell::PieceType::bishop:
                add_bishop_moves(in_board, l, key, cell);
                break;
            case Cell::PieceType::knight:
                add_knight_moves(in_board, l, key, cell);
                break;
            case Cell::PieceType::rook:
                add_rook_moves(in_board, l, key, cell);
                break;
            case Cell::PieceType::queen:
                add_queen_moves(in_board, l, key, cell);
                break;
            case Cell::PieceType::king:
                add_king_moves(in_board, l, key, cell);
                break;
        }

        list<int32> filtered_list = {};
        auto color_pieces = get_piece_keys(cell->get_piece_color());
        int32 king_key = -1;
        for (auto piece_key : color_pieces) {
            if (in_board[piece_key]->get_piece_type() == Cell::PieceType::king) {
                king_key = piece_key;
                break;
            }
        }
        if (skip_filter || king_key == -1) {
            return l;
        }
        for (int32 k : l) {
            auto board_copy = copy_board_map();
            Position start = to_position(key);
            Position goal = to_position(k);

            move_piece(board_copy, start, goal);
            auto final_king_key = king_key;
            if (cell->get_piece_type() == Cell::PieceType::king) {
                final_king_key = k;
            }
            if (!can_be_captured(board_copy, final_king_key)) {
                filtered_list.push_front(k);
            }

            clear_board_map(board_copy);
        }
        return filtered_list;
    }

    list<int32> get_piece_keys(Cell::PieceColor pc) {
        return get_piece_keys(board_map, pc);
    }

    list<int32> get_piece_keys(map<int32, Cell*>& in_board, Cell::PieceColor pc) {
        list<int32> l = {};
        for(const auto& [key, cell] : in_board) {
            if (cell->get_piece_color() == pc) {
                l.push_front(key);
            }
        }
        return l;
    }

    list<int32> get_all_piece_move_keys(Cell::PieceColor pc, bool skip_filter = false) {
        return get_all_piece_move_keys(board_map, pc, skip_filter);
    }

    list<int32> get_possible_move_sources(int32 target, Cell::PieceColor pc) {
        return get_possible_move_sources(board_map, target, pc);
    }

    list<int32> get_possible_move_sources(map<int32, Cell*>& in_board, int32 target, Cell::PieceColor pc) {
        list<int32> l = {};
        auto all_moves = get_all_piece_move_keys(in_board, pc, true);
        for (auto move : all_moves) {
            auto moves = get_valid_moves(in_board, move, true);
            auto k = find(begin(moves), end(moves), target);
            if (k != end(moves)) {
                l.push_front(move);
            }
        }
        return l;
    }

    Position to_position(int32 key) {
        Position pos = Position{get_x(key), get_y(key)};
        return pos;
    }

    bool are_there_valid_moves(Cell::PieceColor pc) {
        return are_there_valid_moves(board_map, pc);
    }

    bool are_there_valid_moves(map<int32, Cell*>& in_board, Cell::PieceColor pc) {
        auto all_moves = get_all_piece_move_keys(in_board, pc);
        return all_moves.size() > 0;
    }

    bool move_piece(Position& start, Position& goal) {
        return move_piece(board_map, start, goal);
    }

    bool move_piece(map<int32, Cell*>& in_board, Position& start, Position& goal) {
        bool is_main_board = &in_board == &board_map;

        #if WITH_EDITOR
        // disabled for now, might be costly
        // UE_LOG(LogChessEngine, Log, TEXT("%s: Move piece from (%d, %d) to (%d, %d)"), is_main_board ? TEXT("MAIN") : TEXT("INNER"), start.x, start.y, goal.x, goal.y);
        #endif

        int32 sp = to_position_key(start);
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

    bool set_piece(map<int32, Cell*>& in_board, Position& pos, Cell::PieceType pt, Cell::PieceColor pc) {
        int32 key = to_position_key(pos);
        if (!is_valid_position(in_board, key)) {
            return false;
        }
        in_board[key]->set_piece(pt, pc);
        return false;
    }

    bool can_be_captured(Position& pos) {
        return can_be_captured(board_map, pos);
    }

    bool can_be_captured(map<int32, Cell*>& in_board, Position& pos) {
        int32 key = to_position_key(pos);
        return can_be_captured(in_board, key);
    }

    int32 evaluate() {
        return evaluate(board_map);
    }

    int32 evaluate(map<int32, Cell*>& in_board)
    {
        // set up some scoring for figures
        int32 score = 0;

        // get all pieces
        auto white_pieces = get_piece_keys(in_board, Cell::PieceColor::white);
        auto black_pieces = get_piece_keys(in_board, Cell::PieceColor::black);

        // count each piece with modifier based on its type
        // whites are positive while blacks are negative
        // check is severely punished
        for (auto piece_key : white_pieces)
        {
            if (in_board[piece_key]->get_piece_type() == Cell::PieceType::king)
            {
                if (can_be_captured(in_board, piece_key))
                {
                    score -= piece_values[Cell::PieceType::king];
                }
            }
            else
            {
                score += piece_values[in_board[piece_key]->get_piece_type()];
            }
        }
        for (auto piece_key : black_pieces)
        {
            if (in_board[piece_key]->get_piece_type() == Cell::PieceType::king)
            {
                if (can_be_captured(in_board, piece_key))
                {
                    score += piece_values[Cell::PieceType::king];
                }
            }
            else
            {
                score -= piece_values[in_board[piece_key]->get_piece_type()];
            }
        }

        return score;
    }

    map<int32, Cell*> copy_board_map() {
        map<int32, Cell*> board_map_copy = {};
        for (const auto& [key, cell] : this->board_map) {
            board_map_copy[key] = new Cell(*cell);
        }
        return board_map_copy;
    }

    map<int32, Cell*> copy_board_map(map<int32, Cell*>& in_board) {
        map<int32, Cell*> board_map_copy = {};
        for (const auto& [key, cell] : in_board) {
            board_map_copy[key] = new Cell(*cell);
        }
        return board_map_copy;
    }

    void clear_board_map(std::map<int32, Cell*>& in_board) {
        for (auto& pair : in_board) {
            delete pair.second;  // Deletes the pointer
        }
        in_board.clear();  // Clears the map
    }

    map<int32, Cell*> board_map;



private:

    using TMoveFn = int32 (*)(const int32);

    static const int32 median = 5;
    static const int32 max = 10;
    static const int32 step_x = 1 << 8;
    const vector<int32> white_pawn_cell_keys = {256, 513, 770, 1027, 1284, 1539, 1794, 2049, 2304};
    const vector<int32> black_pawn_cell_keys = {262, 518, 774, 1030, 1286, 1542, 1798, 2054, 2310};

    inline int32 to_position_key(int32 x, int32 y) {
        return (x << 8) + y;
    }

    inline int32 to_position_key(Position pos) {
        return to_position_key(pos.x, pos.y);
    }

    inline bool is_valid_position(int32 key) {
        return is_valid_position(board_map, key);
    }

    inline bool is_valid_position(map<int32, Cell*>& in_board, int32 key) {
        return in_board.find(key) != in_board.end();
    }

    inline bool is_valid_position(Position& pos) {
        return is_valid_position(board_map, pos);
    }

    inline bool is_valid_position(map<int32, Cell*>& in_board, Position& pos) {
        return is_valid_position(in_board, to_position_key(pos));
    }

    void add_pawn_moves(map<int32, Cell*>& in_board, list<int32>& l, int32 key, Cell* cell) {
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
        int32 move = fn_move(key);
        if (is_valid_position(in_board, move)) {
            add_if_valid(in_board, l, move, cell, false);
            if (!in_board[move]->has_piece() && is_initial_pawn_cell(key, cell)) {
                add_if_valid(in_board, l, fn_move(move), cell, false);
            }
        }
        int32 take = fn_take_1(key);
        add_pawn_take_if_valid(in_board, l, take, cell);
        take = fn_take_2(key);
        add_pawn_take_if_valid(in_board, l, take, cell);
    }

    void add_pawn_take_if_valid(map<int32, Cell*>& in_board, list<int32>& l, int32 key, Cell* cell) {
        if (is_valid_position(in_board, key) && in_board[key]->has_piece_of_opposite_color(cell)) {
            l.push_front(key);
        }
    }

    bool is_initial_pawn_cell(const int32 key, Cell* cell) {
        const vector<int32>* cell_keys;
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

    void add_bishop_moves(map<int32, Cell*>& in_board, list<int32>& l, int32 key, Cell* cell) {
        TMoveFn fns[6] = { &move_diagonally_top_right
                         , &move_diagonally_top_left
                         , &move_diagonally_bottom_right
                         , &move_diagonally_bottom_left
                         , &move_diagonally_right
                         , &move_diagonally_left
                         };
        add_valid_moves(in_board, l, key, fns, 6, cell);
    }

    void add_knight_moves(map<int32, Cell*>& in_board, list<int32>& l, int32 key, Cell* cell) {
        int32 pos;
        pos = move_vertically_up(move_vertically_up(key));
        add_if_valid(in_board, l, move_horizontally_top_right(pos), cell, true);
        add_if_valid(in_board, l, move_horizontally_top_left(pos), cell, true);

        pos = move_vertically_down(move_vertically_down(key));
        add_if_valid(in_board, l, move_horizontally_bottom_right(pos), cell, true);
        add_if_valid(in_board, l, move_horizontally_bottom_left(pos), cell, true);

        pos = move_horizontally_top_right(move_horizontally_top_right(key));
        add_if_valid(in_board, l, move_vertically_up(pos), cell, true);
        add_if_valid(in_board, l, move_horizontally_bottom_right(pos), cell, true);

        pos = move_horizontally_bottom_right(move_horizontally_bottom_right(key));
        add_if_valid(in_board, l, move_vertically_down(pos), cell, true);
        add_if_valid(in_board, l, move_horizontally_top_right(pos), cell, true);

        pos = move_horizontally_bottom_left(move_horizontally_bottom_left(key));
        add_if_valid(in_board, l, move_vertically_down(pos), cell, true);
        add_if_valid(in_board, l, move_horizontally_top_left(pos), cell, true);

        pos = move_horizontally_top_left(move_horizontally_top_left(key));
        add_if_valid(in_board, l, move_vertically_up(pos), cell, true);
        add_if_valid(in_board, l, move_horizontally_bottom_left(pos), cell, true);
    }

    void add_rook_moves(map<int32, Cell*>& in_board, list<int32>& l, int32 key, Cell* cell) {
        TMoveFn fns[6] = { &move_horizontally_top_right
                         , &move_horizontally_top_left
                         , &move_horizontally_bottom_right
                         , &move_horizontally_bottom_left
                         , &move_vertically_up
                         , &move_vertically_down
                         };
        add_valid_moves(in_board, l, key, fns, 6, cell);
    }

    void add_queen_moves(map<int32, Cell*>& in_board, list<int32>& l, int32 key, Cell* cell) {
        add_bishop_moves(in_board, l, key, cell);
        add_rook_moves(in_board, l, key, cell);
    }

    void add_king_moves(map<int32, Cell*>& in_board, list<int32>& l, int32 key, Cell* cell) {
        add_if_valid(in_board, l, move_vertically_up(key), cell, true);
        add_if_valid(in_board, l, move_vertically_down(key), cell, true);
        add_if_valid(in_board, l, move_horizontally_top_right(key), cell, true);
        add_if_valid(in_board, l, move_horizontally_top_left(key), cell, true);
        add_if_valid(in_board, l, move_horizontally_bottom_right(key), cell, true);
        add_if_valid(in_board, l, move_horizontally_bottom_left(key), cell, true);
        add_if_valid(in_board, l, move_diagonally_top_right(key), cell, true);
        add_if_valid(in_board, l, move_diagonally_top_left(key), cell, true);
        add_if_valid(in_board, l, move_diagonally_bottom_right(key), cell, true);
        add_if_valid(in_board, l, move_diagonally_bottom_left(key), cell, true);
        add_if_valid(in_board, l, move_diagonally_right(key), cell, true);
        add_if_valid(in_board, l, move_diagonally_left(key), cell, true);
    }

    void add_valid_moves(map<int32, Cell*>& in_board, list<int32>& l, const int32 key, TMoveFn fns[], int32 fns_count, Cell* cell) {
        int32 current_pos;
        for (int32 i = 0; i < fns_count; i++) {
            TMoveFn fn = fns[i];
            current_pos = fn(key);
            while (is_valid_position(in_board, current_pos)) {
                Cell* c = in_board[current_pos];
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

    inline void add_if_valid(map<int32, Cell*>& in_board, list<int32>& l, int32 key, Cell* cell, bool can_take) {
        if (is_valid_position(in_board, key)) {
            Cell* c = in_board[key];
            if (c->has_piece()) {
                if (c->has_piece_of_opposite_color(cell) && can_take) {
                    l.push_front(key);
                }
            } else {
                l.push_front(key);
            }
        }
    }

    static inline int32 move_vertically_up(const int32 key) {
        return key + 1; // x, y+1
    }

    static inline int32 move_vertically_down(const int32 key) {
        return key - 1; // x, y-1
    }

    static int32 move_horizontally_top_right(const int32 key) {
        if (get_x(key) < median) {
            return key + step_x + 1; // x+1, y+1
        } else {
            return key + step_x; // x+1, y
        }
    }

    static int32 move_horizontally_top_left(const int32 key) {
        if (get_x(key) > median) {
            return key - step_x + 1; // x-1, y+1
        } else {
            return key - step_x; // x-1, y
        }
    }

    static int32 move_horizontally_bottom_right(const int32 key) {
        if (get_x(key) < median) {
            return key + step_x; // x+1, y
        } else {
            return key + step_x - 1; // x+1, y-1
        }
    }

    static int32 move_horizontally_bottom_left(const int32 key) {
        if (get_x(key) > median) {
            return key - step_x; // x-1, y
        } else {
            return key - step_x - 1; // x-1, y-1
        }
    }

    static int32 move_diagonally_top_right(const int32 key) {
        if (get_x(key) < median) {
            return key + step_x + 2; // x+1, y+2
        } else {
            return key + step_x + 1; // x+1, y+1
        }
    }

    static int32 move_diagonally_top_left(const int32 key) {
        if (get_x(key) > median) {
            return key - step_x + 2; // x-1, y+2
        } else {
            return key - step_x + 1; // x-1, y+1
        }
    }

    static int32 move_diagonally_bottom_right(const int32 key) {
        if (get_x(key) < median) {
            return key + step_x - 1; // x+1, y-1
        } else {
            return key + step_x - 2; // x+1, y-2
        }
    }

    static int32 move_diagonally_bottom_left(const int32 key) {
        if (get_x(key) > median) {
            return key - step_x - 1; // x-1, y-1
        } else {
            return key - step_x - 2; // x-1, y-2
        }
    }

    static int32 move_diagonally_right(const int32 key) {
        int32 x = get_x(key);
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

    static int32 move_diagonally_left(const int32 key) {
        int32 x = get_x(key);
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

    static int32 ping(const int32 key) {
        return key;
    }

    static inline int32 get_x(const int32 key) {
        return key >> 8;
    }

    static inline int32 get_y(const int32 key) {
        return key & 0xFF;
    }

    list<int32> get_all_piece_move_keys(map<int32, Cell*>& in_board, Cell::PieceColor pc, bool skip_filter = false) {
        list<int32> all_moves = {};
        auto all_piece_keys = get_piece_keys(in_board, pc);
        for (int32 key : all_piece_keys) {
            auto moves = get_valid_moves(in_board, key, skip_filter);
            all_moves.insert(all_moves.end(), moves.begin(), moves.end());
        }
        return all_moves;
    }

    bool can_be_captured(const int32 key) {
        return can_be_captured(board_map, key);
    }

    bool can_be_captured(map<int32, Cell*>& in_board, const int32 key) {
        Cell::PieceColor pc = in_board[key]->get_opposite_color();
        auto all_moves = get_all_piece_move_keys(in_board, pc, true);
        auto k = find(begin(all_moves), end(all_moves), key);
        return k != end(all_moves);
    }
};
