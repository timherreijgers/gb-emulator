/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:sub_a_from_indirect_hl;

import emulatorlib.cpu;
import emulatorlib.address_bus;
import utilitylib;

namespace EmulatorLib
{

constexpr auto ExecuteSubAFromIndirectHL = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto [result, carry] = UtilityLib::SubWithBorrow(cpuRegisters.accumulator.value, cpuRegisters.zRegister.value);
    cpuRegisters.accumulator = result;

    ApplySubtractionFlags(cpuRegisters, result, carry);
    co_return;
};

} // namespace EmulatorLib
