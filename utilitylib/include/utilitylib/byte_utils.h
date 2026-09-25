/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <utilitylib/integral_suffixes.h>
#include <utilitylib/numeric_limits.h>

#include <concepts>
#include <cstddef>
#include <print>
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

template <>
struct std::formatter<std::byte>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const std::byte& obj, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "0x{:02X}", static_cast<uint8_t>(obj));
    }
};
