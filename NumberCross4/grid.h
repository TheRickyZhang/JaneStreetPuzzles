#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <limits>
#include <cmath>
#include <stack>
#include <ranges>

using namespace std;

struct Coord {
    int y;
    int x;
    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }
    Coord(int _y, int _x) {
        y = _y;
        x = _x;
    }
};

namespace std {
    template<> struct hash<Coord> {
        int operator()(const Coord& coord) const noexcept {
            int h1 = int(hash<int>{}(coord.y));
            int h2 = int(hash<int>{}(coord.x));
            return h1 ^ (h2 << 1);
        }
    };
}

struct NumberGrid {
    vector<vector<int>> grid;
    unordered_map<int, function<bool()>> functionMap;
    unordered_map<int, vector<Coord>> groupNumToCoords;
    unordered_map<Coord, int> coordToGroupNum;
    int size;

    explicit NumberGrid(int _size) : size(_size), grid(_size, vector<int>(_size, -2)) {
        initializeClues();
        initializeGroups();
    };
    vector<long long> separateIntoDecimals(const vector<int>& row);
    vector<vector<int>> separateIntoDigits(const vector<int>& row);
    static bool validateNumbers(const vector<long long>& nums, const function<bool(long long)>& condition);
    static bool validateDigits(const vector<vector<int>>& nums, const function<bool(vector<int>)>& condition);

    void initializeClues();
    void initializeGroups();

    static bool isPerfectSquare(long long n);
    static bool isPalindrome(long long n);
    static bool isPrimePowerPrime(long long n);
    static bool isDigitSum7(const vector<int>& digits);
    static bool isFibonacci(long long n);
    static bool isMultipleOfNum(long long n, int num);
    static bool isProductOfDigitsEnd1(const vector<int>& digits);
};