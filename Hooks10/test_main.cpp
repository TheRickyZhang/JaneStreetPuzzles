#include <gtest/gtest.h>
#include "Hook.h"
#include "Grid.h"

bool compareVectors(const vector<Coord>& vec1, const vector<Coord>& vec2) {
    if (vec1.size() != vec2.size()) return false;
    vector<Coord> sortedVec1 = vec1;
    vector<Coord> sortedVec2 = vec2;

    sort(sortedVec1.begin(), sortedVec1.end(), [](const Coord& a, const Coord& b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
    });
    sort(sortedVec2.begin(), sortedVec2.end(), [](const Coord& a, const Coord& b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
    });

    return sortedVec1 == sortedVec2;
}

class HookTest : public ::testing::Test {
protected:
    Grid testGrid = Grid(9);
};

TEST_F(HookTest, CorrectHookShapeAndOrientation) {
    Hook hookL0(3, HookOrientation::L0, Coord(0, 0));
    vector<Coord> expectedL0 = {{0, 0},
                                {0, 1},
                                {0, 2},
                                {1, 0},
                                {2, 0}};
    ASSERT_TRUE(compareVectors(hookL0.hook, expectedL0));

    Hook hookL90(3, HookOrientation::L90, Coord(0, 2));
    vector<Coord> expectedL90 = {{0, 2},
                                 {0, 3},
                                 {0, 4},
                                 {1, 4},
                                 {2, 4}};
    ASSERT_TRUE(compareVectors(hookL90.hook, expectedL90));

    Hook hookL180(3, HookOrientation::L180, Coord(2, 2));
    vector<Coord> expectedL180 = {{4, 4},
                                  {4, 3},
                                  {4, 2},
                                  {3, 4},
                                  {2, 4}};
    ASSERT_TRUE(compareVectors(hookL180.hook, expectedL180));

    Hook hookL270(3, HookOrientation::L270, Coord(2, 0));
    vector<Coord> expectedL270 = {{2, 0},
                                  {3, 0},
                                  {4, 0},
                                  {4, 1},
                                  {4, 2}};
    ASSERT_TRUE(compareVectors(hookL270.hook, expectedL270));
}

TEST_F(HookTest, GenerateHooksCorrectly) {
    testGrid.generateHooks(Hook(9, HookOrientation::L0, Coord(0, 0)), 1);
    ASSERT_EQ(testGrid.possibleHooks.size(), 15);
    testGrid.generateHooks(Hook(9, HookOrientation::L90, Coord(0, 0)), 2);
    ASSERT_EQ(testGrid.possibleHooks.size(), 105);
    testGrid.generateHooks(Hook(9, HookOrientation::L180, Coord(0, 0)), 3);
    ASSERT_EQ(testGrid.possibleHooks.size(), 455);
    testGrid.generateHooks(Hook(8, HookOrientation::L0, Coord(1, 1)), 1);
    ASSERT_EQ(testGrid.possibleHooks.size(), 11);
    testGrid.generateHooks(Hook(8, HookOrientation::L180, Coord(1, 1)), 2);
    ASSERT_EQ(testGrid.possibleHooks.size(), 78);
    testGrid.generateHooks(Hook(3, HookOrientation::L0, Coord(6, 6)), 3);
    ASSERT_EQ(testGrid.possibleHooks.size(), 1);
    testGrid.generateHooks(Hook(1, HookOrientation::L0, Coord(8, 8)), 1);
    ASSERT_EQ(testGrid.possibleHooks.size(), 1);
    testGrid.generateHooks(Hook(1, HookOrientation::L0, Coord(8, 8)), 6);
    ASSERT_EQ(testGrid.possibleHooks.size(), 0);
}

TEST_F(HookTest, CheckPlaceHook1) {
    Hook tempHook1 = Hook(8, HookOrientation::L0, Coord(1, 1));
    testGrid.generateHooks(tempHook1, 4);
    vector<Coord> hook1 = {Coord(1, 1), Coord(1, 2), Coord(1, 3), Coord(3, 1)};
    auto it = find_if(testGrid.possibleHooks.begin(), testGrid.possibleHooks.end(),
                      [&hook1](const vector<Coord>& vec) {
                          return compareVectors(vec, hook1);
                      });

    ASSERT_NE(it, testGrid.possibleHooks.end());
    testGrid.placeHook(hook1, tempHook1);
    testGrid.numsToUse[4] = true;
    vector<vector<int>> board = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 4, 4, 4, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 4, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    ASSERT_EQ(testGrid.board, board);
    ASSERT_EQ(testGrid.currentSums[Coord(0, 2)], 4);
    ASSERT_EQ(testGrid.currentSums[Coord(1, 4)], 4);
    ASSERT_EQ(testGrid.currentSums[Coord(2, 1)], 8);
    ASSERT_EQ(testGrid.currentSums[Coord(4, 1)], 4);
    ASSERT_EQ(testGrid.findMaxPossibleSum(Coord(0, 2)), 18);
    ASSERT_EQ(testGrid.findMaxPossibleSum(Coord(2, 1)), 18);
    ASSERT_EQ(testGrid.findMaxPossibleSum(Coord(4, 1)), 18);

    Hook tempHook2 = Hook(7, HookOrientation::L0, Coord(2, 2));
    testGrid.generateHooks(tempHook2, 2);
    vector<Coord> hook2 = {Coord(4, 2), Coord(5, 2)};
    it = find_if(testGrid.possibleHooks.begin(), testGrid.possibleHooks.end(),
                 [&hook2](const vector<Coord>& vec) {
                     return compareVectors(vec, hook2);
                 });
    ASSERT_NE(it, testGrid.possibleHooks.end());

    testGrid.placeHook(hook2, tempHook2);
    ASSERT_EQ(testGrid.board, board);
    ASSERT_EQ(testGrid.currentSums[Coord(0, 2)], 4);
    ASSERT_EQ(testGrid.currentSums[Coord(1, 4)], 4);
    ASSERT_EQ(testGrid.currentSums[Coord(2, 1)], 8);
    ASSERT_EQ(testGrid.currentSums[Coord(4, 1)], 4);
    ASSERT_EQ(testGrid.currentSums[Coord(4, 3)], 0);

    Hook tempHook3 = Hook(9, HookOrientation::L0, Coord(0, 0));
    testGrid.generateHooks(tempHook3, 2);
    vector<Coord> hook3 = {Coord(0, 1), Coord(0, 3)};
    it = find_if(testGrid.possibleHooks.begin(), testGrid.possibleHooks.end(),
                 [&hook3](const vector<Coord>& vec) {
                     return compareVectors(vec, hook3);
                 });
    ASSERT_NE(it, testGrid.possibleHooks.end());

    testGrid.placeHook(hook3, tempHook3);
    ASSERT_EQ(testGrid.board, board);

    Hook tempHook4 = Hook(5, HookOrientation::L0, Coord(4, 4));
    testGrid.generateHooks(tempHook4, 5);
    vector<Coord> hook4 = {Coord(5, 4), Coord(6, 4), Coord(8, 4), Coord(4, 6), Coord(4, 8)};
    it = find_if(testGrid.possibleHooks.begin(), testGrid.possibleHooks.end(),
                 [&hook4](const vector<Coord>& vec) {
                     return compareVectors(vec, hook4);
                 });
    ASSERT_NE(it, testGrid.possibleHooks.end());

    if (testGrid.placeHook(hook4, tempHook4)) {
        testGrid.numsToUse[6] = true;
    }
    board = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 4, 4, 4, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 4, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 5, 0, 5},
            {0, 0, 0, 0, 5, 0, 0, 0, 0},
            {0, 0, 0, 0, 5, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 5, 0, 0, 0, 0}
    };
    ASSERT_EQ(testGrid.board, board);
    ASSERT_EQ(testGrid.findMaxPossibleSum(Coord(6, 7)), 34);
    ASSERT_EQ(testGrid.findMaxPossibleSum(Coord(4, 5)), 18);
    ASSERT_EQ(testGrid.findMaxPossibleSum(Coord(7, 4)), 18);
}