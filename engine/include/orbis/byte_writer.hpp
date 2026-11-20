#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>

#include <orbis/endian.hpp>

namespace orbis {

    // Builds a byte sequence by appending values in network byte order
    // Appending cannot fail so no method returns a Result
    class ByteWriter {
        public:
            ByteWriter& u8(std::uint8_t v) {
                buf_.push_back(v);
                return *this;
            }

            ByteWriter& u16(std::uint16_t v) {
                std::array<std::uint8_t, 2> tmp{};
                store_be16(tmp, v);
                buf_.insert(buf_.end(), tmp.begin(), tmp.end());
                return *this;
            }

            ByteWriter& u32(std::uint32_t v) {
                std::array<std::uint8_t, 4> tmp{};
                store_be32(tmp, v);
                buf_.insert(buf_.end(), tmp.begin(), tmp.end());
                return *this;
            }

            // Appends raw bytes unchanged
            ByteWriter& bytes(std::span<const std::uint8_t> data) {
                buf_.insert(buf_.end(), data.begin(), data.end());
                return *this;
            }

            std::size_t size() const noexcept { return buf_.size(); }

            const std::vector<std::uint8_t>& data() const noexcept { return buf_; }

            // Moves the accumulated bytes out and leaves the writer empty
            std::vector<std::uint8_t> release() {
                std::vector<std::uint8_t> out = std::move(buf_);
                buf_.clear();
                return out;
            }

        private:
            std::vector<std::uint8_t> buf_;
    };

}  // namespace orbis
