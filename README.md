# ShoeEngine

A highly modular, data-driven 2D game engine built in C++. The engine's core philosophy is based on JSON-driven architecture, where game objects and their configurations are defined in JSON data files and instantiated through a managed factory system.

## Features

- Data-driven architecture using JSON configuration
- Modular design with clear separation of concerns
- Comprehensive unit testing for all components
- Modern C++17 implementation
- Dependency management via CPM.cmake

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

## Documentation

For detailed information about coding standards, architecture, and contribution guidelines, see [Developer Guidelines](docs/DeveloperGuidelines.md).

## License

[MIT License](LICENSE)
