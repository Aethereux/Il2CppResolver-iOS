// speck.h
// by OPSphystech420
// compile time string Speck-64/128 ecnryption
// c++20
// ----------- tweak ----------------------------------------
#pragma once
#include <cstdint>
#include <cstddef>

namespace speck {
using u64 = std::uint64_t;

/* 64 bit SplitMix */
[[nodiscard]] consteval u64 permute64(u64 x) noexcept
{
    x += 0x9E3779B97F4A7C15ULL;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    return x ^ (x >> 31);
}

/* FNV-1a (64 bit) */
[[nodiscard]] consteval u64 fnv1a64(char const* s, std::size_t n) noexcept
{
    u64 h = 0xcbf29ce484222325ULL;
    for (std::size_t i = 0; i < n; ++i)
        h = (h ^ static_cast<unsigned char>(s[i])) * 0x100000001B3ULL;
    return h;
}

/*— compile-time entropy pool -------------------------------------------------
  |  Mix:     build time & date
  |           build salt (optional) (-DSPECK_BUILD_SALT=0x)
  |  Return:  64 bit value with dispersion        */
[[nodiscard]] consteval u64 compile_time_entropy() noexcept
{
    constexpr char datetime[] = __DATE__ " " __TIME__;
#ifdef SPECK_BUILD_SALT
    return fnv1a64(datetime, sizeof(datetime) - 1) ^ permute64(SPECK_BUILD_SALT);
#else
    return fnv1a64(datetime, sizeof(datetime) - 1);
#endif
}
} // namespace speck

/* Default key – each literal = own (K0,K1) pair
   --------------------------------------------------
   - __COUNTER__   make unique per macro
   - __LINE__      noise tied to location
   - permute64     avalanche & bijection    */
#ifndef SPECK_DEFAULT_KEY_K0
#   define SPECK_DEFAULT_KEY_K0 \
        speck::permute64(speck::compile_time_entropy() ^ (__LINE__ * 0xA511E9B3ULL) ^ __COUNTER__)
#endif

#ifndef SPECK_DEFAULT_KEY_K1
#   define SPECK_DEFAULT_KEY_K1 \
        speck::permute64(speck::compile_time_entropy() ^ (~__LINE__ * 0x7840AB65ULL) ^ (~__COUNTER__))
#endif
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
#include <array>
#include <bit>
#include <cstdint>
#include <string_view>

namespace speck {

// ---------- helpers -----------------------------------------------------------
using  u64 = std::uint64_t;
using  size_type = std::size_t;

constexpr u64 generate_key64(u64 seed)
/* MurmurHash3 finaliser 64 bits with bit dispersion               */
{
    u64 k = seed;
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;
    k |= 0x0101010101010101ULL;
    return k;
}

// ---- Speck-64/128 (block cipher core) ---------------------------------------
struct Speck64_128 {
    static constexpr int ROUNDS = 27;

    std::array<u64, ROUNDS> rk{}; // round key schedule

    /* key expansion: two 64 bit words —> 27 round keys                    */
    constexpr explicit Speck64_128(u64 k0, u64 k1)
    {
        u64 b = k0, a = k1;
        rk[0] = b;
        for (int i = 0; i < ROUNDS - 1; ++i) {
            /* —— Speck round constants ——                                   */
            a = std::rotr(a, 8);          a += b;  a ^= i;
            b = std::rotl(b, 3);          b ^= a;
            rk[i + 1] = b;
        }
    }

    /* one block encrypt (8 bytes)                                         */
    [[nodiscard]] constexpr u64 encrypt(u64 v) const
    {
        u64 x = v, y = 0;                 // treat block as (y || x) little endian
        for (u64 k : rk) {
            x = std::rotr(x, 8);          x += y;  x ^= k;
            y = std::rotl(y, 3);          y ^= x;
        }
        return (y << 32) | (x & 0xFFFFFFFFULL); // pack back (ignored y here)
    }
};

// ---- compile time obfuscator -------------------------------------------------
template <size_type N,
          u64 K0 = SPECK_DEFAULT_KEY_K0,
          u64 K1 = SPECK_DEFAULT_KEY_K1>
class obfuscator {
    static constexpr Speck64_128 CIPHER{K0, K1};

    // compile time encrypted buffer
    std::array<char, N> buf{};

    static constexpr char keystream_byte(size_type idx)
    {
        /* CTR mode: encrypt(counter) and take byte                         */
        const u64 ks  = CIPHER.encrypt(static_cast<u64>(idx / 8));
        return static_cast<char>(ks >> ((idx % 8) * 8));
    }

public:
    constexpr explicit obfuscator(char const (&plain)[N])
    {
        for (size_type i = 0; i < N; ++i)
            buf[i] = plain[i] ^ keystream_byte(i);
    }

    // expose ciphertext for static storage
    constexpr auto const& data() const noexcept { return buf; }
};

// ---- runtime container ------------------------------------------------------
template <size_type N, u64 K0, u64 K1>
class obfuscated_data {
    static constexpr Speck64_128 CIPHER{K0, K1};
    std::array<char, N> buf{};
    bool decrypted{false};

    static constexpr char keystream_byte(size_type idx)
    {
        const u64 ks = CIPHER.encrypt(static_cast<u64>(idx / 8));
        return static_cast<char>(ks >> ((idx % 8) * 8));
    }

public:
    /* copy compile time ciphertext in                                      */
    constexpr explicit obfuscated_data(obfuscator<N, K0, K1> const& o)
        : buf{o.data()} {}

    /* implicit conversion → char* (decrypts on demand)                    */
    operator char*()
    {
        decrypt();
        return buf.data();
    }
    operator std::string_view()
    {
        decrypt();
        return {buf.data(), N - 1};       // ignore terminating '\0'
    }

    [[nodiscard]] bool is_decrypted() const noexcept { return decrypted; }

    void decrypt()
    {
        if (!decrypted) {
            for (size_type i = 0; i < N; ++i)
                buf[i] ^= keystream_byte(i);
            decrypted = true;
        }
    }
    void encrypt()
    {
        if (decrypted) {
            for (size_type i = 0; i < N; ++i)
                buf[i] ^= keystream_byte(i);
            decrypted = false;
        }
    }

    // zeroise on destruction
    ~obfuscated_data()
    {
        for (auto& c : buf) c = 0;
    }
};

// ---- deduce N --------------------------------------------------------------
template <size_type N, u64 K0, u64 K1>
[[nodiscard]] constexpr auto
make_obfuscator(char const (&lit)[N])
{
    return obfuscator<N, K0, K1>{lit};
}

} // namespace speck

// ---------------------------------------------------------------------------
#define SPECK_OBFUSCATE(str) \
    SPECK_OBFUSCATE_KEY(str, SPECK_DEFAULT_KEY_K0, SPECK_DEFAULT_KEY_K1)

#define SPECK_OBFUSCATE_KEY(str, k0, k1)                                        \
    []() -> speck::obfuscated_data<sizeof(str), k0, k1>& {                      \
        static constexpr auto  _obf = speck::make_obfuscator<sizeof(str), k0, k1>(str); \
        static auto            _dat = speck::obfuscated_data<sizeof(str), k0, k1>(_obf); \
        return _dat;                                                            \
    }()

// Generates an NSString* with stack-based decryption using existing types
// We must capture the keys in constexpr variables first to ensure they match
// because SPECK_DEFAULT_KEY uses __COUNTER__ which changes on every expansion.
#define OBF_NSA(str) ([]() -> NSString* { \
    constexpr unsigned long long _k0 = SPECK_DEFAULT_KEY_K0; \
    constexpr unsigned long long _k1 = SPECK_DEFAULT_KEY_K1; \
    constexpr auto _obf = speck::make_obfuscator<sizeof(str), _k0, _k1>(str); \
    auto _dat = speck::obfuscated_data<sizeof(str), _k0, _k1>(_obf); \
    return [NSString stringWithUTF8String:(char*)_dat]; \
}())

/* ---------------------------------------------------------------------------
   SIMON:
     - Change Speck round:  x = rotl(x, 1);  x &= y;  x ^= k;  y = rotl(y, 8);  y ^= x
     - Use 44 rounds for SIMON 64/128
--------------------------------------------------------------------------- */

namespace enc {

/*  Compile time encrypted literal
    – N = size of the literal + '\0'
    – K0/K1 default
                                        */
    template<std::size_t N,
            speck::u64 K0 = SPECK_DEFAULT_KEY_K0,
            speck::u64 K1 = SPECK_DEFAULT_KEY_K1>
    struct StringLiteral
    {
        /* ---------- static const shared by instances -------------------- */
        static constexpr speck::Speck64_128 CIPHER{K0, K1};

        static constexpr char keystream_byte(std::size_t i) noexcept
        {
            const speck::u64 ks = CIPHER.encrypt(i / 8);
            return static_cast<char>(ks >> ((i % 8) * 8));
        }

        /* ---------------- cipher text ----------------------------------- */
        std::array<char, N> cipher{};

        /*  consteval ctor: XOR plain text with the Speck keystream
                            so cipher text is baked into the object         */
        consteval explicit StringLiteral(char const (&plain)[N])
        {
            for (std::size_t i = 0; i < N; ++i)
                cipher[i] = static_cast<char>(plain[i] ^ keystream_byte(i));
        }
        
        mutable std::array<char, N> plain{};
        mutable bool done = false;

        /* ---------- run time ??thread safe ------------------------------ */
        [[nodiscard]] std::string_view decrypt() const
        {
            /*  local cache  -- еach literal decrypts at most
                once a TU, after we get back same view          */
            

            if (!done) {
                for (std::size_t i = 0; i < N; ++i)
                    plain[i] = cipher[i] ^ keystream_byte(i);
                done = true;
            }
            return { plain.data(), N - 1 }; // drop '\0'
        }

        [[nodiscard]] const std::string& str() const
        {
            static const std::string s(decrypt());
            return s;
        }

    };

/*  creates one hidden constexpr variable per call
    returns by reference
    NTTP needs auto&                */
#define ES(str)                                                       \
    []() constexpr -> auto& {                                                  \
        static constexpr ::enc::StringLiteral<sizeof(str)> lit{ str };         \
        return lit;                                                            \
    }()
} // namespace enc
