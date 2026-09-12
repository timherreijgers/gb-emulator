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
concept SubWithBorrowOperand = std::integral<T> || std::is_same_v<T, std::byte>;

template <SubWithBorrowOperand T>
struct [[nodiscard]] SubWithBorrowResult
{
    T result;
    T borrowBits;
};

// Implemented using Borrow-Lookahead subtractor algorithm
constexpr auto SubWithBorrow = [](SubWithBorrowOperand auto left, SubWithBorrowOperand auto right) constexpr noexcept -> SubWithBorrowResult<decltype(left)> {
    static_assert(typeid(decltype(left)) == typeid(decltype(right)));

    const auto result = static_cast<decltype(left)>(left - right);

    const auto gamma = ~left & right;
    const auto pi = ~left | right;

    const auto borrowBits = static_cast<decltype(left)>(gamma | (pi & result));
    return {result, borrowBits};
};

} // namespace UtilityLib
