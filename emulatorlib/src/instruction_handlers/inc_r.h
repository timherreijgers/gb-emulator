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
constexpr auto ExecuteIncR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto [result, carry] = UtilityLib::AddWithCarry(targetRegister.value, 0x01_b);
    targetRegister = result;

    cpuRegisters.flags.value &= CpuFlags::Carry.AsByte();
    cpuRegisters.flags.value |= result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b;
    cpuRegisters.flags.value |= (carry & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b;

    co_return;
};

constexpr auto ExecuteIncB = ExecuteIncR<RegisterB>;
constexpr auto ExecuteIncC = ExecuteIncR<RegisterC>;
constexpr auto ExecuteIncD = ExecuteIncR<RegisterD>;
constexpr auto ExecuteIncE = ExecuteIncR<RegisterE>;
constexpr auto ExecuteIncH = ExecuteIncR<RegisterH>;
constexpr auto ExecuteIncL = ExecuteIncR<RegisterL>;
constexpr auto ExecuteIncA = ExecuteIncR<RegisterA>;

} // namespace EmulatorLib