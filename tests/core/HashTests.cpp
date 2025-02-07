#include "gtest/gtest.h"
#include "core/Hash.h"

using namespace ShoeEngine::Core;

TEST(HashTests, MurmurHash3_ConsistentResults)
{
    const char* testString = "test string";
    auto hash1 = Hash::MurmurHash3(testString, strlen(testString));
    auto hash2 = Hash::MurmurHash3(testString, strlen(testString));
    EXPECT_EQ(hash1, hash2);
}

TEST(HashTests, FNV1a_ConsistentResults)
{
    const char* testString = "test string";
    auto hash1 = Hash::FNV1a(testString, strlen(testString));
    auto hash2 = Hash::FNV1a(testString, strlen(testString));
    EXPECT_EQ(hash1, hash2);
}

TEST(HashTests, Universal_ConsistentResults)
{
    const char* testString = "test string";
    auto hash1 = Hash::Universal(testString);
    auto hash2 = Hash::Universal(testString);
    EXPECT_EQ(hash1, hash2);
}

TEST(HashTests, HashValue_Comparison)
{
    Hash::HashValue hash1("test", 4);
    Hash::HashValue hash2("test", 4);
    EXPECT_EQ(hash1, hash2);
}

TEST(HashTests, HashValue_StringLiteral)
{
    auto hash1 = "test"_h;
    Hash::HashValue hash2("test", 4);
    EXPECT_EQ(hash1, hash2);
}
