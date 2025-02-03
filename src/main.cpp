/**
 * @file main.cpp
 * @brief Entry point for the ShoeEngine application
 */

#include <iostream>
#include "graphics/Window.h"

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
        
        ShoeEngine::Graphics::Window window("ShoeEngine", 800, 600);

        while (window.IsOpen()) {
            if (!window.ProcessEvents()) {
                break;
            }

            window.Clear();
            // Game rendering will go here
            window.Display();
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
