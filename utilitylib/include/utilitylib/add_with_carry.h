/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "byte_utils.h"

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <typeinfo>

namespace UtilityLib
{

template <typename T>
concept IntegralOrByte = std::unsigned_integral<T> || std::is_same_v<T, std::byte>;

template <IntegralOrByte T>
struct [[nodiscard]] AddWithCarryResult
{
    T result;
    T carryBits;
};

constexpr auto AddWithCarry = [](IntegralOrByte auto left, IntegralOrByte auto right) noexcept -> AddWithCarryResult<decltype(left)> {
    static_assert(typeid(decltype(left)) == typeid(decltype(right)));

    const auto result = static_cast<decltype(left)>(left + right);
    const auto carryBits = static_cast<decltype(left)>((left & right) | ((left | right) & static_cast<decltype(left)>(~result)));
    return {result, carryBits};
};

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
