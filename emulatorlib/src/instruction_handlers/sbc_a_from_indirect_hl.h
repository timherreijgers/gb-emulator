/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/flag_helpers.h"
#include "utilitylib/sub_with_borrow.h"

namespace EmulatorLib
{

constexpr auto ExecuteSbcAFromIndirectHL = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto borrowIn = CarryIn(cpuRegisters);
    const auto [result, carry] = UtilityLib::SubWithBorrowIn(cpuRegisters.accumulator.value, cpuRegisters.zRegister.value, borrowIn);
    cpuRegisters.accumulator = result;

    ApplySubtractionFlags(cpuRegisters, result, carry);
    co_return;
};

} // namespace EmulatorLib
