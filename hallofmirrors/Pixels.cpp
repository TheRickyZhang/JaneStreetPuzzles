#include "Pixels.h"
#include <iostream>

double Pixel::getGroupSlope() const {
    return (parentGroup->slope != std::numeric_limits<double>::quiet_NaN()) ? parentGroup->slope : std::numeric_limits<double>::quiet_NaN();
}

// Uses linear regression on points to calculate and assign the slope for a pixel group
void PixelGroup::calculateSlope() {
    if (pixels.empty()) {
        slope = std::numeric_limits<double>::quiet_NaN();
        throw std::runtime_error("NaN group slope");
    }

    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::min();
    unsigned int n = pixels.size();

    // Iterate to calculate relevant sums for x and y, and max/min for x
    for (const auto& p : pixels) {
        sumX += p.c.x;
        sumY += p.c.y;
        sumXY += p.c.x * p.c.y;
        sumX2 += p.c.x * p.c.x;
        if (p.c.x < minX) minX = p.c.x;
        if (p.c.x > maxX) maxX = p.c.x;
    }

    // SET line width threshold for thickness of vertical line, preventing erroneous slope of 0
    const double lineWidthThreshold = 8;
    if (maxX - minX <= lineWidthThreshold) {
        slope = std::numeric_limits<double>::infinity();
        return;
    }

    double denominator = n * sumX2 - sumX * sumX;
    slope = (n * sumXY - sumX * sumY) / denominator;

    // SET slopeValues if common slope values are known to clamp floating point errors
    double slopeValues[] = {0, 0.414213, 1, 2.414213, -0.414213, -1, -2.414213};
    for(double s : slopeValues) {
        if (slope > s - 0.2 && slope < s + 0.2) {
            slope = s;
            break;
        }
    }
    // SET threshold for infinite slope
    if(std::abs(slope) > 50) {
        slope = std::numeric_limits<double>::infinity();
    }
}


void addPixelToGroup(int x, int y, int width, int height, const Pixel& targetColor, std::vector<std::vector<Pixel>>& pixelData, PixelGroup& group) {
    // Implements flood fill algorithm with coordinates on a stack to prevent stack overflow
    std::stack<Coord> pixelStack;
    pixelStack.emplace(x, y);

    while (!pixelStack.empty()) {
        Coord current = pixelStack.top();
        pixelStack.pop();

        int cx = current.x;
        int cy = current.y;

        // Flood fill only proceeds if pixel is in bounds, not included in a group, and matches the target color
        if (cx < 0 || cy < 0 || cx >= width || cy >= height || pixelData[cy][cx].isIncluded || !(pixelData[cy][cx] == targetColor)) {
            continue;
        }

        // Mark the pixel as included in a group and add it to the group
        pixelData[cy][cx].isIncluded = true;
        pixelData[cy][cx].parentGroup = &group;
        group.pixels.emplace_back(pixelData[cy][cx]);

        // Recursive calls for subsequent processing
        pixelStack.emplace(cx + 1, cy);
        pixelStack.emplace(cx - 1, cy);
        pixelStack.emplace(cx, cy + 1);
        pixelStack.emplace(cx, cy - 1);
    }
}