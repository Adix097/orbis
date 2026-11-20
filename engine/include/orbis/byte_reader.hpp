#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include <orbis/endian.hpp>
#include <orbis/result.hpp>

namespace orbis {

    // Reads values in network byte order from a byte sequence it does not own
    // The underlying bytes must outlive the reader
    // every read returns a Result 
    // A read that would run past the end fails with Error::UnexpectedEnd and consumes nothing
    class ByteReader {
        public:
            explicit ByteReader(std::span<const std::uint8_t> data) noexcept : data_(data) {}

            std::size_t position() const noexcept { return pos_; }
            std::size_t remaining() const noexcept { return data_.size() - pos_; }
            bool at_end() const noexcept { return remaining() == 0; }

            Result<std::uint8_t> u8() noexcept {
                if (remaining() < 1) return Error::UnexpectedEnd;
                return data_[pos_++];
            }

            Result<std::uint16_t> u16() noexcept {
                if (remaining() < 2) return Error::UnexpectedEnd;
                const std::uint16_t v = load_be16(data_.subspan(pos_).first<2>());
                pos_ += 2;
                return v;
            }

            Result<std::uint32_t> u32() noexcept {
                if (remaining() < 4) return Error::UnexpectedEnd;
                const std::uint32_t v = load_be32(data_.subspan(pos_).first<4>());
                pos_ += 4;
                return v;
            }

            // Returns a view of the next n bytes (no copy)
            // Compares against remaining() rather than computing pos_ + n,
            Result<std::span<const std::uint8_t>> bytes(std::size_t n) noexcept {
                if (n > remaining()) return Error::UnexpectedEnd;
                const auto view = data_.subspan(pos_, n);
                pos_ += n;
                return view;
            }

        private:
            std::span<const std::uint8_t> data_;
            std::size_t pos_ = 0;
    };

}  // namespace orbis