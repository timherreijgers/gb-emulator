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
#include "utilitylib/bit_mask.h"

namespace EmulatorLib
{

template <ReturnsRegister8Bit TargetRegister>
constexpr auto ExecuteAdcR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto originalAccumulator = cpuRegisters.accumulator.value;
    const auto originalTarget = targetRegister.value;
    const auto carry = (cpuRegisters.flags.value & CpuFlags::Carry.AsByte()) > 0x00_b ? 1 : 0;
    const auto sum = static_cast<int>(originalAccumulator) + static_cast<int>(originalTarget) + carry;
    cpuRegisters.accumulator = static_cast<std::byte>(sum & 0xFF);

    cpuRegisters.flags.value = 0x00_b;
    cpuRegisters.flags.value |= cpuRegisters.accumulator == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b;
    cpuRegisters.flags.value |= ((static_cast<int>(originalAccumulator) & 0x0F) + (static_cast<int>(originalTarget) & 0x0F) + carry > 0x0F) ? CpuFlags::HalfCarry.AsByte() : 0x00_b;
    cpuRegisters.flags.value |= sum > 0xFF ? CpuFlags::Carry.AsByte() : 0x00_b;

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
