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

[[nodiscard]] inline auto ExecuteLoadAFromIndirectN8(AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    cpuRegisters.zRegister = addressBus.ReadFromAddress(0xFF00 | static_cast<uint8_t>(cpuRegisters.zRegister.value));
    co_yield std::monostate{};

    cpuRegisters.accumulator = cpuRegisters.zRegister.value;
    co_return;
};


} // namespace EmulatorLib