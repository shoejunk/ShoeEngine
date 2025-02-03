#pragma once

#include "core/BaseManager.h"
#include "Image.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace ShoeEngine {
namespace Graphics {

/**
 * @class ImageManager
 * @brief Manages creation and storage of Image objects from JSON data
 *
 * This class handles loading and managing Image objects, providing a centralized
 * way to create and access images throughout the engine.
 */
class ImageManager : public Core::BaseManager {
public:
    /**
     * @brief Default constructor
     */
    ImageManager() = default;

    /**
     * @brief Creates Image objects from JSON data
     * @param jsonData JSON object containing image definitions
     * @return bool True if all images were created successfully
     */
    bool CreateFromJson(const nlohmann::json& jsonData) override;

    /**
     * @brief Get the type of objects this manager handles
     * @return std::string "images" as the managed type
     */
    std::string GetManagedType() const override;

    /**
     * @brief Get an image by its ID
     * @param imageId The ID of the image to retrieve
     * @return const Image* Pointer to the image, or nullptr if not found
     */
    const Image* GetImage(const std::string& imageId) const;

    /**
     * @brief Clear all managed images
     */
    void Clear();

private:
    std::unordered_map<std::string, std::unique_ptr<Image>> m_images;
};

} // namespace Graphics
} // namespace ShoeEngine
