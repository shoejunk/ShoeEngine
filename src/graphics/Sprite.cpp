#include "Sprite.h"

namespace ShoeEngine {
namespace Graphics {

Sprite::Sprite()
    : m_sprite(std::make_unique<sf::Sprite>())
    , m_texture(std::make_shared<sf::Texture>())
{
}

Sprite::Sprite(const Image& image)
    : m_sprite(std::make_unique<sf::Sprite>())
    , m_texture(std::make_shared<sf::Texture>())
{
    SetImage(image);
}

void Sprite::SetPosition(float x, float y)
{
    m_sprite->setPosition(x, y);
}

void Sprite::SetRotation(float angle)
{
    m_sprite->setRotation(angle);
}

void Sprite::SetScale(float scaleX, float scaleY)
{
    m_sprite->setScale(scaleX, scaleY);
}

void Sprite::SetOrigin(float x, float y)
{
    m_sprite->setOrigin(x, y);
}

std::pair<float, float> Sprite::GetPosition() const
{
    const auto& pos = m_sprite->getPosition();
    return {pos.x, pos.y};
}

float Sprite::GetRotation() const
{
    return m_sprite->getRotation();
}

std::pair<float, float> Sprite::GetScale() const
{
    const auto& scale = m_sprite->getScale();
    return {scale.x, scale.y};
}

std::pair<float, float> Sprite::GetOrigin() const
{
    const auto& origin = m_sprite->getOrigin();
    return {origin.x, origin.y};
}

std::tuple<float, float, float, float> Sprite::GetLocalBounds() const
{
    const auto& bounds = m_sprite->getLocalBounds();
    return {bounds.left, bounds.top, bounds.width, bounds.height};
}

std::tuple<float, float, float, float> Sprite::GetGlobalBounds() const
{
    const auto& bounds = m_sprite->getGlobalBounds();
    return {bounds.left, bounds.top, bounds.width, bounds.height};
}

void Sprite::SetImage(const Image& image)
{
    m_texture->loadFromImage(image.GetSFMLImage());
    m_sprite->setTexture(*m_texture, true);
}

const sf::Sprite& Sprite::GetSFMLSprite() const
{
    return *m_sprite;
}

} // namespace Graphics
} // namespace ShoeEngine
