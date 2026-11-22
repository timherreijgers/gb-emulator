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
constexpr auto ExecuteSra = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto newBit7 = static_cast<bool>(targetRegister & 0x80_b) ? 0x80_b : 0x00_b;
    const auto newCarry = static_cast<bool>(targetRegister & 0x01_b);
    const auto result = (targetRegister >> 1) | newBit7;
    targetRegister = result;
    cpuRegisters.flags = (static_cast<bool>(result) ? 0x00_b : CpuFlags::Zero.AsByte()) | (newCarry ? CpuFlags::Carry.AsByte() : 0x00_b);

    co_return;
};

constexpr auto ExecuteSraA = ExecuteSra<RegisterA>;
constexpr auto ExecuteSraB = ExecuteSra<RegisterB>;
constexpr auto ExecuteSraC = ExecuteSra<RegisterC>;
constexpr auto ExecuteSraD = ExecuteSra<RegisterD>;
constexpr auto ExecuteSraE = ExecuteSra<RegisterE>;
constexpr auto ExecuteSraH = ExecuteSra<RegisterH>;
constexpr auto ExecuteSraL = ExecuteSra<RegisterL>;

} // namespace EmulatorLib
