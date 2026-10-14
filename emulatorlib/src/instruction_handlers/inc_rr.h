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

template <ReturnsRegister16Bit TargetRegister>
constexpr auto ExecuteIncRR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    TargetRegister{}(cpuRegisters) += 1;
    co_return;
};

constexpr auto ExecuteIncBC = ExecuteIncRR<RegisterBC>;
constexpr auto ExecuteIncDE = ExecuteIncRR<RegisterDE>;
constexpr auto ExecuteIncHL = ExecuteIncRR<RegisterHL>;
constexpr auto ExecuteIncSP = ExecuteIncRR<RegisterSP>;

} // namespace EmulatorLib