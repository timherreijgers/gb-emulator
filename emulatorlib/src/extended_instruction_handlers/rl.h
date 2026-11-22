/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/register_concepts.h"
#include "instruction_handlers/register_io_helpers.h"

namespace EmulatorLib
{

template <ReturnsRegister8Bit TargetRegister>
constexpr auto ExecuteRl = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto carryIn = static_cast<bool>(cpuRegisters.flags & CpuFlags::Carry.AsByte()) ? 0x01_b : 0x00_b;
    const auto newCarry = static_cast<bool>(targetRegister & 0x80_b);
    targetRegister = (targetRegister << 1) | carryIn;
    cpuRegisters.flags = (newCarry ? CpuFlags::Carry.AsByte() : 0x00_b) | (targetRegister == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b);

    co_return;
};

constexpr auto ExecuteRlA = ExecuteRl<RegisterA>;
constexpr auto ExecuteRlB = ExecuteRl<RegisterB>;
constexpr auto ExecuteRlC = ExecuteRl<RegisterC>;
constexpr auto ExecuteRlD = ExecuteRl<RegisterD>;
constexpr auto ExecuteRlE = ExecuteRl<RegisterE>;
constexpr auto ExecuteRlH = ExecuteRl<RegisterH>;
constexpr auto ExecuteRlL = ExecuteRl<RegisterL>;

} // namespace EmulatorLib
