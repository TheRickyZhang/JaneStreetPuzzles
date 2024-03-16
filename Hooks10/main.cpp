#include <iostream>

#include "Grid.h"

// Size is the size of the length/width of each hook, while num is the number values to be placed in the hook
bool tryHooks(Grid& g, vector<HookOrientation>& orientations, int size, int& statesSearched) {
    if (size < 1) {
        if (g.isSolved()) {
            cout << "Hooks successfully placed" << endl;
            return true;
        }
        return false;
    }

    // Iterate over available numbers to use
    for (auto& numPair: g.numsToUse) {
        if (numPair.second) continue; // Skip used numbers
        int num = numPair.first;
        // Iterate over all rotations
        for (auto rotation: orientations) {
            // Iterate over all possible corners given size
            for (int y = 0; y <= boardSize - size; ++y) {
                for (int x = 0; x <= boardSize - size; ++x) {
                    Coord c = Coord(y, x);
                    if (num <= 2 * size - 1) {
                        Hook tempHook = Hook(size, rotation, c);
                        g.generateHooks(tempHook, num);

                        // Mark the number as used
                        numPair.second = true;
                        auto possibleHooksCopy = g.possibleHooks;
                        for (auto& h: possibleHooksCopy) {
                            statesSearched++;
                            // Only recurse if the hook can be placed
                            if (g.placeHook(h, tempHook)) {
                                cout << statesSearched << " states searched" << endl;
                                g.printBoard();
                                if (tryHooks(g, orientations, size - 1, statesSearched)) {
                                    return true;
                                }
                                // Remove only if hook was placed
                                g.removeHook(h, tempHook);
                            }
                        }
                        // Restore values
                        numPair.second = false;
                        g.possibleHooks = possibleHooksCopy;
                    }
                }
            }
        }
    }

    return false;
}


int main() {
    Grid g(boardSize);
    int statesSearched = 0;
    vector<HookOrientation> orientations = {
            HookOrientation::L0, HookOrientation::L90,
            HookOrientation::L180, HookOrientation::L270
    };

    if (tryHooks(g, orientations, boardSize, statesSearched)) {
        cout << "Solution found!\n" << statesSearched << " total states were searched" << endl;
    } else {
        cout << "No solution exists for the given configuration." << endl;
    }
    return 0;
}