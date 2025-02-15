#include "Window.h"

namespace ShoeEngine {
namespace Graphics {

Window::Window(const std::string& title, unsigned int width, unsigned int height)
    : m_window(sf::VideoMode(width, height), title)
{
	m_titleHash = Core::Hash::HashValue(title);
}

Window::~Window() {
    if (m_window.isOpen()) {
        m_window.close();
    }
}

bool Window::IsOpen() const {
    return m_window.isOpen();
}

void Window::Close() {
    m_window.close();
}

bool Window::ProcessEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
            return false;
        }
    }
    return true;
}

void Window::Display() {
    m_window.display();
}

void Window::Clear() {
    m_window.clear(sf::Color::Black);
}

sf::RenderWindow& Window::GetRenderWindow() {
    return m_window;
}

} // namespace Graphics
} // namespace ShoeEngine
