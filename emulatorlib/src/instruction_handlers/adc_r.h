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

namespace EmulatorLib
{

template <ReturnsRegister8Bit TargetRegister>
constexpr auto ExecuteAdcR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto carryIn = CarryIn(cpuRegisters);
    const auto [result, carry] = UtilityLib::AddWithCarryIn(cpuRegisters.accumulator.value, targetRegister.value, carryIn);
    cpuRegisters.accumulator = result;

    ApplyAdditionFlags(cpuRegisters, result, carry);

    co_return;
};

constexpr auto ExecuteAdcB = ExecuteAdcR<RegisterB>;
constexpr auto ExecuteAdcC = ExecuteAdcR<RegisterC>;
constexpr auto ExecuteAdcD = ExecuteAdcR<RegisterD>;
constexpr auto ExecuteAdcE = ExecuteAdcR<RegisterE>;
constexpr auto ExecuteAdcH = ExecuteAdcR<RegisterH>;
constexpr auto ExecuteAdcL = ExecuteAdcR<RegisterL>;
constexpr auto ExecuteAdcA = ExecuteAdcR<RegisterA>;

} // namespace EmulatorLib
