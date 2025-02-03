# ShoeEngine Developer Guidelines

## Overview
ShoeEngine is a highly modular, data-driven 2D game engine built in C++. The engine's core philosophy is based on JSON-driven architecture, where game objects and their configurations are defined in JSON data files and instantiated through a managed factory system.

## Code Organization

### Namespace Structure
- All engine code must reside within the `ShoeEngine` namespace
- Each broad category of functionality must have its own namespace within `ShoeEngine`
- Category namespaces must correspond directly to their source directory under `src/`
- Example structure:
  ```cpp
  namespace ShoeEngine {      // All engine code
  namespace Graphics {        // src/graphics/
      class Window { ... }
  }
  namespace Physics {         // src/physics/
      class RigidBody { ... }
  }
  namespace Audio {          // src/audio/
      class SoundSystem { ... }
  }
  }  // namespace ShoeEngine
  ```

### File Structure
- Header (.h) and implementation (.cpp) files are kept together in the same directory
- Files are organized in logical directories under `src/` based on their functionality
- Example structure:
  ```
  ShoeEngine/
  ├── src/                 # Source files (.h and .cpp)
  │   ├── core/           # Core engine functionality
  │   ├── graphics/       # Graphics related code
  │   ├── input/          # Input handling
  │   ├── physics/        # Physics engine
  │   └── audio/          # Audio system
  ├── docs/               # Documentation with examples
  └── tests/              # Unit tests
  ```

### Naming Conventions
- Namespaces: PascalCase (e.g., `ShoeEngine`, `Graphics`, `Physics`)
- Classes: PascalCase (e.g., `DataManager`, `WindowManager`)
- Functions: PascalCase (e.g., `CreateWindow`, `LoadData`)
- Variables: camelCase (e.g., `windowConfig`, `spriteData`)
- Constants: UPPER_SNAKE_CASE (e.g., `MAX_WINDOW_SIZE`)
- Private member variables: m_camelCase (e.g., `m_windowManager`)
- File names: Same as class name (e.g., `WindowManager.h`, `WindowManager.cpp`)

## Documentation Standards

### Class Documentation
```cpp
/**
 * @class ClassName
 * @brief Brief description of the class
 *
 * Detailed description of the class functionality,
 * including its role in the engine.
 *
 * @note Any important notes about usage
 */
```

### Function Documentation
```cpp
/**
 * @brief Brief description of function purpose
 *
 * Detailed description of what the function does
 *
 * @param paramName Description of parameter
 * @return Description of return value
 * @throws Description of potential exceptions
 *
 * @example
 * // Example usage of the function
 * auto result = object.Function(params);
 *
 * @note Any important implementation notes
 * @see Related functions or classes
 */
```

## Core Architecture

### Data-Driven Design
1. All game objects and configurations are defined in JSON
2. The `DataManager` is the central hub for JSON parsing
3. Specialized managers handle specific object types
4. Example JSON structure:
```json
{
    "window": {
        "title": "Game Window",
        "width": 800,
        "height": 600
    },
    "sprites": [
        {
            "id": "player",
            "texture": "player.png",
            "animations": [...]
        }
    ]
}
```

### Manager System
1. `DataManager`: Central JSON parsing and distribution
2. Specialized Managers (e.g., `WindowManager`, `SpriteManager`):
   - Register with `DataManager`
   - Receive relevant JSON data
   - Create and manage their specific object types

### Error Handling
- Use exceptions for exceptional cases
- Return values or std::optional for expected failure cases
- All errors should be logged with meaningful messages
- Example:
```cpp
class GameError : public std::runtime_error {
    // Custom error class for game-specific errors
};
```

## Best Practices

### Memory Management
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) over raw pointers
- Follow RAII principles
- Avoid circular dependencies
- Use move semantics where appropriate

### Performance
- Minimize heap allocations
- Use references for passing large objects
- Consider object pooling for frequently created/destroyed objects
- Profile early and often

### Testing
- Every class MUST have a corresponding unit test suite
- Unit tests should be organized to mirror the source code structure
- Test files should be named `{ClassName}Tests.cpp`
- Write unit tests for all public interfaces
- Test edge cases and error conditions
- Use mock objects for testing manager interactions
- Aim for high code coverage (target: >90%)
- Example test structure:
```cpp
// WindowManagerTests.cpp
TEST_CASE("WindowManager basic operations") {
    SECTION("creates window from valid JSON") {
        // Test code
    }
    
    SECTION("handles invalid JSON gracefully") {
        // Test code
    }
    
    SECTION("manages multiple windows") {
        // Test code
    }
}
```

Test organization example:
```
tests/
├── core/
│   ├── DataManagerTests.cpp
│   └── WindowManagerTests.cpp
├── graphics/
│   └── SpriteManagerTests.cpp
└── TestMain.cpp
```

No pull request should be merged without corresponding unit tests for new functionality.

## JSON Schema Standards
All JSON configuration files should follow predefined schemas. Example:
```json
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "type": "object",
    "properties": {
        "window": {
            "type": "object",
            "required": ["title", "width", "height"],
            "properties": {
                "title": { "type": "string" },
                "width": { "type": "integer", "minimum": 1 },
                "height": { "type": "integer", "minimum": 1 }
            }
        }
    }
}
```

## Version Control Guidelines
- Use descriptive commit messages
- Follow semantic versioning
- Create feature branches for new features
- Use pull requests for code review

## Build System
- Use CMake for cross-platform build configuration
- Use CPM.cmake for dependency management
- Maintain separate debug and release configurations
- Example CMake structure:
  ```cmake
  # Minimum required CMake version
  cmake_minimum_required(VERSION 3.14)
  
  # Project declaration
  project(ShoeEngine VERSION 0.1.0)
  
  # Download CPM.cmake
  set(CPM_DOWNLOAD_VERSION 0.38.6)
  set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
  if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake...")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION}
    )
  endif()
  include(${CPM_DOWNLOAD_LOCATION})
  
  # Example dependency inclusion with CPM
  CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 10.1.1
  )
  ```

## Dependencies
- Use CPM.cmake for automated dependency management
- All dependencies should be declared in the root CMakeLists.txt
- Each dependency should specify an exact version
- Dependencies are automatically downloaded and built during CMake configuration
- Example dependency declaration:
  ```cmake
  CPMAddPackage(
    NAME dependency_name
    GITHUB_REPOSITORY user/repo
    GIT_TAG v1.0.0
  )
  ```

## Contribution Guidelines
1. Fork the repository
2. Create a feature branch
3. Follow coding standards
4. Write tests
5. Submit pull request
6. Update documentation

## Examples
All examples of engine usage will be included directly in the API documentation of relevant classes and functions.
