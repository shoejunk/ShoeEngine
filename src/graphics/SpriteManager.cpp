#include "SpriteManager.h"
#include "core/Hash.h"
#include <stdexcept>

namespace ShoeEngine {
namespace Graphics {

SpriteManager::SpriteManager(Core::DataManager& dataManager, ImageManager& imageManager)
    : Core::BaseManager(dataManager)
    , m_imageManager(imageManager)
{
    // Register the type string using the base class's protected member
    m_dataManager.RegisterString("sprites");
}

bool SpriteManager::CreateFromJson(const nlohmann::json& jsonData) {
    try {
        for (const auto& [spriteId, spriteData] : jsonData.items()) {
            // Get the image ID for the sprite
            std::string imageId = spriteData.at("image").get<std::string>();
            
            // Register the sprite and image IDs
            m_dataManager.RegisterString(spriteId);
            m_dataManager.RegisterString(imageId);
            
            // Create hash from image ID
            Core::Hash::HashValue imageHash = m_dataManager.RegisterString(imageId);
            
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
            
            // Add the sprite to our map
            Core::Hash::HashValue spriteHash(spriteId.c_str(), static_cast<uint32_t>(spriteId.length()));
            m_sprites[spriteHash] = std::move(sprite);
        }
        
        return true;
    }
    catch (const std::exception&) {
        return false;
    }
}

Core::Hash::HashValue SpriteManager::GetManagedType() const {
    return "sprites"_h;
}

Sprite* SpriteManager::GetSprite(const Core::Hash::HashValue& name) {
    auto it = m_sprites.find(name);
    return it != m_sprites.end() ? it->second.get() : nullptr;
}

void SpriteManager::Clear() {
    m_sprites.clear();
}

nlohmann::json SpriteManager::SerializeToJson() {
	// Use an object instead of an array
	nlohmann::json spritesObject = nlohmann::json::object();

	for (const auto& [spriteHash, sprite] : m_sprites)
	{
		nlohmann::json spriteJson;

		// Get the image ID from the sprite's image
		const Image& image = sprite->GetImage();
		spriteJson["image"] = m_dataManager.GetString(image.GetId());

		// Get position
		auto position = sprite->GetPosition();
		spriteJson["position"] = {
			{ "x", position.first },
			{ "y", position.second }
		};

		// Get rotation
		spriteJson["rotation"] = sprite->GetRotation();

		// Get scale
		auto scale = sprite->GetScale();
		spriteJson["scale"] = {
			{ "x", scale.first },
			{ "y", scale.second }
		};

		// Get origin
		auto origin = sprite->GetOrigin();
		spriteJson["origin"] = {
			{ "x", origin.first },
			{ "y", origin.second }
		};

		// Convert the sprite's hash back to a string, which we�ll use as the object key.
		std::string spriteName = m_dataManager.GetString(spriteHash);

		// Instead of pushing to an array, assign it in an object by sprite name
		spritesObject[spriteName] = spriteJson;
	}

	return spritesObject;
}

} // namespace Graphics
} // namespace ShoeEngine
