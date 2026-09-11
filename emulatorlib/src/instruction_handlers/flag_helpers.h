/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"

#include "utilitylib/bit_mask.h"

namespace EmulatorLib
{

[[nodiscard]] constexpr auto CarryIn(const CpuRegisters& registers) noexcept -> bool
{
    return (registers.flags.value & CpuFlags::Carry.AsByte()) > 0x00_b;
}

constexpr auto ApplyAdditionFlags(CpuRegisters& registers, const std::byte result, const std::byte carryPerBit) noexcept -> void
{
    registers.flags.value = (result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b) |
                            ((carryPerBit & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b) |
                            ((carryPerBit & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b);
}

constexpr auto ApplySubtractionFlags(CpuRegisters& registers, const std::byte result, const std::byte carryPerBit) noexcept -> void
{
    registers.flags.value = CpuFlags::Subtract.AsByte() |
                            (result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b) |
                            ((carryPerBit & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b) |
                            ((carryPerBit & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b);
}

} // namespace EmulatorLib