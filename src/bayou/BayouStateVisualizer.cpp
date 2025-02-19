#include "BayouStateVisualizer.h"
#include "graphics/ImageManager.h"
#include "graphics/Sprite.h"
#include "core/Hash.h"
#include <iostream>

namespace ShoeEngine {
namespace Bayou {

BayouStateVisualizer::BayouStateVisualizer(BayouState& state, ShoeEngine::Graphics::ImageManager& imageManager, int tileSize)
    : m_state(state), m_imageManager(imageManager), m_tileSize(tileSize)
{
    // Initialize sprite pointers to nullptr.
    for (auto& spritePtr : m_pieceSprites) {
        spritePtr = nullptr;
    }
}

std::string BayouStateVisualizer::GetImageIdForPiece(ShoeEngine::Core::Hash::HashValue type) const {
    if (type == "alligator"_h) {
        return "alligator";
    }
    else if (type == "crocodile"_h) {
        return "crocodile";
    }
    return "player_image";
}

void BayouStateVisualizer::Update() {
    // Iterate over every board cell.
    for (int i = 0; i < BayouState::kBoardNumSquares; ++i) {
        uint8_t cell = m_state.m_board[i];
        int row = i / 8;
        int col = i % 8;
        sf::Vector2f position(static_cast<float>(col * m_tileSize), static_cast<float>(row * m_tileSize));

        if (BayouState::IsOccupied(cell)) {
            // Decode which player's piece occupies this cell.
            int playerId = BayouState::GetPlayerId(cell);
            int pieceIndex = BayouState::GetPieceIndex(cell);
            if (pieceIndex < 0 || pieceIndex >= 64) {
                continue;
            }
            const auto& piece = m_state.m_playerPieces[playerId][pieceIndex];
            std::string imageId = GetImageIdForPiece(piece.m_type);

            // Retrieve the image from the ImageManager.
            const auto* image = m_imageManager.GetImage(imageId);
            if (!image) {
                std::cerr << "Error: Image with id '" << imageId << "' not found in ImageManager." << std::endl;
                continue;
            }

            // Create or update the sprite for this board cell.
            if (!m_pieceSprites[i]) {
                m_pieceSprites[i] = std::make_unique<ShoeEngine::Graphics::Sprite>();
            }
            m_pieceSprites[i]->SetImage(*image);
            m_pieceSprites[i]->SetPosition(position.x, position.y);

            // Compute scale to fit the sprite within the grid square.
            float imageWidth  = static_cast<float>(image->GetWidth());
            float imageHeight = static_cast<float>(image->GetHeight());
            float scaleX = m_tileSize / imageWidth;
            float scaleY = m_tileSize / imageHeight;
            m_pieceSprites[i]->SetScale(scaleX, scaleY);
        }
        else {
            // Remove sprite if cell becomes empty.
            if (m_pieceSprites[i]) {
                m_pieceSprites[i].reset();
            }
        }
    }
}

void BayouStateVisualizer::Render(ShoeEngine::Graphics::Window& window) {
    // Draw all active piece sprites.
    for (const auto& spritePtr : m_pieceSprites) {
        if (spritePtr) {
            window.GetRenderWindow().draw(spritePtr->GetSFMLSprite());
        }
    }
}

} // namespace Bayou
} // namespace ShoeEngine
