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
concept SubWithBorrowOperand = std::integral<T> || std::is_same_v<T, std::byte>;

template <SubWithBorrowOperand T>
struct SubWithBorrowResult
{
    T result;
    T borrowBits;
};

// Implemented using Borrow-Lookahead subtractor algorithm
template <SubWithBorrowOperand T>
[[nodiscard]] constexpr auto SubWithBorrow(T left, T right) noexcept -> SubWithBorrowResult<T>
{
    const T result = static_cast<T>(left - right);

    const auto gamma = ~left & right;
    const auto pi = ~left | right;

    const T borrowBits = static_cast<T>(gamma | (pi & result));
    return {result, borrowBits};
}

} // namespace UtilityLib
