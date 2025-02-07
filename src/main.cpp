/**
 * @file main.cpp
 * @brief Entry point for the ShoeEngine application
 */

#include <iostream>
#include "core/DataManager.h"
#include "graphics/WindowManager.h"
#include "graphics/ImageManager.h"
#include "graphics/SpriteManager.h"
#include "input/InputManager.h"
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
        auto inputManager = std::make_unique<ShoeEngine::Input::InputManager>();

        // Store raw pointers before moving
        auto* winManager = windowManager.get();
        auto* imgManager = imageManager.get();
        auto* sprManager = spriteManager.get();
        auto* inpManager = inputManager.get();

        // Register managers
        dataManager.RegisterManager(std::move(windowManager));
        dataManager.RegisterManager(std::move(imageManager));
        dataManager.RegisterManager(std::move(spriteManager));
        dataManager.RegisterManager(std::move(inputManager));

        // Load game configuration from JSON
        if (!dataManager.LoadFromFile("data/sample.json")) {
            throw std::runtime_error("Failed to load game configuration");
        }

        if (!winManager || winManager->GetWindows().empty()) {
            throw std::runtime_error("No windows were created from configuration");
        }

        // Set initial context for input
        inpManager->SetContext("gameplay"_h);

        // Debug: Print loaded inputs
        std::cout << "Loaded inputs:\n";
        for (const auto& name : {"move_left"_h, "move_right"_h, "move_up"_h, "move_down"_h}) {
            if (auto* input = inpManager->GetInput(name)) {
                std::cout << "  " << name << " loaded successfully\n";
            } else {
                std::cout << "  " << name << " failed to load\n";
            }
        }

        // Movement speed (pixels per frame)
        const float MOVEMENT_SPEED = 0.1f;

        // Main game loop
        while (winManager->ProcessEvents()) {
            // Handle input and move sprite
            if (auto* sprite = sprManager->GetSprite("player_sprite"_h)) {
                // Check each input and apply movement if active
                if (auto* input = inpManager->GetInput("move_left"_h)) {
                    if (input->IsActive()) {
                        sprite->Move(-MOVEMENT_SPEED, 0);
                    }
                }
                if (auto* input = inpManager->GetInput("move_right"_h)) {
                    if (input->IsActive()) {
                        sprite->Move(MOVEMENT_SPEED, 0);
                    }
                }
                if (auto* input = inpManager->GetInput("move_up"_h)) {
                    if (input->IsActive()) {
                        sprite->Move(0, -MOVEMENT_SPEED);
                    }
                }
                if (auto* input = inpManager->GetInput("move_down"_h)) {
                    if (input->IsActive()) {
                        sprite->Move(0, MOVEMENT_SPEED);
                    }
                }
            }

            winManager->ClearAll();

            // Get the main window
            auto& window = winManager->GetWindows()[0]->GetRenderWindow();

            // Draw all sprites
            if (auto* sprite = sprManager->GetSprite("player_sprite"_h)) {
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
