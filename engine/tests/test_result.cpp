#include <cstdint>
#include <string_view>

#include <catch2/catch_test_macros.hpp>

#include <orbis/result.hpp>

TEST_CASE("Result holding a value reports success", "[result]") {
    orbis::Result<int> r = 42;

    REQUIRE(r.ok());
    REQUIRE(static_cast<bool>(r));
    REQUIRE(r.value() == 42);
}

TEST_CASE("Result holding an error reports failure", "[result]") {
    orbis::Result<int> r = orbis::Error::UnexpectedEnd;

    REQUIRE_FALSE(r.ok());
    REQUIRE_FALSE(static_cast<bool>(r));
    REQUIRE(r.error() == orbis::Error::UnexpectedEnd);
}

TEST_CASE("Result value can be modified in place", "[result]") {
    orbis::Result<std::uint16_t> r = std::uint16_t{7};

    r.value() = 9;
    REQUIRE(r.value() == 9);
}

TEST_CASE("Errors have readable descriptions", "[result]") {
    REQUIRE(orbis::to_string(orbis::Error::UnexpectedEnd) == "unexpected end of input");
}