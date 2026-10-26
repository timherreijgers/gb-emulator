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

inline auto ExecuteRlca(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    const auto newBit0 = static_cast<bool>(cpuRegisters.accumulator & 0x80_b) ? 0x01_b : 0x00_b;
    cpuRegisters.accumulator = (cpuRegisters.accumulator << 1) | newBit0;
    cpuRegisters.flags = 0x00_b | (static_cast<bool>(newBit0) ? CpuFlags::Carry.AsByte() : 0x00_b);

    co_return;
}

} // namespace EmulatorLib