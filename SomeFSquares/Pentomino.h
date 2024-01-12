#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;


// Eight types of pentomino orientations, with reversed (r) and rotation (degrees clockwise)
enum PentominoTypes { f, f90, f180, f270, fr, fr90, fr180, fr270 };

struct Pentomino {
    vector<vector<int>> shape;
    PentominoTypes type;
    int scale;
    Pentomino(PentominoTypes p, int _scale);
};

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

ostream& operator<<(ostream& os, const Coord& coord);

struct Group {
    vector<Coord> cells;
    int sum = 0;
};

struct SumInfo {
    int index;
    int difference;
    bool operator<(const SumInfo& other) const {
        return difference < other.difference;
    }
};

class Grid {
public:
    vector<vector<int>> grid;
    vector<int> rowSums;
    vector<int> colSums;
    vector<int> groupSums;
    vector<Group> groups;
    static const int sumOfGroup = 24;
    map<Coord, int> cellToGroupMap;
    map<int, int> expectedRowSums;
    map<int, int> expectedColSums;
    priority_queue<SumInfo> rowQueue, colQueue;

    int rows, cols;

    Grid(int rows, int cols) : rows(rows), cols(cols), grid(rows, vector<int>(cols, 0)),
                               rowSums(rows, 0), colSums(cols, 0), groupSums(20, 0)
    { initializeGroups(); }
    void printBoard();
    size_t hash();
    vector<int> findRowPlacementCoordinates(const Pentomino& p);
    vector<int> findColPlacementCoordinates(const Pentomino& p);
    void updateSumsAndQueues();

    void revertToOriginalState(const vector<vector<int>>& originalArea,
                               const vector<int>& originalRowSums,
                               const vector<int>& originalColSums,
                               const vector<int>& originalGroupSums,
                               int row, int col, int affectedRows, int affectedCols);

    bool placePentomino(const Pentomino &p, int row, int col);

    bool removePentomino(const Pentomino &p, int row, int col);

    bool checkSums(const Grid& g);

    void initializeGroups();
};