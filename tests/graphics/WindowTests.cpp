#include <gtest/gtest.h>
#include "graphics/Window.h"

namespace ShoeEngine {
namespace Graphics {
namespace Tests {

TEST(WindowTests, Constructor_CreatesOpenWindow) {
    Window window("Test Window", 800, 600);
    EXPECT_TRUE(window.IsOpen());
}

TEST(WindowTests, Close_ClosesWindow) {
    Window window("Test Window", 800, 600);
    window.Close();
    EXPECT_FALSE(window.IsOpen());
}

TEST(WindowTests, GetRenderWindow_ReturnsValidWindow) {
    Window window("Test Window", 800, 600);
    sf::RenderWindow& renderWindow = window.GetRenderWindow();
    EXPECT_TRUE(renderWindow.isOpen());
}

} // namespace Tests
} // namespace Graphics
} // namespace ShoeEngine
