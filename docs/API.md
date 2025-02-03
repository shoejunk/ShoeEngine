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

## Core

### BaseManager Class
`ShoeEngine::Core::BaseManager`

The BaseManager class serves as an abstract base class for all managers that handle object creation from JSON data. Specific managers (e.g., SpriteManager, TextureManager) should inherit from this class and implement its virtual methods.

#### Virtual Methods

##### `virtual bool CreateFromJson(const nlohmann::json& jsonData) = 0`
Pure virtual function that must be implemented by derived classes to handle object creation from JSON data.
- **Parameters:**
  - `jsonData`: JSON data containing object definitions
- **Returns:** `true` if objects were created successfully, `false` otherwise

##### `virtual std::string GetManagedType() const = 0`
Pure virtual function that must be implemented by derived classes to identify the type of objects they manage.
- **Returns:** String identifier for the manager's object type

### DataManager Class
`ShoeEngine::Core::DataManager`

The DataManager class serves as a central manager for loading and distributing JSON data to type-specific managers. It reads JSON data files and routes the data to appropriate managers based on object types.

#### Constructor
```cpp
DataManager()
```
Creates a new DataManager instance.

#### Methods

##### `bool RegisterManager(std::unique_ptr<BaseManager> manager)`
Registers a new manager for handling a specific object type.
- **Parameters:**
  - `manager`: Unique pointer to a manager derived from BaseManager
- **Returns:** `true` if registration was successful, `false` if type already registered

##### `bool LoadFromFile(const std::string& filePath)`
Loads and processes JSON data from a file.
- **Parameters:**
  - `filePath`: Path to the JSON file
- **Returns:** `true` if loading and processing was successful

##### `bool ProcessData(const nlohmann::json& jsonData)`
Processes JSON data directly, distributing it to registered managers.
- **Parameters:**
  - `jsonData`: The JSON data to process
- **Returns:** `true` if processing was successful

#### Example Usage
```cpp
// Create and register a sprite manager
auto spriteManager = std::make_unique<SpriteManager>();
dataManager.RegisterManager(std::move(spriteManager));

// Load and process data from a JSON file
dataManager.LoadFromFile("data/game_objects.json");
```

#### JSON Data Format
The JSON data should be structured with top-level keys matching the managed types:
```json
{
    "sprites": {
        // Sprite-specific data handled by SpriteManager
    },
    "textures": {
        // Texture-specific data handled by TextureManager
    }
}
