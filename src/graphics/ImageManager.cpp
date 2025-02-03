#include "ImageManager.h"
#include <stdexcept>

namespace ShoeEngine {
namespace Graphics {

bool ImageManager::CreateFromJson(const nlohmann::json& jsonData) {
    try {
        for (const auto& [imageId, imageData] : jsonData.items()) {
            // Get the file path for the image
            std::string filePath = imageData.at("file").get<std::string>();
            
            // Create a new image
            auto image = std::make_unique<Image>();
            
            // Load the image from file
            if (!image->LoadFromFile(filePath)) {
                throw std::runtime_error("Failed to load image: " + filePath);
            }
            
            // Store the image
            m_images[imageId] = std::move(image);
        }
        return true;
    }
    catch (const std::exception& e) {
        // Log error and return false
        return false;
    }
}

std::string ImageManager::GetManagedType() const {
    return "images";
}

const Image* ImageManager::GetImage(const std::string& imageId) const {
    auto it = m_images.find(imageId);
    return it != m_images.end() ? it->second.get() : nullptr;
}

void ImageManager::Clear() {
    m_images.clear();
}

} // namespace Graphics
} // namespace ShoeEngine
