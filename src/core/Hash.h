#pragma once

#include <cstdint>  // for uint32_t
#include <cstddef>  // for size_t
#include <string>   // for std::string

namespace ShoeEngine {
namespace Core {

/**
 * @class Hash
 * @brief Provides various hash functions for string and data hashing
 * 
 * Implements multiple hash algorithms including MurmurHash3, FNV-1a, and universal hash.
 * Can be used for hash table implementations and data integrity checks.
 */
class Hash
{
public:
    /**
     * @brief Computes MurmurHash3 for the given data
     * @param key Pointer to data to hash
     * @param len Length of data in bytes
     * @param seed Optional seed value
     * @return 32-bit hash value
     *
     * Note: Marked constexpr, but may fail to compile on some compilers/standards
     *       due to pointer arithmetic.
     */
    static constexpr std::uint32_t MurmurHash3(const char* key, std::uint32_t len, std::uint32_t seed = 0)
    {
        constexpr std::uint32_t c1 = 0xcc9e2d51;
        constexpr std::uint32_t c2 = 0x1b873593;
        constexpr std::uint32_t r1 = 15;
        constexpr std::uint32_t r2 = 13;
        constexpr std::uint32_t m  = 5;
        constexpr std::uint32_t n  = 0xe6546b64;

        std::uint32_t hash = seed;

        const int numBlocks = len / 4;
        const std::uint32_t* blocks = (const std::uint32_t*)(key);

        for (int i = 0; i < numBlocks; i++) {
            std::uint32_t k = blocks[i];
            k *= c1;
            k = (k << r1) | (k >> (32 - r1)); // Rotate left
            k *= c2;

            hash ^= k;
            hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
        }

		const std::uint8_t* tail = (const std::uint8_t*)(key + numBlocks * 4);
        std::uint32_t k1 = 0;

        switch (len & 3) {
        case 3: k1 ^= (tail[2] << 16);
            [[fallthrough]];
        case 2: k1 ^= (tail[1] << 8);
            [[fallthrough]];
        case 1:
            k1 ^= tail[0];
            k1 *= c1;
            k1 = (k1 << r1) | (k1 >> (32 - r1));
            k1 *= c2;
            hash ^= k1;
            break;
        }

        hash ^= len;
        hash ^= (hash >> 16);
        hash *= 0x85ebca6b;
        hash ^= (hash >> 13);
        hash *= 0xc2b2ae35;
        hash ^= (hash >> 16);

        return hash;
    }

    /**
     * @brief Computes FNV-1a hash for the given data
     * @param key Pointer to data to hash
     * @param len Length of data in bytes
     * @param seed Optional seed value
     * @return 32-bit hash value
     *
     * Note: Marked constexpr, but may fail to compile on some compilers/standards
     *       due to pointer arithmetic.
     */
    static constexpr uint32_t FNV1a(const char* key, uint32_t len, uint32_t seed = 0)
    {
        constexpr uint32_t fnv_prime = 16777619;
        uint32_t hash = seed;

        for (uint32_t i = 0; i < len; i++)
        {
            hash ^= static_cast<uint8_t>(key[i]);
            hash *= fnv_prime;
        }

        return hash;
    }

    /**
     * @brief Computes universal hash for the given string
     * @param str Null-terminated string to hash
     * @param seed Optional seed value
     * @return 32-bit hash value
     *
     * Note: Marked constexpr, but may fail to compile on some compilers/standards
     *       due to modulo operation, etc.
     */
    static constexpr uint32_t Universal(const char* str, uint32_t seed = 0)
    {
        constexpr uint32_t mod  = 1'000'000'009U;
        constexpr uint32_t base = 257U;

        uint32_t hash = seed;
        for (uint32_t i = 0; str[i] != '\0'; ++i)
        {
            hash = (hash * base + static_cast<unsigned char>(str[i])) % mod;
        }
        return hash;
    }

    /**
     * @class HashValue
     * @brief Represents a computed hash value with comparison operations
     */
    class HashValue
    {
    public:
        constexpr HashValue()
            : m_hash(0)
        {
        }

        constexpr HashValue(uint32_t h)
            : m_hash(h)
        {
        }

        // Originally constexpr: calls FNV1a, which may cause issues on some compilers
        // if FNV1a is not truly constexpr. We'll keep it as it was.
        constexpr HashValue(const char* key, uint32_t len)
            : m_hash(FNV1a(key, len))
        {
        }

        // Conversion operator to uint32_t
        constexpr operator uint32_t() const { return m_hash; }

        // Comparisons
        constexpr bool operator==(const HashValue& rhs) const { return m_hash == rhs.m_hash; }
        constexpr bool operator==(uint32_t rhs)        const { return m_hash == rhs; }

        // Combines this hash with a new key using FNV1a again
        constexpr HashValue operator+(const std::string& key) const
        {
            // Cast size_t to uint32_t if you prefer 32-bit
            return HashValue(FNV1a(key.c_str(), static_cast<uint32_t>(key.size()), m_hash));
        }

        uint32_t m_hash;
    };

    /**
     * @struct Hasher
     * @brief Functor for using HashValue in STL containers
     */
    struct Hasher
    {
        size_t operator()(const HashValue& h) const
        {
            // Typically safe to cast 32-bit to size_t
            // (though on 64-bit systems size_t is larger).
            return static_cast<size_t>(h.m_hash);
        }
    };
};

} // namespace Core
} // namespace ShoeEngine

/**
 * @brief User-defined literal operator for creating a HashValue from a string literal.
 * @param key String literal
 * @param len Length of the literal
 * @return HashValue computed by FNV1a
 *
 * Originally constexpr as well.
 */
constexpr ShoeEngine::Core::Hash::HashValue operator""_h(const char* key, size_t len)
{
	// Casting size_t (often 64-bit) to uint32_t if you want 32-bit hashing
	return ShoeEngine::Core::Hash::HashValue(key, static_cast<uint32_t>(len));
}
