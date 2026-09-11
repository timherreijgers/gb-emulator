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
#include "utilitylib/sub_with_borrow.h"

namespace EmulatorLib
{

template <ReturnsRegister8Bit TargetRegister>
constexpr auto ExecuteAddR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto [result, carry] = UtilityLib::AddWithCarry(cpuRegisters.accumulator.value, targetRegister.value);
    cpuRegisters.accumulator = result;

    ApplyAdditionFlags(cpuRegisters, result, carry);

    co_return;
};

template <ReturnsRegister8Bit TargetRegister>
constexpr auto ExecuteSubR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto [result, carry] = UtilityLib::SubWithBorrow(cpuRegisters.accumulator.value, targetRegister.value);
    cpuRegisters.accumulator = result;

    ApplySubtractionFlags(cpuRegisters, result, carry);

    co_return;
};

template <ReturnsRegister8Bit TargetRegister>
constexpr auto ExecuteMathOperandR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters, std::function<UtilityLib::SubWithBorrowResult<std::byte>(std::byte, std::byte)> operand, std::function<void(CpuRegisters&, std::byte, std::byte)> flagFunction) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto [result, carry] = operand(cpuRegisters.accumulator.value, targetRegister.value);
    // const auto [result, carry] = UtilityLib::SubWithBorrow(cpuRegisters.accumulator.value, targetRegister.value);
    cpuRegisters.accumulator = result;

    flagFunction(cpuRegisters, result, carry);

    co_return;
};

constexpr auto ExecuteAddA = ExecuteAddR<RegisterA>;
constexpr auto ExecuteAddB = ExecuteAddR<RegisterB>;
constexpr auto ExecuteAddC = ExecuteAddR<RegisterC>;
constexpr auto ExecuteAddD = ExecuteAddR<RegisterD>;
constexpr auto ExecuteAddE = ExecuteAddR<RegisterE>;
constexpr auto ExecuteAddH = ExecuteAddR<RegisterH>;
constexpr auto ExecuteAddL = ExecuteAddR<RegisterL>;

constexpr auto ExecuteSubA = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) {
    return ExecuteMathOperandR<RegisterA>(addressBus, cpuRegisters, UtilityLib::SubWithBorrow<std::byte>, ApplySubtractionFlags);
};
constexpr auto ExecuteSubB = ExecuteSubR<RegisterB>;
constexpr auto ExecuteSubC = ExecuteSubR<RegisterC>;
constexpr auto ExecuteSubD = ExecuteSubR<RegisterD>;
constexpr auto ExecuteSubE = ExecuteSubR<RegisterE>;
constexpr auto ExecuteSubH = ExecuteSubR<RegisterH>;
constexpr auto ExecuteSubL = ExecuteSubR<RegisterL>;

} // namespace EmulatorLib
