#include "BoardRenderer.h"
#include <stdexcept>

namespace ShoeEngine {
namespace Bayou {

BoardRenderer::BoardRenderer(const std::string& boardImagePath, int tileSize)
    : m_tileSize(tileSize)
{
    if (!m_boardTexture.loadFromFile(boardImagePath)) {
        throw std::runtime_error("Failed to load board image: " + boardImagePath);
    }
    m_boardSprite.setTexture(m_boardTexture);
}

void BoardRenderer::Render(ShoeEngine::Graphics::Window& window) {
    // Get the underlying SFML RenderWindow from the engine Window.
    window.GetRenderWindow().draw(m_boardSprite);
}

bool BoardRenderer::GetBoardCell(float x, float y, int& row, int& col) const {
    // Assuming the board is drawn at (0,0) and spans 8*m_tileSize in width and height.
    if (x < 0 || y < 0 || x >= 8 * m_tileSize || y >= 8 * m_tileSize) {
        return false;
    }
    col = static_cast<int>(x) / m_tileSize;
    row = static_cast<int>(y) / m_tileSize;
    return true;
}

} // namespace Bayou
} // namespace ShoeEngine
