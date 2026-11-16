#pragma once

#include <exception>
#include <utility>
#include <variant>

#include <orbis/error.hpp>

namespace orbis {

    // Holds either a value of type T or an Error
    // Malformed input is reported through Result 
    // calling value() on a failed Result, 
    // or error() on a successful one, is a programming bug and
    // terminates the program
    template <typename T>
    class [[nodiscard]] Result {
        public:
            Result(T value) : data_(std::in_place_index<0>, std::move(value)) {}
            Result(Error error) : data_(std::in_place_index<1>, error) {}
        
            bool ok() const noexcept { return data_.index() == 0; }
            explicit operator bool() const noexcept { return ok(); }
        
            T& value() noexcept {
                if (!ok()) std::terminate();
                return *std::get_if<0>(&data_);
            }
        
            const T& value() const noexcept {
                if (!ok()) std::terminate();
                return *std::get_if<0>(&data_);
            }
        
            Error error() const noexcept {
                if (ok()) std::terminate();
                return *std::get_if<1>(&data_);
            }
        
        private:
            std::variant<T, Error> data_;
    };

}  // namespace orbis
