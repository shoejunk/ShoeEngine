#include "SpriteManager.h"
#include <stdexcept>

namespace ShoeEngine {
namespace Graphics {

SpriteManager::SpriteManager(ImageManager& imageManager)
    : m_imageManager(imageManager)
{
}

bool SpriteManager::CreateFromJson(const nlohmann::json& jsonData) {
    try {
        for (const auto& [spriteId, spriteData] : jsonData.items()) {
            // Get the image ID for the sprite
            std::string imageId = spriteData.at("image").get<std::string>();
            
            // Get the image from the ImageManager
            const Image* image = m_imageManager.GetImage(imageId);
            if (!image) {
                throw std::runtime_error("Image not found: " + imageId);
            }
            
            // Create a new sprite with the image
            auto sprite = std::make_unique<Sprite>(*image);
            
            // Set position if specified
            if (spriteData.contains("position")) {
                const auto& position = spriteData["position"];
                sprite->SetPosition(
                    position.at("x").get<float>(),
                    position.at("y").get<float>()
                );
            }
            
            // Set rotation if specified
            if (spriteData.contains("rotation")) {
                sprite->SetRotation(spriteData["rotation"].get<float>());
            }
            
            // Set scale if specified
            if (spriteData.contains("scale")) {
                const auto& scale = spriteData["scale"];
                sprite->SetScale(
                    scale.at("x").get<float>(),
                    scale.at("y").get<float>()
                );
            }
            
            // Set origin if specified
            if (spriteData.contains("origin")) {
                const auto& origin = spriteData["origin"];
                sprite->SetOrigin(
                    origin.at("x").get<float>(),
                    origin.at("y").get<float>()
                );
            }
            
            // Store the sprite
            m_sprites[spriteId] = std::move(sprite);
        }
        return true;
    }
    catch (const std::exception& e) {
        // Log error and return false
        return false;
    }
}

std::string SpriteManager::GetManagedType() const {
    return "sprites";
}

const Sprite* SpriteManager::GetSprite(const std::string& spriteId) const {
    auto it = m_sprites.find(spriteId);
    return it != m_sprites.end() ? it->second.get() : nullptr;
}

void SpriteManager::Clear() {
    m_sprites.clear();
}

} // namespace Graphics
} // namespace ShoeEngine
