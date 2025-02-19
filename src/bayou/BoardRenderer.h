#pragma once

#include <SFML/Graphics.hpp>
#include "graphics/Window.h"

namespace ShoeEngine {
	namespace Bayou {

		/**
		 * @class BoardRenderer
		 * @brief Renders grid lines for the game board.
		 *
		 * This class draws an 8×8 grid (or a grid defined by gridCount) using the specified tile size.
		 */
		class BoardRenderer {
		public:
			/**
			 * @brief Constructs a BoardRenderer.
			 * @param tileSize The size of each board cell in pixels (default: 64).
			 */
			BoardRenderer(int tileSize = 64);

			/**
			 * @brief Renders the grid lines.
			 * @param window The engine Window to draw on.
			 */
			void Render(ShoeEngine::Graphics::Window& window);

			/**
			 * @brief Converts screen coordinates to board cell coordinates.
			 * @param x The x-coordinate in screen space.
			 * @param y The y-coordinate in screen space.
			 * @param row Output parameter for the board row.
			 * @param col Output parameter for the board column.
			 * @return True if the coordinates are within the grid, false otherwise.
			 */
			bool GetBoardCell(float x, float y, int& row, int& col) const;

			/**
			 * @brief Gets the tile size.
			 * @return The tile size in pixels.
			 */
			int GetTileSize() const { return m_tileSize; }

		private:
			int m_tileSize;
		};

	} // namespace Bayou
} // namespace ShoeEngine
