#include <cstdint>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <orbis/byte_writer.hpp>

using Bytes = std::vector<std::uint8_t>;

TEST_CASE("ByteWriter starts empty", "[byte_writer]") {
    orbis::ByteWriter w;
    REQUIRE(w.size() == 0);
    REQUIRE(w.data().empty());
}

TEST_CASE("ByteWriter appends integers in network byte order", "[byte_writer]") {
    orbis::ByteWriter w;
    w.u8(0xAB).u16(0x1234).u32(0xDEADBEEFu);

    REQUIRE(w.size() == 7);
    REQUIRE(w.data() == Bytes{0xAB, 0x12, 0x34, 0xDE, 0xAD, 0xBE, 0xEF});
}

TEST_CASE("ByteWriter appends raw bytes unchanged", "[byte_writer]") {
    const Bytes payload{0x48, 0x65, 0x6C, 0x6C, 0x6F};  // "Hello"

    orbis::ByteWriter w;
    w.u8(0x01).bytes(payload);

    REQUIRE(w.data() == Bytes{0x01, 0x48, 0x65, 0x6C, 0x6C, 0x6F});
}

TEST_CASE("ByteWriter accepts an empty byte range", "[byte_writer]") {
    orbis::ByteWriter w;
    w.bytes(Bytes{});

    REQUIRE(w.size() == 0);
}

TEST_CASE("ByteWriter release hands over the bytes and resets", "[byte_writer]") {
    orbis::ByteWriter w;
    w.u16(0xCAFE);

    Bytes out = w.release();

    REQUIRE(out == Bytes{0xCA, 0xFE});
    REQUIRE(w.size() == 0);

    // The writer is reusable after release()
    w.u8(0x01);
    REQUIRE(w.data() == Bytes{0x01});
}

TEST_CASE("ByteWriter can build the plan's header prefix", "[byte_writer]") {
    // version=1, source=1, destination=2, payload length=5
    orbis::ByteWriter w;
    w.u8(1).u16(1).u16(2).u16(5);

    REQUIRE(w.data() == Bytes{0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x05});
}