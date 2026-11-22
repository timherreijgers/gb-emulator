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
constexpr auto ExecuteSrl = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto newCarry = static_cast<bool>(targetRegister & 0x01_b);
    const auto result = targetRegister >> 1;
    targetRegister = result;
    cpuRegisters.flags = (static_cast<bool>(result) ? 0x00_b : CpuFlags::Zero.AsByte()) | (newCarry ? CpuFlags::Carry.AsByte() : 0x00_b);

    co_return;
};

constexpr auto ExecuteSrlA = ExecuteSrl<RegisterA>;
constexpr auto ExecuteSrlB = ExecuteSrl<RegisterB>;
constexpr auto ExecuteSrlC = ExecuteSrl<RegisterC>;
constexpr auto ExecuteSrlD = ExecuteSrl<RegisterD>;
constexpr auto ExecuteSrlE = ExecuteSrl<RegisterE>;
constexpr auto ExecuteSrlH = ExecuteSrl<RegisterH>;
constexpr auto ExecuteSrlL = ExecuteSrl<RegisterL>;

} // namespace EmulatorLib
