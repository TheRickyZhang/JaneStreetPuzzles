#ifndef HALLOFMIRRORS_REFLECTION_H
#define HALLOFMIRRORS_REFLECTION_H

#include <cmath>
#include <optional>
#include <stdexcept>
#include <iostream>
#include "Pixels.h"

// Enum for three-state logic to accommodate horizontal or vertical movement
enum struct MovementState { True, False, Nan };

// Structure to retain the state of a line over drawing iterations
struct LineDrawingState {
    int currentX;
    int lastY;
    bool isDrawing;
    int cooldown;  //Iterations until collisions are allowed again to prevent immediate consecutive collisions
    MovementState movingRight;
    MovementState movingUp;
    LineDrawingState(int cx, int ly, MovementState mr, MovementState mu) : currentX(cx), isDrawing(true), cooldown(0), movingRight(mr), movingUp(mu), lastY(ly) {}
};

//Line in normal xy-plane with +x->right, +y->down (flipped y due to image, but all calculations consistent)
struct Line {
    double slope;
    double intercept; // Y-intercept, Nan if infinite slope
    std::vector<int> operator()(int x, int lastY, LineDrawingState state) const; // Generates y-values for an x-value
    Line(double _slope, Coord point) : slope(_slope),
    intercept(_slope == std::numeric_limits<double>::infinity() ? std::numeric_limits<double>::quiet_NaN() : point.y - _slope * point.x) {}
};

// Helping functions for angle and slope calculations
double calculateAngle(double slope, const LineDrawingState& s);
double getNewAngle(Line line, Pixel p, LineDrawingState s);
double getNewSlope(Line line, Pixel p, LineDrawingState s);

// Function to draw a line, detect collisions, and update state variables with each x value shift
void drawLineAndDetectCollision(std::vector<std::vector<Pixel>>& image, Line& line, const Pixel& lineColor,
                                const std::vector<PixelGroup>& groups, LineDrawingState& state);

#endif //HALLOFMIRRORS_REFLECTION_H
