#include <gtest/gtest.h>
#include "Pentomino.h"
#include "Grid.h"

class PentominoTest : public ::testing::Test {
protected:
    Grid testGrid = Grid(17, 17);
};

TEST_F(PentominoTest, InitializeCorrectFShape) {
    Pentomino p(f, 1);
    vector<vector<int>> expectedShape = {
            {0, 1, 1},
            {1, 1, 0},
            {0, 1, 0}
    };
    ASSERT_EQ(p.shape, expectedShape);
}

TEST_F(PentominoTest, ScalingUpDuplicatesElements) {
    Pentomino p(f, 2);
    vector<vector<int>> expectedShape = {
            {0, 0, 2, 2, 2, 2},
            {0, 0, 2, 2, 2, 2},
            {2, 2, 2, 2, 0, 0},
            {2, 2, 2, 2, 0, 0},
            {0, 0, 2, 2, 0, 0},
            {0, 0, 2, 2, 0, 0}
    };
    ASSERT_EQ(p.shape, expectedShape);
}

TEST_F(PentominoTest, ScalingUpTriplicatesElements) {
    Pentomino p(fr90, 3);
    vector<vector<int>> expectedShape = {
            {0, 0, 0, 0, 0, 0, 3, 3, 3},
            {0, 0, 0, 0, 0, 0, 3, 3, 3},
            {0, 0, 0, 0, 0, 0, 3, 3, 3},
            {3, 3, 3, 3, 3, 3, 3, 3, 3},
            {3, 3, 3, 3, 3, 3, 3, 3, 3},
            {3, 3, 3, 3, 3, 3, 3, 3, 3},
            {0, 0, 0, 3, 3, 3, 0, 0, 0},
            {0, 0, 0, 3, 3, 3, 0, 0, 0},
            {0, 0, 0, 3, 3, 3, 0, 0, 0}
    };
    ASSERT_EQ(p.shape, expectedShape);
}

//Check for any missing or duplicate coordinates created with initializeGroup()
TEST_F(PentominoTest, GroupCoverage) {
    Grid grid(17, 17);
    vector<vector<int>> grid_coverage(17, vector<int>(17, 0));

    for (const Group& g : grid.groups) {
        for (const Coord& c : g.cells) {
            grid_coverage[c.y][c.x] += 1;
        }
    }

    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 17; x++) {
            EXPECT_LE(grid_coverage[y][x], 1) << "Duplicate coordinate found at: " << Coord(y, x);
            EXPECT_GE(grid_coverage[y][x], 1) << "Missing coordinate at: " << Coord(y, x);
        }
    }
}

TEST_F(PentominoTest, PlacePentominoUpdatesGridCorrectly) {
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 1), 1, 1));
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::f180, 1), 14, 14));
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::fr, 2), 10, 0));

    vector<vector<int>> expectedGrid1 = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
            {0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0}
    };
    ASSERT_EQ(expectedGrid1, testGrid.grid);
}

TEST_F(PentominoTest, placePentominoFailsOnOutOfBounds) {
    ASSERT_FALSE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 1), -1, 0));
    ASSERT_FALSE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 1), 15, 0));
    ASSERT_FALSE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 3), 4, 9));
}

TEST_F(PentominoTest, placePentominoFailsOnOverlap){
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 1), 1, 1));
    ASSERT_FALSE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 1), 0, 0));

    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 2), 6, 6));
    ASSERT_FALSE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 2), 9, 6));
}

TEST_F(PentominoTest, ColumnSumCorrectlyUpdates) {
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::fr, 1), 0, 0));
    ASSERT_EQ(testGrid.colSums[0], 1);
    ASSERT_EQ(testGrid.colSums[1], 3);
    ASSERT_EQ(testGrid.colSums[2], 1);
    ASSERT_EQ(testGrid.colSums[3], 0);
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::fr, 2), 2, 2));
    ASSERT_EQ(testGrid.colSums[1], 3);
    ASSERT_EQ(testGrid.colSums[2], 5);
    ASSERT_EQ(testGrid.colSums[3], 4);
    ASSERT_EQ(testGrid.colSums[5], 12);
}

TEST_F(PentominoTest, RowSumCorrectlyUpdates) {
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::fr, 1), 0, 0));
    ASSERT_EQ(testGrid.rowSums[0], 2);
    ASSERT_EQ(testGrid.rowSums[1], 2);
    ASSERT_EQ(testGrid.rowSums[2], 1);
    ASSERT_EQ(testGrid.rowSums[3], 0);
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::fr, 2), 2, 2));
    ASSERT_EQ(testGrid.rowSums[1], 2);
    ASSERT_EQ(testGrid.rowSums[2], 9);
    ASSERT_EQ(testGrid.rowSums[3], 8);
    ASSERT_EQ(testGrid.rowSums[5], 8);
}

TEST_F(PentominoTest, GroupSumCorrectlyUpdates) {
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::fr, 1), 0, 0));
    ASSERT_EQ(testGrid.groupSums[0], 4);
    ASSERT_EQ(testGrid.groupSums[1], 1);
    ASSERT_EQ(testGrid.groupSums[2], 0);
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::fr, 2), 2, 2));
    ASSERT_EQ(testGrid.groupSums[0], 4);
    ASSERT_EQ(testGrid.groupSums[2], 10);
    ASSERT_EQ(testGrid.groupSums[7], 10);
}

TEST_F(PentominoTest, placeAdjacentPentominos){
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::fr, 2), 2, 2));
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::fr, 1), 0, 0));
    vector<vector<int>> expectedGrid2 = {
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    ASSERT_EQ(testGrid.grid, expectedGrid2);
}

TEST_F(PentominoTest, removePentominoSucceeds){
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 1), 5, 5));
    ASSERT_TRUE(testGrid.removePentomino(Pentomino(PentominoTypes::f, 1), 5, 5));
    vector<vector<int>> expectedGrid1 = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    ASSERT_EQ(testGrid.grid, expectedGrid1);
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 2), 6, 8));
    ASSERT_TRUE(testGrid.removePentomino(Pentomino(PentominoTypes::f, 2), 6, 8));
    ASSERT_EQ(testGrid.grid, expectedGrid1);
}

TEST_F(PentominoTest, removePentominoFails){
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 1), 5, 5));
    ASSERT_FALSE(testGrid.removePentomino(Pentomino(PentominoTypes::f, 1), 4, 5));
    ASSERT_TRUE(testGrid.placePentomino(Pentomino(PentominoTypes::f, 2), 6, 8));
    ASSERT_FALSE(testGrid.removePentomino(Pentomino(PentominoTypes::f, 2), 7, 8));
    vector<vector<int>> expectedGrid2 = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    ASSERT_EQ(testGrid.grid, expectedGrid2);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}