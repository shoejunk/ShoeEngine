#include "ImageManager.h"
#include "core/DataManager.h"
#include <stdexcept>
#include <iostream>

namespace ShoeEngine {
namespace Graphics {

ImageManager::ImageManager(Core::DataManager& dataManager)
	: Core::BaseManager(dataManager) {
	dataManager.RegisterString("images");
}

bool ImageManager::CreateFromJson(const nlohmann::json& jsonData) {
    try {
        for (const auto& [imageId, imageData] : jsonData.items()) {
            // Get the file path for the image
            std::string filePath = imageData.at("file").get<std::string>();
			Core::Hash::HashValue filePathHash = m_dataManager.RegisterString(filePath);

            // Create a new image
            auto image = std::make_unique<Image>();
            
            // Load the image from file
            if (!image->LoadFromFile(filePath)) {
                throw std::runtime_error("Failed to load image: " + filePath);
            }
            
            // Create hash from image ID
            Core::Hash::HashValue hashId(imageId.c_str(), static_cast<uint32_t>(imageId.length()));
			image->SetId(hashId);

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

nlohmann::json ImageManager::SerializeToJson() {
	nlohmann::json imagesObject = nlohmann::json::object();

	// Iterate through all images
	for (const auto& [imageHash, image] : m_images) {
		nlohmann::json imageJson;
		// Set the "file" property for each image. 
		// This assumes that Image has a method GetFile() returning the file path.
		imageJson["file"] = m_dataManager.GetString(image->GetFilePathHash());

		// Get the original image ID string using the DataManager, which is used as the key.
		std::string imageName = m_dataManager.GetString(imageHash);
		imagesObject[imageName] = imageJson;
	}

	return imagesObject;
}

} // namespace Graphics
} // namespace ShoeEngine
