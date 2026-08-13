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
concept IntegralOrByte = std::integral<T> || std::is_same_v<T, std::byte>;

template <IntegralOrByte T>
struct AddWithCarryResult
{
    T result;
    T carryBits;
};

template <IntegralOrByte T>
[[nodiscard]] constexpr auto AddWithCarry(T left, T right) noexcept -> AddWithCarryResult<T>
{
    const T result = static_cast<T>(left + right);
    const T carryBits = static_cast<T>((left & right) | ((left | right) & static_cast<T>(~result)));
    return {result, carryBits};
}

} // namespace UtilityLib