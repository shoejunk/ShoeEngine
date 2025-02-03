#pragma once

#include "core/BaseManager.h"
#include "Sprite.h"
#include "ImageManager.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace ShoeEngine {
namespace Graphics {

/**
 * @class SpriteManager
 * @brief Manages creation and storage of Sprite objects from JSON data
 *
 * This class handles loading and managing Sprite objects, providing a centralized
 * way to create and access sprites throughout the engine.
 */
class SpriteManager : public Core::BaseManager {
public:
    /**
     * @brief Constructor taking a reference to the ImageManager
     * @param imageManager Reference to the ImageManager for sprite creation
     */
    explicit SpriteManager(ImageManager& imageManager);

    /**
     * @brief Creates Sprite objects from JSON data
     * @param jsonData JSON object containing sprite definitions
     * @return bool True if all sprites were created successfully
     */
    bool CreateFromJson(const nlohmann::json& jsonData) override;

    /**
     * @brief Get the type of objects this manager handles
     * @return std::string "sprites" as the managed type
     */
    std::string GetManagedType() const override;

    /**
     * @brief Get a sprite by its ID
     * @param spriteId The ID of the sprite to retrieve
     * @return const Sprite* Pointer to the sprite, or nullptr if not found
     */
    const Sprite* GetSprite(const std::string& spriteId) const;

    /**
     * @brief Clear all managed sprites
     */
    void Clear();

private:
    ImageManager& m_imageManager;
    std::unordered_map<std::string, std::unique_ptr<Sprite>> m_sprites;
};

} // namespace Graphics
} // namespace ShoeEngine
