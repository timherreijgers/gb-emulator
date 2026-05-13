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

template <ReturnsRegister16Bit ReadLocation>
constexpr auto ExecuteLoadIndirectFromA = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler {
    const auto address = ReadLocation{}(cpuRegisters).value;
    addressBus.WriteToAddress(address, cpuRegisters.accumulator);
    co_yield std::monostate{};

    // We need this co_return here to make sure the execution of the opcode takes 2 M-cycles. We cannot read the next instruction at the same
    // time we write something on the data bus.
    co_return;
};

constexpr auto ExecuteLoadIndirectBCFromA = ExecuteLoadIndirectFromA<RegisterBC>;
constexpr auto ExecuteLoadIndirectDEFromA = ExecuteLoadIndirectFromA<RegisterDE>;


} // namespace EmulatorLib