#include <gtest/gtest.h>
#include "graphics/Image.h"
#include <vector>

using namespace ShoeEngine::Graphics;

class ImageTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a small test image with red pixels
        testPixels = std::vector<uint8_t>(4 * 4 * 4, 0); // 4x4 image, 4 channels (RGBA)
        for (size_t i = 0; i < testPixels.size(); i += 4) {
            testPixels[i] = 255;     // R
            testPixels[i + 1] = 0;   // G
            testPixels[i + 2] = 0;   // B
            testPixels[i + 3] = 255; // A
        }
    }

    std::vector<uint8_t> testPixels;
};

TEST_F(ImageTests, DefaultConstructor) {
    Image img;
    EXPECT_EQ(img.GetWidth(), 0);
    EXPECT_EQ(img.GetHeight(), 0);
}

TEST_F(ImageTests, ConstructFromPixels) {
    Image img(testPixels.data(), 4, 4);
    EXPECT_EQ(img.GetWidth(), 4);
    EXPECT_EQ(img.GetHeight(), 4);
    
    const uint8_t* pixels = img.GetPixels();
    ASSERT_NE(pixels, nullptr);
    // Check first pixel is red
    EXPECT_EQ(pixels[0], 255); // R
    EXPECT_EQ(pixels[1], 0);   // G
    EXPECT_EQ(pixels[2], 0);   // B
    EXPECT_EQ(pixels[3], 255); // A
}

TEST_F(ImageTests, InvalidConstructionThrows) {
    EXPECT_THROW(Image(nullptr, 4, 4), std::invalid_argument);
    EXPECT_THROW(Image(testPixels.data(), 0, 4), std::invalid_argument);
    EXPECT_THROW(Image(testPixels.data(), 4, 0), std::invalid_argument);
}

TEST_F(ImageTests, LoadInvalidFile) {
    Image img;
    EXPECT_FALSE(img.LoadFromFile("nonexistent.png"));
}

TEST_F(ImageTests, CloneImage) {
    Image original(testPixels.data(), 4, 4);
    Image clone = original.Clone();
    
    EXPECT_EQ(clone.GetWidth(), original.GetWidth());
    EXPECT_EQ(clone.GetHeight(), original.GetHeight());
    
    const uint8_t* originalPixels = original.GetPixels();
    const uint8_t* clonePixels = clone.GetPixels();
    
    // Compare first pixel
    EXPECT_EQ(clonePixels[0], originalPixels[0]);
    EXPECT_EQ(clonePixels[1], originalPixels[1]);
    EXPECT_EQ(clonePixels[2], originalPixels[2]);
    EXPECT_EQ(clonePixels[3], originalPixels[3]);
}
