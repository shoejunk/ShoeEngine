#pragma once

#include "core/Hash.h"
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

	/**
	* @brief Gets the hash value of the window title
	* @return Hash value of the window title
	*/
	Core::Hash::HashValue GetTitleHash() const { return m_titleHash; }

	/**
	* @brief Sets the hash value of the window title
	*/
	void SetTitleHash(Core::Hash::HashValue titleHash) { m_titleHash = titleHash; }

	uint32_t GetWidth() const { return m_window.getSize().x; }

	uint32_t GetHeight() const { return m_window.getSize().y; }

private:
    sf::RenderWindow m_window;  ///< The SFML window instance
	Core::Hash::HashValue m_titleHash; ///< Hash value for the window title
};

} // namespace Graphics
} // namespace ShoeEngine
