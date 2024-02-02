#include <functional>
#include "Grid.h"

void Grid::printBoard(){
    for (auto& row : grid){
        for (auto& num : row){
            cout << num << ", ";
        }
        cout << endl;
    }
    cout << endl;
}

size_t Grid::hash() {
    std::hash<int> hasher;
    size_t hashValue = 0;
    for (const auto& row : grid) {
        for (int cell : row) {
            hashValue ^= hasher(cell);
            hashValue = hashValue * 31;
        }
    }
    return hashValue;
}

/*
// Returns y coordinates for placement ranked on the amount contributing to the row with the greatest difference
vector<int> Grid::findRowPlacementCoordinates(const Pentomino& p) const{
    if(rowQueue.empty()){
        return {};
    }
    vector<int> temp;
    if(p.type == f || p.type == fr){
        for(int y = max(0, rowQueue.top().index - 2 * p.scale + 1); y < min(rows, rowQueue.top().index + 1); y++){
            temp.push_back(y);
        }
        for(int y = max(0, rowQueue.top().index - 3 * p.scale + 1); y < min(rows, rowQueue.top().index - 2 * p.scale + 1); y++){
            temp.push_back(y);
        }
    } else if(p.type == f180 || p.type == fr180){
        for(int y = max(0, rowQueue.top().index - 3 * p.scale + 1); y < min(rows, rowQueue.top().index - p.scale + 1); y++){
            temp.push_back(y);
        }
        for(int y = max(0, rowQueue.top().index - p.scale + 1); y < min(rows, rowQueue.top().index + 1); y++){
            temp.push_back(y);
        }
    } else{
        for(int y = max(0, rowQueue.top().index - 2 * p.scale + 1); y < min(rows, rowQueue.top().index - p.scale + 1); y++){
            temp.push_back(y);
        }
        for(int y = max(0, rowQueue.top().index - 3 * p.scale + 1); y < min(rows, rowQueue.top().index + 1); y++){
            if(find(temp.begin(), temp.end(), y) == temp.end()){
                temp.push_back(y);
            }
        }
    }
    for(int y = 0; y <= rows - 3*p.scale; y++){
        if(find(temp.begin(), temp.end(), y) == temp.end()){
            temp.push_back(y);
        }
    }
    return temp;
}

vector<int> Grid::findColPlacementCoordinates(const Pentomino& p) const {
    if(colQueue.empty()) {
        return {};
    }
    vector<int> temp;
    if(p.type == f270 || p.type == fr270){
        for(int x = max(0, colQueue.top().index - 2 * p.scale + 1); x < min(cols, colQueue.top().index + 1); x++){
            temp.push_back(x);
        }
        for(int x = max(0, colQueue.top().index - 3 * p.scale + 1); x < min(cols, colQueue.top().index - 2 * p.scale + 1); x++){
            temp.push_back(x);
        }
    } else if(p.type == f90 || p.type == fr90){
        for(int x = max(0, colQueue.top().index - 3 * p.scale + 1); x < min(cols, colQueue.top().index - p.scale + 1); x++){
            temp.push_back(x);
        }
        for(int x = max(0, colQueue.top().index - p.scale + 1); x < min(cols, colQueue.top().index + 1); x++){
            temp.push_back(x);
        }
    } else {
        for(int x = max(0, colQueue.top().index - 2 * p.scale + 1); x < min(cols, colQueue.top().index - p.scale + 1); x++){
            temp.push_back(x);
        }
        for(int x = max(0, colQueue.top().index - 3 * p.scale + 1); x < min(cols, colQueue.top().index + 1); x++){
            if(find(temp.begin(), temp.end(), x) == temp.end()){
                temp.push_back(x);
            }
        }
    }
    for(int x = 0; x <= cols - 3*p.scale; x++){
        if(find(temp.begin(), temp.end(), x) == temp.end()){
            temp.push_back(x);
        }
    }
    return temp;
}
  */

// Reverts a given area of grid to previously stored state
void Grid::revertToOriginalState(const vector<vector<int>>& originalArea,
                                 const vector<int>& originalRowSums,
                                 const vector<int>& originalColSums,
                                 const vector<int>& originalGroupSums,
                                 const vector<int>& originalZeroesInCols,
                                 const vector<int>& originalZeroesInRows,
                                 const vector<int>& originalZeroesInGroups,
                                 int row, int col, int affectedRows, int affectedCols) {
    for (int revertY = row; revertY < affectedRows; ++revertY) {
        for (int revertX = col; revertX < affectedCols; ++revertX) {
            grid[revertY][revertX] = originalArea[revertY - row][revertX - col];
        }
    }
    rowSums = originalRowSums;
    colSums = originalColSums;
    groupSums = originalGroupSums;
    zeroesInCols = originalZeroesInCols;
    zeroesInRows = originalZeroesInRows;
    zeroesInGroups = originalZeroesInGroups;
}

bool Grid::placePentomino(const Pentomino &p, int row, int col) {
    int affectedRows = row + 3 * p.scale;
    int affectedCols = col + 3 * p.scale;
    if (row < 0 || affectedRows > rows || col < 0 || affectedCols > cols) {
        return false;
    }

    // Create a copy of only the affected area
    vector<vector<int>> originalArea(affectedRows - row, vector<int>(affectedCols - col));
    for (int y = row; y < affectedRows; ++y) {
        for (int x = col; x < affectedCols; ++x) {
            originalArea[y - row][x - col] = grid[y][x];
        }
    }

    vector<int> originalRowSums = rowSums;
    vector<int> originalColSums = colSums;
    vector<int> originalGroupSums = groupSums;
    vector<int> originalZeroesInCols = zeroesInCols;
    vector<int> originalZeroesInRows = zeroesInRows;
    vector<int> originalZeroesInGroups = zeroesInGroups;

    // Attempt to place the pentomino
    for (int y = row; y < affectedRows; ++y) {
        for (int x = col; x < affectedCols; ++x) {
            if (grid[y][x] != 0 && p.shape[y - row][x - col] != 0) {
                // Overlap detected, revert affected area and sums, return false
                revertToOriginalState(originalArea, originalRowSums, originalColSums, originalGroupSums, originalZeroesInCols,
                                      originalZeroesInRows, originalZeroesInGroups, row, col, affectedRows, affectedCols);
                return false;
            } else if (grid[y][x] == 0 && p.shape[y - row][x - col] != 0) {
                grid[y][x] = p.shape[y - row][x - col];
                rowSums[y] += p.shape[y - row][x - col];
                colSums[x] += p.shape[y - row][x - col];
                groupSums[cellToGroupMap[Coord(y, x)]] += p.shape[y - row][x - col];
                zeroesInRows[y]--;
                zeroesInCols[x]--;
                zeroesInGroups[cellToGroupMap[Coord(y, x)]]--;
            }
            // Handle cases where expected key is not defined (row 5 and column 11)
            if (expectedRowSums.find(y) != expectedRowSums.end()) {
                if(rowSums[y] > expectedRowSums.at(y)) {
                    revertToOriginalState(originalArea, originalRowSums, originalColSums, originalGroupSums, originalZeroesInCols,
                                          originalZeroesInRows, originalZeroesInGroups, row, col, affectedRows, affectedCols);
                    return false;
                }
            }
            if (expectedColSums.find(x) != expectedColSums.end()) {
                if (colSums[x] > expectedColSums.at(x)) {
                    revertToOriginalState(originalArea, originalRowSums, originalColSums, originalGroupSums, originalZeroesInCols,
                                          originalZeroesInRows, originalZeroesInGroups, row, col, affectedRows, affectedCols);
                    return false;
                }
            }
            if (groupSums[cellToGroupMap[Coord(y, x)]] > sumOfGroup) {
                revertToOriginalState(originalArea, originalRowSums, originalColSums, originalGroupSums, originalZeroesInCols,
                                      originalZeroesInRows, originalZeroesInGroups, row, col, affectedRows, affectedCols);
                return false;
            }

            if (!prune(p.scale)) {
                revertToOriginalState(originalArea, originalRowSums, originalColSums, originalGroupSums, originalZeroesInCols,
                                      originalZeroesInRows, originalZeroesInGroups, row, col, affectedRows, affectedCols);
                return false;
            }
        }
    }
    return true;
}

bool Grid::removePentomino(const Pentomino &p, int row, int col){
    int affectedRows = row + 3 * p.scale;
    int affectedCols = col + 3 * p.scale;

    if (row < 0 || affectedRows > rows || col < 0 || affectedCols > cols) {
        return false;
    }

    /* Assuming removePentomino is always given valid coordinates, no need for extraneous copies and checks

    vector<vector<int>> originalArea(affectedRows - row, vector<int>(affectedCols - col));
    for (int y = row; y < affectedRows; ++y) {
        for (int x = col; x < affectedCols; ++x) {
            originalArea[y - row][x - col] = grid[y][x];
        }
    }

    vector<int> originalRowSums = rowSums;
    vector<int> originalColSums = colSums;
    vector<int> originalGroupSums = groupSums;
    vector<int> originalZeroesInCols = zeroesInCols;
    vector<int> originalZeroesInRows = zeroesInRows;
    vector<int> originalZeroesInGroups = zeroesInGroups;
    */

    // Check to make sure values to be removed match expected Pentomino shape
    for (int y = row; y < affectedRows; ++y) {
        for (int x = col; x < affectedCols; ++x) {
            if (p.shape[y - row][x - col] != 0) {
                // Incorrect pentomino values, revert affected area and sums and return false
                /*
                if(p.shape[y - row][x - col] != grid[y][x]) {
                    revertToOriginalState(originalArea, originalRowSums, originalColSums, originalGroupSums, originalZeroesInCols,
                                          originalZeroesInRows, originalZeroesInGroups, row, col, affectedRows, affectedCols);
                    return false;
                } else {
                 */
                    grid[y][x] = 0;
                    rowSums[y] -= p.shape[y - row][x - col];
                    colSums[x] -= p.shape[y - row][x - col];
                    groupSums[cellToGroupMap[Coord(y, x)]] -= p.shape[y - row][x - col];
                    zeroesInRows[y]++;
                    zeroesInCols[x]++;
                    zeroesInGroups[cellToGroupMap[Coord(y, x)]]++;
                // }
            }
        }
    }
    return true;
}

// Returns true only if all sums match expected sums for rows, cols, groups
bool Grid::checkSums(const Grid& g) {
    for(auto& temp : g.groupSums){
        if(temp != Grid::sumOfGroup){
            return false;
        }
    }

    auto checkRowSum = [&g](const std::pair<int, int>& p) { return g.rowSums[p.first] == p.second; };
    auto checkColSum = [&g](const std::pair<int, int>& p) { return g.colSums[p.first] == p.second; };

    return std::all_of(expectedRowSums.begin(), expectedRowSums.end(), checkRowSum) &&
           std::all_of(expectedColSums.begin(), expectedColSums.end(), checkColSum);
}

// Initialize customs groups and cell to group map based on problem
void Grid::initializeData() {
    expectedRowSums = {
            {0, 14}, {1, 24}, {2, 24}, {3, 39}, {4, 43}, {6, 22}, {7, 23}, {8, 29},
            {9, 28}, {10, 34}, {11, 36}, {12, 29}, {13, 26}, {14, 26}, {15, 24},
            {16, 20}
    };

    expectedColSums = {
            {0, 13}, {1, 20}, {2, 22}, {3, 28}, {4, 30}, {5, 36}, {6, 35}, {7, 39},
            {8, 49}, {9, 39}, {10, 39}, {12, 23}, {13, 32}, {14, 23}, {15, 17},
            {16, 13}
    };

    // Create custom groups according to initial grid
    groups.push_back(Group{{Coord(0, 0), Coord(0, 1), Coord (1, 0), Coord(1, 1),
                            Coord(1, 2), Coord(2, 0), Coord(3, 0), Coord(4, 0),
                            Coord(5, 0), Coord(6, 0), Coord(7, 0), Coord(8, 0),
                            Coord(9, 0), Coord(10, 0), Coord(11, 0), Coord(12, 0),
                            Coord(13, 0), Coord(14, 0), Coord(14, 4), Coord(14, 5),
                            Coord(15, 0), Coord(15, 1),Coord(15, 4), Coord(16, 0),
                            Coord(16, 1), Coord(16, 2),Coord(16, 3), Coord(16, 4),
                            Coord(16, 5)}});
    groups.push_back(Group{{Coord(2, 1), Coord(2, 2), Coord (2, 3), Coord(3, 1),
                            Coord(3, 2), Coord(4, 1), Coord(5, 1), Coord(6, 1),
                            Coord(7, 1), Coord(8, 1), Coord(9, 1), Coord(10, 1),
                            Coord(11, 1), Coord(12, 1), Coord(13, 1), Coord(14, 1),
                            Coord(14, 2), Coord(15, 2), Coord(15, 3)}});
    groups.push_back(Group{{Coord(0, 2), Coord(0, 3), Coord (1, 3), Coord(1, 4),
                            Coord(1, 5), Coord(1, 6), Coord(2, 4), Coord(2, 5),
                            Coord(2, 6), Coord(3, 3), Coord(3, 4), Coord(3, 5),
                            Coord(3, 6), Coord(4, 3)}});
    groups.push_back(Group{{Coord(4, 2), Coord(5, 2), Coord (5, 3), Coord(5, 5),
                            Coord(6, 3),Coord(6, 4), Coord(6, 5), Coord(7, 4)}});
    groups.push_back(Group{{Coord(6, 2), Coord(7, 2), Coord (7, 3), Coord(8, 2),
                            Coord(8, 3), Coord(8, 4), Coord(9, 2), Coord(9, 3),
                            Coord(9, 4), Coord(9, 5), Coord(10, 2), Coord(10, 3),
                            Coord(10, 4), Coord(11, 2), Coord(11, 3), Coord(12, 2)}});
    groups.push_back(Group{{Coord(10, 7), Coord(10, 8), Coord (11, 6), Coord(11, 7),
                            Coord(11, 8), Coord(12, 3), Coord(12, 4), Coord(12, 5),
                            Coord(12, 6), Coord(12, 7), Coord(13, 2), Coord(13, 3),
                            Coord(13, 4), Coord(13, 5),Coord(13, 6), Coord(13, 7),
                            Coord(14, 3)}});
    groups.push_back(Group{{Coord(0, 4), Coord(0, 5), Coord (0, 6), Coord(0, 7),
                            Coord(0, 8), Coord(0, 9), Coord(1, 7), Coord(1, 8),
                            Coord(1, 9), Coord(2, 7), Coord(2, 9), Coord(3, 9)}});
    groups.push_back(Group{{Coord(2, 8), Coord(3, 7), Coord (3, 8), Coord(4, 4),
                            Coord(4, 5),Coord(4, 6), Coord(4, 7), Coord(5, 4)}});
    groups.push_back(Group{{Coord(5, 9), Coord(6, 9), Coord (7, 7), Coord(7, 8),
                            Coord(7, 9), Coord(8, 7), Coord(9, 6), Coord(9, 7),
                            Coord(10, 5), Coord(10, 6), Coord(11, 4), Coord(11, 5)}});
    groups.push_back(Group{{Coord(4, 8), Coord(5, 6), Coord (5, 7), Coord(5, 8),
                            Coord(6, 6), Coord(6, 7), Coord(6, 8), Coord(7, 5),
                            Coord(7, 6), Coord(8, 5), Coord(8, 6)}});
    groups.push_back(Group{{Coord(14, 6), Coord(15, 5), Coord (15, 6), Coord(16, 6),
                            Coord(16, 7), Coord(16, 8), Coord(16, 9), Coord(16, 10),
                            Coord(16, 11), Coord(16, 12)}});
    groups.push_back(Group{{Coord(11, 9), Coord(12, 8), Coord (12, 9), Coord(12, 10),
                            Coord(13, 8),Coord(13, 10), Coord(14, 7), Coord(14, 8)}});
    groups.push_back(Group{{Coord(13, 9), Coord(14, 9), Coord (14, 12), Coord(14, 13),
                            Coord(15, 7), Coord(15, 8), Coord(15, 9), Coord(15, 10),
                            Coord(15, 11), Coord(15, 12), Coord(15, 13), Coord(16, 13)}});
    groups.push_back(Group{{Coord(4, 9), Coord(4, 10), Coord (5, 10), Coord(6, 10),
                            Coord(7, 10), Coord(7, 12), Coord(8, 8), Coord(8, 9),
                            Coord(8, 10), Coord(8, 11), Coord(8, 12), Coord(9, 10),
                            Coord(9, 12)}});
    groups.push_back(Group{{Coord(9, 8), Coord(9, 9), Coord (10, 9), Coord(10, 10),
                            Coord(11, 10),Coord(11, 11), Coord(12, 11), Coord(12, 12)}});
    groups.push_back(Group{{Coord(0, 10), Coord(0, 11), Coord (0, 12), Coord(0, 13),
                            Coord(0, 14), Coord(0, 15), Coord(0, 16), Coord(1, 10),
                            Coord(1, 15), Coord(1, 16), Coord(2, 10), Coord(2, 12),
                            Coord(2, 13),  Coord(2, 16), Coord(3, 10),Coord(3, 11),
                            Coord(3, 12),Coord(3, 16), Coord(4, 11),Coord(4, 12),
                            Coord(4, 16), Coord(5, 11), Coord(5, 12), Coord(6, 11),
                            Coord(6, 12), Coord(7, 11)}});
    groups.push_back(Group{{Coord(11, 12), Coord(11, 13), Coord (12, 13), Coord(13, 11),
                            Coord(13, 12), Coord(13, 13), Coord(13, 14), Coord(14, 10),
                            Coord(14, 11), Coord(14, 14), Coord(14, 15)}});
    groups.push_back(Group{{Coord(1, 11), Coord(1, 12), Coord (1, 13), Coord(1, 14),
                            Coord(2, 11), Coord(2, 14), Coord(3, 13), Coord(3, 14),
                            Coord(4, 13), Coord(5, 13), Coord(5, 14), Coord(6, 14)}});
    groups.push_back(Group{{Coord(6, 13), Coord(7, 13), Coord (8, 13), Coord(8, 14),
                            Coord(9, 11), Coord(9, 13), Coord(10, 11), Coord(10, 12),
                            Coord(10, 13), Coord(10, 14), Coord(10, 15), Coord(11, 15)}});
    groups.push_back(Group{{Coord(2, 15), Coord(3, 15), Coord (4, 14), Coord(4, 15),
                            Coord(5, 15), Coord(5, 16), Coord(6, 15), Coord(6, 16),
                            Coord(7, 14), Coord(7, 15), Coord(7, 16), Coord(8, 15),
                            Coord(8, 16), Coord(9, 14), Coord(9, 15), Coord(9, 16),
                            Coord(10, 16), Coord(11, 14), Coord(11, 16), Coord(12, 14),
                            Coord(12, 15), Coord(12, 16),Coord(13, 15), Coord(13, 16),
                            Coord(14, 16), Coord(15, 14), Coord(15, 15), Coord(15, 16),
                            Coord(16, 14), Coord(16, 15), Coord(16, 16)}});

    for(int i = 0; i < groups.size(); ++i){
        for(const Coord& coord : groups[i].cells){
            cellToGroupMap[coord] = i;
        }
        zeroesInGroups.push_back(static_cast<int>(groups[i].cells.size()));
    }

}

// Returns false if there is no way to satisfy sums by assigning currSize to all zero cells
bool Grid::prune(const int& currSize) {
    for(int i = 0; i < grid.size(); ++i){
        if (i != 5 && zeroesInRows[i] > 0 && static_cast<double>(expectedRowSums.at(i) - rowSums[i]) / zeroesInRows[i] > currSize) {
            return false;
        }
        if (i != 11 && zeroesInCols[i] > 0 && static_cast<double>(expectedColSums.at(i) - colSums[i]) / zeroesInCols[i] > currSize) {
            return false;
        }
    }

    for(int i = 0; i < groups.size(); ++i) {
        if (zeroesInGroups[i] > 0 && static_cast<double>(Grid::sumOfGroup - groupSums[i]) / zeroesInGroups[i] > currSize) {
            return false;
        }
    }
    return true;
}


void pruneGrids(vector<Grid>& grids, int currSize){
    vector<Grid> tempGrids;
    for (Grid& g : grids){
        bool flag = true;
        for(int i = 0; i < g.grid.size(); ++i){
            if (i != 5 && g.zeroesInRows[i] > 0 && static_cast<double>(g.expectedRowSums.at(i) - g.rowSums[i]) / g.zeroesInRows[i] > currSize) {
                flag = false;
                break;
            }
            if (i != 11 && g.zeroesInCols[i] > 0 && static_cast<double>(g.expectedColSums.at(i) - g.colSums[i]) / g.zeroesInCols[i] > currSize) {
                flag = false;
                break;
            }
        }

        for(int i = 0; i < g.groups.size(); ++i) {
            if (g.zeroesInGroups[i] > 0 && static_cast<double>(Grid::sumOfGroup - g.groupSums[i]) / g.zeroesInGroups[i] > currSize) {
                flag = false;
                break;
            }
        }

        if(flag){
            tempGrids.push_back(g);
        }
    }
    grids = std::move(tempGrids);
}


unsigned long long Grid::calculateAnswer() {
    map<Coord, bool> isIncludedMap;
    vector<int> groupSizes;
    function<void(int, int, int&)> floodFill = [&](int _y, int _x, int& size) {
        if(_y >= 0 && _y < rows && _x >= 0 && _x < cols && grid[_y][_x] == 0 && !isIncludedMap[Coord(_y, _x)]) {
            isIncludedMap[Coord(_y, _x)] = true;
            size++;  // Increment the size of the current group
            floodFill(_y + 1, _x, size);
            floodFill(_y - 1, _x, size);
            floodFill(_y, _x + 1, size);
            floodFill(_y, _x - 1, size);
        }
    };
    for(int y = 0; y < rows; ++y) {
        for(int x = 0; x < cols; ++x) {
            if(grid[y][x] == 0 && !isIncludedMap[Coord(y, x)]) {
                int groupSize = 0;
                floodFill(y, x, groupSize);
                if (groupSize > 0) {
                    groupSizes.push_back(groupSize);
                }
            }
        }
    }
    unsigned long long product = 1;
    for (int size : groupSizes) {
        product *= size;
    }
    return product;
}