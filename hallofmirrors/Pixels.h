#ifndef HALLOFMIRRORS_PIXELS_H
#define HALLOFMIRRORS_PIXELS_H

#include <vector>
#include <optional>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <stack>

// Basic coordinate struct
struct Coord {
    int x, y;
    Coord() : x(0), y(0) {}
    Coord(int _x, int _y) : x(_x), y(_y) {};
    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }
};

struct PixelGroup;

// Basic pixel struct with rgb values, coordinate, and a parent group
struct Pixel {
    uint8_t r, g, b;
    Coord c;
    bool isIncluded;   // True if included in any PixelGroup->coordinates
    PixelGroup* parentGroup;  // Pointer to parent PixelGroup

    Pixel() : r(0), g(0), b(255), c(Coord()), isIncluded(false), parentGroup(nullptr) {}
    Pixel(uint8_t _r, uint8_t _g, uint8_t _b, int _x, int _y) : r(_r), g(_g), b(_b), c(Coord(_x, _y)), isIncluded(false), parentGroup(nullptr) {}

    //SET color difference tolerance (acts as approximately equal to)
    bool operator==(const Pixel &other) const {
        return (std::abs(r - other.r) <= 25) && std::abs(g - other.g) <= 100 && std::abs(b - other.b) <= 120;
    }
    [[nodiscard]] double getGroupSlope() const;
};

// Struct containing a vector of pixels and a slope for the entire group (mirror)
struct PixelGroup {
    std::vector<Pixel> pixels;
    double slope;
    PixelGroup() : pixels(), slope(std::numeric_limits<double>::quiet_NaN()){}
    void calculateSlope(); // Calculates and sets the value of groupSlope for all Pixels
};

// Recursively adds a pixel and all adjacent pixels to a specified group
void addPixelToGroup(int x, int y, int width, int height, const Pixel& targetColor, std::vector<std::vector<Pixel>>& pixelData, PixelGroup& group);

#endif //HALLOFMIRRORS_PIXELS_H
