#include "Grid.h"

void Grid::sanityCheck() const {
    // Verify that no inconsistencies between board value and isUsed are present
    for (int y = 0; y < board.size(); ++y) {
        for (int x = 0; x < board[y].size(); ++x) {
            Coord c(y, x);
            bool isUsed = usedCoords.at(c);
            bool isEmpty = board[y][x] == 0;
            if (!isUsed && !isEmpty) {
                cout << "board[" << y << "][" << x << "] is marked " << isUsed << "but =" << board[y][x];
                throw runtime_error("Sanity Check Failed: Filled cell marked as unused.");
            }
        }
    }
}

struct State {
    vector<Coord>::iterator it;
    vector<Coord> currentCombination;
    int numLeft;

    State(vector<Coord>::iterator it, vector<Coord> currentCombination, int numLeft)
            : it(it), currentCombination(std::move(currentCombination)), numLeft(numLeft) {}
};

// Iterative method to generate all possible coordinate combinations using states on a stack
void generateHooksIterative(vector<Coord>& available, int num, vector<vector<Coord>>& allCombinations) {
    stack<State> states;
    states.push(State(available.begin(), {}, num));

    while (!states.empty()) {
        auto& [it, currentCombination, numLeft] = states.top();

        // Record combination and start new search with next element
        if (numLeft == 0) {
            allCombinations.push_back(currentCombination);
            states.pop();
            if (!states.empty()) {
                states.top().it++;
            }
            continue;
        }

        // Backtrack and increment iterator
        if (it == available.end()) {
            states.pop();
            if (!states.empty()) {
                states.top().it++;
            }
            continue;
        }

        // Break if not enough elements left to complete with desired amount
        if (distance(it, available.end()) < numLeft) {
            states.pop();
            if (!states.empty()) {
                states.top().it++;
            }
            continue;
        }

        // Create a new state with added element to combination
        currentCombination.push_back(*it);
        states.emplace(next(it), currentCombination, numLeft - 1);

        // Backtrack
        currentCombination.pop_back();
    }
}

// Generates n choose k hook combinations, discarding numbered squares as they must be empty
void Grid::generateHooks(const Hook& h, int num) {
    if (num < 1 || num > 9) {
        throw runtime_error("Num out of bounds");
    }
    possibleHooks = {};
    vector<Coord> available;
    for (auto& elem: h.hook) {
        if (orthogonalSums.find(elem) == orthogonalSums.end()) {
            available.push_back(elem);
        }
    }
    generateHooksIterative(available, num, possibleHooks);
}

// Counts empty neighbors using a boolean for location, which is needed for maxPossibleSum calculation
// [0] is +y, [1] is -y, [2] is +x, [3] is -x
neighborStates Grid::countEmptyNeighbors(const Coord& c) {
    neighborStates neighbors = neighborStates();
    if (c.y + 1 < board.size() && board[c.y + 1][c.x] == 0) {
        if (!usedCoords[Coord(c.y + 1, c.x)]) {
            neighbors.states[0] = true;
        }
    }
    if (c.y - 1 >= 0 && board[c.y - 1][c.x] == 0) {
        if (!usedCoords[Coord(c.y - 1, c.x)]) {
            neighbors.states[1] = true;
        }
    }
    if (c.x + 1 < board[0].size() && board[c.y][c.x + 1] == 0) {
        if (!usedCoords[Coord(c.y, c.x + 1)]) {
            neighbors.states[2] = true;
        }
    }
    if (c.x - 1 >= 0 && board[c.y][c.x - 1] == 0) {
        if (!usedCoords[Coord(c.y, c.x - 1)]) {
            neighbors.states[3] = true;
        }
    }
    return neighbors;
}

// Return the num largest unused numbers in numsToUse in descending order
vector<int> Grid::findMaxUnused(int num) const {
    vector<int> temp(num);
    priority_queue<int, vector<int>, greater<>> minHeap;

    for (auto& i: numsToUse) {
        if (!i.second) {
            if (minHeap.size() < num) {
                minHeap.push(i.first);
            } else if (i.first > minHeap.top()) {
                minHeap.pop();
                minHeap.push(i.first);
            }
        }
    }

    int index = 0;
    while (!minHeap.empty()) {
        temp.at(index) = minHeap.top();
        minHeap.pop();
        index++;
    }
    reverse(temp.begin(), temp.end());
    return temp;
}

// Finds maximum possible sum of empty orthogonal neighbors based on the heuristic of a hook occupying up to any 2 cells
int Grid::findMaxPossibleSum(const Coord& c) {
    auto neighbors = countEmptyNeighbors(c);
    if (neighbors.total() <= 1) {
        return neighbors.total() * findMaxUnused(1)[0];
    } else if (neighbors.total() == 2) {
        return 2 * findMaxUnused(1)[0];
    } else if (neighbors.total() == 3) {
        return 2 * findMaxUnused(2)[0] + findMaxUnused(2)[1];
    } else {
        return 2 * (findMaxUnused(2)[0] + findMaxUnused(2)[1]);
    }
}

bool Grid::placeHook(vector<Coord>& coords, Hook& h) {
    sanityCheck();
    int num = int(coords.size());
    // Store previous values and sums in case restoration needed
    unordered_map<Coord, int, hash<Coord>> previousVals;
    for (auto& c: coords) {
        previousVals[c] = board[c.y][c.x];
    }
    unordered_map<Coord, int, hash<Coord>> temp = currentSums;
    bool valid = true;

    for (auto& c: coords) {
        // Can't place hook on invalid or existing coordinates
        if (c.y >= boardSize || c.y < 0 || c.x >= boardSize || c.x < 0 || board[c.y][c.x] != 0 || usedCoords[c]) {
            valid = false;
        }
        board[c.y][c.x] = num;
        if (orthogonalSums.find(Coord(c.y, c.x + 1)) != orthogonalSums.end()) {
            currentSums[Coord(c.y, c.x + 1)] += num;
        }
        if (orthogonalSums.find(Coord(c.y - 1, c.x)) != orthogonalSums.end()) {
            currentSums[Coord(c.y - 1, c.x)] += num;
        }
        if (orthogonalSums.find(Coord(c.y, c.x - 1)) != orthogonalSums.end()) {
            currentSums[Coord(c.y, c.x - 1)] += num;
        }
        if (orthogonalSums.find(Coord(c.y + 1, c.x)) != orthogonalSums.end()) {
            currentSums[Coord(c.y + 1, c.x)] += num;
        }
    }

    // Check for ability to reach orthogonal sums given current values
    for (auto& coordVal: currentSums) {
        if (coordVal.second > orthogonalSums[coordVal.first]) {
            // cout << "1 Cannot place " << num << " | " << coordVal.second << " > " << orthogonalSums[coordVal.first] << endl;
            valid = false;
        }
        if (coordVal.second < orthogonalSums[coordVal.first] && (countEmptyNeighbors(coordVal.first).total() == 0 ||
                                                                 findMaxPossibleSum(coordVal.first) <
                                                                 orthogonalSums[coordVal.first] -
                                                                 currentSums[coordVal.first])) {
            // cout << "2 Cannot place " << num << " at " << coordVal.first.y << ", " << coordVal.first.x << " | "
            // << coordVal.second << " < " << orthogonalSums[coordVal.first] << endl;
            valid = false;
        }
    }

    if (valid) {
        for (Coord& c: h.hook) {
            // Mark as used
            if (orthogonalSums.find(c) == orthogonalSums.end()) {
                usedCoords[c] = true;
            }
        }
        sanityCheck();
        return true;
    } else {
        // Revert board and currentSums to their previous states
        for (auto& c: coords) {
            board[c.y][c.x] = previousVals[c];
        }
        currentSums = temp;
        sanityCheck();
        return false;
    }
}


void Grid::removeHook(vector<Coord>& coords, Hook& h) {
    sanityCheck();
    // Revert actions of addHook
    int num = int(coords.size());
    for (Coord& c: h.hook) {
        usedCoords[c] = false;
    }
    for (auto& c: coords) {
        if (board[c.y][c.x] != num) {
            throw runtime_error("Incorrect removal");
        }
        board[c.y][c.x] = 0;
        if (orthogonalSums.find(Coord(c.y, c.x + 1)) != orthogonalSums.end()) {
            currentSums[Coord(c.y, c.x + 1)] -= num;
        }
        if (orthogonalSums.find(Coord(c.y - 1, c.x)) != orthogonalSums.end()) {
            currentSums[Coord(c.y - 1, c.x)] -= num;
        }
        if (orthogonalSums.find(Coord(c.y, c.x - 1)) != orthogonalSums.end()) {
            currentSums[Coord(c.y, c.x - 1)] -= num;
        }
        if (orthogonalSums.find(Coord(c.y + 1, c.x)) != orthogonalSums.end()) {
            currentSums[Coord(c.y + 1, c.x)] -= num;
        }
    }
    sanityCheck();
}

bool Grid::isSolved() {
    for (auto h: orthogonalSums) {
        if (currentSums[h.first] != h.second) {
            return false;
        }
    }
    return true;
}


void Grid::initializeBoard() {
    if (board.size() != 9) {
        cout << "Only compatible with 9x9 grid";
        return;
    }
    orthogonalSums[Coord(0, 2)] = 22;
    orthogonalSums[Coord(0, 7)] = 15;
    orthogonalSums[Coord(1, 4)] = 14;
    orthogonalSums[Coord(2, 1)] = 9;
    orthogonalSums[Coord(2, 6)] = 19;
    orthogonalSums[Coord(4, 1)] = 5;
    orthogonalSums[Coord(4, 3)] = 11;
    orthogonalSums[Coord(4, 5)] = 22;
    orthogonalSums[Coord(4, 7)] = 22;
    orthogonalSums[Coord(6, 2)] = 9;
    orthogonalSums[Coord(6, 7)] = 31;
    orthogonalSums[Coord(7, 4)] = 12;
    orthogonalSums[Coord(8, 1)] = 18;
    orthogonalSums[Coord(8, 6)] = 7;
}

// ' represents odd summed squares and " even summed squares for orientation clarity
void Grid::printBoard() {
    string temp;
    for (int y = 0; y < board.size(); ++y) {
        for (int x = 0; x < board[y].size(); ++x) {
            if (orthogonalSums.find(Coord(y, x)) != orthogonalSums.end()) {
                if (orthogonalSums[Coord(y, x)] % 2 == 0) {
                    temp += "\" ";
                } else {
                    temp += "' ";
                }
            } else {
                temp += to_string(board[y][x]) + " ";
            }
        }
        temp += "\n";
    }
    cout << temp << endl;
}
