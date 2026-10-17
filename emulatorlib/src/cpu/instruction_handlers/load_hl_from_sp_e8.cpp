/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:load_hl_from_sp_e8;

import emulatorlib.cpu;
import emulatorlib.address_bus;
import utilitylib;

namespace EmulatorLib
{

[[nodiscard]] inline auto ExecuteLoadSPFromHlPlusE8(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    const auto zSign = UtilityLib::BitMask<7> & cpuRegisters.zRegister;
    const auto [result, carryPerBit] = UtilityLib::AddWithCarry(static_cast<std::byte>(cpuRegisters.stackPointer.value), static_cast<std::byte>(cpuRegisters.zRegister.value));
    cpuRegisters.lRegister = result;
    cpuRegisters.flags = 0x00_b | ((carryPerBit & UtilityLib::BitMask<3>) > 0_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b) | ((carryPerBit & UtilityLib::BitMask<7>) > 0_b ? CpuFlags::Carry.AsByte() : 0x00_b);
    co_yield std::monostate{};

    const auto adjustment = zSign > 0_b ? 0xFF_b : 0x00_b;
    cpuRegisters.hRegister = static_cast<std::byte>(cpuRegisters.stackPointer.value >> 8) + adjustment +
                             (CarryIn(cpuRegisters) ? 0x01_b : 0x00_b);
    co_return;
}

} // namespace EmulatorLib
