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

inline auto ExecuteRla(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    const auto newValue = static_cast<uint16_t>(cpuRegisters.accumulator.value) << 1;
    const auto newFlagsValue = newValue & 0x100 ? CpuFlags::Carry.AsByte() : 0x00_b;

    cpuRegisters.accumulator = static_cast<std::byte>(newValue) | ((cpuRegisters.flags & CpuFlags::Carry.AsByte()) > 0x00_b ? 0x01_b : 0x00_b);
    cpuRegisters.flags = newFlagsValue;

    co_return;
}

} // namespace EmulatorLib