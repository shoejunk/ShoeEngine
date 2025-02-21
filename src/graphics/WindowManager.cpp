#include "WindowManager.h"
#include "core/Hash.h"
#include "core/DataManager.h"

using namespace ShoeEngine::Core;

namespace ShoeEngine {
namespace Graphics {

WindowManager::WindowManager(Core::DataManager& dataManager)
	: Core::BaseManager(dataManager) {
	m_dataManager.RegisterString("windows");
}

bool WindowManager::CreateFromJson(const nlohmann::json& jsonData) {
    if (!jsonData.is_object()) {
        return false;
    }

    try {
        for (const auto& [windowName, windowConfig] : jsonData.items()) {
            if (!windowConfig.is_object()) {
                return false;
            }

            bool makeNew = true;
            size_t windowIndex = 0;

            // Check if the window name is already registered
            for (size_t i = 0; i < m_windowHashes.size(); ++i) {
                Core::Hash::HashValue window = m_windowHashes[i];
                if (window == Core::Hash::HashValue(windowName)) {
                    makeNew = false;
                    windowIndex = i;
                    break;
                }
            }

            // Use default values for optional fields
            std::string title = windowConfig.value("title", "ShoeEngine Window");
            Core::Hash::HashValue titleHash = m_dataManager.RegisterString(title);
            unsigned int width = windowConfig.value<unsigned int>("width", 800);
            unsigned int height = windowConfig.value<unsigned int>("height", 600);

            Core::Hash::HashValue windowHash = m_dataManager.RegisterString(windowName);
            m_windowHashes.push_back(windowHash);

            if (makeNew) {
                m_windows.push_back(std::make_unique<Window>(title, width, height));
            } else {
                auto& window = m_windows[windowIndex];
                window->SetTitleHash(titleHash);
                window->GetRenderWindow().setTitle(title);
                window->GetRenderWindow().setSize(sf::Vector2u(width, height));
            }
        }
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

Hash::HashValue WindowManager::GetManagedType() const {
    return "windows"_h;
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

nlohmann::json WindowManager::SerializeToJson() {
	nlohmann::json windowsObject = nlohmann::json::object();
	// Assume that m_windows and m_windowHashes are kept in sync.
	for (size_t i = 0; i < m_windows.size(); ++i) {
		const auto& window = m_windows[i];
		nlohmann::json windowJson;
		// These getters are assumed to be available in the Window class.
		windowJson["title"] = m_dataManager.GetString(window->GetTitleHash());
		windowJson["width"] = window->GetWidth();
		windowJson["height"] = window->GetHeight();

		// Retrieve the original window name using the stored hash.
		std::string windowName = m_dataManager.GetString(m_windowHashes[i]);
		windowsObject[windowName] = windowJson;
	}

	return windowsObject;
}

} // namespace Graphics
} // namespace ShoeEngine
