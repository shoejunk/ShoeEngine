#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "graphics/Window.h"

namespace ShoeEngine {
namespace Bayou {

/**
 * @class BoardRenderer
 * @brief Renders the game board background and handles board cell click detection.
 *
 * This class draws a board image (or grid) and converts screen coordinates
 * to board cell positions (using an 8×8 layout).
 */
class BoardRenderer {
public:
    /**
     * @brief Constructs a BoardRenderer with a specified board image.
     * @param boardImagePath The file path to the board background image.
     * @param tileSize The size of each board cell in pixels (default: 64).
     */
    BoardRenderer(const std::string& boardImagePath, int tileSize = 64);

    /**
     * @brief Renders the board background.
     * @param window The engine Window to draw on.
     */
    void Render(ShoeEngine::Graphics::Window& window);

    /**
     * @brief Converts screen coordinates to board cell coordinates.
     * @param x The x-coordinate in screen space.
     * @param y The y-coordinate in screen space.
     * @param row Output parameter for the board row (0–7).
     * @param col Output parameter for the board column (0–7).
     * @return True if the coordinates are within the board, false otherwise.
     */
    bool GetBoardCell(float x, float y, int& row, int& col) const;

    /**
     * @brief Gets the tile size of the board cells.
     * @return The tile size in pixels.
     */
    int GetTileSize() const { return m_tileSize; }

private:
    int m_tileSize;
    sf::Texture m_boardTexture;
    sf::Sprite m_boardSprite;
};

} // namespace Bayou
} // namespace ShoeEngine
