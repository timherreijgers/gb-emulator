/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/register_concepts.h"
#include "instruction_handlers/register_io_helpers.h"

namespace EmulatorLib
{

template <ReturnsRegister16Bit RegisterDestinationAddress, ReturnsRegister8Bit ReadLocation, RegisterModifier Modifier = RegisterNoModifier>
constexpr auto ExecuteLoadIndirectFrom = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler {
    const auto address = RegisterDestinationAddress{}(cpuRegisters).value;
    Modifier{}(RegisterDestinationAddress{}(cpuRegisters));
    addressBus.WriteToAddress(address, ReadLocation{}(cpuRegisters).value);
    co_yield std::monostate{};

    // We need this co_return here to make sure the execution of the opcode takes 2 M-cycles. We cannot read the next instruction at the same
    // time we write something on the data bus.
    co_return;
};

constexpr auto ExecuteLoadIndirectBCFromA = ExecuteLoadIndirectFrom<RegisterBC, RegisterA>;
constexpr auto ExecuteLoadIndirectDEFromA = ExecuteLoadIndirectFrom<RegisterDE, RegisterA>;
constexpr auto ExecuteLoadIndirectHLFromA = ExecuteLoadIndirectFrom<RegisterHL, RegisterA>;
constexpr auto ExecuteLoadIndirectHLIncrementFromA = ExecuteLoadIndirectFrom<RegisterHL, RegisterA, Register16BitIncrement>;
constexpr auto ExecuteLoadIndirectHLDecrementFromA = ExecuteLoadIndirectFrom<RegisterHL, RegisterA, Register16BitDecrement>;


} // namespace EmulatorLib