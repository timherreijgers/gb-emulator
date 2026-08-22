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

template <ReturnsRegister16Bit WriteLocation>
constexpr auto ExecutePopRr = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    WriteLocation{}(cpuRegisters) = cpuRegisters.wzRegister.value;
    co_return;
};

constexpr auto ExecutePopBC = ExecutePopRr<RegisterBC>;
constexpr auto ExecutePopDE = ExecutePopRr<RegisterDE>;
constexpr auto ExecutePopHL = ExecutePopRr<RegisterHL>;
constexpr auto ExecutePopAF = ExecutePopRr<RegisterAF>;

} // namespace EmulatorLib