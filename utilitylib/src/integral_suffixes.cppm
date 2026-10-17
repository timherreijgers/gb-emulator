/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <cstddef>
#include <cstdint>

export module utilitylib:integral_suffixes;

export constexpr auto operator""_b(unsigned long long value) -> std::byte
{
    return static_cast<std::byte>(value);
}

export constexpr auto operator""_u8(unsigned long long value) -> uint8_t
{
    return static_cast<uint8_t>(value);
}

export constexpr auto operator""_s8(unsigned long long value) -> int8_t
{
    return static_cast<int8_t>(value);
}

export constexpr auto operator""_u16(unsigned long long value) -> uint16_t
{
    return static_cast<uint16_t>(value);
}

export constexpr auto operator""_s16(unsigned long long value) -> int16_t
{
    return static_cast<int16_t>(value);
}

export constexpr auto operator""_u32(unsigned long long value) -> uint32_t
{
    return static_cast<uint32_t>(value);
}

export constexpr auto operator""_s32(unsigned long long value) -> int32_t
{
    return static_cast<int32_t>(value);
}

export constexpr auto operator""_u64(unsigned long long value) -> uint64_t
{
    return static_cast<uint64_t>(value);
}

export constexpr auto operator""_s64(unsigned long long value) -> int64_t
{
    return static_cast<int64_t>(value);
}
