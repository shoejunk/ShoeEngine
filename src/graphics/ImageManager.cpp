#include "ImageManager.h"
#include <stdexcept>
#include <iostream>

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
            
            // Create hash from image ID
            Core::Hash::HashValue hashId(imageId.c_str(), static_cast<uint32_t>(imageId.length()));
            
            // Store the image
            m_images[hashId] = std::move(image);
        }
        return true;
    }
    catch (const std::exception& e) {
		std::cerr << "Error loading images: " << e.what() << "\n";
        return false;
    }
}

Core::Hash::HashValue ImageManager::GetManagedType() const {
    return "images"_h;
}

const Image* ImageManager::GetImage(const Core::Hash::HashValue& imageId) const {
    auto it = m_images.find(imageId);
    return it != m_images.end() ? it->second.get() : nullptr;
}

void ImageManager::Clear() {
    m_images.clear();
}

} // namespace Graphics
} // namespace ShoeEngine
