#include <gtest/gtest.h>
#include "graphics/ImageManager.h"
#include "core/Hash.h"
#include <nlohmann/json.hpp>
#include <fstream>

using namespace ShoeEngine::Graphics;
using namespace ShoeEngine::Core;
using json = nlohmann::json;

class ImageManagerTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a test image file
        std::vector<uint8_t> pixels(4 * 4 * 4, 255); // 4x4 white image
        testImage.reset(new Image(pixels.data(), 4, 4));
        testImage->SaveToFile("test_image.png");
        
        // Create test JSON
        testJson = {
            {"test_image", {
                {"file", "test_image.png"}
            }}
        };
    }

    void TearDown() override {
        // Clean up test file
        std::remove("test_image.png");
    }

    std::unique_ptr<Image> testImage;
    json testJson;
    ImageManager manager;
};

TEST_F(ImageManagerTests, GetManagedType) {
    EXPECT_EQ(manager.GetManagedType(), "images"_h);
}

TEST_F(ImageManagerTests, CreateFromJson) {
    EXPECT_TRUE(manager.CreateFromJson(testJson));
    
    const Image* image = manager.GetImage("test_image"_h);
    ASSERT_NE(image, nullptr);
    EXPECT_EQ(image->GetWidth(), 4);
    EXPECT_EQ(image->GetHeight(), 4);
}

TEST_F(ImageManagerTests, GetNonexistentImage) {
    EXPECT_EQ(manager.GetImage("nonexistent"_h), nullptr);
}

TEST_F(ImageManagerTests, Clear) {
    EXPECT_TRUE(manager.CreateFromJson(testJson));
    EXPECT_NE(manager.GetImage("test_image"_h), nullptr);
    
    manager.Clear();
    EXPECT_EQ(manager.GetImage("test_image"_h), nullptr);
}
