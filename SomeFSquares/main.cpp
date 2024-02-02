#include "Pentomino.h"
#include "Grid.h"
#include <unordered_set>
#include <chrono>
#include <thread>

/* There are a total of 20 groups of squares that must have a common sum
 * The sum of the numbers in the rows and columns is 899 + x + y, which can be reasonably bounded from 900 to 980.
 *  Since each square is shared by one column and one row, the total sum is s/2 -> 450 to 480.
 *  Thus, each group must have a sum of 23 or 24, which must be 24 due to the requirement of placing size 3 pentominos.
 *
 *  An 'f' of size n will have a maximum sum along n consecutive rows/diagonals of 3n^2. Thus, the maximum possible f
 *  is size 3.
 */
int counter = 0;

void placeMaximumSizePentominos(Grid &grid, vector<Pentomino>& pentominos, int& maxPlaced, vector<Grid>& outputGrids,
                                int& placedCount, unordered_multiset<size_t>& visitedStates) {
    size_t currentState = grid.hash();
    if (visitedStates.find(currentState) != visitedStates.end()) {
        return;
    }
    visitedStates.insert(currentState);

    if (placedCount > maxPlaced) {
        outputGrids.clear();
        maxPlaced = placedCount;
        outputGrids.push_back(grid); // Save the new maximum grid
    } else if (placedCount == maxPlaced) {
        outputGrids.push_back(grid); // Save grid only if it matches the current maximum
    }

    if (counter % 10000 == 0 && pentominos[0].scale == 1){
        cout << counter << " Pentominos Placed" << endl;
        grid.printBoard();
    }

    for (const Pentomino& p : pentominos) {
        for (int y = 0; y < grid.rows - 3 * p.scale + 1; ++y) {
            for (int x = 0; x < grid.cols - 3 * p.scale + 1; ++x) {
                if (grid.placePentomino(p, y, x)) {
                    counter++;
                    if(p.scale == 1 && grid.checkSums(grid)) {
                        outputGrids = {grid};
                        break;
                    }
                    placedCount++;
                    placeMaximumSizePentominos(grid, pentominos, maxPlaced, outputGrids, placedCount, visitedStates);
                    grid.removePentomino(p, y, x);
                    placedCount--;
                }
            }
        }
    }
}


int main() {
    Grid grid = Grid(17, 17);
    unordered_multiset<size_t> visitedStates;
    vector<Pentomino> pentominos3;
    vector<Pentomino> pentominos2;
    vector<Pentomino> pentominos1;

    for (int i = f; i <= fr270; i++) {
        auto type = static_cast<PentominoTypes>(i);
        pentominos3.emplace_back(type, 3);
        pentominos2.emplace_back(type, 2);
        pentominos1.emplace_back(type, 1);
    }
    int placedCount = 0, maxPlaced = 0;
    vector<Grid> size3Grids;
    placeMaximumSizePentominos(grid, pentominos3, maxPlaced, size3Grids, placedCount, visitedStates);
    pruneGrids(size3Grids, 2);
    for (Grid& g : size3Grids) {
        g.printBoard();
    }

    placedCount = 0, maxPlaced = 0, visitedStates = {};
    vector<Grid> size2Grids;
    for (Grid& grid3 : size3Grids) {
        placeMaximumSizePentominos(grid3, pentominos2, maxPlaced, size2Grids, placedCount, visitedStates);
    }
    pruneGrids(size2Grids, 1);
    for (Grid& g : size2Grids) {
        g.printBoard();
    }

    placedCount = 0, maxPlaced = 0, visitedStates = {};
    vector<Grid> size1Grids;
    for (Grid& grid2 : size2Grids) {
        placeMaximumSizePentominos(grid2, pentominos1, maxPlaced, size1Grids, placedCount, visitedStates);
    }
    for (Grid& g : size1Grids) {
        g.printBoard();
        cout << "Placed " << counter << " Pentominoes to reach solution" << endl;
        cout << "Solution: " << g.calculateAnswer() << " (product of sizes of all 0 filled sections)";
    }

    return 0;
}