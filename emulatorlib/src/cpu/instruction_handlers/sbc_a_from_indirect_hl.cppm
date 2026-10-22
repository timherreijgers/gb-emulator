/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module emulatorlib.cpu:sbc_a_from_indirect_hl;

import std;

import emulatorlib.cpu;
import emulatorlib.address_bus;
import utilitylib;

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
