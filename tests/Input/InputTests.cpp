#include <gtest/gtest.h>
#include "input/Input.h"
#include "input/InputManager.h"
#include <nlohmann/json.hpp>
#include "Core/DataManager.h"

using namespace ShoeEngine;
using namespace ShoeEngine::Input;

TEST(InputTests, ContextActivation) {
	Core::DataManager dm;
	KeyboardInput input("test"_h);

	input.SetContext(dm.RegisterString("combat"));

	// Test matching context
	EXPECT_TRUE(input.IsActive(dm.RegisterString("combat")));

	// Test non-matching context
	EXPECT_FALSE(input.IsActive(dm.RegisterString("menu")));
}

TEST(InputManagerTests, ContextFiltering) {
	Core::DataManager dm;
	InputManager im(dm);

	nlohmann::json jsonData = { {
		{"name", "attack"},
		{"type", "keyboard"},
		{"key", "space"},
		{"context", "combat"}
	} };

	im.CreateFromJson(jsonData);
	im.SetContext(dm.RegisterString("menu"));

	auto* input = im.GetInput("attack"_h);
	ASSERT_NE(input, nullptr);
	EXPECT_FALSE(input->IsActive(im.GetCurrentContext()));
}