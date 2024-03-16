#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

const int boardSize = 9;

// Standard coordinate class
struct Coord {
    int y, x;

    Coord(int y, int x) : y(y), x(x) {}

    [[nodiscard]] int sum() const {
        return y + x;
    }

    bool operator==(const Coord& other) const {
        return y == other.y && x == other.x;
    }

    bool operator<(const Coord& other) const {
        return y == other.y ? x < other.x : y < other.y;
    }
};

// Hash function for coordinates
namespace std {
    template<>
    struct hash<Coord> {
        size_t operator()(const Coord& c) const {
            const size_t prime = 31;
            size_t result = prime + hash<int>()(c.y);
            result = prime * result + hash<int>()(c.x);
            return result;
        }
    };
}

// Counts empty neighbors using a boolean for location, which is needed for maxPossibleSum calculation
// [0] is +y, [1] is -y, [2] is +x, [3] is -x
struct neighborStates {
    vector<bool> states;

    neighborStates() : states(4, false) {}

    [[nodiscard]] int ySum() const { return states[0] + states[1]; }

    [[nodiscard]] int xSum() const { return states[2] + states[3]; }

    [[nodiscard]] int total() const { return states[0] + states[1] + states[2] + states[3]; }

    [[nodiscard]] pair<int, int> hookSums() const {
        int hookSum1 = states[0] + states[2];
        int hookSum2 = states[1] + states[3];
        return hookSum1 > hookSum2 ? make_pair(hookSum1, hookSum2) : make_pair(hookSum2, hookSum1);
    }
};

enum class HookOrientation {
    L0, L90, L180, L270
};

// Represents possible coordinates for each hook
struct Hook {
    vector<Coord> hook;
    int size;
    HookOrientation orientation;
    Coord bottomLeft;

    explicit Hook(int size, HookOrientation orientation, Coord bottomLeft)
            : size(size), orientation(orientation), bottomLeft(bottomLeft) {
        if (size < 0 || size > boardSize) {
            throw runtime_error("Hook size out of bounds");
        }

        // bottomLeft is always the point with smallest x and y in the "box" the hook resides in
        if (orientation == HookOrientation::L0) {
            int newY = bottomLeft.y, newX = bottomLeft.x;
            for (int i = 0; i < size; ++i) {
                hook.emplace_back(newY + i, newX);
            }
            for (int i = 1; i < size; ++i) {
                hook.emplace_back(newY, newX + i);
            }
        } else if (orientation == HookOrientation::L90) {
            int newY = bottomLeft.y, newX = bottomLeft.x + size - 1;
            for (int i = 0; i < size; ++i) {
                hook.emplace_back(newY + i, newX);
            }
            for (int i = 1; i < size; ++i) {
                hook.emplace_back(newY, newX - i);
            }
        } else if (orientation == HookOrientation::L180) {
            int newY = bottomLeft.y + size - 1, newX = bottomLeft.x + size - 1;
            for (int i = 0; i < size; ++i) {
                hook.emplace_back(newY - i, newX);
            }
            for (int i = 1; i < size; ++i) {
                hook.emplace_back(newY, newX - i);
            }
        } else if (orientation == HookOrientation::L270) {
            int newY = bottomLeft.y + size - 1, newX = bottomLeft.x;
            for (int i = 0; i < size; ++i) {
                hook.emplace_back(newY - i, newX);
            }
            for (int i = 1; i < size; ++i) {
                hook.emplace_back(newY, newX + i);
            }
        }
    }

    bool operator==(const Hook& h) const { return hook == h.hook; }

    bool operator<(const Hook& h) const {
        return this->size < h.size;
    }
};
