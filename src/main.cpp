/**
 * @file main.cpp
 * @brief Entry point for the ShoeEngine application
 */

#include <iostream>
#include "core/DataManager.h"
#include "graphics/WindowManager.h"
#include "graphics/ImageManager.h"
#include "bayou/BayouStateManager.h"
#include "bayou/BayouStateVisualizer.h"
#include "Input/InputManager.h"
#include "Input/Input.h"
#include <vector>

using namespace ShoeEngine;

int main() {
	try {
		std::cout << "ShoeEngine initializing..." << std::endl;

		// Create and configure the data manager.
		Core::DataManager dataManager;

		// Create and register managers.
		// We no longer need SpriteManager or InputManager.
		auto windowManager = std::make_unique<Graphics::WindowManager>(dataManager);
		auto imageManager = std::make_unique<Graphics::ImageManager>(dataManager);
		auto stateManager = std::make_unique<Bayou::BayouStateManager>(dataManager);
		auto inputManager = std::make_unique<Input::InputManager>(dataManager);

		// Save raw pointers before transferring ownership.
		auto* winManager = windowManager.get();
		auto* imgManager = imageManager.get();
		auto* bayouStateManager = stateManager.get();
		auto* inpManager = inputManager.get();

		dataManager.RegisterManager(std::move(windowManager));
		dataManager.RegisterManager(std::move(imageManager));
		dataManager.RegisterManager(std::move(stateManager));
		dataManager.RegisterManager(std::move(inputManager));

		// Load game configuration from JSON.
		if (!dataManager.LoadFromFile("data/data.json")) {
			throw std::runtime_error("Failed to load game configuration");
		}
		dataManager.LoadFromFile("data/user/autosave.json");

		if (!winManager || winManager->GetWindows().empty()) {
			throw std::runtime_error("No windows were created from configuration");
		}

		// Create the BayouStateVisualizer, using the loaded Bayou state and ImageManager.
		Bayou::BayouStateVisualizer stateVisualizer(bayouStateManager->GetState(), *imgManager);

		// Main game loop
		while (winManager->ProcessEvents()) {
			winManager->ClearAll();

			// Update the visualizer to reflect the current game state.
			stateVisualizer.Update();

			// Check if the left mouse button is pressed and handle the click.
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				stateVisualizer.HandleMouseClick(*winManager->GetWindows()[0]);
			}

			// Render the game state using the visualizer.
			stateVisualizer.Render(*winManager->GetWindows()[0]);

			winManager->DisplayAll();
		}

		dataManager.SaveToFile("data/user/autosave.json");

		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		return 1;
	}
}
