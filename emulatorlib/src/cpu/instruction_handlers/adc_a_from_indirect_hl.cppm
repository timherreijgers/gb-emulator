/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:adc_a_from_indirect_hl;

import emulatorlib.cpu;
import emulatorlib.address_bus;
import utilitylib;

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
