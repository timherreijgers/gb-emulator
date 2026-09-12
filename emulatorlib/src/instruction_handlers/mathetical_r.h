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

#include <concepts>
#include <type_traits>

namespace EmulatorLib
{

template <typename T>
concept MathOperand =
    std::invocable<T, std::byte, std::byte> && (std::same_as<
                                                    std::invoke_result_t<T, std::byte, std::byte>,
                                                    UtilityLib::AddWithCarryResult<std::byte>> ||
                                                std::same_as<
                                                    std::invoke_result_t<T, std::byte, std::byte>,
                                                    UtilityLib::SubWithBorrowResult<std::byte>>);

template <typename T>
concept SetFlagFunction =
    std::invocable<T, CpuRegisters&, std::byte, std::byte> && std::same_as<
                                                                  std::invoke_result_t<T, CpuRegisters&, std::byte, std::byte>,
                                                                  void>;

template <ReturnsRegister8Bit TargetRegister, MathOperand Operand, SetFlagFunction FlagFunction>
constexpr auto ExecuteMathOperandR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);

    const auto [result, carry] = Operand{}(cpuRegisters.accumulator.value, targetRegister.value);
    cpuRegisters.accumulator = result;

    FlagFunction{}(cpuRegisters, result, carry);

    co_return;
};

constexpr auto ExecuteAddA = ExecuteMathOperandR<RegisterA, decltype(UtilityLib::AddWithCarry), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAddB = ExecuteMathOperandR<RegisterB, decltype(UtilityLib::AddWithCarry), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAddC = ExecuteMathOperandR<RegisterC, decltype(UtilityLib::AddWithCarry), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAddD = ExecuteMathOperandR<RegisterD, decltype(UtilityLib::AddWithCarry), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAddE = ExecuteMathOperandR<RegisterE, decltype(UtilityLib::AddWithCarry), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAddH = ExecuteMathOperandR<RegisterH, decltype(UtilityLib::AddWithCarry), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAddL = ExecuteMathOperandR<RegisterL, decltype(UtilityLib::AddWithCarry), decltype(ApplyAdditionFlags)>;

constexpr auto ExecuteSubA = ExecuteMathOperandR<RegisterA, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubB = ExecuteMathOperandR<RegisterB, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubC = ExecuteMathOperandR<RegisterC, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubD = ExecuteMathOperandR<RegisterD, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubE = ExecuteMathOperandR<RegisterE, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubH = ExecuteMathOperandR<RegisterH, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubL = ExecuteMathOperandR<RegisterL, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;

} // namespace EmulatorLib
