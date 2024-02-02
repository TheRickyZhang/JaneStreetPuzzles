#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <queue>
#include "Pentomino.h"

using namespace std;

// All logic associated with problem grid
class Grid {
public:
    vector<vector<int>> grid;
    int rows, cols;

    // Data for current grid sums and sum constraints
    vector<int> rowSums;
    vector<int> colSums;
    vector<int> groupSums;
    vector<Group> groups;
    static const int sumOfGroup = 24;
    map<Coord, int> cellToGroupMap;
    map<int, int> expectedRowSums;
    map<int, int> expectedColSums;
    vector<int> zeroesInCols;
    vector<int> zeroesInRows;
    vector<int> zeroesInGroups;

    // Constructor
    Grid(int rows, int cols) : rows(rows), cols(cols), grid(rows, vector<int>(cols, 0)),
                               rowSums(rows, 0), colSums(cols, 0), groupSums(20, 0),
                               zeroesInCols(cols, rows), zeroesInRows(rows, cols)
    { initializeData(); }
    void initializeData();

    // Place and remove Pentominos from grid using top left corner, returning true if placed successfully
    bool placePentomino(const Pentomino &p, int row, int col);
    bool removePentomino(const Pentomino &p, int row, int col);

    // Various helper functions
    void revertToOriginalState(const vector<vector<int>>& originalArea,
                               const vector<int>& originalRowSums,
                               const vector<int>& originalColSums,
                               const vector<int>& originalGroupSums,
                               const vector<int>& originalZeroesInCols,
                               const vector<int>& originalZeroesInRows,
                               const vector<int>& originalZeroesInGroups,
                               int row, int col, int affectedRows, int affectedCols);
    bool prune(const int& currSize);
    void printBoard();
    size_t hash();

    // Check if matches solution
    bool checkSums(const Grid& g);
    unsigned long long calculateAnswer();
};

// Remove grids with no possibility of meeting sum requirements
void pruneGrids(vector<Grid>& grids, int currSize);