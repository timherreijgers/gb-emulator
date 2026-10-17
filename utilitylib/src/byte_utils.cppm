/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <concepts>
#include <cstddef>
#include <print>
#include <utility>

export module utilitylib:byte_utils;

export [[nodiscard]] constexpr auto operator-(const std::byte left, const std::byte right) -> std::byte
{
    return static_cast<std::byte>(std::to_underlying(left) - std::to_underlying(right));
}

export [[nodiscard]] constexpr auto operator-(const std::byte left, std::integral auto right) -> std::byte
{
    return static_cast<std::byte>(std::to_underlying(left) - right);
}

export [[nodiscard]] constexpr auto operator+(const std::byte left, const std::byte right) -> std::byte
{
    return static_cast<std::byte>(std::to_underlying(left) + std::to_underlying(right));
}

export [[nodiscard]] constexpr auto operator+(const std::byte left, std::integral auto right) -> std::byte
{
    return static_cast<std::byte>(std::to_underlying(left) + right);
}

export template <>
struct std::formatter<std::byte>
{
    constexpr auto parse(std::format_parse_context& context) const noexcept
    {
        return context.begin();
    }

    auto format(const std::byte& object, std::format_context& context) const noexcept
    {
        return std::format_to(context.out(), "0x{:02X}", static_cast<uint8_t>(object));
    }
};

export namespace UtilityLib
{

template <typename T>
struct numeric_limits : std::numeric_limits<T>
{
};

template <>
struct numeric_limits<std::byte> : std::numeric_limits<unsigned char>
{
};

} // namespace UtilityLib