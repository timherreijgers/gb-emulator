/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <concepts>
#include <cstddef>

namespace UtilityLib
{

template <typename T>
concept IntegralOrByte = std::unsigned_integral<T> || std::is_same_v<T, std::byte>;

template <IntegralOrByte T>
struct [[nodiscard]] MathematicalResult
{
    T result;
    T carryBits;
};

} // namespace UtilityLib
