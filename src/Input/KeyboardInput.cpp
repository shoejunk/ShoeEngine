#include "Input/Input.h"
#include <SFML/Window/Keyboard.hpp>

namespace ShoeEngine {
	namespace Input {

		void KeyboardInput::Update() {
			// State updated during IsActive check
		}

		bool KeyboardInput::IsActive() const {
			return sf::Keyboard::isKeyPressed(m_key);
		}

	} // namespace Input
} // namespace ShoeEngine