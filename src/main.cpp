/**
 * @file main.cpp
 * @brief Entry point for the ShoeEngine application
 */

#include <iostream>
#include "core/DataManager.h"
#include "graphics/WindowManager.h"

/**
 * @brief Main entry point for the ShoeEngine application
 * 
 * @return int Exit code (0 for success, non-zero for failure)
 * 
 * @example
 * // Basic usage
 * int main() {
 *     // Initialize engine
 *     auto engine = ShoeEngine::Create();
 *     
 *     // Load game data from JSON
 *     engine->LoadGame("game_config.json");
 *     
 *     // Run game loop
 *     engine->Run();
 *     
 *     return 0;
 * }
 */
int main() {
    try {
        std::cout << "ShoeEngine initializing..." << std::endl;
        
        // Create and configure the data manager
        ShoeEngine::Core::DataManager dataManager;
        auto windowManager = std::make_unique<ShoeEngine::Graphics::WindowManager>();
        dataManager.RegisterManager(std::move(windowManager));

        // Load game configuration from JSON
        if (!dataManager.LoadFromFile("data/sample.json")) {
            throw std::runtime_error("Failed to load game configuration");
        }

        // Get the window manager back from the data manager
        auto* winManager = dynamic_cast<ShoeEngine::Graphics::WindowManager*>(
            dataManager.GetManager("windows"));
        
        if (!winManager || winManager->GetWindows().empty()) {
            throw std::runtime_error("No windows were created from configuration");
        }

        // Main game loop
        while (winManager->ProcessEvents()) {
            winManager->ClearAll();
            // Game rendering will go here
            winManager->DisplayAll();
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
