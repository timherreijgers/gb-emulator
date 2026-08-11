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

template <ReturnsRegister8Bit RegisterDestination, ReturnsRegister16Bit RegisterSource, RegisterModifier16Bit Modifier = RegisterNoModifier>
constexpr auto ExecuteLoadRIndirectRr = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler {
    const auto address = RegisterSource{}(cpuRegisters).value;
    Modifier{}(RegisterSource{}(cpuRegisters));
    const auto value = addressBus.ReadFromAddress(address);
    co_yield std::monostate{};

    RegisterDestination{}(cpuRegisters) = value;
    co_return;
};

constexpr auto ExecuteLoadIndirectAFromBC = ExecuteLoadRIndirectRr<RegisterA, RegisterBC>;
constexpr auto ExecuteLoadIndirectAFromDE = ExecuteLoadRIndirectRr<RegisterA, RegisterDE>;
constexpr auto ExecuteLoadIndirectAFromHLIncrement = ExecuteLoadRIndirectRr<RegisterA, RegisterHL, Register16BitIncrement>;
constexpr auto ExecuteLoadIndirectAFromHLDecrement = ExecuteLoadRIndirectRr<RegisterA, RegisterHL, Register16BitDecrement>;
constexpr auto ExecuteLoadIndirectBFromHL = ExecuteLoadRIndirectRr<RegisterB, RegisterHL>;
constexpr auto ExecuteLoadIndirectCFromHL = ExecuteLoadRIndirectRr<RegisterC, RegisterHL>;
constexpr auto ExecuteLoadIndirectDFromHL = ExecuteLoadRIndirectRr<RegisterD, RegisterHL>;
constexpr auto ExecuteLoadIndirectEFromHL = ExecuteLoadRIndirectRr<RegisterE, RegisterHL>;
constexpr auto ExecuteLoadIndirectHFromHL = ExecuteLoadRIndirectRr<RegisterH, RegisterHL>;
constexpr auto ExecuteLoadIndirectLFromHL = ExecuteLoadRIndirectRr<RegisterL, RegisterHL>;
constexpr auto ExecuteLoadIndirectAFromHL = ExecuteLoadRIndirectRr<RegisterA, RegisterHL>;

} // namespace EmulatorLib