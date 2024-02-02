#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

// Contains logic for representing Pentominos and other useful structs

// Eight types of pentomino orientations, with reversed (r) and rotation (degrees clockwise)
enum PentominoTypes { f, f90, f180, f270, fr, fr90, fr180, fr270 };

// Represents a scale n Pentomino with a 2D vector size 3n x 3n
struct Pentomino {
    vector<vector<int>> shape;
    PentominoTypes type;
    int scale;
    Pentomino(PentominoTypes p, int _scale);
    bool operator==(const Pentomino& p) const{
        return type == p.type && scale == p.scale;
    }
};

// Basic 2D Coordinate struct
struct Coord {
    int x;
    int y;
    Coord(int _y, int _x) : y(_y), x(_x) {}
    bool operator==(Coord& other) const{
        return other.y == y && other.x == x;
    }
    bool operator<(const Coord& other) const {
        if (x == other.x) return y < other.y;
        return x < other.x;
    }
};

// Custom groups of cells as given in problem
struct Group {
    vector<Coord> cells;
    int sum = 0;
};

ostream& operator<<(ostream& os, const Coord& coord);

