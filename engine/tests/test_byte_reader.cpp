#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <orbis/byte_reader.hpp>
#include <orbis/byte_writer.hpp>

using Bytes = std::vector<std::uint8_t>;

TEST_CASE("ByteReader reads integers in network byte order", "[byte_reader]") {
    const Bytes in{0xAB, 0x12, 0x34, 0xDE, 0xAD, 0xBE, 0xEF};
    orbis::ByteReader r(in);

    auto a = r.u8();
    REQUIRE(a.ok());
    REQUIRE(a.value() == 0xAB);

    auto b = r.u16();
    REQUIRE(b.ok());
    REQUIRE(b.value() == 0x1234);

    auto c = r.u32();
    REQUIRE(c.ok());
    REQUIRE(c.value() == 0xDEADBEEFu);

    REQUIRE(r.at_end());
    REQUIRE(r.position() == 7);
}

TEST_CASE("ByteReader tracks position and remaining", "[byte_reader]") {
    const Bytes in{1, 2, 3, 4};
    orbis::ByteReader r(in);

    REQUIRE(r.remaining() == 4);
    REQUIRE(r.u16().ok());
    REQUIRE(r.position() == 2);
    REQUIRE(r.remaining() == 2);
}

TEST_CASE("ByteReader fails on empty input", "[byte_reader]") {
    const Bytes in{};
    orbis::ByteReader r(in);

    REQUIRE(r.u8().error() == orbis::Error::UnexpectedEnd);
    REQUIRE(r.u16().error() == orbis::Error::UnexpectedEnd);
    REQUIRE(r.u32().error() == orbis::Error::UnexpectedEnd);
    REQUIRE(r.bytes(1).error() == orbis::Error::UnexpectedEnd);
}

TEST_CASE("ByteReader failed read consumes nothing", "[byte_reader]") {
    const Bytes in{0x01, 0x02, 0x03};
    orbis::ByteReader r(in);

    auto fail = r.u32();  // only 3 bytes available
    REQUIRE_FALSE(fail.ok());
    REQUIRE(r.position() == 0);
    REQUIRE(r.remaining() == 3);

    // The bytes are still readable afterwards
    auto ok = r.u16();
    REQUIRE(ok.ok());
    REQUIRE(ok.value() == 0x0102);
}

TEST_CASE("ByteReader bytes returns a view of the next n bytes", "[byte_reader]") {
    const Bytes in{0x01, 0x48, 0x65, 0x6C, 0x6C, 0x6F};
    orbis::ByteReader r(in);

    REQUIRE(r.u8().ok());

    auto payload = r.bytes(5);
    REQUIRE(payload.ok());
    REQUIRE(payload.value().size() == 5);
    REQUIRE(payload.value()[0] == 0x48);
    REQUIRE(payload.value()[4] == 0x6F);
    REQUIRE(r.at_end());
}

TEST_CASE("ByteReader bytes accepts zero length and exact remainder", "[byte_reader]") {
    const Bytes in{1, 2};
    orbis::ByteReader r(in);

    auto none = r.bytes(0);
    REQUIRE(none.ok());
    REQUIRE(none.value().empty());

    auto all = r.bytes(2);
    REQUIRE(all.ok());
    REQUIRE(r.at_end());
}

TEST_CASE("ByteReader bytes rejects lengths beyond the input", "[byte_reader]") {
    const Bytes in{1, 2, 3};
    orbis::ByteReader r(in);

    REQUIRE(r.bytes(4).error() == orbis::Error::UnexpectedEnd);
    REQUIRE(r.u8().ok());
    REQUIRE(r.bytes(std::numeric_limits<std::size_t>::max()).error() == orbis::Error::UnexpectedEnd);
    REQUIRE(r.remaining() == 2);
}

TEST_CASE("ByteWriter output can be read back by ByteReader", "[byte_reader]") {
    const Bytes payload{0x48, 0x65, 0x6C, 0x6C, 0x6F};

    orbis::ByteWriter w;
    w.u8(1).u16(0xBEEF).u32(0x01020304u).bytes(payload);
    const Bytes wire = w.release();

    orbis::ByteReader r(wire);
    REQUIRE(r.u8().value() == 1);
    REQUIRE(r.u16().value() == 0xBEEF);
    REQUIRE(r.u32().value() == 0x01020304u);

    auto back = r.bytes(payload.size());
    REQUIRE(back.ok());
    REQUIRE(Bytes(back.value().begin(), back.value().end()) == payload);
    REQUIRE(r.at_end());
}