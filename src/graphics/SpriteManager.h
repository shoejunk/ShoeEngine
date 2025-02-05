#pragma once

#include "core/BaseManager.h"
#include "graphics/Sprite.h"
#include "graphics/ImageManager.h"
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
     * @brief Gets a sprite by name
     * @param name The name of the sprite to get
     * @return Pointer to the sprite, or nullptr if not found
     */
    Sprite* GetSprite(const std::string& name);

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
