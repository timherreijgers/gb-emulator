/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/instruction_handler.h"
#include "load_a_from_indirect_address.h"
#include "load_a_from_indirect_c.h"
#include "load_indirect_address_from_a.h"
#include "load_indirect_c_from_a.h"
#include "load_r_r.h"

#include "instruction_handlers/load_a16_sp.h"
#include "instruction_handlers/load_indirect_n8.h"
#include "instruction_handlers/load_indirect_r.h"
#include "instruction_handlers/load_r_indirect_rr.h"
#include "instruction_handlers/load_r_n8.h"
#include "instruction_handlers/load_rr_n16.h"

#include <array>
#include <stdexcept>

namespace EmulatorLib
{

InstructionHandler UnimplementedOpcode(AddressBus& /*addressBus*/, CpuRegisters& /*cpuRegisters*/)
{
    throw std::runtime_error("Unimplemented opcode");
}

InstructionHandler ExecuteNoop(AddressBus& /*addressBus*/, CpuRegisters& /*cpuRegisters*/)
{
    co_return;
}

[[nodiscard]] constexpr auto CreateOpcodeHandlers() noexcept -> std::array<std::function<InstructionHandler(AddressBus&, CpuRegisters&)>, 256>
{
    std::array<std::function<InstructionHandler(AddressBus&, CpuRegisters&)>, 256> handlers{};
    handlers.fill(UnimplementedOpcode);

    handlers[0x00] = ExecuteNoop;
    handlers[0x01] = ExecuteLoadBCn16;
    handlers[0x02] = ExecuteLoadIndirectBCFromA;
    handlers[0x06] = ExecuteLoadBn8;
    handlers[0x08] = ExecuteLoadA16SP;
    handlers[0x0A] = ExecuteLoadIndirectAFromBC;
    handlers[0x0E] = ExecuteLoadCn8;

    handlers[0x11] = ExecuteLoadDEn16;
    handlers[0x12] = ExecuteLoadIndirectDEFromA;
    handlers[0x16] = ExecuteLoadDn8;
    handlers[0x1A] = ExecuteLoadIndirectAFromDE;
    handlers[0x1E] = ExecuteLoadEn8;

    handlers[0x21] = ExecuteLoadHLn16;
    handlers[0x22] = ExecuteLoadIndirectHLIncrementFromA;
    handlers[0x26] = ExecuteLoadHn8;
    handlers[0x2A] = ExecuteLoadIndirectAFromHLIncrement;
    handlers[0x2E] = ExecuteLoadLn8;

    handlers[0x31] = ExecuteLoadSPn16;
    handlers[0x32] = ExecuteLoadIndirectHLDecrementFromA;
    handlers[0x36] = ExecuteLoadIndirectHlN8;
    handlers[0x3A] = ExecuteLoadIndirectAFromHLDecrement;
    handlers[0x3E] = ExecuteLoadAn8;

    handlers[0x40] = ExecuteLoad_B_B;
    handlers[0x41] = ExecuteLoad_B_C;
    handlers[0x42] = ExecuteLoad_B_D;
    handlers[0x43] = ExecuteLoad_B_E;
    handlers[0x44] = ExecuteLoad_B_H;
    handlers[0x45] = ExecuteLoad_B_L;
    handlers[0x46] = ExecuteLoadIndirectBFromHL;
    handlers[0x47] = ExecuteLoad_B_A;
    handlers[0x48] = ExecuteLoad_C_B;
    handlers[0x49] = ExecuteLoad_C_C;
    handlers[0x4A] = ExecuteLoad_C_D;
    handlers[0x4B] = ExecuteLoad_C_E;
    handlers[0x4C] = ExecuteLoad_C_H;
    handlers[0x4D] = ExecuteLoad_C_L;
    handlers[0x4E] = ExecuteLoadIndirectCFromHL;
    handlers[0x4F] = ExecuteLoad_C_A;


    handlers[0x50] = ExecuteLoad_D_B;
    handlers[0x51] = ExecuteLoad_D_C;
    handlers[0x52] = ExecuteLoad_D_D;
    handlers[0x53] = ExecuteLoad_D_E;
    handlers[0x54] = ExecuteLoad_D_H;
    handlers[0x55] = ExecuteLoad_D_L;
    handlers[0x56] = ExecuteLoadIndirectDFromHL;
    handlers[0x57] = ExecuteLoad_D_A;
    handlers[0x58] = ExecuteLoad_E_B;
    handlers[0x59] = ExecuteLoad_E_C;
    handlers[0x5A] = ExecuteLoad_E_D;
    handlers[0x5B] = ExecuteLoad_E_E;
    handlers[0x5C] = ExecuteLoad_E_H;
    handlers[0x5D] = ExecuteLoad_E_L;
    handlers[0x5E] = ExecuteLoadIndirectEFromHL;
    handlers[0x5F] = ExecuteLoad_E_A;

    handlers[0x60] = ExecuteLoad_H_B;
    handlers[0x61] = ExecuteLoad_H_C;
    handlers[0x62] = ExecuteLoad_H_D;
    handlers[0x63] = ExecuteLoad_H_E;
    handlers[0x64] = ExecuteLoad_H_H;
    handlers[0x65] = ExecuteLoad_H_L;
    handlers[0x66] = ExecuteLoadIndirectHFromHL;
    handlers[0x67] = ExecuteLoad_H_A;
    handlers[0x68] = ExecuteLoad_L_B;
    handlers[0x69] = ExecuteLoad_L_C;
    handlers[0x6A] = ExecuteLoad_L_D;
    handlers[0x6B] = ExecuteLoad_L_E;
    handlers[0x6C] = ExecuteLoad_L_H;
    handlers[0x6D] = ExecuteLoad_L_L;
    handlers[0x6E] = ExecuteLoadIndirectLFromHL;
    handlers[0x6F] = ExecuteLoad_L_A;

    handlers[0x70] = ExecuteLoadIndirectHLFromB;
    handlers[0x71] = ExecuteLoadIndirectHLFromC;
    handlers[0x72] = ExecuteLoadIndirectHLFromD;
    handlers[0x73] = ExecuteLoadIndirectHLFromE;
    handlers[0x74] = ExecuteLoadIndirectHLFromH;
    handlers[0x75] = ExecuteLoadIndirectHLFromL;
    handlers[0x77] = ExecuteLoadIndirectHLFromA;
    handlers[0x78] = ExecuteLoad_A_B;
    handlers[0x79] = ExecuteLoad_A_C;
    handlers[0x7A] = ExecuteLoad_A_D;
    handlers[0x7B] = ExecuteLoad_A_E;
    handlers[0x7C] = ExecuteLoad_A_H;
    handlers[0x7D] = ExecuteLoad_A_L;
    handlers[0x7E] = ExecuteLoadIndirectAFromHL;
    handlers[0x7F] = ExecuteLoad_A_A;

    handlers[0xE0] = ExecuteLoadIndirectA8FromA;
    handlers[0xE2] = ExecuteLoadIndirectCFromA;
    handlers[0xEA] = ExecuteLoadIndirectA16FromA;

    handlers[0xF0] = ExecuteLoadAFromIndirectA8;
    handlers[0xF2] = ExecuteLoadAFromIndirectC;
    handlers[0xFA] = ExecuteLoadAFromIndirectA16;

    return handlers;
}

} // namespace EmulatorLib