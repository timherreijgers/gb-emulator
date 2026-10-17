/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <concepts>
#include <cstddef>
#include <typeinfo>

export module utilitylib:mathematical;

import :byte_utils;

namespace UtilityLib
{

export template <typename T>
concept IntegralOrByte = std::unsigned_integral<T> || std::is_same_v<T, std::byte>;

export template <IntegralOrByte T>
struct [[nodiscard]] MathematicalResult
{
    T result;
    T carryBits;
};

export constexpr auto AddWithCarry = [](IntegralOrByte auto left, IntegralOrByte auto right) noexcept -> MathematicalResult<decltype(left)> {
    static_assert(typeid(decltype(left)) == typeid(decltype(right)));

    const auto result = static_cast<decltype(left)>(left + right);
    const auto carryBits = static_cast<decltype(left)>((left & right) | ((left | right) & static_cast<decltype(left)>(~result)));
    return {result, carryBits};
};

export template <IntegralOrByte T>
[[nodiscard]] constexpr auto AddWithCarryIn(T left, T right, bool carryIn) noexcept -> MathematicalResult<T>
{
    const auto [result, carryBits] = AddWithCarry(left, right);
    if (!carryIn)
    {
        return {result, carryBits};
    }

    const auto [resultWithCarry, carryBitsWithCarry] = AddWithCarry(result, static_cast<T>(1));
    return {resultWithCarry, static_cast<T>(carryBits | carryBitsWithCarry)};
}

// Implemented using Borrow-Lookahead subtractor algorithm
export constexpr auto SubWithBorrow = [](IntegralOrByte auto left, IntegralOrByte auto right) constexpr noexcept -> MathematicalResult<decltype(left)> {
    static_assert(typeid(decltype(left)) == typeid(decltype(right)));

    const auto result = static_cast<decltype(left)>(left - right);

    const auto gamma = ~left & right;
    const auto pi = ~left | right;

    const auto borrowBits = static_cast<decltype(left)>(gamma | (pi & result));
    return {result, borrowBits};
};

export constexpr auto SubWithBorrowIn = [](IntegralOrByte auto left, IntegralOrByte auto right, bool borrowIn) constexpr noexcept -> MathematicalResult<decltype(left)> {
    static_assert(typeid(decltype(left)) == typeid(decltype(right)));

    const auto [result, borrowBits] = SubWithBorrow(left, right);
    if (!borrowIn)
    {
        return {result, borrowBits};
    }

    const auto [resultWithBorrow, borrowBitsWithBorrow] = SubWithBorrow(result, static_cast<decltype(left)>(1));
    return {resultWithBorrow, static_cast<decltype(left)>(borrowBits | borrowBitsWithBorrow)};
};

} // namespace UtilityLib
