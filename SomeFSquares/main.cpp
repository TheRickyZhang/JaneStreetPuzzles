#include "Pentomino.h"
#include <unordered_set>

/* There are a total of 20 groups of squares that must have a common sum
 * The sum of the numbers in the rows and columns is 899 + x + y, which can be reasonably bounded from 900 to 980.
 *  Since each square is shared by one column and one row, the total sum is s/2 -> 450 to 480.
 *  Thus, each group must have a sum of 23 or 24, which must be 24 due to the requirement of placing size 3 pentominos.
 *
 *  An 'f' of size n will have a maximum sum along n consecutive rows/diagonals of 3n^2. Thus, the maximum possible f
 *  is size 3.
 */

unordered_multiset<size_t> visitedStates;

/*
bool solvePuzzle(Grid &grid, vector<Pentomino>& pentominos) {
    size_t currentState = grid.hash();
    if(visitedStates.find(currentState) != visitedStates.end()){
        return false;
    }
    if (grid.checkSums(grid)) {
        return true; // All sum conditions are met, puzzle is solved
    }
    for (const Pentomino& p : pentominos) {
        for (int y = 0; y <= grid.rows - p.scale * 3; ++y) {
            for (int x = 0; x <= grid.cols - p.scale * 3; ++x) {
                if (grid.placePentomino(p, y, x)) {
                    //grid.printBoard();
                    visitedStates.insert(currentState);
                    if(p.scale == 3){
                        cout << "Placed size 3" << endl;
                    }
                    if(p.scale == 2){
                        cout << "Placed size 2" << endl;
                    }
                    if(visitedStates.size() % 1000 == 0){
                        cout << visitedStates.size() << endl;
                    }
                    if (solvePuzzle(grid, pentominos)) {
                        return true; // Recursive call with the next pentomino
                    }
                    else{
                        grid.removePentomino(p, y, x); // Backtrack
                        //grid.printBoard();
                    }
                }
            }
        }
    }
    return false;
}
*/


bool solvePuzzle(Grid &grid, vector<Pentomino>& pentominos) {
    size_t currentState = grid.hash();
    if(visitedStates.find(currentState) != visitedStates.end()){
        return false;
    }
    if (grid.checkSums(grid)) {
        return true;
    }
    for (const Pentomino& p : pentominos) {
        vector<int> yValues = grid.findRowPlacementCoordinates(p);
        vector<int> xValues = grid.findColPlacementCoordinates(p);
        if(yValues.empty() || xValues.empty()) {
            continue;
        }
        for(int y : yValues){
            for(int x : xValues){
                if (grid.placePentomino(p, y, x)) {
                    //grid.printBoard();
                    visitedStates.insert(currentState);
                    if(p.scale == 3){
                        cout << "Placed size 3" << endl;
                    }
                    if(visitedStates.size() % 1000 == 0){
                        cout << visitedStates.size() << endl;
                    }
                    if (solvePuzzle(grid, pentominos)) {
                        return true; // Recursive call with the next pentomino
                    }
                    else{
                        grid.removePentomino(p, y, x); // Backtrack
                        //grid.printBoard();
                    }
                }
            }
        }
    }
    return false;
}


int main() {
    Grid grid = Grid(17, 17);
    vector<Pentomino> pentominos;
    for (int i = f; i <= fr270; i++) {
        auto type = static_cast<PentominoTypes>(i);
        for(int j = 3; j >= 1; j--){
            pentominos.emplace_back(type, j);
        }
    }
    if(solvePuzzle(grid, pentominos)){
        grid.printBoard();
    } else{
        cout << "No solution!";
    }

    return 0;
}