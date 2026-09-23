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
concept MathOperandWithoutRegisters =
    std::invocable<T, std::byte, std::byte> &&
    std::same_as<
        std::invoke_result_t<T, std::byte, std::byte>,
        UtilityLib::MathematicalResult<std::byte>>;

template <typename T>
concept MathOperandWithRegisters =
    std::invocable<T, const CpuRegisters&, std::byte, std::byte> &&
    std::same_as<
        std::invoke_result_t<T, const CpuRegisters&, std::byte, std::byte>,
        UtilityLib::MathematicalResult<std::byte>>;

template <typename T>
concept MathOperand = MathOperandWithoutRegisters<T> || MathOperandWithRegisters<T>;

template <typename T>
concept SetFlagFunction =
    std::invocable<T, CpuRegisters&, std::byte, std::byte> && std::same_as<
                                                                  std::invoke_result_t<T, CpuRegisters&, std::byte, std::byte>,
                                                                  void>;

constexpr auto AdcWithCarryWrapper = [](const CpuRegisters& cpuRegisters, UtilityLib::IntegralOrByte auto left, UtilityLib::IntegralOrByte auto right) {
    const auto carryIn = CarryIn(cpuRegisters);
    return UtilityLib::AddWithCarryIn(left, right, carryIn);
};

constexpr auto SbcWithBorrowWrapper = [](const CpuRegisters& cpuRegisters, UtilityLib::IntegralOrByte auto left, UtilityLib::IntegralOrByte auto right) {
    const auto borrowIn = CarryIn(cpuRegisters);
    return UtilityLib::SubWithBorrowIn(left, right, borrowIn);
};

constexpr auto AndOperand = [](std::byte left, std::byte right) noexcept -> UtilityLib::MathematicalResult<std::byte> {
    const auto result = static_cast<std::byte>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
    return {result, 0x00_b};
};

constexpr auto OrOperand = [](std::byte left, std::byte right) noexcept -> UtilityLib::MathematicalResult<std::byte> {
    const auto result = static_cast<std::byte>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
    return {result, 0x00_b};
};

constexpr auto XorOperand = [](std::byte left, std::byte right) noexcept -> UtilityLib::MathematicalResult<std::byte> {
    const auto result = static_cast<std::byte>(static_cast<uint8_t>(left) ^ static_cast<uint8_t>(right));
    return {result, 0x00_b};
};

template <ReturnsRegister8Bit TargetRegister, MathOperand Operand, SetFlagFunction FlagFunction>
constexpr auto ExecuteMathOperandR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);

    const auto [result, carry] = [&] {
        if constexpr (MathOperandWithoutRegisters<Operand>)
        {
            return Operand{}(cpuRegisters.accumulator.value, targetRegister.value);
        }

        if constexpr (MathOperandWithRegisters<Operand>)
        {
            return Operand{}(cpuRegisters, cpuRegisters.accumulator.value, targetRegister.value);
        }
    }();

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

constexpr auto ExecuteAdcA = ExecuteMathOperandR<RegisterA, decltype(AdcWithCarryWrapper), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAdcB = ExecuteMathOperandR<RegisterB, decltype(AdcWithCarryWrapper), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAdcC = ExecuteMathOperandR<RegisterC, decltype(AdcWithCarryWrapper), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAdcD = ExecuteMathOperandR<RegisterD, decltype(AdcWithCarryWrapper), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAdcE = ExecuteMathOperandR<RegisterE, decltype(AdcWithCarryWrapper), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAdcH = ExecuteMathOperandR<RegisterH, decltype(AdcWithCarryWrapper), decltype(ApplyAdditionFlags)>;
constexpr auto ExecuteAdcL = ExecuteMathOperandR<RegisterL, decltype(AdcWithCarryWrapper), decltype(ApplyAdditionFlags)>;

constexpr auto ExecuteSubA = ExecuteMathOperandR<RegisterA, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubB = ExecuteMathOperandR<RegisterB, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubC = ExecuteMathOperandR<RegisterC, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubD = ExecuteMathOperandR<RegisterD, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubE = ExecuteMathOperandR<RegisterE, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubH = ExecuteMathOperandR<RegisterH, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSubL = ExecuteMathOperandR<RegisterL, decltype(UtilityLib::SubWithBorrow), decltype(ApplySubtractionFlags)>;

constexpr auto ExecuteSbcA = ExecuteMathOperandR<RegisterA, decltype(SbcWithBorrowWrapper), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSbcB = ExecuteMathOperandR<RegisterB, decltype(SbcWithBorrowWrapper), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSbcC = ExecuteMathOperandR<RegisterC, decltype(SbcWithBorrowWrapper), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSbcD = ExecuteMathOperandR<RegisterD, decltype(SbcWithBorrowWrapper), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSbcE = ExecuteMathOperandR<RegisterE, decltype(SbcWithBorrowWrapper), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSbcH = ExecuteMathOperandR<RegisterH, decltype(SbcWithBorrowWrapper), decltype(ApplySubtractionFlags)>;
constexpr auto ExecuteSbcL = ExecuteMathOperandR<RegisterL, decltype(SbcWithBorrowWrapper), decltype(ApplySubtractionFlags)>;

constexpr auto ExecuteAndA = ExecuteMathOperandR<RegisterA, decltype(AndOperand), decltype(ApplyAndFlags)>;
constexpr auto ExecuteAndB = ExecuteMathOperandR<RegisterB, decltype(AndOperand), decltype(ApplyAndFlags)>;
constexpr auto ExecuteAndC = ExecuteMathOperandR<RegisterC, decltype(AndOperand), decltype(ApplyAndFlags)>;
constexpr auto ExecuteAndD = ExecuteMathOperandR<RegisterD, decltype(AndOperand), decltype(ApplyAndFlags)>;
constexpr auto ExecuteAndE = ExecuteMathOperandR<RegisterE, decltype(AndOperand), decltype(ApplyAndFlags)>;
constexpr auto ExecuteAndH = ExecuteMathOperandR<RegisterH, decltype(AndOperand), decltype(ApplyAndFlags)>;
constexpr auto ExecuteAndL = ExecuteMathOperandR<RegisterL, decltype(AndOperand), decltype(ApplyAndFlags)>;

constexpr auto ExecuteOrA = ExecuteMathOperandR<RegisterA, decltype(OrOperand), decltype(ApplyOrFlags)>;
constexpr auto ExecuteOrB = ExecuteMathOperandR<RegisterB, decltype(OrOperand), decltype(ApplyOrFlags)>;
constexpr auto ExecuteOrC = ExecuteMathOperandR<RegisterC, decltype(OrOperand), decltype(ApplyOrFlags)>;
constexpr auto ExecuteOrD = ExecuteMathOperandR<RegisterD, decltype(OrOperand), decltype(ApplyOrFlags)>;
constexpr auto ExecuteOrE = ExecuteMathOperandR<RegisterE, decltype(OrOperand), decltype(ApplyOrFlags)>;
constexpr auto ExecuteOrH = ExecuteMathOperandR<RegisterH, decltype(OrOperand), decltype(ApplyOrFlags)>;
constexpr auto ExecuteOrL = ExecuteMathOperandR<RegisterL, decltype(OrOperand), decltype(ApplyOrFlags)>;

constexpr auto ExecuteXorA = ExecuteMathOperandR<RegisterA, decltype(XorOperand), decltype(ApplyXorFlags)>;
constexpr auto ExecuteXorB = ExecuteMathOperandR<RegisterB, decltype(XorOperand), decltype(ApplyXorFlags)>;
constexpr auto ExecuteXorC = ExecuteMathOperandR<RegisterC, decltype(XorOperand), decltype(ApplyXorFlags)>;
constexpr auto ExecuteXorD = ExecuteMathOperandR<RegisterD, decltype(XorOperand), decltype(ApplyXorFlags)>;
constexpr auto ExecuteXorE = ExecuteMathOperandR<RegisterE, decltype(XorOperand), decltype(ApplyXorFlags)>;
constexpr auto ExecuteXorH = ExecuteMathOperandR<RegisterH, decltype(XorOperand), decltype(ApplyXorFlags)>;
constexpr auto ExecuteXorL = ExecuteMathOperandR<RegisterL, decltype(XorOperand), decltype(ApplyXorFlags)>;

} // namespace EmulatorLib
