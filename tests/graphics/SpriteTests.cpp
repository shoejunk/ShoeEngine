#include <gtest/gtest.h>
#include "graphics/Sprite.h"
#include <vector>

using namespace ShoeEngine::Graphics;

class SpriteTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a small test image (2x2 red square)
        testPixels = std::vector<uint8_t>(2 * 2 * 4, 0);
        for (size_t i = 0; i < testPixels.size(); i += 4) {
            testPixels[i] = 255;     // R
            testPixels[i + 1] = 0;   // G
            testPixels[i + 2] = 0;   // B
            testPixels[i + 3] = 255; // A
        }
        testImage = Image(testPixels.data(), 2, 2);
    }

    std::vector<uint8_t> testPixels;
    Image testImage;
};

TEST_F(SpriteTests, DefaultConstructor) {
    Sprite sprite;
    auto [x, y] = sprite.GetPosition();
    EXPECT_FLOAT_EQ(x, 0.0f);
    EXPECT_FLOAT_EQ(y, 0.0f);
    EXPECT_FLOAT_EQ(sprite.GetRotation(), 0.0f);
}

TEST_F(SpriteTests, ConstructWithImage) {
    Sprite sprite(testImage);
    auto [left, top, width, height] = sprite.GetLocalBounds();
    EXPECT_FLOAT_EQ(width, 2.0f);
    EXPECT_FLOAT_EQ(height, 2.0f);
}

TEST_F(SpriteTests, SetPosition) {
    Sprite sprite;
    sprite.SetPosition(100.0f, 200.0f);
    auto [x, y] = sprite.GetPosition();
    EXPECT_FLOAT_EQ(x, 100.0f);
    EXPECT_FLOAT_EQ(y, 200.0f);
}

TEST_F(SpriteTests, SetRotation) {
    Sprite sprite;
    sprite.SetRotation(45.0f);
    EXPECT_FLOAT_EQ(sprite.GetRotation(), 45.0f);
}

TEST_F(SpriteTests, SetScale) {
    Sprite sprite;
    sprite.SetScale(2.0f, 3.0f);
    auto [scaleX, scaleY] = sprite.GetScale();
    EXPECT_FLOAT_EQ(scaleX, 2.0f);
    EXPECT_FLOAT_EQ(scaleY, 3.0f);
}

TEST_F(SpriteTests, SetOrigin) {
    Sprite sprite;
    sprite.SetOrigin(10.0f, 20.0f);
    auto [x, y] = sprite.GetOrigin();
    EXPECT_FLOAT_EQ(x, 10.0f);
    EXPECT_FLOAT_EQ(y, 20.0f);
}

TEST_F(SpriteTests, GlobalBounds) {
    Sprite sprite(testImage);
    sprite.SetPosition(100.0f, 100.0f);
    sprite.SetScale(2.0f, 2.0f);
    
    auto [left, top, width, height] = sprite.GetGlobalBounds();
    EXPECT_FLOAT_EQ(width, 4.0f);  // 2 * 2 (original width * scale)
    EXPECT_FLOAT_EQ(height, 4.0f); // 2 * 2 (original height * scale)
    EXPECT_FLOAT_EQ(left, 100.0f);
    EXPECT_FLOAT_EQ(top, 100.0f);
}
