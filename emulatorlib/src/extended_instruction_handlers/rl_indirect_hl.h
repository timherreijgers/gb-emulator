/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/register_concepts.h"
#include "instruction_handlers/register_io_helpers.h"

namespace EmulatorLib
{

inline auto ExecuteRlIndirectHL(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto carryIn = static_cast<bool>(cpuRegisters.flags & CpuFlags::Carry.AsByte()) ? 0x01_b : 0x00_b;
    const auto newCarry = static_cast<bool>(cpuRegisters.zRegister & 0x80_b);

    cpuRegisters.zRegister = (cpuRegisters.zRegister << 1) | carryIn;
    cpuRegisters.flags = (newCarry ? CpuFlags::Carry.AsByte() : 0x00_b) | (cpuRegisters.zRegister == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b);

    addressBus.WriteToAddress(cpuRegisters.hlRegister, cpuRegisters.zRegister);
    co_yield std::monostate{};

    co_return;
}

} // namespace EmulatorLib
