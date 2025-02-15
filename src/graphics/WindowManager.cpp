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
    try {
        for (const auto& [windowName, windowConfig] : jsonData.items()) {
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

			// Handle both empty and non-empty JSON objects
            const nlohmann::json& config = windowConfig.is_object() ? windowConfig : nlohmann::json::object();
            
            // Use value() with default values for optional fields
            std::string title = config.value("title", "ShoeEngine Window");
			Core::Hash::HashValue titleHash = m_dataManager.RegisterString(title);
            unsigned int width = config.value<unsigned int>("width", 800);
            unsigned int height = config.value<unsigned int>("height", 600);

			// Register the window name with DataManager to obtain a hash for later retrieval.
			Core::Hash::HashValue windowHash = m_dataManager.RegisterString(windowName);
			m_windowHashes.push_back(windowHash);

			if (makeNew) {
				m_windows.push_back(std::make_unique<Window>(title, width, height));
			}
			else {
				// Update the existing window
				auto& window = m_windows[windowIndex];
				window->SetTitleHash(titleHash);
				window->GetRenderWindow().setTitle(title);
				window->GetRenderWindow().setSize(sf::Vector2u(width, height));
			}
        }
        return true;
    }
    catch (const std::exception&) {
        // Log error here when logging system is implemented
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
