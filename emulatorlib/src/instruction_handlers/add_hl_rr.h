/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/flag_helpers.h"
#include "instruction_handlers/register_concepts.h"
#include "instruction_handlers/register_io_helpers.h"
#include "utilitylib/add_with_carry.h"
#include "utilitylib/bit_mask.h"

namespace EmulatorLib
{

template <ReturnsRegister16Bit SourceRegister>
constexpr auto ExecuteAddHlRR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    const auto source = SourceRegister{}(cpuRegisters).value;
    const auto [result, carryPerBit] = UtilityLib::AddWithCarry(cpuRegisters.lRegister.value, static_cast<std::byte>(source));
    cpuRegisters.lRegister = result;
    cpuRegisters.flags = (cpuRegisters.flags.value & CpuFlags::Zero.AsByte()) |
                         ((carryPerBit & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b) |
                         ((carryPerBit & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b);
    co_yield std::monostate{};

    const auto [highResult, highCarry] = UtilityLib::AddWithCarryIn(
        cpuRegisters.hRegister.value, static_cast<std::byte>(source >> 8), CarryIn(cpuRegisters));
    cpuRegisters.hRegister = highResult;
    cpuRegisters.flags = (cpuRegisters.flags.value & CpuFlags::Zero.AsByte()) |
                         ((highCarry & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b) |
                         ((highCarry & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b);
    co_return;
};

constexpr auto ExecuteAddHlBC = ExecuteAddHlRR<RegisterBC>;
constexpr auto ExecuteAddHlDE = ExecuteAddHlRR<RegisterDE>;
constexpr auto ExecuteAddHlHL = ExecuteAddHlRR<RegisterHL>;
constexpr auto ExecuteAddHlSP = ExecuteAddHlRR<RegisterSP>;

} // namespace EmulatorLib
