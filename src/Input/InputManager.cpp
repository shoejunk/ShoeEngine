#include "InputManager.h"
#include <iostream>

namespace ShoeEngine {
namespace Input {

InputManager::InputManager() = default;

bool InputManager::CreateFromJson(const nlohmann::json& jsonData) {
    try {
        if (!jsonData.is_array()) {
            std::cerr << "Invalid input configuration: expected array of inputs\n";
            return false;
        }

        for (const auto& inputData : jsonData) {
            if (!CreateInput(inputData)) {
                return false;
            }
        }

        return true;
    }
    catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << "\n";
        return false;
    }
}

bool InputManager::CreateInput(const nlohmann::json& inputData) {
    try {
        if (!inputData.contains("name") || !inputData.contains("type")) {
            std::cerr << "Invalid input data: missing required fields\n";
            return false;
        }

        std::string name = inputData["name"];
        std::string typeStr = inputData["type"];
        Input::Type type;

        if (typeStr == "keyboard") {
            type = Input::Type::Keyboard;
        }
        else if (typeStr == "mouseButton") {
            type = Input::Type::MouseButton;
        }
        else if (typeStr == "mouseAxis") {
            type = Input::Type::MouseAxis;
        }
        else {
            std::cerr << "Invalid input type: " << typeStr << "\n";
            return false;
        }

        // Create hash from input name
        Core::Hash::HashValue nameHash(name.c_str(), name.length());
        auto input = std::make_unique<Input>(nameHash, type);

        if (inputData.contains("context")) {
            std::string context = inputData["context"];
            Core::Hash::HashValue contextHash(context.c_str(), context.length());
            input->SetContext(contextHash);
        }

        // Configure the input based on its type
        if (type == Input::Type::Keyboard && inputData.contains("key")) {
            std::string keyStr = inputData["key"];
            sf::Keyboard::Key key = sf::Keyboard::Unknown;

            // Map string key names to SFML keys
            if (keyStr == "left") key = sf::Keyboard::Left;
            else if (keyStr == "right") key = sf::Keyboard::Right;
            else if (keyStr == "up") key = sf::Keyboard::Up;
            else if (keyStr == "down") key = sf::Keyboard::Down;
            else if (keyStr == "space") key = sf::Keyboard::Space;
            else if (keyStr == "escape") key = sf::Keyboard::Escape;
            else {
                std::cerr << "Unknown keyboard key: " << keyStr << "\n";
                return false;
            }

            input->SetKey(key);
        }
        else if (type == Input::Type::MouseButton && inputData.contains("button")) {
            std::string buttonStr = inputData["button"];
            sf::Mouse::Button button = sf::Mouse::Left;

            // Map string button names to SFML mouse buttons
            if (buttonStr == "left") button = sf::Mouse::Left;
            else if (buttonStr == "right") button = sf::Mouse::Right;
            else if (buttonStr == "middle") button = sf::Mouse::Middle;
            else {
                std::cerr << "Unknown mouse button: " << buttonStr << "\n";
                return false;
            }

            input->SetMouseButton(button);
        }

        m_inputs[nameHash] = std::move(input);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating input: " << e.what() << "\n";
        return false;
    }
}

void InputManager::Update() {
    for (const auto& [name, input] : m_inputs) {
        if (input->GetContext() == m_currentContext) {
            input->Update();
        }
    }
}

void InputManager::SetContext(const Core::Hash::HashValue& context) {
    m_currentContext = context;
}

Input* InputManager::GetInput(const Core::Hash::HashValue& name) {
    auto it = m_inputs.find(name);
    return it != m_inputs.end() ? it->second.get() : nullptr;
}

} // namespace Input
} // namespace ShoeEngine
