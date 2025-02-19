#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <memory>
#include "BayouState.h"
#include "graphics/Sprite.h"
#include "graphics/Window.h"
#include "BoardRenderer.h"  // Include board renderer.

namespace ShoeEngine {
	namespace Graphics {
		class ImageManager;
	}
	namespace Bayou {

		/**
		 * @class BayouStateVisualizer
		 * @brief Visualizes the current state of the Bayou board game.
		 *
		 * This class observes a BayouState instance and renders both the grid (via BoardRenderer)
		 * and the game pieces. The grid configuration (tile size and grid count) is passed in,
		 * allowing for customization.
		 */
		class BayouStateVisualizer {
		public:
			/**
			 * @brief Constructs a BayouStateVisualizer.
			 * @param state The BayouState instance to visualize.
			 * @param imageManager Reference to the ImageManager for retrieving images.
			 * @param tileSize The size (in pixels) of each board cell (default: 64).
			 * @param gridCount The number of rows/columns on the board (default: 8).
			 */
			BayouStateVisualizer(BayouState& state, ShoeEngine::Graphics::ImageManager& imageManager,
				int tileSize = 64, int gridCount = 8);

			/**
			 * @brief Updates the visual representation to match the current game state.
			 */
			void Update();

			/**
			 * @brief Renders the grid and game pieces onto the provided window.
			 * @param window The engine Window to draw on.
			 */
			void Render(ShoeEngine::Graphics::Window& window);

			/**
			 * @brief Handles mouse click events on the board.
			 * @param window The window in which the click occurred.
			 */
			void HandleMouseClick(ShoeEngine::Graphics::Window& window);

		private:
			/**
			 * @brief Helper function to get the image id for a given piece type.
			 * @param type The hash value representing the piece type.
			 * @return The image id corresponding to the piece.
			 */
			std::string GetImageIdForPiece(ShoeEngine::Core::Hash::HashValue type) const;

			BayouState& m_state;
			ShoeEngine::Graphics::ImageManager& m_imageManager;
			int m_tileSize;
			int m_gridCount;
			BoardRenderer m_boardRenderer;
			std::array<std::unique_ptr<ShoeEngine::Graphics::Sprite>, BayouState::kBoardNumSquares> m_pieceSprites;
		};

	} // namespace Bayou
} // namespace ShoeEngine
