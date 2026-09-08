/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "byte_utils.h"

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace UtilityLib
{

template <typename T>
concept IntegralOrByte = std::unsigned_integral<T> || std::is_same_v<T, std::byte>;

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

template <IntegralOrByte T>
[[nodiscard]] constexpr auto AddWithCarryIn(T left, T right, bool carryIn) noexcept -> AddWithCarryResult<T>
{
    const auto [result, carryBits] = AddWithCarry(left, right);
    if (!carryIn)
    {
        return {result, carryBits};
    }

    const auto [resultWithCarry, carryBitsWithCarry] = AddWithCarry(result, static_cast<T>(1));
    return {resultWithCarry, static_cast<T>(carryBits | carryBitsWithCarry)};
}

} // namespace UtilityLib
