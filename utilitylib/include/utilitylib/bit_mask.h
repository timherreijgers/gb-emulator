/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace UtilityLib
{

constexpr auto BitMask = [](const std::uint8_t bit) {
    return static_cast<std::byte>(1 << bit);
};

} // namespace UtilityLib