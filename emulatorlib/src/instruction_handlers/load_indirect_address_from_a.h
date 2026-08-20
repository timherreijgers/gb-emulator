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

[[nodiscard]] inline auto ExecuteLoadIndirectA8FromA(AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    addressBus.WriteToAddress(0xFF00 | static_cast<uint8_t>(cpuRegisters.zRegister.value), cpuRegisters.accumulator.value);
    co_yield std::monostate{};

    // We need this co_return here to make sure the execution of the opcode takes 3 M-cycles. We cannot read the next instruction at the same
    // time we write something on the data bus.
    co_return;
}

[[nodiscard]] inline auto ExecuteLoadIndirectA16FromA(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.wzRegister.value, cpuRegisters.accumulator.value);
    co_yield std::monostate{};

    // We need this co_return here to make sure the execution of the opcode takes 3 M-cycles. We cannot read the next instruction at the same
    // time we write something on the data bus.
    co_return;
}

} // namespace EmulatorLib