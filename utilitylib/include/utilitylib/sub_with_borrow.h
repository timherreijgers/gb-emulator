/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "utilitylib/mathematical_result.h"

#include <typeinfo>

namespace UtilityLib
{

// Implemented using Borrow-Lookahead subtractor algorithm
constexpr auto SubWithBorrow = [](IntegralOrByte auto left, IntegralOrByte auto right) constexpr noexcept -> MathematicalResult<decltype(left)> {
    static_assert(typeid(decltype(left)) == typeid(decltype(right)));

    const auto result = static_cast<decltype(left)>(left - right);

    const auto gamma = ~left & right;
    const auto pi = ~left | right;

    const auto borrowBits = static_cast<decltype(left)>(gamma | (pi & result));
    return {result, borrowBits};
};

} // namespace UtilityLib
