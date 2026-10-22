/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module emulatorlib.cpu:cp_a_from_indirect_hl;

import std;

import emulatorlib.cpu;
import emulatorlib.address_bus;
import utilitylib;

namespace EmulatorLib
{

constexpr auto ExecuteCpAFromIndirectHL = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto [result, carry] = UtilityLib::SubWithBorrow(cpuRegisters.accumulator.value, cpuRegisters.zRegister.value);

    ApplySubtractionFlags(cpuRegisters, result, carry);
    co_return;
};

} // namespace EmulatorLib
