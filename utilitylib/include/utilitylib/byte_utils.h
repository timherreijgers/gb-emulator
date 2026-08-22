/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <utilitylib/byte_limits.h>
#include <utilitylib/integral_suffixes.h>

#include <concepts>
#include <cstddef>
#include <utility>

[[nodiscard]] constexpr auto operator-(const std::byte lhs, const std::byte rhs) -> std::byte
{
    return static_cast<std::byte>(std::to_underlying(lhs) - std::to_underlying(rhs));
}

[[nodiscard]] constexpr auto operator-(const std::byte lhs, std::integral auto rhs) -> std::byte
{
    return static_cast<std::byte>(std::to_underlying(lhs) - rhs);
}

[[nodiscard]] constexpr auto operator+(const std::byte lhs, const std::byte rhs) -> std::byte
{
    return static_cast<std::byte>(std::to_underlying(lhs) + std::to_underlying(rhs));
}

[[nodiscard]] constexpr auto operator+(const std::byte lhs, std::integral auto rhs) -> std::byte
{
    return static_cast<std::byte>(std::to_underlying(lhs) + rhs);
}