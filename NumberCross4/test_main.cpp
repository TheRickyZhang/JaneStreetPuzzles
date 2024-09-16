#include <gtest/gtest.h>
#include <iostream>

#include "Grid.h"

class ConditionTests : public ::testing::Test {
protected:
    NumberGrid testGrid = NumberGrid(11);
};

TEST_F(ConditionTests, CheckGroupInitialization) {
    unordered_map<Coord, int> allCoords;

    for (int y = 0; y < testGrid.grid.size(); y++) {
        for (int x = 0; x < testGrid.grid[0].size(); x++) {
            allCoords[Coord(y, x)] = 1;
        }
    }
    for(auto & groupNumToCoord : testGrid.groupNumToCoords) {
        for(auto& c : groupNumToCoord.second) {
            allCoords[c]--;
        }
    }
    for(auto& pair : allCoords) {
        cout << pair.first.y << ", " << pair.first.x << endl;
        ASSERT_EQ(pair.second, 0);
    }
}

TEST_F(ConditionTests, CheckRow0) {
    testGrid.grid[0] = {1, 6, -1, 2, 5, -1, 2, 2, 5, -1, -1};
    ASSERT_EQ(testGrid.functionMap[0](), true);
    testGrid.grid[0] = {1, 6, -1, 2, 5, -1, 2, 2, 4, -1, -1};
    ASSERT_EQ(testGrid.functionMap[0](), false);
    testGrid.grid[0] = {3, 8, 8, 0, 9, -1, 3, 8, 8, 0, 9};
    ASSERT_EQ(testGrid.functionMap[0](), true);
    testGrid.grid[0] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[0](), true);
    testGrid.grid[0] = {9, 9, 8, 8, 0, 0, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[0](), false);
}

TEST_F(ConditionTests, CheckRow1) {
    testGrid.grid[1] = {1, 2, -1, 2, 3, -1, 2, 2, 3, -1, -1};
    ASSERT_EQ(testGrid.functionMap[1](), true);
    testGrid.grid[1] = {1, 2, -1, 2, 5, -1, 2, 2, 3, -1, -1};
    ASSERT_EQ(testGrid.functionMap[1](), false);
    testGrid.grid[1] = {1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 2};
    ASSERT_EQ(testGrid.functionMap[1](), true);
    testGrid.grid[1] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[1](), true);
    testGrid.grid[1] = {9, 9, 8, 8, 9, 9, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[1](), false);
}


TEST_F(ConditionTests, CheckRow2) {
    testGrid.grid[2] = {-1, 2, 1, 8, 7, -1, 2, 0, 4, 8, -1};
    ASSERT_EQ(testGrid.functionMap[2](), true);
    testGrid.grid[2] = {2, 4, 3, -1, -1, 1, 3, 3, 2, -1};
    ASSERT_EQ(testGrid.functionMap[2](), false);
    testGrid.grid[2] = {9, 4, 9, 3, 1, 8, 7, 7, 1, 3, 3};
    ASSERT_EQ(testGrid.functionMap[2](), true);
    testGrid.grid[2] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[2](), true);
    testGrid.grid[2] = {-1, 1, 9, 4, 8, 7, 1, 7, 2, -1, -1};
    ASSERT_EQ(testGrid.functionMap[2](), false);
}

TEST_F(ConditionTests, CheckRow3) {
    testGrid.grid[3] = {-1, 2, 1, 2, 2, -1, 2, 0, 4, 1, -1};
    ASSERT_EQ(testGrid.functionMap[3](), true);
    testGrid.grid[3] = {2, 4, 1, -1, -1, 1, 3, 3, 7, -1};
    ASSERT_EQ(testGrid.functionMap[3](), false);
    testGrid.grid[3] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1};
    ASSERT_EQ(testGrid.functionMap[3](), true);
    testGrid.grid[3] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[3](), true);
    testGrid.grid[3] = {-1, 1, 3, 4, 8, 7, 1, 7, 2, -1, -1};
    ASSERT_EQ(testGrid.functionMap[3](), false);
}

TEST_F(ConditionTests, CheckRow4) {
    testGrid.grid[4] = {-1, 1, 5, 9, 7, -1, 2, 5, 8, 4, -1};
    ASSERT_EQ(testGrid.functionMap[4](), true);
    testGrid.grid[4] = {2, 4, 1, -1, -1, 1, 3, 3, 7, -1};
    ASSERT_EQ(testGrid.functionMap[4](), false);
    testGrid.grid[4] = {1, 2, 5, 8, 6, 2, 6, 9, 0, 2, 5};
    ASSERT_EQ(testGrid.functionMap[4](), true);
    testGrid.grid[4] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[4](), true);
    testGrid.grid[4] = {-1, 3, 9, 0, 8, 8, 1, 6, 8, -1, -1};
    ASSERT_EQ(testGrid.functionMap[4](), false);
}

TEST_F(ConditionTests, CheckRow5) {
    testGrid.grid[5] = {1, 6, -1, 2, 5, -1, 2, 2, 5, -1, -1};
    ASSERT_EQ(testGrid.functionMap[5](), true);
    testGrid.grid[5] = {1, 6, -1, 2, 5, -1, 2, 2, 4, -1, -1};
    ASSERT_EQ(testGrid.functionMap[5](), false);
    testGrid.grid[5] = {3, 8, 8, 0, 9, -1, 3, 8, 8, 0, 9};
    ASSERT_EQ(testGrid.functionMap[5](), true);
    testGrid.grid[5] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[5](), true);
    testGrid.grid[5] = {9, 9, 8, 8, 0, 0, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[5](), false);
}

TEST_F(ConditionTests, CheckRow6) {
    testGrid.grid[6] = {3, 7, -1, 7, 4, -1, 4, 0, 7, -1, -1};
    ASSERT_EQ(testGrid.functionMap[6](), true);
    testGrid.grid[6] = {7, 4, -1, 3, 7, -1, 1, 0, 0, -1, -1};
    ASSERT_EQ(testGrid.functionMap[6](), false);
    testGrid.grid[6] = {1, 8, 2, 0, 4, -1, 1, 8, 2, 0, 4};
    ASSERT_EQ(testGrid.functionMap[6](), true);
    testGrid.grid[6] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[6](), true);
    testGrid.grid[6] = {3, 0, 5, 6, 5, 7, 0, 0, 0, 0, 1};
    ASSERT_EQ(testGrid.functionMap[6](), false);
}

TEST_F(ConditionTests, CheckRow7) {
    testGrid.grid[7] = {1, 6, 1,-1, -1, -1, 1, 6, 1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[7](), true);
    testGrid.grid[7] = {1, 1, -1, 4, 6, -1, 2, 7, 2, -1, -1};
    ASSERT_EQ(testGrid.functionMap[7](), false);
    testGrid.grid[7] = {-1, 1, 6, 1, 1, 6, 1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[7](), true);
    testGrid.grid[7] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[7](), true);
    testGrid.grid[7] = {2, 6, 7, 7, 6, 2, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[7](), false);
}

TEST_F(ConditionTests, CheckRow8) {
    testGrid.grid[8] = {1, 1, 1, 1, 1, 1, 1, -1, 1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[8](), true);
    testGrid.grid[8] = {1, 1, -1, 4, 6, -1, 3, 7, 1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[8](), false);
    testGrid.grid[8] = {-1, 1, 9, 1, 1, 9, 1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[8](), true);
    testGrid.grid[8] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[8](), true);
    testGrid.grid[8] = {1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[8](), false);
}

TEST_F(ConditionTests, CheckRow9) {
    testGrid.grid[9] = {8, 8, 8, 8, 8, 8, 0, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[9](), true);
    testGrid.grid[9] = {8, 8, -1, 8, 8, -1, 3, 7, 1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[9](), false);
    testGrid.grid[9] = {-1, 1, 5, 7, 5, 2, 0, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[9](), true);
    testGrid.grid[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(testGrid.functionMap[9](), true);
    testGrid.grid[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1};
    ASSERT_EQ(testGrid.functionMap[9](), false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

