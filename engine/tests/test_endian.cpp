#include <array>
#include <cstdint>

#include <catch2/catch_test_macros.hpp>

#include <orbis/endian.hpp>

TEST_CASE("store_be16 writes the most significant byte first", "[endian]") {
    std::array<std::uint8_t, 2> out{};

    orbis::store_be16(out, 0x1234);
    REQUIRE(out[0] == 0x12);
    REQUIRE(out[1] == 0x34);

    // Asymmetric values catch a swapped or masked-away byte
    orbis::store_be16(out, 0x00FF);
    REQUIRE(out[0] == 0x00);
    REQUIRE(out[1] == 0xFF);

    orbis::store_be16(out, 0xFF00);
    REQUIRE(out[0] == 0xFF);
    REQUIRE(out[1] == 0x00);
}

TEST_CASE("store_be32 writes the most significant byte first", "[endian]") {
    std::array<std::uint8_t, 4> out{};

    orbis::store_be32(out, 0x01020304u);
    REQUIRE(out == std::array<std::uint8_t, 4>{0x01, 0x02, 0x03, 0x04});

    orbis::store_be32(out, 0xDEADBEEFu);
    REQUIRE(out == std::array<std::uint8_t, 4>{0xDE, 0xAD, 0xBE, 0xEF});
}

TEST_CASE("load_be16 and load_be32 read network byte order", "[endian]") {
    const std::array<std::uint8_t, 2> two{0x12, 0x34};
    REQUIRE(orbis::load_be16(two) == 0x1234);

    const std::array<std::uint8_t, 4> four{0xDE, 0xAD, 0xBE, 0xEF};
    REQUIRE(orbis::load_be32(four) == 0xDEADBEEFu);
}

TEST_CASE("store then load round-trips boundary values", "[endian]") {
    for (std::uint16_t v : {
        std::uint16_t{0}, 
        std::uint16_t{1}, 
        std::uint16_t{0x7FFF}, 
        std::uint16_t{0x8000}, 
        std::uint16_t{0xFFFF}}) {
        std::array<std::uint8_t, 2> buf{};
        orbis::store_be16(buf, v);
        REQUIRE(orbis::load_be16(buf) == v);
    }

    for (std::uint32_t v : {0u, 1u, 0x7FFFFFFFu, 0x80000000u, 0xFFFFFFFFu}) {
        std::array<std::uint8_t, 4> buf{};
        orbis::store_be32(buf, v);
        REQUIRE(orbis::load_be32(buf) == v);
    }
}