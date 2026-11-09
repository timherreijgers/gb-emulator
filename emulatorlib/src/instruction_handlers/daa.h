/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"
#include "utilitylib/byte_utils.h"

namespace EmulatorLib
{

inline auto ExecuteDaa(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    if (static_cast<bool>(cpuRegisters.flags & CpuFlags::Subtract.AsByte()))
    {
        auto adjustment = 0x00_b;
        adjustment += static_cast<bool>(cpuRegisters.flags & CpuFlags::HalfCarry.AsByte()) ? 0x06_b : 0x00_b;
        adjustment += static_cast<bool>(cpuRegisters.flags & CpuFlags::Carry.AsByte()) ? 0x60_b : 0x00_b;

        cpuRegisters.accumulator -= adjustment;
    }
    else
    {
        const auto addHalfCarryOffset = static_cast<bool>(cpuRegisters.flags & CpuFlags::HalfCarry.AsByte()) || (cpuRegisters.accumulator & 0x0F_b) > 0x09_b;
        const auto addCarryOffset = static_cast<bool>(cpuRegisters.flags & CpuFlags::Carry.AsByte()) || (cpuRegisters.accumulator > 0x99_b);

        auto adjustment = 0x00_b;
        adjustment += addHalfCarryOffset ? 0x06_b : 0x00_b;
        adjustment += addCarryOffset ? 0x60_b : 0x00_b;
        cpuRegisters.accumulator += adjustment;

        cpuRegisters.flags &= ~CpuFlags::Carry.AsByte();
        cpuRegisters.flags |= addCarryOffset ? CpuFlags::Carry.AsByte() : 0x00_b;
    }

    cpuRegisters.flags &= ~(CpuFlags::Zero.AsByte() | CpuFlags::HalfCarry.AsByte());
    cpuRegisters.flags |= cpuRegisters.accumulator == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b;
    co_return;
}

} // namespace EmulatorLib
