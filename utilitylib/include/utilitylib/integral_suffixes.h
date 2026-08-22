/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <cstddef>
#include <cstdint>

constexpr std::byte operator""_b(unsigned long long v)
{
    return static_cast<std::byte>(v);
}

constexpr uint8_t operator""_u8(unsigned long long v)
{
    return static_cast<uint8_t>(v);
}

constexpr int8_t operator""_s8(unsigned long long v)
{
    return static_cast<int8_t>(v);
}

constexpr uint16_t operator""_u16(unsigned long long v)
{
    return static_cast<uint16_t>(v);
}

constexpr int16_t operator""_s16(unsigned long long v)
{
    return static_cast<int16_t>(v);
}

constexpr uint32_t operator""_u32(unsigned long long v)
{
    return static_cast<uint32_t>(v);
}

constexpr int32_t operator""_s32(unsigned long long v)
{
    return static_cast<int32_t>(v);
}

constexpr uint64_t operator""_u64(unsigned long long v)
{
    return static_cast<uint64_t>(v);
}

constexpr int64_t operator""_s64(unsigned long long v)
{
    return static_cast<int64_t>(v);
}