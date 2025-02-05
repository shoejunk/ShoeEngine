#include "Input.h"

namespace ShoeEngine {
namespace Input {

Input::Input(const std::string& name, Type type)
    : m_name(name)
    , m_type(type)
    , m_wasPressed(false) {
}

void Input::SetKey(sf::Keyboard::Key key) {
    if (m_type == Type::Keyboard) {
        m_key = key;
    }
}

void Input::SetMouseButton(sf::Mouse::Button button) {
    if (m_type == Type::MouseButton) {
        m_mouseButton = button;
    }
}

void Input::SetCallback(std::function<void()> callback) {
    m_callback = callback;
}

void Input::Update() {
    bool isPressed = false;

    switch (m_type) {
        case Type::Keyboard:
            isPressed = sf::Keyboard::isKeyPressed(m_key);
            break;
        case Type::MouseButton:
            isPressed = sf::Mouse::isButtonPressed(m_mouseButton);
            break;
        case Type::MouseAxis:
            // Handle mouse axis input if needed
            break;
    }

    // Trigger callback on the rising edge (when first pressed)
    if (isPressed && !m_wasPressed && m_callback) {
        m_callback();
    }

    m_wasPressed = isPressed;
}

bool Input::IsActive() const {
    if (m_type == Type::Keyboard) {
        return sf::Keyboard::isKeyPressed(m_key);
    }
    else {
        return sf::Mouse::isButtonPressed(m_mouseButton);
    }
}

} // namespace Input
} // namespace ShoeEngine
