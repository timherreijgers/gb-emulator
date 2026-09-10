/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/flag_helpers.h"
#include "utilitylib/add_with_carry.h"

namespace EmulatorLib
{

constexpr auto ExecuteAdcAFromIndirectHL = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto carryIn = CarryIn(cpuRegisters);
    const auto [result, carry] = UtilityLib::AddWithCarryIn(cpuRegisters.accumulator.value, cpuRegisters.zRegister.value, carryIn);
    cpuRegisters.accumulator = result;

    ApplyAdditionFlags(cpuRegisters, result, carry);
    co_return;
};

} // namespace EmulatorLib
