#include "Image.h"
#include "core/Hash.h"
#include <stdexcept>

namespace ShoeEngine {
namespace Graphics {

Image::Image()
    : m_image(std::make_unique<sf::Image>())
{
}

Image::Image(const std::string& filePath)
    : m_image(std::make_unique<sf::Image>())
{
	m_filePathHash = Core::Hash::HashValue(filePath);
    if (!LoadFromFile(filePath)) {
        throw std::runtime_error("Failed to load image from file: " + filePath);
    }
}

Image::Image(const uint8_t* pixels, unsigned int width, unsigned int height)
    : m_image(std::make_unique<sf::Image>())
{
    if (!pixels || width == 0 || height == 0) {
        throw std::invalid_argument("Invalid pixel data or dimensions");
    }
    m_image->create(width, height, pixels);
}

bool Image::LoadFromFile(const std::string& filePath)
{
	m_filePathHash = Core::Hash::HashValue(filePath);
	return m_image->loadFromFile(filePath);
}

bool Image::SaveToFile(const std::string& filePath) const
{
    return m_image->saveToFile(filePath);
}

unsigned int Image::GetWidth() const
{
    return m_image->getSize().x;
}

unsigned int Image::GetHeight() const
{
    return m_image->getSize().y;
}

const uint8_t* Image::GetPixels() const
{
    return m_image->getPixelsPtr();
}

Image Image::Clone() const
{
    Image newImage;
    *newImage.m_image = *m_image;
    return newImage;
}

const sf::Image& Image::GetSFMLImage() const
{
    return *m_image;
}

} // namespace Graphics
} // namespace ShoeEngine
