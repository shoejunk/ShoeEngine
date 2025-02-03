#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include "Image.h"

namespace ShoeEngine {
namespace Graphics {

/**
 * @class Sprite
 * @brief A wrapper class for handling 2D sprites in the engine
 *
 * This class provides functionality to create and manipulate 2D sprites,
 * including position, rotation, scale, and texture management.
 */
class Sprite {
public:
    /**
     * @brief Default constructor creating an empty sprite
     */
    Sprite();

    /**
     * @brief Constructor that creates a sprite from an Image
     * @param image The source image to create the sprite from
     */
    explicit Sprite(const Image& image);

    /**
     * @brief Sets the sprite's position
     * @param x X coordinate
     * @param y Y coordinate
     */
    void SetPosition(float x, float y);

    /**
     * @brief Sets the sprite's rotation
     * @param angle Rotation angle in degrees
     */
    void SetRotation(float angle);

    /**
     * @brief Sets the sprite's scale
     * @param scaleX Scale factor on X axis
     * @param scaleY Scale factor on Y axis
     */
    void SetScale(float scaleX, float scaleY);

    /**
     * @brief Sets the sprite's origin (rotation/scale center point)
     * @param x X coordinate of the origin
     * @param y Y coordinate of the origin
     */
    void SetOrigin(float x, float y);

    /**
     * @brief Gets the sprite's position
     * @return Pair of x, y coordinates
     */
    std::pair<float, float> GetPosition() const;

    /**
     * @brief Gets the sprite's rotation
     * @return Rotation angle in degrees
     */
    float GetRotation() const;

    /**
     * @brief Gets the sprite's scale
     * @return Pair of x, y scale factors
     */
    std::pair<float, float> GetScale() const;

    /**
     * @brief Gets the sprite's origin
     * @return Pair of x, y coordinates of the origin
     */
    std::pair<float, float> GetOrigin() const;

    /**
     * @brief Gets the sprite's local bounds
     * @return Tuple of left, top, width, height
     */
    std::tuple<float, float, float, float> GetLocalBounds() const;

    /**
     * @brief Gets the sprite's global bounds (transformed by position/rotation/scale)
     * @return Tuple of left, top, width, height
     */
    std::tuple<float, float, float, float> GetGlobalBounds() const;

    /**
     * @brief Sets a new image for the sprite
     * @param image The new image to use
     */
    void SetImage(const Image& image);

    /**
     * @brief Get the underlying SFML sprite
     * @return Reference to the internal SFML sprite
     */
    const sf::Sprite& GetSFMLSprite() const;

private:
    std::unique_ptr<sf::Sprite> m_sprite;     ///< Underlying SFML sprite
    std::shared_ptr<sf::Texture> m_texture;   ///< Texture used by the sprite
};

} // namespace Graphics
} // namespace ShoeEngine
