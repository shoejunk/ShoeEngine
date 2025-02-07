#include <gtest/gtest.h>
#include "input/Input.h"
#include "input/InputManager.h"
#include <nlohmann/json.hpp>

using namespace ShoeEngine::Input;

class InputTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(InputTest, CreateKeyboardInput) {
    Input input("jump"_h, Input::Type::Keyboard);
    input.SetKey(sf::Keyboard::Space);
    EXPECT_EQ(input.GetName(), "jump"_h);
}

TEST_F(InputTest, CreateMouseInput) {
    Input input("shoot"_h, Input::Type::MouseButton);
    input.SetMouseButton(sf::Mouse::Left);
    EXPECT_EQ(input.GetName(), "shoot"_h);
}

TEST_F(InputTest, InputContext) {
    Input input("menu"_h, Input::Type::Keyboard);
    input.SetContext("MainMenu"_h);
    EXPECT_EQ(input.GetContext(), "MainMenu"_h);
}

class InputManagerTest : public ::testing::Test {
protected:
    InputManager manager;
    
    void SetUp() override {
    }
};

TEST_F(InputManagerTest, CreateFromValidJson) {
    nlohmann::json jsonData = {{
        {"name", "jump"},
        {"type", "keyboard"},
        {"key", "space"},
        {"context", "gameplay"}
    }};
    
    EXPECT_TRUE(manager.CreateFromJson(jsonData));
    EXPECT_NE(manager.GetInput("jump"_h), nullptr);
}

TEST_F(InputManagerTest, CreateFromInvalidJson) {
    nlohmann::json jsonData = {{
        {"type", "keyboard"} // Missing name
    }};
    
    EXPECT_FALSE(manager.CreateFromJson(jsonData));
}

TEST_F(InputManagerTest, ContextHandling) {
    nlohmann::json jsonData = {{
        {"name", "menu"},
        {"type", "keyboard"},
        {"key", "escape"},
        {"context", "gameplay"}
    }};
    
    EXPECT_TRUE(manager.CreateFromJson(jsonData));
    manager.SetContext("gameplay"_h);
    auto input = manager.GetInput("menu"_h);
    EXPECT_NE(input, nullptr);
    EXPECT_EQ(input->GetContext(), "gameplay"_h);
}
