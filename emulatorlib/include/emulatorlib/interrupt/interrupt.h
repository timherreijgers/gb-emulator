/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "utilitylib/bit_mask.h"

#include <cstdint>

namespace EmulatorLib
{

enum class Interrupt : std::uint8_t
{
    VBlank = static_cast<uint8_t>(UtilityLib::BitMask<0>),
    Lcd = static_cast<uint8_t>(UtilityLib::BitMask<1>),
    Timer = static_cast<uint8_t>(UtilityLib::BitMask<2>),
    Serial = static_cast<uint8_t>(UtilityLib::BitMask<3>),
    Joypad = static_cast<uint8_t>(UtilityLib::BitMask<4>)
};

} // namespace EmulatorLib
