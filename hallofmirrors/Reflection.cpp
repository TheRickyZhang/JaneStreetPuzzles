#include "Reflection.h"
#include <iostream>

//SET tolerance used for floating point calculations
const double epsilon = 1e-2;

// Function to return the next y-values in correct order traversed by a line
// for a given x and last visited y, taking into account the line's slope and movement state
std::vector<int> Line::operator()(int x, int lastY, LineDrawingState state) const {
    std::vector<int> coords;
    // If infinite slope, generate all y until a certain limit
    if (std::isinf(slope) && state.movingUp == MovementState::True) {
        for (int y = lastY; y <= 1600 && coords.size() < 1600; y += 1) {
            if (y >= 0 && y <= 1600) {
                coords.emplace_back(y);
            }
        }
        return coords;
    }
    else if (std::isinf(slope) && state.movingUp == MovementState::False) {
        for (int y = lastY; y >= 0 && coords.size() < 1600; y -= 1) {
            if (y >= 0 && y <= 1600) {
                coords.emplace_back(y);
            }
        }
        return coords;
    }

    // If not infinite slope, use slope and floor/ceiling rounding to approximate next y values
    double y_start = slope * x + intercept;
    double y_end = slope * (x + 1) + intercept;

    int y_start_rounded = static_cast<int>(std::round(y_start));
    int y_end_rounded = static_cast<int>(std::round(y_end));

    int y_min = std::min(y_start_rounded, y_end_rounded);
    int y_max = std::max(y_start_rounded, y_end_rounded);

    // Iterate based on the direction of the line's movement
    bool isMovingUp = state.movingUp == MovementState::True;
    if (isMovingUp) {
        for (int y = lastY; y <= y_max && coords.size() < 1600; ++y) {
            if (y >= 0 && y <= 1600) {
                coords.emplace_back(y);
            }
        }
    } else {
        for (int y = lastY; y >= y_min && coords.size() < 1600; --y) {
            if (y >= 0 && y <= 1600) {
                coords.emplace_back(y);
            }
        }
    }
    return coords;
}

/*
    Calculate angle based on slope, retaining direction based on line movement state
    Note: the line movement state also incorrectly applies to mirror slopes, but since the shift is by a magnitude
    of pi, and reflection angle is given by 2(mirror) - line, the resulting angle is the same modulo 2pi
*/
double calculateAngle(double slope, const LineDrawingState& s) {
    if (std::isnan(slope)) {
        throw std::runtime_error("Slope is unassigned");
    }
    if (std::isinf(slope)) {
        return s.movingUp == MovementState::True ? M_PI / 2 : -M_PI / 2;
    }
    double tempAngle = std::atan(slope);
    // Adjust angle by pi if line is going left
    if(s.movingRight == MovementState::False) {
        tempAngle = tempAngle > 0 ? tempAngle - M_PI : tempAngle + M_PI;
    }
    return tempAngle;
}

// Returns predicted post collision angle based on pre-collision state and angles
double getNewAngle(const Line line, const Pixel p, const LineDrawingState s) {
    //Formula for reflection angle derived from trigonometry and law of reflection (output = 2 mirror - line)
    double inputSlopeAngle = calculateAngle(p.getGroupSlope(), s);
    double inputLineAngle = calculateAngle(line.slope, s);
    double outputAngle = 2 * inputSlopeAngle - inputLineAngle;

    // Normalize angle to range (-2pi, 2pi)
    if(std::abs(outputAngle) > 2 * M_PI){
        outputAngle = outputAngle > 0 ? outputAngle - 2 * M_PI : outputAngle + 2 * M_PI;
    }
    return outputAngle;
}

// Calculates predicted post-collision slope
double getNewSlope(Line line, Pixel p, LineDrawingState s) {
    double x = 2 * calculateAngle(p.getGroupSlope(), s) - calculateAngle(line.slope, s);
    // Returns infinity for approximately vertical slope
    if((std::abs(x) - M_PI/2 < epsilon && std::abs(x) - M_PI/2 > -epsilon) || std::abs(std::tan(x)) > 99) {
        return std::numeric_limits<double>::infinity();
    }
    //SET slopeList if common slope values are known to clamp floating point errors
    double slopeList[] = {0, M_PI/8, -M_PI/8,1, -1};
    for(double d : slopeList) {
        if(std::abs(std::tan(x) - d) < epsilon && std::abs(std::tan(x) - d) > 0) {
            return d;
        }
    }
    return std::tan(x);
}

int mirrorCount = 0;

void drawLineAndDetectCollision(std::vector<std::vector<Pixel>>& image, Line& line, const Pixel& lineColor, const std::vector<PixelGroup>& groups, LineDrawingState& state) {
    const auto height = image.size();
    const auto width = image[0].size();
    const auto y_values = line(state.currentX, state.lastY, state);

    // Process all y-values for one x-value, knowing all y values are in order
    for (int i = 0; i < int(y_values.size()); i += 1) {
        int y = y_values[i];
        //Set lastY to current Y
        state.lastY = y;

        //SET: Vertical distance buffer for cooldown
        if(i > 25){state.cooldown = 0;}

        if (y >= 0 && y < height) {
            // Cooldown prevents searching for collisions if a collision recently occurred
            if(state.cooldown == 0) {
                for (const auto &group: groups) {
                    // Search all pixels in groups, and if current (x, y) matches pixel's (x, y), proceed with deflection
                    auto it = std::find_if(group.pixels.begin(), group.pixels.end(),
                                           [state, y](const Pixel &pixel) {
                                               return pixel.c.x == state.currentX && pixel.c.y == y;
                                           });
                    if (it != group.pixels.end()) {
                        mirrorCount++;

                        //Use new calculated angle and slope to determine direction of movement
                        double newAngle = getNewAngle(line, *it, state);
                        double newSlope = getNewSlope(line, *it, state);

                        //Separate logic if previous line slope is infinity
                        if (std::isinf(line.slope)) {
                            //Set movingRight based on output angle
                            if (std::isinf(newSlope)) {
                                state.movingRight = MovementState::Nan;
                            } else if (newAngle < M_PI / 2 + epsilon && newAngle > -M_PI / 2 - epsilon) {
                                state.movingRight = MovementState::True;
                            } else {
                                state.movingRight = MovementState::False;
                            }

                            //Set movingUp based on updated movingRight and angle
                            if (std::abs(newSlope) <= epsilon) {
                                state.movingUp = MovementState::Nan;
                            } else if (state.movingRight == MovementState::Nan) {
                                state.movingUp = (newAngle > 0 && newAngle < M_PI) ? MovementState::True : MovementState::False;
                            } else if ((newSlope > 0 && state.movingRight == MovementState::True) ||
                                (newSlope < 0 && state.movingRight == MovementState::False)) {
                                state.movingUp = MovementState::True;
                            } else if ((newSlope < 0 && state.movingRight == MovementState::True) ||
                                       (newSlope > 0 && state.movingRight == MovementState::False)) {
                                state.movingUp = MovementState::False;
                            }
                        } else {
                            // Similar logic for non-infinite slope
                            if (std::isinf(newSlope)) {
                                state.movingRight = MovementState::Nan;
                            } else if (newAngle < M_PI / 2 + epsilon && newAngle > -M_PI / 2 - epsilon) {
                               state.movingRight = MovementState::True;
                            } else {
                               state.movingRight = MovementState::False;
                            }
                            if (!std::isinf(newSlope) && std::abs(newSlope) <= epsilon) {
                                state.movingUp = MovementState::Nan;
                            }
                            else if (state.movingRight == MovementState::Nan) {
                                state.movingUp = (newAngle > -epsilon && newAngle < M_PI + epsilon) ? MovementState::True : MovementState::False;
                            }
                            else if ((newSlope > 0 && state.movingRight == MovementState::True) ||
                                (newSlope < 0 && state.movingRight == MovementState::False)) {
                                state.movingUp = MovementState::True;
                            } else if ((newSlope < 0 && state.movingRight == MovementState::True) ||
                                       (newSlope > 0 && state.movingRight == MovementState::False)) {
                                state.movingUp = MovementState::False;
                            }
                        }
                        // Update line with new slope and point
                        line = Line(newSlope, Coord(state.currentX, y));
                        std::cout << "Mirror #" << mirrorCount << "\n";

                        //SET arbitrary value for cooldown
                        state.cooldown = std::abs(newSlope) > 3 ? 1 : 15;

                        return;
                    }
                }
            }
            // Draw the line
            image[y][state.currentX] = lineColor;
        }
    }

    // Increment x value and cooldown to prepare for next iteration of function
    state.currentX += state.movingRight == MovementState::True ? 1 : -1;
    state.cooldown = std::max(0, state.cooldown - 1);

    // Stop drawing if out of bounds
    if (state.currentX >= width || state.currentX < 0) {
        state.isDrawing = false;
    }
}




