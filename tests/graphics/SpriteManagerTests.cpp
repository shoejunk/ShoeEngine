#include <gtest/gtest.h>
#include "graphics/SpriteManager.h"
#include <nlohmann/json.hpp>
#include <fstream>

using namespace ShoeEngine::Graphics;
using json = nlohmann::json;

class SpriteManagerTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a test image file
        std::vector<uint8_t> pixels(4 * 4 * 4, 255); // 4x4 white image
        testImage.reset(new Image(pixels.data(), 4, 4));
        testImage->SaveToFile("test_image.png");
        
        // Create test JSON for image
        json imageJson = {
            {"test_image", {
                {"file", "test_image.png"}
            }}
        };
        
        // Load the image into the image manager
        imageManager.CreateFromJson(imageJson);
        
        // Create test JSON for sprite
        testJson = {
            {"test_sprite", {
                {"image", "test_image"},
                {"position", {
                    {"x", 100.0f},
                    {"y", 200.0f}
                }},
                {"rotation", 45.0f},
                {"scale", {
                    {"x", 2.0f},
                    {"y", 2.0f}
                }},
                {"origin", {
                    {"x", 2.0f},
                    {"y", 2.0f}
                }}
            }}
        };
    }

    void TearDown() override {
        // Clean up test file
        std::remove("test_image.png");
    }

    std::unique_ptr<Image> testImage;
    ImageManager imageManager;
    SpriteManager spriteManager{imageManager};
    json testJson;
};

TEST_F(SpriteManagerTests, GetManagedType) {
    EXPECT_EQ(spriteManager.GetManagedType(), "sprites");
}

TEST_F(SpriteManagerTests, CreateFromJson) {
    EXPECT_TRUE(spriteManager.CreateFromJson(testJson));
    
    const Sprite* sprite = spriteManager.GetSprite("test_sprite");
    ASSERT_NE(sprite, nullptr);
    
    auto [x, y] = sprite->GetPosition();
    EXPECT_FLOAT_EQ(x, 100.0f);
    EXPECT_FLOAT_EQ(y, 200.0f);
    
    EXPECT_FLOAT_EQ(sprite->GetRotation(), 45.0f);
    
    auto [scaleX, scaleY] = sprite->GetScale();
    EXPECT_FLOAT_EQ(scaleX, 2.0f);
    EXPECT_FLOAT_EQ(scaleY, 2.0f);
    
    auto [originX, originY] = sprite->GetOrigin();
    EXPECT_FLOAT_EQ(originX, 2.0f);
    EXPECT_FLOAT_EQ(originY, 2.0f);
}

TEST_F(SpriteManagerTests, CreateWithInvalidImage) {
    json invalidJson = {
        {"test_sprite", {
            {"image", "nonexistent_image"}
        }}
    };
    
    EXPECT_FALSE(spriteManager.CreateFromJson(invalidJson));
}

TEST_F(SpriteManagerTests, GetNonexistentSprite) {
    EXPECT_EQ(spriteManager.GetSprite("nonexistent"), nullptr);
}

TEST_F(SpriteManagerTests, Clear) {
    EXPECT_TRUE(spriteManager.CreateFromJson(testJson));
    EXPECT_NE(spriteManager.GetSprite("test_sprite"), nullptr);
    
    spriteManager.Clear();
    EXPECT_EQ(spriteManager.GetSprite("test_sprite"), nullptr);
}
