#include <gtest/gtest.h>
#include "input/Input.h"
#include "input/InputManager.h"
#include <nlohmann/json.hpp>
#include "core/DataManager.h"

using namespace ShoeEngine;
using namespace ShoeEngine::Input;

TEST(InputManagerTest, ContextFiltering) {
    Core::DataManager dm;
    InputManager im(dm);

    nlohmann::json jsonData = {
        {"combat", {
            {
                {"name", "attack"},
                {"type", "keyboard"},
                {"key", "space"}
            }
        }},
        {"menu", {
            {
                {"name", "menu_select"},
                {"type", "keyboard"},
                {"key", "enter"}
            }
        }}
    };

    ASSERT_TRUE(im.CreateFromJson(jsonData));

    // Test without any context
    EXPECT_EQ(im.GetInput("attack"_h), nullptr);
    EXPECT_EQ(im.GetInput("menu_select"_h), nullptr);

    // Test with combat context
    im.PushContext("combat");
    auto* attackInput = im.GetInput("attack"_h);
    ASSERT_NE(attackInput, nullptr);
    EXPECT_EQ(im.GetInput("menu_select"_h), nullptr);

    // Pop combat context before pushing menu context
    im.PopContext();

    // Test with menu context
    im.PushContext("menu");
    auto* menuInput = im.GetInput("menu_select"_h);
    ASSERT_NE(menuInput, nullptr);
    EXPECT_EQ(im.GetInput("attack"_h), nullptr);

    // Test popping context
    im.PopContext();
    EXPECT_EQ(im.GetInput("attack"_h), nullptr);
    EXPECT_EQ(im.GetInput("menu_select"_h), nullptr);
}