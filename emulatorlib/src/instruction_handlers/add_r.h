/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/register_concepts.h"
#include "instruction_handlers/register_io_helpers.h"
#include "utilitylib/add_with_carry.h"
#include "utilitylib/bit_mask.h"

namespace EmulatorLib
{

template <ReturnsRegister8Bit TargetRegister>
constexpr auto ExecuteAddR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto [result, carry] = UtilityLib::AddWithCarry(cpuRegisters.accumulator.value, targetRegister.value);
    cpuRegisters.accumulator = result;

    cpuRegisters.flags.value = 0x00_b;
    cpuRegisters.flags.value |= result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b;
    cpuRegisters.flags.value |= (carry & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b;
    cpuRegisters.flags.value |= (carry & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b;

    co_return;
};

constexpr auto ExecuteAddB = ExecuteAddR<RegisterB>;
constexpr auto ExecuteAddC = ExecuteAddR<RegisterC>;
constexpr auto ExecuteAddD = ExecuteAddR<RegisterD>;
constexpr auto ExecuteAddE = ExecuteAddR<RegisterE>;
constexpr auto ExecuteAddH = ExecuteAddR<RegisterH>;
constexpr auto ExecuteAddL = ExecuteAddR<RegisterL>;
constexpr auto ExecuteAddA = ExecuteAddR<RegisterA>;

} // namespace EmulatorLib
