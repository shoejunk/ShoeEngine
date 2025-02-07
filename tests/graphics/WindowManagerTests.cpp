#include <gtest/gtest.h>
#include "graphics/WindowManager.h"
#include "core/Hash.h"
#include <nlohmann/json.hpp>

using namespace ShoeEngine::Graphics;
using namespace ShoeEngine::Core;
using json = nlohmann::json;

class WindowManagerTests : public ::testing::Test {
protected:
    WindowManager manager;
};

TEST_F(WindowManagerTests, GetManagedTypeReturnsWindows) {
    EXPECT_EQ(manager.GetManagedType(), "windows"_h);
}

TEST_F(WindowManagerTests, CreateFromJsonWithValidData) {
    json windowData = {
        {"main_window", {
            {"title", "Test Window"},
            {"width", 800},
            {"height", 600}
        }}
    };

    EXPECT_TRUE(manager.CreateFromJson(windowData));
    EXPECT_EQ(manager.GetWindows().size(), 1);
}

TEST_F(WindowManagerTests, CreateFromJsonWithMultipleWindows) {
    json windowData = {
        {"main_window", {
            {"title", "Main Window"},
            {"width", 800},
            {"height", 600}
        }},
        {"debug_window", {
            {"title", "Debug Window"},
            {"width", 400},
            {"height", 300}
        }}
    };

    EXPECT_TRUE(manager.CreateFromJson(windowData));
    EXPECT_EQ(manager.GetWindows().size(), 2);
}

TEST_F(WindowManagerTests, CreateFromJsonWithDefaultValues) {
    json windowData = {
        {"minimal_window", {}}
    };

    EXPECT_TRUE(manager.CreateFromJson(windowData));
    EXPECT_EQ(manager.GetWindows().size(), 1);
}

TEST_F(WindowManagerTests, CreateFromJsonWithInvalidData) {
    json invalidData = {
        {"bad_window", {
            {"width", "not_a_number"},
            {"height", "also_not_a_number"}
        }}
    };

    EXPECT_FALSE(manager.CreateFromJson(invalidData));
    EXPECT_EQ(manager.GetWindows().size(), 0);
}
