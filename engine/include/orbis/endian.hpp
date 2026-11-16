#pragma once

#include <cstdint>
#include <span>

namespace orbis {

    // All functions use network byte order (big-endian): the most significant
    // byte comes first they are implemented with shifts so they behave the
    // same on any host CPU regardless of its own byte order

    constexpr void store_be16(std::span<std::uint8_t, 2> out, std::uint16_t v) noexcept {
        out[0] = static_cast<std::uint8_t>(v >> 8);
        out[1] = static_cast<std::uint8_t>(v & 0xFFu);
    }

    constexpr void store_be32(std::span<std::uint8_t, 4> out, std::uint32_t v) noexcept {
        out[0] = static_cast<std::uint8_t>(v >> 24);
        out[1] = static_cast<std::uint8_t>((v >> 16) & 0xFFu);
        out[2] = static_cast<std::uint8_t>((v >> 8) & 0xFFu);
        out[3] = static_cast<std::uint8_t>(v & 0xFFu);
    }

    constexpr std::uint16_t load_be16(std::span<const std::uint8_t, 2> in) noexcept {
        return static_cast<std::uint16_t>((std::uint16_t{in[0]} << 8) | in[1]);
    }

    constexpr std::uint32_t load_be32(std::span<const std::uint8_t, 4> in) noexcept {
        return (std::uint32_t{in[0]} << 24) | (std::uint32_t{in[1]} << 16) | (std::uint32_t{in[2]} << 8) | std::uint32_t{in[3]};
    }

}  // namespace orbis