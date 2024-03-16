#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <iostream>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <stdexcept>
#include <queue>
#include "Hook.h"
using namespace std;


struct Grid {
    vector<vector<int>> board;

    unordered_map<Coord, int, hash<Coord>> orthogonalSums;
    unordered_map<Coord, int, hash<Coord>> currentSums;
    vector<vector<Coord>> possibleHooks;
    map<int, bool, greater<>> numsToUse;
    unordered_map<Coord, bool, hash<Coord>> usedCoords;

    // Initialize specific constraints for puzzle and usages of state trackers as false
    explicit Grid(int size) : board(size, vector<int>(size, 0)) {
        initializeBoard();
        for (int i = 0; i < boardSize; ++i) {
            numsToUse[i+1] = false;
            for (int j = 0; j < boardSize; ++j) {
                usedCoords[Coord(i, j)] = false;
            }
        }
    }

    void generateHooks(const Hook& h, int num);
    neighborStates countEmptyNeighbors(const Coord& c);
    vector<int> findMaxUnused(int num) const;
    int findMaxPossibleSum(const Coord& c);
    bool placeHook(vector<Coord>&, Hook& h);
    void removeHook(vector<Coord>&, Hook& h);
    bool isSolved();
    void sanityCheck() const; // :(

    void initializeBoard();
    void printBoard();
};