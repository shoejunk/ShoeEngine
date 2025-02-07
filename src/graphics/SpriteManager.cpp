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
            
            // Create hash from image ID
            Core::Hash::HashValue imageHash(imageId.c_str(), imageId.length());
            
            // Get the image from the ImageManager
            const Image* image = m_imageManager.GetImage(imageHash);
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
            
            // Create hash from sprite ID and store the sprite
            Core::Hash::HashValue spriteHash(spriteId.c_str(), spriteId.length());
            m_sprites[spriteHash] = std::move(sprite);
        }
        return true;
    }
    catch (const std::exception& e) {
        // Log error and return false
        return false;
    }
}

Core::Hash::HashValue SpriteManager::GetManagedType() const {
    return "sprites"_h;
}

Sprite* SpriteManager::GetSprite(const Core::Hash::HashValue& name) {
    auto it = m_sprites.find(name);
    if (it != m_sprites.end()) {
        return it->second.get();
    }
    return nullptr;
}

void SpriteManager::Clear() {
    m_sprites.clear();
}

} // namespace Graphics
} // namespace ShoeEngine
