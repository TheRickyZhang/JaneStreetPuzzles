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

vector<int> Grid::findRowPlacementCoordinates(const Pentomino& p){
    if(rowQueue.empty()){
        return {};
    }
    vector<int> temp;
    if(p.type == f || p.type == fr){
        int startY = max(0, rowQueue.top().index);
        int endY = min(cols, rowQueue.top().index + 2 * p.scale);
        for(int y = startY; y < endY; y++){
            temp.push_back(y);
        }
    } else if(p.type == f180 || p.type == fr180){
        int startY = max(0, rowQueue.top().index + p.scale);
        int endY = min(cols, rowQueue.top().index + 3 * p.scale);
        for(int y = startY; y < endY; y++){
            temp.push_back(y);
        }
    } else{
        int startY = max(0, rowQueue.top().index + p.scale);
        int endY = min(cols, rowQueue.top().index + 2 * p.scale);
        for(int y = startY; y < endY; y++){
            temp.push_back(y);
        }
    }
    if(!temp.empty()){
        return temp;
    }
    rowQueue.pop();
    return findRowPlacementCoordinates(p);
}
vector<int> Grid::findColPlacementCoordinates(const Pentomino& p){
    if(colQueue.empty()){
        return {};
    }
    vector<int> temp;
    if(p.type == f270 || p.type == fr270){
        int startX = max(0, colQueue.top().index);
        int endX = min(cols, colQueue.top().index + 2 * p.scale);
        for(int x = startX; x < endX; x++){
            temp.push_back(x);
        }
    } else if(p.type == f90 || p.type == fr90){
        int startX = max(0, colQueue.top().index + p.scale);
        int endX = min(cols, colQueue.top().index + 3 * p.scale);
        for(int x = startX; x < endX; x++){
            temp.push_back(x);
        }
    } else{
        int startX = max(0, colQueue.top().index + p.scale);
        int endX = min(cols, colQueue.top().index + 2 * p.scale);
        for(int x = startX; x < endX; x++){
            temp.push_back(x);
        }
    }
    if(!temp.empty()){
        return temp;
    }
    colQueue.pop();
    return findColPlacementCoordinates(p);
}

void Grid::updateSumsAndQueues() {
    while (!rowQueue.empty()) rowQueue.pop();
    while (!colQueue.empty()) colQueue.pop();

    for (int i = 0; i < rows; ++i) {
        rowQueue.push({i, rowSums[i]});
    }
    for (int i = 0; i < cols; ++i) {
        colQueue.push({i, colSums[i]});
    }
}


void Grid::revertToOriginalState(const vector<vector<int>>& originalArea,
                                 const vector<int>& originalRowSums,
                                 const vector<int>& originalColSums,
                                 const vector<int>& originalGroupSums,
                                 int row, int col, int affectedRows, int affectedCols) {
    for (int revertY = row; revertY < affectedRows; ++revertY) {
        for (int revertX = col; revertX < affectedCols; ++revertX) {
            grid[revertY][revertX] = originalArea[revertY - row][revertX - col];
        }
    }
    rowSums = originalRowSums;
    colSums = originalColSums;
    groupSums = originalGroupSums;
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

    // Attempt to place the pentomino
    for (int y = row; y < affectedRows; ++y) {
        for (int x = col; x < affectedCols; ++x) {
            if (grid[y][x] != 0 && p.shape[y - row][x - col] != 0) {
                // Overlap detected, revert affected area and sum vectors to original and return false
                revertToOriginalState(originalArea, originalRowSums, originalColSums, originalGroupSums, row, col, affectedRows, affectedCols);
                return false;
            } else if (grid[y][x] == 0 && p.shape[y - row][x - col] != 0) {
                grid[y][x] = p.shape[y - row][x - col];
                rowSums[y] += p.shape[y - row][x - col];
                colSums[x] += p.shape[y - row][x - col];
                groupSums[cellToGroupMap[Coord(y, x)]] += p.shape[y - row][x - col];
            }
            //Handle cases where expected key is not defined (row 5 and column 11)
            bool rowSumCheck = true;
            bool colSumCheck = true;
            if (expectedRowSums.find(y) != expectedRowSums.end()) {
                rowSumCheck = (rowSums[y] <= expectedRowSums.at(y));
            }
            if (expectedColSums.find(x) != expectedColSums.end()) {
                colSumCheck = (colSums[x] <= expectedColSums.at(x));
            }
            bool groupSumCheck = (groupSums[cellToGroupMap[Coord(y, x)]] <= sumOfGroup);

            if (!rowSumCheck || !colSumCheck || !groupSumCheck) {
                revertToOriginalState(originalArea, originalRowSums, originalColSums, originalGroupSums, row, col, affectedRows, affectedCols);
                return false;
            }
        }
    }
    updateSumsAndQueues();
    return true;
}

bool Grid::removePentomino(const Pentomino &p, int row, int col){
    int affectedRows = row + 3 * p.scale;
    int affectedCols = col + 3 * p.scale;
    if (row < 0 || affectedRows > rows || col < 0 || affectedCols > cols) {
        return false;
    }

    // Create a copy of the affected area
    vector<vector<int>> originalArea(affectedRows - row, vector<int>(affectedCols - col));
    for (int y = row; y < affectedRows; ++y) {
        for (int x = col; x < affectedCols; ++x) {
            originalArea[y - row][x - col] = grid[y][x];
        }
    }


    vector<int> originalRowSums = rowSums;
    vector<int> originalColSums = colSums;
    vector<int> originalGroupSums = groupSums;

    // Check to make sure values aligned with Pentomino to be removed
    for (int y = row; y < affectedRows; ++y) {
        for (int x = col; x < affectedCols; ++x) {
            if (p.shape[y - row][x - col] != 0) {
                // Incorrect pentomino values, revert affected area to original and return false
                if(p.shape[y - row][x - col] != grid[y][x]) {
                    revertToOriginalState(originalArea, originalRowSums, originalColSums, originalGroupSums, row, col, affectedRows, affectedCols);
                    return false;
                } else {
                    grid[y][x] = 0;
                    rowSums[y] -= p.shape[y - row][x - col];
                    colSums[x] -= p.shape[y - row][x - col];
                    groupSums[cellToGroupMap[Coord(y, x)]] -= p.shape[y - row][x - col];
                }
            }
        }
    }
    updateSumsAndQueues();
    return true;
}

#include <map>

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


void Grid::initializeGroups() {
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

    for(int i = 0; i < groups.size(); ++i){ // NOLINT
        for(const Coord& coord : groups[i].cells){
            cellToGroupMap[coord] = i;
            // Grid is initially all 0, but if initial values present, they will be added to group sum;
            groups[i].sum += grid[coord.y][coord.x];
        }
    }

    for (int i = 0; i < rows; ++i) {
        if(i != 5){
            rowQueue.push({i, expectedRowSums[i] - rowSums[i]});
        }
    }
    for (int i = 0; i < cols; ++i) {
        if(i != 11){
            colQueue.push({i, expectedColSums[i] - colSums[i]});
        }
    }
}