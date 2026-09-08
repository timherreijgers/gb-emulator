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

constexpr auto ExecuteAddAn8 = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    const auto [result, carry] = UtilityLib::AddWithCarry(cpuRegisters.accumulator.value, cpuRegisters.zRegister.value);
    cpuRegisters.accumulator = result;

    ApplyAdditionFlags(cpuRegisters, result, carry);

    co_return;
};

} // namespace EmulatorLib
