#pragma once

#include <SFML/Graphics/Image.hpp>
#include <string>
#include <vector>
#include <memory>

namespace ShoeEngine {
namespace Graphics {

/**
 * @class Image
 * @brief A wrapper class for handling 2D images in the engine
 *
 * This class provides functionality to load, manipulate and store image data.
 * It serves as a wrapper around SFML's image functionality while providing
 * a more engine-specific interface.
 */
class Image {
public:
    /**
     * @brief Default constructor creating an empty image
     */
    Image();

    /**
     * @brief Constructor that loads an image from a file
     * @param filePath Path to the image file to load
     * @throws std::runtime_error if the image cannot be loaded
     */
    explicit Image(const std::string& filePath);

    /**
     * @brief Constructor that creates an image from raw pixel data
     * @param pixels Array of pixel data in RGBA format
     * @param width Width of the image in pixels
     * @param height Height of the image in pixels
     * @throws std::invalid_argument if pixels is null or dimensions are invalid
     */
    Image(const uint8_t* pixels, unsigned int width, unsigned int height);

    /**
     * @brief Loads an image from a file
     * @param filePath Path to the image file to load
     * @return true if loading was successful, false otherwise
     */
    bool LoadFromFile(const std::string& filePath);

    /**
     * @brief Saves the image to a file
     * @param filePath Path where to save the image
     * @return true if saving was successful, false otherwise
     */
    bool SaveToFile(const std::string& filePath) const;

    /**
     * @brief Gets the width of the image
     * @return Width in pixels
     */
    unsigned int GetWidth() const;

    /**
     * @brief Gets the height of the image
     * @return Height in pixels
     */
    unsigned int GetHeight() const;

    /**
     * @brief Gets the pixel array of the image
     * @return Const pointer to the pixel array in RGBA format
     */
    const uint8_t* GetPixels() const;

    /**
     * @brief Creates a copy of the image
     * @return New Image instance with the same content
     */
    Image Clone() const;

    /**
     * @brief Get the underlying SFML image
     * @return Reference to the internal SFML image
     */
    const sf::Image& GetSFMLImage() const;

private:
    std::unique_ptr<sf::Image> m_image; ///< Underlying SFML image
};

} // namespace Graphics
} // namespace ShoeEngine
