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

template <IntegralOrByte T, IntegralOrByte... Ts>
    requires(sizeof...(Ts) >= 2)
[[nodiscard]] constexpr auto AddWithCarry(T first, Ts... rest) noexcept -> AddWithCarryResult<T>
{
    T accResult = first;
    T accCarry = static_cast<T>(0);
    (
        [&](T val) {
            const auto [r, c] = AddWithCarry(accResult, val);
            accResult = r;
            accCarry = static_cast<T>(accCarry | c);
        }(static_cast<T>(rest)),
        ...);
    return {accResult, accCarry};
}

} // namespace UtilityLib