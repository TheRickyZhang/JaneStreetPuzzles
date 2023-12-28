#include <gtest/gtest.h>
#include "C:/Users/ricky/Projects/JaneStreet/HallOfMirrors/Pixels.h"
#include "C:/Users/ricky/Projects/JaneStreet/HallOfMirrors/Reflection.h"
#include "C:/Users/ricky/Projects/JaneStreet/HallOfMirrors/stb_image.h"
#include "C:/Users/ricky/Projects/JaneStreet/HallOfMirrors/main.cpp"

TEST(LineTest, GeneratesCorrectCoordinates) {
    Line line1 = Line(0, Coord(0, 12)); // Line with slope 0 and intercept 12
    auto coords1 = line1(0, 12); // Call the function
    ASSERT_EQ(coords1.size(), 1); // Check the size of the output vector
    EXPECT_EQ(coords1[0], 12); // Check the first value in the vector

    Line line2 = Line(0.42, Coord(0, 12));
    auto coords2 = line2(10, -99);
    EXPECT_EQ(coords2.size(), 1);
    EXPECT_EQ(coords2[0], 4);
}

TEST(CalculateAngleTest, ReturnsCorrectAngle) {
    double slope = 1.0;
    double angle = calculateAngle(slope);
    EXPECT_NEAR(angle, M_PI/4, 0.001); // Check if angle is close to π/4
}

TEST(GetNewAngleTest, CalculatesCorrectAngle) {
    Line line1 = Line(0, Coord(0, 0));
    Pixel p; // Set up pixel with appropriate group slope
    p.parentGroup->slope = M_PI/8;
    double newAngle = getNewAngle(line1, p);
    EXPECT_NEAR(newAngle, 1, 0.001); // Check if new angle is as expected
}

TEST(DrawLineAndDetectCollisionTest, DeterminesDirectionCorrectly) {
    int width, height, channels;
    unsigned char* data = stbi_load("mirrors.png", &width, &height, &channels, 3);

    std::vector<std::vector<Pixel>> pixelData(height, std::vector<Pixel>(width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * 3;
            pixelData[y][x] = Pixel(data[idx], data[idx + 1], data[idx + 2], x, y);
        }
    }

    std::vector<PixelGroup> groups;
    groups.reserve(100);  //Reserve memory for number of mirrors to prevent memory errors
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            //Find all mirror groups, add them to a vector of PixelGroups, then set the slope for each group
            if (pixelData[y][x] == Orange && !pixelData[y][x].isIncluded) {
                groups.emplace_back();
                PixelGroup& newGroup = groups.back();
                addPixelToGroup(x, y, width, height, Orange, pixelData, newGroup);
                newGroup.calculateSlope();
            }
        }
    }
    Line line = Line(0, Coord(230, 245));
    Pixel lineColor(0, 0, 255, -1, -1);
    LineDrawingState state = LineDrawingState(230, true, 245);

    drawLineAndDetectCollision(pixelData, line, lineColor, groups, state);
    // Check specific pixels in the image for changes
    EXPECT_EQ(state.currentX, 231);
    EXPECT_EQ(state.movingRight, expected_x);
}


