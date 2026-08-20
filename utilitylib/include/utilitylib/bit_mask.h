/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace UtilityLib
{

template <uint8_t mask>
concept ValidBitMask = mask < 8;

template <uint8_t mask>
    requires ValidBitMask<mask>
constexpr auto BitMask = static_cast<std::byte>(1 << mask);

} // namespace UtilityLib