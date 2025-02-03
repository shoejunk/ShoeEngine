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

### WindowManager Class
`ShoeEngine::Graphics::WindowManager`

The WindowManager class manages the creation and lifecycle of Window objects from JSON configuration data. It inherits from BaseManager to integrate with the DataManager system.

#### Constructor
```cpp
WindowManager()
```
Creates a new WindowManager instance.

#### Methods

##### `bool CreateFromJson(const nlohmann::json& jsonData) override`
Creates Window objects based on JSON configuration data.
- **Parameters:**
  - `jsonData`: JSON object containing window configurations
- **Returns:** `true` if windows were created successfully, `false` if an error occurred

##### `std::string GetManagedType() const override`
Returns the type identifier for this manager.
- **Returns:** `"windows"` as the managed type

##### `bool ProcessEvents()`
Processes events for all managed windows.
- **Returns:** `true` if any windows are still open, `false` if all windows are closed

##### `void ClearAll()`
Clears all managed windows with a black background.

##### `void DisplayAll()`
Updates all managed windows with any changes since the last call.

##### `const std::vector<std::unique_ptr<Window>>& GetWindows() const`
Gets all managed window instances.
- **Returns:** Const reference to the vector of window pointers

#### JSON Configuration Format
```json
{
    "windows": {
        "window_name": {
            "title": "Window Title",
            "width": 800,
            "height": 600
        }
    }
}
```
- **Properties:**
  - `title`: (string, optional) Window title text, defaults to "ShoeEngine Window"
  - `width`: (number, optional) Window width in pixels, defaults to 800
  - `height`: (number, optional) Window height in pixels, defaults to 600

### Image Class
`ShoeEngine::Graphics::Image`

The Image class provides a wrapper around SFML's image functionality, allowing for loading and manipulation of image data.

#### Constructor
```cpp
Image(const uint8_t* pixels, unsigned int width, unsigned int height)
```
Creates a new image from raw pixel data.
- **Parameters:**
  - `pixels`: Raw pixel data in RGBA format
  - `width`: Image width in pixels
  - `height`: Image height in pixels

#### Methods

##### `bool LoadFromFile(const std::string& filename)`
Loads an image from a file.
- **Parameters:**
  - `filename`: Path to the image file
- **Returns:** `true` if loading was successful

##### `bool SaveToFile(const std::string& filename)`
Saves the image to a file.
- **Parameters:**
  - `filename`: Path to save the image to
- **Returns:** `true` if saving was successful

### ImageManager Class
`ShoeEngine::Graphics::ImageManager`

The ImageManager class manages the creation and storage of Image objects from JSON configuration data.

#### Methods

##### `bool CreateFromJson(const nlohmann::json& jsonData)`
Creates images from JSON configuration data.
- **Parameters:**
  - `jsonData`: JSON data containing image definitions
- **Returns:** `true` if creation was successful

##### `const Image* GetImage(const std::string& imageId) const`
Retrieves an image by its ID.
- **Parameters:**
  - `imageId`: Unique identifier for the image
- **Returns:** Pointer to the Image object, or nullptr if not found

### Sprite Class
`ShoeEngine::Graphics::Sprite`

The Sprite class wraps SFML's sprite functionality, providing methods for sprite transformation and rendering.

#### Methods

##### `void SetPosition(float x, float y)`
Sets the position of the sprite.
- **Parameters:**
  - `x`: X coordinate
  - `y`: Y coordinate

##### `void SetRotation(float angle)`
Sets the rotation of the sprite.
- **Parameters:**
  - `angle`: Rotation angle in degrees

##### `void SetScale(float x, float y)`
Sets the scale factors of the sprite.
- **Parameters:**
  - `x`: Horizontal scale factor
  - `y`: Vertical scale factor

##### `void SetOrigin(float x, float y)`
Sets the origin point of the sprite for transformations.
- **Parameters:**
  - `x`: X coordinate of the origin
  - `y`: Y coordinate of the origin

### SpriteManager Class
`ShoeEngine::Graphics::SpriteManager`

The SpriteManager class manages the creation and storage of Sprite objects from JSON configuration data.

#### Methods

##### `bool CreateFromJson(const nlohmann::json& jsonData)`
Creates sprites from JSON configuration data.
- **Parameters:**
  - `jsonData`: JSON data containing sprite definitions
- **Returns:** `true` if creation was successful

##### `const Sprite* GetSprite(const std::string& spriteId) const`
Retrieves a sprite by its ID.
- **Parameters:**
  - `spriteId`: Unique identifier for the sprite
- **Returns:** Pointer to the Sprite object, or nullptr if not found

#### JSON Configuration Format
```json
{
    "images": {
        "image_id": {
            "file": "path/to/image.png"
        }
    },
    "sprites": {
        "sprite_id": {
            "image": "image_id",
            "position": {
                "x": 0,
                "y": 0
            },
            "rotation": 0,
            "scale": {
                "x": 1,
                "y": 1
            },
            "origin": {
                "x": 0,
                "y": 0
            }
        }
    }
}
```

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
