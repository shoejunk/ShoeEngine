#include "WindowManager.h"

namespace ShoeEngine {
namespace Graphics {

bool WindowManager::CreateFromJson(const nlohmann::json& jsonData) {
    try {
        for (const auto& [windowName, windowConfig] : jsonData.items()) {
            // Handle both empty and non-empty JSON objects
            const nlohmann::json& config = windowConfig.is_object() ? windowConfig : nlohmann::json::object();
            
            // Use value() with default values for optional fields
            std::string title = config.value("title", "ShoeEngine Window");
            unsigned int width = config.value<unsigned int>("width", 800);
            unsigned int height = config.value<unsigned int>("height", 600);

            m_windows.push_back(std::make_unique<Window>(title, width, height));
        }
        return true;
    }
    catch (const std::exception& e) {
        // Log error here when logging system is implemented
        return false;
    }
}

std::string WindowManager::GetManagedType() const {
    return "windows";
}

bool WindowManager::ProcessEvents() {
    bool anyWindowOpen = false;
    for (auto& window : m_windows) {
        if (window->IsOpen()) {
            if (window->ProcessEvents()) {
                anyWindowOpen = true;
            }
        }
    }
    return anyWindowOpen;
}

void WindowManager::ClearAll() {
    for (auto& window : m_windows) {
        if (window->IsOpen()) {
            window->Clear();
        }
    }
}

void WindowManager::DisplayAll() {
    for (auto& window : m_windows) {
        if (window->IsOpen()) {
            window->Display();
        }
    }
}

const std::vector<std::unique_ptr<Window>>& WindowManager::GetWindows() const {
    return m_windows;
}

} // namespace Graphics
} // namespace ShoeEngine
