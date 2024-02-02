#include "Pentomino.h"

ostream& operator<<(ostream& os, const Coord& coord) {
    os << "(" << coord.y << ", " << coord.x << ")";
    return os;
}

Pentomino::Pentomino(PentominoTypes p, int _scale) {
    if(_scale < 1){
        return;
    }
    type = p;
    scale = _scale;
    // Represent pentomino shape as 2D vector
    switch(p){
        case f:
            shape = {
                    {0, 1, 1},
                    {1, 1, 0},
                    {0, 1, 0}
            }; break;
        case f90:
            shape = {
                    {0, 1, 0},
                    {1, 1, 1},
                    {0, 0, 1}
            }; break;
        case f180:
            shape = {
                    {0, 1, 0},
                    {0, 1, 1},
                    {1, 1, 0}
            }; break;
        case f270:
            shape = {
                    {1, 0, 0},
                    {1, 1, 1},
                    {0, 1, 0}
            }; break;
        case fr:
            shape = {
                    {1, 1, 0},
                    {0, 1, 1},
                    {0, 1, 0}
            }; break;
        case fr90:
            shape = {
                    {0, 0, 1},
                    {1, 1, 1},
                    {0, 1, 0}
            }; break;
        case fr180:
            shape = {
                    {0, 1, 0},
                    {1, 1, 0},
                    {0, 1, 1}
            }; break;
        case fr270:
            shape = {
                    {0, 1, 0},
                    {1, 1, 1},
                    {1, 0, 0}
            }; break;
        default:
            break;
    }
    // Scale pentomino up by an integer factor
    if (scale > 1) {
        vector<vector<int>> tempShape;
        for (auto& row : shape) {
            vector<int> scaledRow;
            for (auto& cell : row) {
                for (int i = 0; i < scale; ++i) {
                    scaledRow.push_back(cell * scale);
                }
            }
            for (int i = 0; i < scale; ++i) {
                tempShape.push_back(scaledRow);
            }
        }
        shape = tempShape;
    }
}