/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

namespace EmulatorLib
{

inline auto ExecuteRra(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    const auto newValue = static_cast<uint16_t>(cpuRegisters.accumulator.value) << 7;
    const auto newFlagsValue = newValue & 0x80 ? CpuFlags::Carry.AsByte() : 0x00_b;

    cpuRegisters.accumulator = static_cast<std::byte>(newValue >> 8) | ((cpuRegisters.flags & CpuFlags::Carry.AsByte()) > 0x00_b ? 0x80_b : 0x00_b);
    cpuRegisters.flags = newFlagsValue;

    co_return;
}

} // namespace EmulatorLib