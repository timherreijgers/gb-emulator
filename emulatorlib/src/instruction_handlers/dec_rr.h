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
constexpr auto ExecuteDecRR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    TargetRegister{}(cpuRegisters) -= 1;
    co_yield std::monostate{};

    co_return;
};

constexpr auto ExecuteDecBC = ExecuteDecRR<RegisterBC>;
constexpr auto ExecuteDecDE = ExecuteDecRR<RegisterDE>;
constexpr auto ExecuteDecHL = ExecuteDecRR<RegisterHL>;
constexpr auto ExecuteDecSP = ExecuteDecRR<RegisterSP>;

} // namespace EmulatorLib