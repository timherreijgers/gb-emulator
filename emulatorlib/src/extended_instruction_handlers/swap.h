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
constexpr auto ExecuteSwap = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto result = (targetRegister << 4) | (targetRegister >> 4);
    targetRegister = result;
    cpuRegisters.flags = static_cast<bool>(result) ? 0x00_b : CpuFlags::Zero.AsByte();

    co_return;
};

constexpr auto ExecuteSwapA = ExecuteSwap<RegisterA>;
constexpr auto ExecuteSwapB = ExecuteSwap<RegisterB>;
constexpr auto ExecuteSwapC = ExecuteSwap<RegisterC>;
constexpr auto ExecuteSwapD = ExecuteSwap<RegisterD>;
constexpr auto ExecuteSwapE = ExecuteSwap<RegisterE>;
constexpr auto ExecuteSwapH = ExecuteSwap<RegisterH>;
constexpr auto ExecuteSwapL = ExecuteSwap<RegisterL>;

} // namespace EmulatorLib
