/**
 * @file main.cpp
 * @brief Entry point for the ShoeEngine application
 */

#include <iostream>
#include "core/DataManager.h"
#include "graphics/WindowManager.h"
#include "graphics/ImageManager.h"
#include "graphics/SpriteManager.h"
#include <vector>

/**
 * @brief Main entry point for the ShoeEngine application
 * 
 * @return int Exit code (0 for success, non-zero for failure)
 */
int main() {
    try {
        std::cout << "ShoeEngine initializing..." << std::endl;
        
        // Create test image if it doesn't exist
        {
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

            ShoeEngine::Graphics::Image image(pixels.data(), 32, 32);
            image.SaveToFile("data/images/player.png");
        }
        
        // Create and configure the data manager
        ShoeEngine::Core::DataManager dataManager;

        // Create and register managers
        auto windowManager = std::make_unique<ShoeEngine::Graphics::WindowManager>();
        auto imageManager = std::make_unique<ShoeEngine::Graphics::ImageManager>();
        auto spriteManager = std::make_unique<ShoeEngine::Graphics::SpriteManager>(*imageManager);

        // Store raw pointers before moving
        auto* winManager = windowManager.get();
        auto* imgManager = imageManager.get();
        auto* sprManager = spriteManager.get();

        // Register managers
        dataManager.RegisterManager(std::move(windowManager));
        dataManager.RegisterManager(std::move(imageManager));
        dataManager.RegisterManager(std::move(spriteManager));

        // Load game configuration from JSON
        if (!dataManager.LoadFromFile("data/sample.json")) {
            throw std::runtime_error("Failed to load game configuration");
        }

        if (!winManager || winManager->GetWindows().empty()) {
            throw std::runtime_error("No windows were created from configuration");
        }

        // Main game loop
        while (winManager->ProcessEvents()) {
            winManager->ClearAll();

            // Get the main window
            auto& window = winManager->GetWindows()[0]->GetRenderWindow();

            // Draw all sprites
            if (const auto* sprite = sprManager->GetSprite("player_sprite")) {
                window.draw(sprite->GetSFMLSprite());
            }

            winManager->DisplayAll();
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
