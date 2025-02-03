#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace ShoeEngine {
namespace Graphics {

/**
 * @class Window
 * @brief Manages the game window and its properties
 *
 * The Window class provides a wrapper around SFML's window management,
 * handling window creation, events, and basic window operations.
 * It follows the RAII pattern for resource management.
 */
class Window {
public:
    /**
     * @brief Creates a window with the specified properties
     * @param title The window title
     * @param width The window width in pixels
     * @param height The window height in pixels
     */
    Window(const std::string& title, unsigned int width, unsigned int height);

    /**
     * @brief Destructor ensures proper cleanup of window resources
     */
    ~Window();

    /**
     * @brief Checks if the window is currently open
     * @return true if the window is open, false otherwise
     */
    bool IsOpen() const;

    /**
     * @brief Closes the window
     */
    void Close();

    /**
     * @brief Processes all pending window events
     * @return true if the window should remain open, false if it should close
     */
    bool ProcessEvents();

    /**
     * @brief Displays the contents of the window
     */
    void Display();

    /**
     * @brief Clears the window with a black background
     */
    void Clear();

    /**
     * @brief Gets the underlying SFML window
     * @return Reference to the SFML window
     */
    sf::RenderWindow& GetRenderWindow();

private:
    sf::RenderWindow m_window;  ///< The SFML window instance
};

} // namespace Graphics
} // namespace ShoeEngine
