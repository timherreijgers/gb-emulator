/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <cstddef>
#include <cstdint>

export module utilitylib:bit_mask;

namespace UtilityLib
{

export template <uint8_t mask>
concept ValidBitMask = mask < 8;

export template <uint8_t mask>
    requires ValidBitMask<mask>
constexpr auto BitMask = static_cast<std::byte>(1 << mask);

} // namespace UtilityLib
