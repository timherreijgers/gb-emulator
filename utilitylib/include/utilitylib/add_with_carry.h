/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <concepts>

namespace UtilityLib
{

template<std::integral T>
struct AddWithCarryResult
{
    T result;
    T carryBits;
};

template<std::unsigned_integral T>
[[nodiscard]] constexpr auto AddWithCarry(T left, T right) noexcept -> AddWithCarryResult<T>
{
    const T result = static_cast<T>(left + right);
    const T carryBits = static_cast<T>((left & right) | ((left | right) & static_cast<T>(~result)));
    return {result, carryBits};
}

} // namespace UtilityLib