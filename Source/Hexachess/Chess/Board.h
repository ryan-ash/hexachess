// class Board {
//     public:

//     Board() {
//         for (int x = 0; x <= 10; x++) {
//             int y_max = 5 + x;
//             if (y_max > 10) {
//                 y_max = 10 - y_max % 10;
//             }
//             for (int y = 0; y <= y_max; y++) {
//                 Point p = new Point(x, y);
//                 Cell c = new Cell();
//                 board_map[p] = c;
//             }
//         }
//     }

//     bool is_valid_cell_at(int x, int y) {
//         Point p = new Point(x, y);
//         if (board_map.find(p)) {

//         }
//     }

//     private:
//         TMap<Point, Cell, Point::HashFunction> board_map;
// };