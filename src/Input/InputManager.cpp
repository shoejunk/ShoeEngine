#include "InputManager.h"
#include "core/DataManager.h"
#include <nlohmann/json.hpp>
#include <iostream>

namespace ShoeEngine {
	namespace Input {

		using namespace Core;

		InputManager::InputManager(Core::DataManager& dataManager)
			: BaseManager(dataManager)
		{
			// Create default global context
			auto& globalContext = m_contexts["global"_h];
			globalContext.nameHash = "global"_h;
			globalContext.name = "global";
			m_activeContextStack.push_back("global"_h);
			m_dataManager.RegisterString("inputs");
		}

		bool InputManager::CreateFromJson(const nlohmann::json& jsonData) {
			try {
				for (const auto& [contextName, inputArray] : jsonData.items()) {
					// Register context name
					const Hash::HashValue contextHash = m_dataManager.RegisterString(contextName);

					InputContext* context = GetOrCreateContext(contextName);

					for (const auto& inputData : inputArray) {
						auto input = CreateInput(inputData);
						if (!input) continue;

						// Register input name
						const Hash::HashValue inputNameHash = m_dataManager.RegisterString(inputData.at("name"));

						context->inputs[inputNameHash] = std::move(input);
					}
				}
				return true;
			}
			catch (const nlohmann::json::exception& e) {
				std::cerr << "InputManager JSON error: " << e.what() << "\n";
				return false;
			}
		}

		Hash::HashValue InputManager::GetManagedType() const {
			return "inputs"_h;
		}

		nlohmann::json InputManager::SerializeToJson() {
			nlohmann::json output;

			for (const auto& [contextHash, context] : m_contexts) {
				nlohmann::json contextJson = nlohmann::json::array();

				for (const auto& [inputHash, input] : context.inputs) {
					nlohmann::json inputJson;
					inputJson["name"] = m_dataManager.GetString(inputHash);
					inputJson["type"] = [&]() {
						switch (input->GetType()) {
						case Input::Type::Keyboard: return "keyboard";
						case Input::Type::MouseButton: return "mouseButton";
						case Input::Type::MouseAxis: return "mouseAxis";
						default: return "unknown";
						}
						}();

					// Add type-specific data
					switch (input->GetType()) {
					case Input::Type::Keyboard: {
						auto kbInput = static_cast<KeyboardInput*>(input.get());
						inputJson["key"] = m_dataManager.GetString(
							KeyToHash(kbInput->GetKey())
						);
						break;
					}
					case Input::Type::MouseButton: {
						auto mbInput = static_cast<MouseButtonInput*>(input.get());
						inputJson["button"] = mbInput->GetButton();
						break;
					}
					case Input::Type::MouseAxis: {
						auto maInput = static_cast<MouseAxisInput*>(input.get());
						inputJson["axis"] = maInput->IsXAxis() ? "x" : "y";
						break;
					}
					}

					contextJson.push_back(inputJson);
				}

				output[context.name] = contextJson;
			}

			return output;
		}

		void InputManager::PushContext(const std::string& contextName) {
			const Hash::HashValue contextHash = m_dataManager.RegisterString(contextName);

			if (m_contexts.find(contextHash) == m_contexts.end()) {
				GetOrCreateContext(contextName);
			}

			m_activeContextStack.push_back(contextHash);
		}

		void InputManager::PopContext() {
			if (m_activeContextStack.size() > 1) {
				m_activeContextStack.pop_back();
			}
		}

		Input* InputManager::GetInput(Hash::HashValue name) {
			// Check contexts in reverse order (newest first)
			for (auto it = m_activeContextStack.rbegin(); it != m_activeContextStack.rend(); ++it) {
				auto& context = m_contexts[*it];
				auto inputIt = context.inputs.find(name);
				if (inputIt != context.inputs.end()) {
					return inputIt->second.get();
				}
			}
			return nullptr;
		}

		InputManager::InputContext* InputManager::GetOrCreateContext(const std::string& contextName) {
			const Hash::HashValue contextHash = m_dataManager.RegisterString(contextName);

			auto& context = m_contexts[contextHash];
			if (context.name.empty()) {
				context.name = contextName;
				context.nameHash = contextHash;
			}
			return &context;
		}

		std::unique_ptr<Input> InputManager::CreateInput(const nlohmann::json& inputData) {
			try {
				const std::string typeStr = inputData.at("type");
				const std::string name = inputData.at("name");
				const Hash::HashValue nameHash = m_dataManager.RegisterString(name);

				if (typeStr == "keyboard") {
					const std::string keyStr = inputData.at("key");
					auto keyHash = m_dataManager.RegisterString(keyStr);

					const auto key = HashToKey(keyHash);

					auto input = std::make_unique<KeyboardInput>(nameHash);
					input->SetKey(key);
					return input;
				}
				else if (typeStr == "mouseButton") {
					const int button = inputData.at("button");
					auto input = std::make_unique<MouseButtonInput>(nameHash);
					input->SetButton(static_cast<sf::Mouse::Button>(button));
					return input;
				}
				else if (typeStr == "mouseAxis") {
					const std::string axis = inputData.at("axis");
					auto input = std::make_unique<MouseAxisInput>(nameHash, axis == "x");
					return input;
				}

				std::cerr << "Unknown input type: " << typeStr << "\n";
				return nullptr;
			}
			catch (const nlohmann::json::exception& e) {
				std::cerr << "Input creation error: " << e.what() << "\n";
				return nullptr;
			}
		}

		sf::Keyboard::Key InputManager::HashToKey(Core::Hash::HashValue keyHash) const {
			switch (keyHash) {
			case "A"_h: return sf::Keyboard::A;
			case "B"_h: return sf::Keyboard::B;
			case "C"_h: return sf::Keyboard::C;
			case "D"_h: return sf::Keyboard::D;
			case "E"_h: return sf::Keyboard::E;
			case "F"_h: return sf::Keyboard::F;
			case "G"_h: return sf::Keyboard::G;
			case "H"_h: return sf::Keyboard::H;
			case "I"_h: return sf::Keyboard::I;
			case "J"_h: return sf::Keyboard::J;
			case "K"_h: return sf::Keyboard::K;
			case "L"_h: return sf::Keyboard::L;
			case "M"_h: return sf::Keyboard::M;
			case "N"_h: return sf::Keyboard::N;
			case "O"_h: return sf::Keyboard::O;
			case "P"_h: return sf::Keyboard::P;
			case "Q"_h: return sf::Keyboard::Q;
			case "R"_h: return sf::Keyboard::R;
			case "S"_h: return sf::Keyboard::S;
			case "T"_h: return sf::Keyboard::T;
			case "U"_h: return sf::Keyboard::U;
			case "V"_h: return sf::Keyboard::V;
			case "W"_h: return sf::Keyboard::W;
			case "X"_h: return sf::Keyboard::X;
			case "Y"_h: return sf::Keyboard::Y;
			case "Z"_h: return sf::Keyboard::Z;

			case "Num0"_h: return sf::Keyboard::Num0;
			case "Num1"_h: return sf::Keyboard::Num1;
			case "Num2"_h: return sf::Keyboard::Num2;
			case "Num3"_h: return sf::Keyboard::Num3;
			case "Num4"_h: return sf::Keyboard::Num4;
			case "Num5"_h: return sf::Keyboard::Num5;
			case "Num6"_h: return sf::Keyboard::Num6;
			case "Num7"_h: return sf::Keyboard::Num7;
			case "Num8"_h: return sf::Keyboard::Num8;
			case "Num9"_h: return sf::Keyboard::Num9;

			case "Escape"_h: return sf::Keyboard::Escape;
			case "LControl"_h: return sf::Keyboard::LControl;
			case "LShift"_h: return sf::Keyboard::LShift;
			case "LAlt"_h: return sf::Keyboard::LAlt;
			case "LSystem"_h: return sf::Keyboard::LSystem;
			case "RControl"_h: return sf::Keyboard::RControl;
			case "RShift"_h: return sf::Keyboard::RShift;
			case "RAlt"_h: return sf::Keyboard::RAlt;
			case "RSystem"_h: return sf::Keyboard::RSystem;
			case "Menu"_h: return sf::Keyboard::Menu;

			case "LBracket"_h: return sf::Keyboard::LBracket;
			case "RBracket"_h: return sf::Keyboard::RBracket;
			case "Semicolon"_h: return sf::Keyboard::Semicolon;
			case "Comma"_h: return sf::Keyboard::Comma;
			case "Period"_h: return sf::Keyboard::Period;
			case "Quote"_h: return sf::Keyboard::Quote;
			case "Slash"_h: return sf::Keyboard::Slash;
			case "Backslash"_h: return sf::Keyboard::Backslash;
			case "Tilde"_h: return sf::Keyboard::Tilde;
			case "Equal"_h: return sf::Keyboard::Equal;
			case "Hyphen"_h: return sf::Keyboard::Hyphen;
			case "Space"_h: return sf::Keyboard::Space;
			case "Enter"_h: return sf::Keyboard::Enter;
			case "Backspace"_h: return sf::Keyboard::Backspace;
			case "Tab"_h: return sf::Keyboard::Tab;

			case "PageUp"_h: return sf::Keyboard::PageUp;
			case "PageDown"_h: return sf::Keyboard::PageDown;
			case "End"_h: return sf::Keyboard::End;
			case "Home"_h: return sf::Keyboard::Home;
			case "Insert"_h: return sf::Keyboard::Insert;
			case "Delete"_h: return sf::Keyboard::Delete;

			case "Add"_h: return sf::Keyboard::Add;
			case "Subtract"_h: return sf::Keyboard::Subtract;
			case "Multiply"_h: return sf::Keyboard::Multiply;
			case "Divide"_h: return sf::Keyboard::Divide;

			case "Left"_h: return sf::Keyboard::Left;
			case "Right"_h: return sf::Keyboard::Right;
			case "Up"_h: return sf::Keyboard::Up;
			case "Down"_h: return sf::Keyboard::Down;

			case "Numpad0"_h: return sf::Keyboard::Numpad0;
			case "Numpad1"_h: return sf::Keyboard::Numpad1;
			case "Numpad2"_h: return sf::Keyboard::Numpad2;
			case "Numpad3"_h: return sf::Keyboard::Numpad3;
			case "Numpad4"_h: return sf::Keyboard::Numpad4;
			case "Numpad5"_h: return sf::Keyboard::Numpad5;
			case "Numpad6"_h: return sf::Keyboard::Numpad6;
			case "Numpad7"_h: return sf::Keyboard::Numpad7;
			case "Numpad8"_h: return sf::Keyboard::Numpad8;
			case "Numpad9"_h: return sf::Keyboard::Numpad9;

			case "F1"_h: return sf::Keyboard::F1;
			case "F2"_h: return sf::Keyboard::F2;
			case "F3"_h: return sf::Keyboard::F3;
			case "F4"_h: return sf::Keyboard::F4;
			case "F5"_h: return sf::Keyboard::F5;
			case "F6"_h: return sf::Keyboard::F6;
			case "F7"_h: return sf::Keyboard::F7;
			case "F8"_h: return sf::Keyboard::F8;
			case "F9"_h: return sf::Keyboard::F9;
			case "F10"_h: return sf::Keyboard::F10;
			case "F11"_h: return sf::Keyboard::F11;
			case "F12"_h: return sf::Keyboard::F12;
			case "F13"_h: return sf::Keyboard::F13;
			case "F14"_h: return sf::Keyboard::F14;
			case "F15"_h: return sf::Keyboard::F15;

			case "Pause"_h: return sf::Keyboard::Pause;

			default: return sf::Keyboard::Unknown;
			}
		}

		Core::Hash::HashValue InputManager::KeyToHash(sf::Keyboard::Key key) const {
			switch (key) {
			case sf::Keyboard::A:           return "A"_h;
			case sf::Keyboard::B:           return "B"_h;
			case sf::Keyboard::C:           return "C"_h;
			case sf::Keyboard::D:           return "D"_h;
			case sf::Keyboard::E:           return "E"_h;
			case sf::Keyboard::F:           return "F"_h;
			case sf::Keyboard::G:           return "G"_h;
			case sf::Keyboard::H:           return "H"_h;
			case sf::Keyboard::I:           return "I"_h;
			case sf::Keyboard::J:           return "J"_h;
			case sf::Keyboard::K:           return "K"_h;
			case sf::Keyboard::L:           return "L"_h;
			case sf::Keyboard::M:           return "M"_h;
			case sf::Keyboard::N:           return "N"_h;
			case sf::Keyboard::O:           return "O"_h;
			case sf::Keyboard::P:           return "P"_h;
			case sf::Keyboard::Q:           return "Q"_h;
			case sf::Keyboard::R:           return "R"_h;
			case sf::Keyboard::S:           return "S"_h;
			case sf::Keyboard::T:           return "T"_h;
			case sf::Keyboard::U:           return "U"_h;
			case sf::Keyboard::V:           return "V"_h;
			case sf::Keyboard::W:           return "W"_h;
			case sf::Keyboard::X:           return "X"_h;
			case sf::Keyboard::Y:           return "Y"_h;
			case sf::Keyboard::Z:           return "Z"_h;

			case sf::Keyboard::Num0:        return "Num0"_h;
			case sf::Keyboard::Num1:        return "Num1"_h;
			case sf::Keyboard::Num2:        return "Num2"_h;
			case sf::Keyboard::Num3:        return "Num3"_h;
			case sf::Keyboard::Num4:        return "Num4"_h;
			case sf::Keyboard::Num5:        return "Num5"_h;
			case sf::Keyboard::Num6:        return "Num6"_h;
			case sf::Keyboard::Num7:        return "Num7"_h;
			case sf::Keyboard::Num8:        return "Num8"_h;
			case sf::Keyboard::Num9:        return "Num9"_h;

			case sf::Keyboard::Escape:      return "Escape"_h;
			case sf::Keyboard::LControl:    return "LControl"_h;
			case sf::Keyboard::LShift:      return "LShift"_h;
			case sf::Keyboard::LAlt:        return "LAlt"_h;
			case sf::Keyboard::LSystem:     return "LSystem"_h;
			case sf::Keyboard::RControl:    return "RControl"_h;
			case sf::Keyboard::RShift:      return "RShift"_h;
			case sf::Keyboard::RAlt:        return "RAlt"_h;
			case sf::Keyboard::RSystem:     return "RSystem"_h;
			case sf::Keyboard::Menu:        return "Menu"_h;

			case sf::Keyboard::LBracket:    return "LBracket"_h;
			case sf::Keyboard::RBracket:    return "RBracket"_h;
			case sf::Keyboard::Semicolon:   return "Semicolon"_h;
			case sf::Keyboard::Comma:       return "Comma"_h;
			case sf::Keyboard::Period:      return "Period"_h;
			case sf::Keyboard::Quote:       return "Quote"_h;
			case sf::Keyboard::Slash:       return "Slash"_h;
			case sf::Keyboard::Backslash:   return "Backslash"_h;
			case sf::Keyboard::Tilde:       return "Tilde"_h;
			case sf::Keyboard::Equal:       return "Equal"_h;
			case sf::Keyboard::Hyphen:      return "Hyphen"_h;
			case sf::Keyboard::Space:       return "Space"_h;
			case sf::Keyboard::Enter:       return "Enter"_h;
			case sf::Keyboard::Backspace:   return "Backspace"_h;
			case sf::Keyboard::Tab:         return "Tab"_h;

			case sf::Keyboard::PageUp:      return "PageUp"_h;
			case sf::Keyboard::PageDown:    return "PageDown"_h;
			case sf::Keyboard::End:         return "End"_h;
			case sf::Keyboard::Home:        return "Home"_h;
			case sf::Keyboard::Insert:      return "Insert"_h;
			case sf::Keyboard::Delete:      return "Delete"_h;

			case sf::Keyboard::Add:         return "Add"_h;
			case sf::Keyboard::Subtract:    return "Subtract"_h;
			case sf::Keyboard::Multiply:    return "Multiply"_h;
			case sf::Keyboard::Divide:      return "Divide"_h;

			case sf::Keyboard::Left:        return "Left"_h;
			case sf::Keyboard::Right:       return "Right"_h;
			case sf::Keyboard::Up:          return "Up"_h;
			case sf::Keyboard::Down:        return "Down"_h;

			case sf::Keyboard::Numpad0:     return "Numpad0"_h;
			case sf::Keyboard::Numpad1:     return "Numpad1"_h;
			case sf::Keyboard::Numpad2:     return "Numpad2"_h;
			case sf::Keyboard::Numpad3:     return "Numpad3"_h;
			case sf::Keyboard::Numpad4:     return "Numpad4"_h;
			case sf::Keyboard::Numpad5:     return "Numpad5"_h;
			case sf::Keyboard::Numpad6:     return "Numpad6"_h;
			case sf::Keyboard::Numpad7:     return "Numpad7"_h;
			case sf::Keyboard::Numpad8:     return "Numpad8"_h;
			case sf::Keyboard::Numpad9:     return "Numpad9"_h;

			case sf::Keyboard::F1:          return "F1"_h;
			case sf::Keyboard::F2:          return "F2"_h;
			case sf::Keyboard::F3:          return "F3"_h;
			case sf::Keyboard::F4:          return "F4"_h;
			case sf::Keyboard::F5:          return "F5"_h;
			case sf::Keyboard::F6:          return "F6"_h;
			case sf::Keyboard::F7:          return "F7"_h;
			case sf::Keyboard::F8:          return "F8"_h;
			case sf::Keyboard::F9:          return "F9"_h;
			case sf::Keyboard::F10:         return "F10"_h;
			case sf::Keyboard::F11:         return "F11"_h;
			case sf::Keyboard::F12:         return "F12"_h;
			case sf::Keyboard::F13:         return "F13"_h;
			case sf::Keyboard::F14:         return "F14"_h;
			case sf::Keyboard::F15:         return "F15"_h;

			case sf::Keyboard::Pause:       return "Pause"_h;

			default:                        return "Unknown"_h;
			}
		}

	} // namespace Input
} // namespace ShoeEngine