#include <gtest/gtest.h>
#include "graphics/SpriteManager.h"
#include "core/Hash.h"
#include "core/DataManager.h"
#include <nlohmann/json.hpp>
#include <fstream>

using namespace ShoeEngine::Graphics;
using namespace ShoeEngine::Core;
using json = nlohmann::json;

class SpriteManagerTests : public ::testing::Test {
protected:
    SpriteManagerTests() 
        : imageManager(dataManager)
        , spriteManager(dataManager, imageManager)
    {}

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

    DataManager dataManager;
    std::unique_ptr<Image> testImage;
    json testJson;
    ImageManager imageManager;
    SpriteManager spriteManager;
};

TEST_F(SpriteManagerTests, GetManagedType) {
    EXPECT_EQ(spriteManager.GetManagedType(), "sprites"_h);
}

TEST_F(SpriteManagerTests, CreateFromJson) {
    EXPECT_TRUE(spriteManager.CreateFromJson(testJson));
    
    const Sprite* sprite = spriteManager.GetSprite("test_sprite"_h);
    ASSERT_NE(sprite, nullptr);
    
    auto position = sprite->GetPosition();
    EXPECT_EQ(position.first, 100.0f);
    EXPECT_EQ(position.second, 200.0f);
    
    EXPECT_EQ(sprite->GetRotation(), 45.0f);
    
    auto scale = sprite->GetScale();
    EXPECT_EQ(scale.first, 2.0f);
    EXPECT_EQ(scale.second, 2.0f);
    
    auto origin = sprite->GetOrigin();
    EXPECT_EQ(origin.first, 2.0f);
    EXPECT_EQ(origin.second, 2.0f);
}

TEST_F(SpriteManagerTests, CreateWithInvalidImage) {
    json invalidJson = {
        {"test_sprite", {
            {"image", "nonexistent_image"},
            {"position", {{"x", 0.0f}, {"y", 0.0f}}}
        }}
    };
    
    EXPECT_FALSE(spriteManager.CreateFromJson(invalidJson));
}

TEST_F(SpriteManagerTests, GetNonexistentSprite) {
    EXPECT_EQ(spriteManager.GetSprite("nonexistent"_h), nullptr);
}

TEST_F(SpriteManagerTests, Clear) {
    EXPECT_TRUE(spriteManager.CreateFromJson(testJson));
    EXPECT_NE(spriteManager.GetSprite("test_sprite"_h), nullptr);
    
    spriteManager.Clear();
    EXPECT_EQ(spriteManager.GetSprite("test_sprite"_h), nullptr);
}

TEST_F(SpriteManagerTests, SerializeEmptyManager) {
    json serialized = spriteManager.SerializeToJson();
    EXPECT_TRUE(serialized.empty());
}

TEST_F(SpriteManagerTests, SerializeAndDeserialize) {
    // Create initial sprite
    EXPECT_TRUE(spriteManager.CreateFromJson(testJson));
    
    // Serialize
    json serialized = spriteManager.SerializeToJson();
    
    // Clear and verify
    spriteManager.Clear();
    EXPECT_EQ(spriteManager.GetSprite("test_sprite"_h), nullptr);
    
    // Deserialize
    EXPECT_TRUE(spriteManager.CreateFromJson(serialized));
    
    // Verify sprite was restored correctly
    const Sprite* sprite = spriteManager.GetSprite("test_sprite"_h);
    ASSERT_NE(sprite, nullptr);
    
    auto position = sprite->GetPosition();
    EXPECT_EQ(position.first, 100.0f);
    EXPECT_EQ(position.second, 200.0f);
    
    EXPECT_EQ(sprite->GetRotation(), 45.0f);
    
    auto scale = sprite->GetScale();
    EXPECT_EQ(scale.first, 2.0f);
    EXPECT_EQ(scale.second, 2.0f);
    
    auto origin = sprite->GetOrigin();
    EXPECT_EQ(origin.first, 2.0f);
    EXPECT_EQ(origin.second, 2.0f);
}

TEST_F(SpriteManagerTests, SerializeMultipleSprites) {
    // Create first sprite
    EXPECT_TRUE(spriteManager.CreateFromJson(testJson));
    
    // Create second sprite
    json secondSpriteJson = {
        {"test_sprite2", {
            {"image", "test_image"},
            {"position", {
                {"x", 300.0f},
                {"y", 400.0f}
            }},
            {"rotation", 90.0f},
            {"scale", {
                {"x", 3.0f},
                {"y", 3.0f}
            }},
            {"origin", {
                {"x", 1.0f},
                {"y", 1.0f}
            }}
        }}
    };
    
    EXPECT_TRUE(spriteManager.CreateFromJson(secondSpriteJson));
    
    // Serialize both sprites
    json serialized = spriteManager.SerializeToJson();
    
    // Clear and verify
    spriteManager.Clear();
    EXPECT_EQ(spriteManager.GetSprite("test_sprite"_h), nullptr);
    EXPECT_EQ(spriteManager.GetSprite("test_sprite2"_h), nullptr);
    
    // Deserialize
    EXPECT_TRUE(spriteManager.CreateFromJson(serialized));
    
    // Verify first sprite
    const Sprite* sprite1 = spriteManager.GetSprite("test_sprite"_h);
    ASSERT_NE(sprite1, nullptr);
    
    auto position1 = sprite1->GetPosition();
    EXPECT_EQ(position1.first, 100.0f);
    EXPECT_EQ(position1.second, 200.0f);
    
    // Verify second sprite
    const Sprite* sprite2 = spriteManager.GetSprite("test_sprite2"_h);
    ASSERT_NE(sprite2, nullptr);
    
    auto position2 = sprite2->GetPosition();
    EXPECT_EQ(position2.first, 300.0f);
    EXPECT_EQ(position2.second, 400.0f);
}
