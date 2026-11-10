/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "utilitylib/add_with_carry.h"
#include "utilitylib/bit_mask.h"

namespace EmulatorLib
{

[[nodiscard]] inline auto ExecuteAddSPe8(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    const auto [lowByte, carryPerBit] = UtilityLib::AddWithCarry(
        static_cast<std::byte>(cpuRegisters.stackPointer.value), static_cast<std::byte>(cpuRegisters.zRegister.value));
    cpuRegisters.flags = 0x00_b |
                         ((carryPerBit & UtilityLib::BitMask<3>) > 0_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b) |
                         ((carryPerBit & UtilityLib::BitMask<7>) > 0_b ? CpuFlags::Carry.AsByte() : 0x00_b);
    co_yield std::monostate{};

    const auto offsetSign = (UtilityLib::BitMask<7> & cpuRegisters.zRegister) > 0_b ? 0xFF_b : 0x00_b;
    const auto highByte = static_cast<std::byte>(cpuRegisters.stackPointer.value >> 8) + offsetSign +
                          ((carryPerBit & UtilityLib::BitMask<7>) > 0_b ? 0x01_b : 0x00_b);
    cpuRegisters.stackPointer = static_cast<uint16_t>((static_cast<uint16_t>(highByte) << 8) |
                                                      static_cast<uint8_t>(lowByte));
    co_return;
}

} // namespace EmulatorLib
