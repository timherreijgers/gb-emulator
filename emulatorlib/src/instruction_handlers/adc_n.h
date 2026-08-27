/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/register_io_helpers.h"
#include "utilitylib/add_with_carry.h"
#include "utilitylib/bit_mask.h"

namespace EmulatorLib
{

constexpr auto ExecuteAdcAn8 = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    const auto carryIn = (cpuRegisters.flags.value & CpuFlags::Carry.AsByte()) > 0x00_b ? 0x01_b : 0x00_b;
    const auto [result, carry] = UtilityLib::AddWithCarry(cpuRegisters.accumulator.value, cpuRegisters.zRegister.value, carryIn);
    cpuRegisters.accumulator = result;

    cpuRegisters.flags.value = 0x00_b;
    cpuRegisters.flags.value |= result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b;
    cpuRegisters.flags.value |= (carry & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b;
    cpuRegisters.flags.value |= (carry & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b;

    co_return;
};

} // namespace EmulatorLib
