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

template <ReturnsRegister8Bit WriteLocation, ReturnsRegister8Bit SourceLocation>
constexpr auto ExecuteLoadRegReg = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    WriteLocation{}(cpuRegisters) = SourceLocation{}(cpuRegisters).value;
    co_return;
};

constexpr auto ExecuteLoad_A_A = ExecuteLoadRegReg<RegisterA, RegisterA>;
constexpr auto ExecuteLoad_A_B = ExecuteLoadRegReg<RegisterA, RegisterB>;
constexpr auto ExecuteLoad_A_C = ExecuteLoadRegReg<RegisterA, RegisterC>;
constexpr auto ExecuteLoad_A_D = ExecuteLoadRegReg<RegisterA, RegisterD>;
constexpr auto ExecuteLoad_A_E = ExecuteLoadRegReg<RegisterA, RegisterE>;
constexpr auto ExecuteLoad_A_H = ExecuteLoadRegReg<RegisterA, RegisterH>;
constexpr auto ExecuteLoad_A_L = ExecuteLoadRegReg<RegisterA, RegisterL>;

constexpr auto ExecuteLoad_B_A = ExecuteLoadRegReg<RegisterB, RegisterA>;
constexpr auto ExecuteLoad_B_B = ExecuteLoadRegReg<RegisterB, RegisterB>;
constexpr auto ExecuteLoad_B_C = ExecuteLoadRegReg<RegisterB, RegisterC>;
constexpr auto ExecuteLoad_B_D = ExecuteLoadRegReg<RegisterB, RegisterD>;
constexpr auto ExecuteLoad_B_E = ExecuteLoadRegReg<RegisterB, RegisterE>;
constexpr auto ExecuteLoad_B_H = ExecuteLoadRegReg<RegisterB, RegisterH>;
constexpr auto ExecuteLoad_B_L = ExecuteLoadRegReg<RegisterB, RegisterL>;

constexpr auto ExecuteLoad_C_A = ExecuteLoadRegReg<RegisterC, RegisterA>;
constexpr auto ExecuteLoad_C_B = ExecuteLoadRegReg<RegisterC, RegisterB>;
constexpr auto ExecuteLoad_C_C = ExecuteLoadRegReg<RegisterC, RegisterC>;
constexpr auto ExecuteLoad_C_D = ExecuteLoadRegReg<RegisterC, RegisterD>;
constexpr auto ExecuteLoad_C_E = ExecuteLoadRegReg<RegisterC, RegisterE>;
constexpr auto ExecuteLoad_C_H = ExecuteLoadRegReg<RegisterC, RegisterH>;
constexpr auto ExecuteLoad_C_L = ExecuteLoadRegReg<RegisterC, RegisterL>;

constexpr auto ExecuteLoad_D_A = ExecuteLoadRegReg<RegisterD, RegisterA>;
constexpr auto ExecuteLoad_D_B = ExecuteLoadRegReg<RegisterD, RegisterB>;
constexpr auto ExecuteLoad_D_C = ExecuteLoadRegReg<RegisterD, RegisterC>;
constexpr auto ExecuteLoad_D_D = ExecuteLoadRegReg<RegisterD, RegisterD>;
constexpr auto ExecuteLoad_D_E = ExecuteLoadRegReg<RegisterD, RegisterE>;
constexpr auto ExecuteLoad_D_H = ExecuteLoadRegReg<RegisterD, RegisterH>;
constexpr auto ExecuteLoad_D_L = ExecuteLoadRegReg<RegisterD, RegisterL>;

constexpr auto ExecuteLoad_E_A = ExecuteLoadRegReg<RegisterE, RegisterA>;
constexpr auto ExecuteLoad_E_B = ExecuteLoadRegReg<RegisterE, RegisterB>;
constexpr auto ExecuteLoad_E_C = ExecuteLoadRegReg<RegisterE, RegisterC>;
constexpr auto ExecuteLoad_E_D = ExecuteLoadRegReg<RegisterE, RegisterD>;
constexpr auto ExecuteLoad_E_E = ExecuteLoadRegReg<RegisterE, RegisterE>;
constexpr auto ExecuteLoad_E_H = ExecuteLoadRegReg<RegisterE, RegisterH>;
constexpr auto ExecuteLoad_E_L = ExecuteLoadRegReg<RegisterE, RegisterL>;

constexpr auto ExecuteLoad_H_A = ExecuteLoadRegReg<RegisterH, RegisterA>;
constexpr auto ExecuteLoad_H_B = ExecuteLoadRegReg<RegisterH, RegisterB>;
constexpr auto ExecuteLoad_H_C = ExecuteLoadRegReg<RegisterH, RegisterC>;
constexpr auto ExecuteLoad_H_D = ExecuteLoadRegReg<RegisterH, RegisterD>;
constexpr auto ExecuteLoad_H_E = ExecuteLoadRegReg<RegisterH, RegisterE>;
constexpr auto ExecuteLoad_H_H = ExecuteLoadRegReg<RegisterH, RegisterH>;
constexpr auto ExecuteLoad_H_L = ExecuteLoadRegReg<RegisterH, RegisterL>;

constexpr auto ExecuteLoad_L_A = ExecuteLoadRegReg<RegisterL, RegisterA>;
constexpr auto ExecuteLoad_L_B = ExecuteLoadRegReg<RegisterL, RegisterB>;
constexpr auto ExecuteLoad_L_C = ExecuteLoadRegReg<RegisterL, RegisterC>;
constexpr auto ExecuteLoad_L_D = ExecuteLoadRegReg<RegisterL, RegisterD>;
constexpr auto ExecuteLoad_L_E = ExecuteLoadRegReg<RegisterL, RegisterE>;
constexpr auto ExecuteLoad_L_H = ExecuteLoadRegReg<RegisterL, RegisterH>;
constexpr auto ExecuteLoad_L_L = ExecuteLoadRegReg<RegisterL, RegisterL>;

} // namespace EmulatorLib