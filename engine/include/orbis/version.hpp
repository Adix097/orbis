#pragma once

#include <string_view>

namespace orbis {

    // Returns the ORBIS engine version as "major.minor.patch".
    std::string_view version() noexcept;

} // namespace orbis