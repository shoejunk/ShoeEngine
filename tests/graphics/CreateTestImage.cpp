#include "graphics/Image.h"
#include <vector>

// This is a helper program to create a test image
int main() {
    using namespace ShoeEngine::Graphics;

    // Create a 32x32 blue square with a white border
    std::vector<uint8_t> pixels(32 * 32 * 4, 0);
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 32; ++x) {
            int index = (y * 32 + x) * 4;
            if (x == 0 || x == 31 || y == 0 || y == 31) {
                // White border
                pixels[index] = 255;     // R
                pixels[index + 1] = 255; // G
                pixels[index + 2] = 255; // B
                pixels[index + 3] = 255; // A
            } else {
                // Blue fill
                pixels[index] = 0;       // R
                pixels[index + 1] = 0;   // G
                pixels[index + 2] = 255; // B
                pixels[index + 3] = 255; // A
            }
        }
    }

    Image image(pixels.data(), 32, 32);
    image.SaveToFile("data/images/player.png");
    return 0;
}
