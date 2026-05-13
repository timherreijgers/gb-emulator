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
constexpr auto ExecuteLoadRegN16 = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler {
    // Load byte from memory into register Z
    const auto val = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    cpuRegisters.zRegister = val;
    co_yield std::monostate{};

    // Load byte from memory into register W
    const auto val2 = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    cpuRegisters.wRegister = val2;
    co_yield std::monostate{};

    // Load byte from memory into register WZ into 16 bit register
    WriteLocation{}(cpuRegisters) = cpuRegisters.wzRegister.value;
    co_return;
};

constexpr auto ExecuteLoadBCn16 = ExecuteLoadRegN16<RegisterBC>;
constexpr auto ExecuteLoadDEn16 = ExecuteLoadRegN16<RegisterDE>;
constexpr auto ExecuteLoadHLn16 = ExecuteLoadRegN16<RegisterHL>;
constexpr auto ExecuteLoadSPn16 = ExecuteLoadRegN16<RegisterSP>;

} // namespace EmulatorLib