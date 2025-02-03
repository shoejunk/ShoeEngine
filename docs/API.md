# ShoeEngine API Documentation

## Graphics

### Window Class
`ShoeEngine::Graphics::Window`

The Window class provides a high-level interface for managing game windows. It wraps SFML's window functionality in a more manageable, RAII-compliant interface.

#### Constructor
```cpp
Window(const std::string& title, unsigned int width, unsigned int height)
```
Creates a new window with the specified title and dimensions.
- **Parameters:**
  - `title`: Window title text
  - `width`: Window width in pixels
  - `height`: Window height in pixels

#### Methods

##### `bool IsOpen() const`
Checks if the window is currently open.
- **Returns:** `true` if the window is open, `false` otherwise

##### `void Close()`
Closes the window.

##### `bool ProcessEvents()`
Processes all pending window events.
- **Returns:** `true` if the window should remain open, `false` if it should close
- **Note:** Automatically handles the window close button event

##### `void Display()`
Updates the window with any changes since the last call.

##### `void Clear()`
Clears the window with a black background.

##### `sf::RenderWindow& GetRenderWindow()`
Provides access to the underlying SFML window.
- **Returns:** Reference to the SFML RenderWindow
- **Note:** Use with caution as it exposes SFML implementation details
