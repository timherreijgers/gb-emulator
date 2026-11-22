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

inline auto ExecuteSlaIndirectHL(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto newCarry = static_cast<bool>(cpuRegisters.zRegister & 0x80_b);
    const auto result = cpuRegisters.zRegister << 1;
    cpuRegisters.zRegister = result;
    cpuRegisters.flags = (static_cast<bool>(result) ? 0x00_b : CpuFlags::Zero.AsByte()) | (newCarry ? CpuFlags::Carry.AsByte() : 0x00_b);
    addressBus.WriteToAddress(cpuRegisters.hlRegister, cpuRegisters.zRegister);
    co_yield std::monostate{};

    co_return;
}

} // namespace EmulatorLib
