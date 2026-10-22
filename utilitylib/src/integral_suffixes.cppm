/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

export module utilitylib:integral_suffixes;

import std;

export constexpr auto operator""_b(unsigned long long value) -> std::byte
{
    return static_cast<std::byte>(value);
}

export constexpr auto operator""_u8(unsigned long long value) -> std::uint8_t
{
    return static_cast<std::uint8_t>(value);
}

export constexpr auto operator""_s8(unsigned long long value) -> std::int8_t
{
    return static_cast<std::int8_t>(value);
}

export constexpr auto operator""_u16(unsigned long long value) -> std::uint16_t
{
    return static_cast<std::uint16_t>(value);
}

export constexpr auto operator""_s16(unsigned long long value) -> std::int16_t
{
    return static_cast<std::int16_t>(value);
}

export constexpr auto operator""_u32(unsigned long long value) -> std::uint32_t
{
    return static_cast<std::uint32_t>(value);
}

export constexpr auto operator""_s32(unsigned long long value) -> std::int32_t
{
    return static_cast<std::int32_t>(value);
}

export constexpr auto operator""_u64(unsigned long long value) -> std::uint64_t
{
    return static_cast<std::uint64_t>(value);
}

export constexpr auto operator""_s64(unsigned long long value) -> std::int64_t
{
    return static_cast<std::int64_t>(value);
}
