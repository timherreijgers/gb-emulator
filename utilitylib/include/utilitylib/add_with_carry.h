/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "utilitylib/mathematical_result.h"

#include <typeinfo>

namespace UtilityLib
{

constexpr auto AddWithCarry = [](IntegralOrByte auto left, IntegralOrByte auto right) noexcept -> MathematicalResult<decltype(left)> {
    static_assert(typeid(decltype(left)) == typeid(decltype(right)));

    const auto result = static_cast<decltype(left)>(left + right);
    const auto carryBits = static_cast<decltype(left)>((left & right) | ((left | right) & static_cast<decltype(left)>(~result)));
    return {result, carryBits};
};

template <IntegralOrByte T>
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

} // namespace UtilityLib
