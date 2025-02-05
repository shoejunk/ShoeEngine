#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <functional>
#include <string>
#include <unordered_map>

namespace ShoeEngine {
namespace Input {

/**
 * @class Input
 * @brief Represents a configurable input binding that can trigger actions
 *
 * This class handles both keyboard and mouse input, allowing for flexible
 * input configurations that can be loaded from JSON data.
 */
class Input {
public:
    /**
     * @brief Enum defining the type of input
     */
    enum class Type {
        Keyboard,
        MouseButton,
        MouseAxis
    };

    /**
     * @brief Constructor for Input class
     * @param name The unique identifier for this input binding
     * @param type The type of input (keyboard, mouse button, or mouse axis)
     */
    Input(const std::string& name, Type type);

    /**
     * @brief Set the keyboard key for this input
     * @param key The SFML keyboard key
     */
    void SetKey(sf::Keyboard::Key key);

    /**
     * @brief Set the mouse button for this input
     * @param button The SFML mouse button
     */
    void SetMouseButton(sf::Mouse::Button button);

    /**
     * @brief Set the action callback for this input
     * @param callback The function to call when input is triggered
     */
    void SetCallback(std::function<void()> callback);

    /**
     * @brief Get the name of this input binding
     * @return The input binding's name
     */
    const std::string& GetName() const { return m_name; }

    /**
     * @brief Update the input state and trigger callbacks if necessary
     */
    void Update();

    /**
     * @brief Set the context for this input
     * @param context The context identifier
     */
    void SetContext(const std::string& context) { m_context = context; }

    /**
     * @brief Get the current context
     * @return The current context identifier
     */
    const std::string& GetContext() const { return m_context; }

    /**
     * @brief Checks if this input is currently active (pressed)
     * @return true if the input is active, false otherwise
     */
    bool IsActive() const;

private:
    std::string m_name;
    Type m_type;
    std::string m_context;
    std::function<void()> m_callback;
    
    union {
        sf::Keyboard::Key m_key;
        sf::Mouse::Button m_mouseButton;
    };

    bool m_wasPressed;
};

} // namespace Input
} // namespace ShoeEngine
