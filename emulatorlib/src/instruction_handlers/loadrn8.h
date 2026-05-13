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

template <ReturnsRegister8Bit WriteLocation>
constexpr auto ExecuteLoadRegN8 = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler {
    // Load byte from memory into register Z
    const auto val = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    cpuRegisters.zRegister = val;
    co_yield std::monostate{};

    // Load byte from memory into register Z into register B
    WriteLocation{}(cpuRegisters) = cpuRegisters.zRegister.value;
    co_return;
};

constexpr auto ExecuteLoadAn8 = ExecuteLoadRegN8<RegisterA>;
constexpr auto ExecuteLoadBn8 = ExecuteLoadRegN8<RegisterB>;
constexpr auto ExecuteLoadCn8 = ExecuteLoadRegN8<RegisterC>;
constexpr auto ExecuteLoadDn8 = ExecuteLoadRegN8<RegisterD>;
constexpr auto ExecuteLoadEn8 = ExecuteLoadRegN8<RegisterE>;
constexpr auto ExecuteLoadHn8 = ExecuteLoadRegN8<RegisterH>;
constexpr auto ExecuteLoadLn8 = ExecuteLoadRegN8<RegisterL>;

} // namespace EmulatorLib