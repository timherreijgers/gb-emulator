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
constexpr auto ExecuteRlc = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto newBit0 = static_cast<bool>(targetRegister & 0x80_b) ? 0x01_b : 0x00_b;
    targetRegister = (targetRegister << 1) | newBit0;
    cpuRegisters.flags = (targetRegister == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b) | (static_cast<bool>(newBit0) ? CpuFlags::Carry.AsByte() : 0x00_b);

    co_return;
};

constexpr auto ExecuteRlcA = ExecuteRlc<RegisterA>;
constexpr auto ExecuteRlcB = ExecuteRlc<RegisterB>;
constexpr auto ExecuteRlcC = ExecuteRlc<RegisterC>;
constexpr auto ExecuteRlcD = ExecuteRlc<RegisterD>;
constexpr auto ExecuteRlcE = ExecuteRlc<RegisterE>;
constexpr auto ExecuteRlcH = ExecuteRlc<RegisterH>;
constexpr auto ExecuteRlcL = ExecuteRlc<RegisterL>;

} // namespace EmulatorLib
