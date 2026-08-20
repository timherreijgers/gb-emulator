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

template <ReturnsRegister16Bit WriteLocation, ReturnsRegister16Bit SourceLocation>
constexpr auto ExecuteLoadReg16Reg16 = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    WriteLocation{}(cpuRegisters) = SourceLocation{}(cpuRegisters).value;
    co_yield std::monostate{};

    // We need this co_return here to make sure the execution of the opcode takes 2 M-cycles. We cannot read the next instruction at the same
    // time we write something on the data bus.
    co_return;
};

constexpr auto ExecuteLoad_SP_HL = ExecuteLoadReg16Reg16<RegisterSP, RegisterHL>;

} // namespace EmulatorLib