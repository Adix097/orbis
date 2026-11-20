#pragma once

#include <string_view>

namespace orbis {

    enum class Error {
        UnexpectedEnd,  // tried to read more bytes than remain in the input
    };

    constexpr std::string_view to_string(Error e) noexcept {
        switch (e) {
            case Error::UnexpectedEnd:
                return "unexpected end of input";
        }
        return "unknown error";
    }

}  // namespace orbis