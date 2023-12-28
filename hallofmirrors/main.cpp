#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <SFML/Graphics.hpp>
#include "stb_image_write.h"
#include "stb_image.h"
#include "Pixels.h"
#include "Reflection.h"


// Convert pixelData back into flat image data for saving or processing
std::vector<unsigned char> flattenPixelData(const std::vector<std::vector<Pixel>>& pixelData) {
    std::vector<unsigned char> flatData;
    for (const auto& row : pixelData) {
        for (const auto& pixel : row) {
            flatData.push_back(pixel.r);
            flatData.push_back(pixel.g);
            flatData.push_back(pixel.b);
        }
    }
    return flatData;
}

// Convert pixelData into SFML Image format for rendering
sf::Image convertPixelData(const std::vector<std::vector<Pixel>>& pixelData) {
    sf::Image temp;
    unsigned int width = pixelData[0].size();
    unsigned int height = pixelData.size();

    temp.create(width, height);
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            const Pixel& p = pixelData[y][x]; // Corrected indexing
            temp.setPixel(x, y, sf::Color(p.r, p.g, p.b));
        }
    }
    return temp;
}

// Save the image data to a file in PNG format
int saveImage(const std::string& filename, int width, int height, const std::vector<unsigned char>& data) {
    if (stbi_write_png(filename.c_str(), width, height, 3, data.data(), width * 3)) {
        return 0;
    }
    return -1;
}

int main() {
    // SET Pixel color to detect as mirror (only RGB dependent)
    const Pixel colorToDetect(228, 108, 10, -1, -1);

    // Extract image data with stb library into width, height, channels, pixelData
    int width, height, channels;
    unsigned char* data = stbi_load("mirrors.png", &width, &height, &channels, 3);
    if (!data) {
        std::cerr << "Error: could not load image" << std::endl;
        return 1;
    }

    std::vector<std::vector<Pixel>> pixelData(height, std::vector<Pixel>(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * 3;
            pixelData[y][x] = Pixel(data[idx], data[idx + 1], data[idx + 2], x, y);
        }
    }

    // Identify and group adjacent pixels of the mirror color
    std::vector<PixelGroup> groups;
    groups.reserve(100);   //Pre-allocate memory for number of mirrors to optimize performance
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            //Find all mirror groups, add them to a vector of PixelGroups, then set the slope for each group
            if (pixelData[y][x] == colorToDetect && !pixelData[y][x].isIncluded) {
                groups.emplace_back();
                PixelGroup& newGroup = groups.back();
                addPixelToGroup(x, y, width, height, colorToDetect, pixelData, newGroup);
                newGroup.calculateSlope();
            }
        }
    }

    //SET initial light line to be drawn
    Line line = Line(-1, Coord(1164, 794));

    //SET color of line
    Pixel lineColor(0, 0, 255, -1, -1);

    //SET state of line: initial position and movement direction
    MovementState initialMovingRight = MovementState::False;
    MovementState initialMovingUp = MovementState::True;
    LineDrawingState state = LineDrawingState(1164, 794, initialMovingRight, initialMovingUp);

    //SET SFML window dimensions
    sf::RenderWindow window(sf::VideoMode(1571, 1607), "Image Display");
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Image image;
    //SET file name
    if(!image.loadFromFile("mirrors.png")) {
        return -1;
    }

    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sf::Image sfmlImage = convertPixelData(pixelData);

    // Main loop for window display and interaction
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event{};
        if (!state.isDrawing) {
            event.type = sf::Event::Closed;
        }
        while (window.pollEvent(event)) {
            //Saves image after closing
            if (event.type == sf::Event::Closed) {
                window.close();
                auto flatData = flattenPixelData(pixelData);
                if (saveImage("output_image.png", width, height, flatData) == 0) {
                    std::cout << "Image saved successfully." << std::endl;
                } else {
                    std::cerr << "Failed to save the image." << std::endl;
                }
                system("start output_image.png");
            }
        }

        //SET time between line updates in milliseconds
        if (state.isDrawing && clock.getElapsedTime().asMilliseconds() >= 2) {
            drawLineAndDetectCollision(pixelData, line, lineColor, groups, state);
            // Live window drawing of image updating: comment out for optimized performance /*
            sfmlImage = convertPixelData(pixelData);
            texture.loadFromImage(sfmlImage);
            sprite.setTexture(texture);
             // */
            clock.restart();
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}