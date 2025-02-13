#include "Input.h"
#include <SFML/Window/Mouse.hpp>

namespace ShoeEngine {
	namespace Input {

		void MouseAxisInput::Update() {
			// Update mouse position tracking
			sf::Vector2i currentPos = sf::Mouse::getPosition();
			m_lastMousePos = currentPos;
		}

		bool MouseAxisInput::IsActive() const {
			sf::Vector2i currentPos = sf::Mouse::getPosition();
			return m_isXAxis ? (currentPos.x != m_lastMousePos.x)
				: (currentPos.y != m_lastMousePos.y);
		}

	} // namespace Input
} // namespace ShoeEngine