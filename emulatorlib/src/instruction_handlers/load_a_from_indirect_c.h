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

[[nodiscard]] inline auto ExecuteLoadAFromIndirectC(AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    cpuRegisters.accumulator = addressBus.ReadFromAddress(0xFF00 | static_cast<uint8_t>(cpuRegisters.cRegister.value));
    co_yield std::monostate{};

    // We need this co_return here to make sure the execution of the opcode takes 5 M-cycles. We cannot read the next instruction at the same
    // time we write something on the data bus.
    co_return;
};


} // namespace EmulatorLib