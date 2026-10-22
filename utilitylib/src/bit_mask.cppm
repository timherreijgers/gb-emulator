/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

export module utilitylib:bit_mask;

import std;

namespace UtilityLib
{

export template <std::uint8_t mask>
concept ValidBitMask = mask < 8;

export template <std::uint8_t mask>
    requires ValidBitMask<mask>
constexpr auto BitMask = static_cast<std::byte>(1 << mask);

} // namespace UtilityLib
