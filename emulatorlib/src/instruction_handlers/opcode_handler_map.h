/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/adc_a_from_indirect_hl.h"
#include "instruction_handlers/add_a_from_indirect_hl.h"
#include "instruction_handlers/and_a_from_indirect_hl.h"
#include "instruction_handlers/call_a16.h"
#include "instruction_handlers/cp_a_from_indirect_hl.h"
#include "instruction_handlers/dec_r.h"
#include "instruction_handlers/dec_rr.h"
#include "instruction_handlers/disable_interrupts.h"
#include "instruction_handlers/enable_interrupts.h"
#include "instruction_handlers/inc_r.h"
#include "instruction_handlers/inc_rr.h"
#include "instruction_handlers/jump_a16.h"
#include "instruction_handlers/jump_hl.h"
#include "instruction_handlers/load_a16_sp.h"
#include "instruction_handlers/load_a_from_indirect_address.h"
#include "instruction_handlers/load_a_from_indirect_c.h"
#include "instruction_handlers/load_hl_from_sp_e8.h"
#include "instruction_handlers/load_indirect_address_from_a.h"
#include "instruction_handlers/load_indirect_c_from_a.h"
#include "instruction_handlers/load_indirect_n8.h"
#include "instruction_handlers/load_indirect_r.h"
#include "instruction_handlers/load_r_indirect_rr.h"
#include "instruction_handlers/load_r_n8.h"
#include "instruction_handlers/load_r_r.h"
#include "instruction_handlers/load_rr_n16.h"
#include "instruction_handlers/load_rr_rr.h"
#include "instruction_handlers/mathetical_r.h"
#include "instruction_handlers/or_a_from_indirect_hl.h"
#include "instruction_handlers/pop_rr.h"
#include "instruction_handlers/push_rr.h"
#include "instruction_handlers/ret.h"
#include "instruction_handlers/ret_conditional.h"
#include "instruction_handlers/reti.h"
#include "instruction_handlers/rst.h"
#include "instruction_handlers/sbc_a_from_indirect_hl.h"
#include "instruction_handlers/sub_a_from_indirect_hl.h"
#include "instruction_handlers/xor_a_from_indirect_hl.h"

#include <array>
#include <format>
#include <stdexcept>

namespace EmulatorLib
{

inline InstructionHandler UnimplementedOpcode(AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters)
{
    throw std::runtime_error(std::format("Unimplemented opcode: {}", cpuRegisters.instructionRegister.value));
}

inline InstructionHandler IllegalOpcode(AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters)
{
    throw std::runtime_error(std::format("Illegal opcode: {}", cpuRegisters.instructionRegister.value));
}

inline InstructionHandler ExecuteNoop(AddressBus& /*addressBus*/, CpuRegisters& /*cpuRegisters*/) noexcept
{
    co_return;
}

[[nodiscard]] inline auto CreateOpcodeHandlers() noexcept -> std::array<OpcodeHandler, 256>
{
    std::array<OpcodeHandler, 256> handlers{};
    handlers.fill(OpcodeHandler{.name = "UNIMPLEMENTED", .handler = UnimplementedOpcode});

    handlers[0x00] = {.name = "NOOP", .handler = ExecuteNoop};
    handlers[0x01] = {.name = "LD BC, nn", .handler = ExecuteLoadBCn16};
    handlers[0x02] = {.name = "LD (BC), A", .handler = ExecuteLoadIndirectBCFromA};
    handlers[0x03] = {.name = "INC BC", .handler = ExecuteIncBC};
    handlers[0x04] = {.name = "INC B", .handler = ExecuteIncB};
    handlers[0x05] = {.name = "DEC B", .handler = ExecuteDecB};
    handlers[0x06] = {.name = "LD B, n", .handler = ExecuteLoadBn8};
    handlers[0x07] = {.name = "RLCA", .handler = UnimplementedOpcode};
    handlers[0x08] = {.name = "LD (a16), SP", .handler = ExecuteLoadA16SP};
    handlers[0x09] = {.name = "ADD HL, BC", .handler = UnimplementedOpcode};
    handlers[0x0A] = {.name = "LD A, (BC)", .handler = ExecuteLoadIndirectAFromBC};
    handlers[0x0B] = {.name = "DEC BC", .handler = ExecuteDecBC};
    handlers[0x0C] = {.name = "INC C", .handler = ExecuteIncC};
    handlers[0x0D] = {.name = "DEC C", .handler = ExecuteDecC};
    handlers[0x0E] = {.name = "LD C, n", .handler = ExecuteLoadCn8};
    handlers[0x0F] = {.name = "RRCA", .handler = UnimplementedOpcode};

    handlers[0x10] = {.name = "STOP 0", .handler = UnimplementedOpcode};
    handlers[0x11] = {.name = "LD DE, nn", .handler = ExecuteLoadDEn16};
    handlers[0x12] = {.name = "LD (DE), A", .handler = ExecuteLoadIndirectDEFromA};
    handlers[0x13] = {.name = "INC DE", .handler = ExecuteIncDE};
    handlers[0x14] = {.name = "INC D", .handler = ExecuteIncD};
    handlers[0x15] = {.name = "DEC D", .handler = ExecuteDecD};
    handlers[0x16] = {.name = "LD D, n", .handler = ExecuteLoadDn8};
    handlers[0x17] = {.name = "RLA", .handler = UnimplementedOpcode};
    handlers[0x18] = {.name = "JR e8", .handler = ExecuteJumpRelativeE8};
    handlers[0x19] = {.name = "ADD HL, DE", .handler = UnimplementedOpcode};
    handlers[0x1A] = {.name = "LD A, (DE)", .handler = ExecuteLoadIndirectAFromDE};
    handlers[0x1B] = {.name = "DEC DE", .handler = ExecuteDecDE};
    handlers[0x1C] = {.name = "INC E", .handler = ExecuteIncE};
    handlers[0x1D] = {.name = "DEC E", .handler = ExecuteDecE};
    handlers[0x1E] = {.name = "LD E, n", .handler = ExecuteLoadEn8};
    handlers[0x1F] = {.name = "RRA", .handler = UnimplementedOpcode};

    handlers[0x20] = {.name = "JR NZ, e8", .handler = ExecuteJumpRelativeNzE8};
    handlers[0x21] = {.name = "LD HL, nn", .handler = ExecuteLoadHLn16};
    handlers[0x22] = {.name = "LD (HL+), A", .handler = ExecuteLoadIndirectHLIncrementFromA};
    handlers[0x23] = {.name = "INC HL", .handler = ExecuteIncHL};
    handlers[0x24] = {.name = "INC H", .handler = ExecuteIncH};
    handlers[0x25] = {.name = "DEC H", .handler = ExecuteDecH};
    handlers[0x26] = {.name = "LD H, n", .handler = ExecuteLoadHn8};
    handlers[0x27] = {.name = "DAA", .handler = UnimplementedOpcode};
    handlers[0x28] = {.name = "JR Z, e8", .handler = ExecuteJumpRelativeZE8};
    handlers[0x29] = {.name = "ADD HL, HL", .handler = UnimplementedOpcode};
    handlers[0x2A] = {.name = "LD A, (HL+)", .handler = ExecuteLoadIndirectAFromHLIncrement};
    handlers[0x2B] = {.name = "DEC HL", .handler = ExecuteDecHL};
    handlers[0x2C] = {.name = "INC L", .handler = ExecuteIncL};
    handlers[0x2D] = {.name = "DEC L", .handler = ExecuteDecL};
    handlers[0x2E] = {.name = "LD L, n", .handler = ExecuteLoadLn8};
    handlers[0x2F] = {.name = "CPL", .handler = UnimplementedOpcode};

    handlers[0x30] = {.name = "JR NC, e8", .handler = ExecuteJumpRelativeNcE8};
    handlers[0x31] = {.name = "LD SP, nn", .handler = ExecuteLoadSPn16};
    handlers[0x32] = {.name = "LD (HL-), A", .handler = ExecuteLoadIndirectHLDecrementFromA};
    handlers[0x33] = {.name = "INC SP", .handler = ExecuteIncSP};
    handlers[0x34] = {.name = "INC (HL)", .handler = UnimplementedOpcode};
    handlers[0x35] = {.name = "DEC (HL)", .handler = UnimplementedOpcode};
    handlers[0x36] = {.name = "LD (HL), n", .handler = ExecuteLoadIndirectHlN8};
    handlers[0x37] = {.name = "SCF", .handler = UnimplementedOpcode};
    handlers[0x38] = {.name = "JR C, e8", .handler = ExecuteJumpRelativeCE8};
    handlers[0x39] = {.name = "ADD HL, SP", .handler = UnimplementedOpcode};
    handlers[0x3A] = {.name = "LD A, (HL-)", .handler = ExecuteLoadIndirectAFromHLDecrement};
    handlers[0x3B] = {.name = "DEC SP", .handler = ExecuteDecSP};
    handlers[0x3C] = {.name = "INC A", .handler = ExecuteIncA};
    handlers[0x3D] = {.name = "DEC A", .handler = ExecuteDecA};
    handlers[0x3E] = {.name = "LD A, n", .handler = ExecuteLoadAn8};
    handlers[0x3F] = {.name = "CCF", .handler = UnimplementedOpcode};

    handlers[0x40] = {.name = "LD B, B", .handler = ExecuteLoad_B_B};
    handlers[0x41] = {.name = "LD B, C", .handler = ExecuteLoad_B_C};
    handlers[0x42] = {.name = "LD B, D", .handler = ExecuteLoad_B_D};
    handlers[0x43] = {.name = "LD B, E", .handler = ExecuteLoad_B_E};
    handlers[0x44] = {.name = "LD B, H", .handler = ExecuteLoad_B_H};
    handlers[0x45] = {.name = "LD B, L", .handler = ExecuteLoad_B_L};
    handlers[0x46] = {.name = "LD B, (HL)", .handler = ExecuteLoadIndirectBFromHL};
    handlers[0x47] = {.name = "LD B, A", .handler = ExecuteLoad_B_A};
    handlers[0x48] = {.name = "LD C, B", .handler = ExecuteLoad_C_B};
    handlers[0x49] = {.name = "LD C, C", .handler = ExecuteLoad_C_C};
    handlers[0x4A] = {.name = "LD C, D", .handler = ExecuteLoad_C_D};
    handlers[0x4B] = {.name = "LD C, E", .handler = ExecuteLoad_C_E};
    handlers[0x4C] = {.name = "LD C, H", .handler = ExecuteLoad_C_H};
    handlers[0x4D] = {.name = "LD C, L", .handler = ExecuteLoad_C_L};
    handlers[0x4E] = {.name = "LD C, (HL)", .handler = ExecuteLoadIndirectCFromHL};
    handlers[0x4F] = {.name = "LD C, A", .handler = ExecuteLoad_C_A};

    handlers[0x50] = {.name = "LD D, B", .handler = ExecuteLoad_D_B};
    handlers[0x51] = {.name = "LD D, C", .handler = ExecuteLoad_D_C};
    handlers[0x52] = {.name = "LD D, D", .handler = ExecuteLoad_D_D};
    handlers[0x53] = {.name = "LD D, E", .handler = ExecuteLoad_D_E};
    handlers[0x54] = {.name = "LD D, H", .handler = ExecuteLoad_D_H};
    handlers[0x55] = {.name = "LD D, L", .handler = ExecuteLoad_D_L};
    handlers[0x56] = {.name = "LD D, (HL)", .handler = ExecuteLoadIndirectDFromHL};
    handlers[0x57] = {.name = "LD D, A", .handler = ExecuteLoad_D_A};
    handlers[0x58] = {.name = "LD E, B", .handler = ExecuteLoad_E_B};
    handlers[0x59] = {.name = "LD E, C", .handler = ExecuteLoad_E_C};
    handlers[0x5A] = {.name = "LD E, D", .handler = ExecuteLoad_E_D};
    handlers[0x5B] = {.name = "LD E, E", .handler = ExecuteLoad_E_E};
    handlers[0x5C] = {.name = "LD E, H", .handler = ExecuteLoad_E_H};
    handlers[0x5D] = {.name = "LD E, L", .handler = ExecuteLoad_E_L};
    handlers[0x5E] = {.name = "LD E, (HL)", .handler = ExecuteLoadIndirectEFromHL};
    handlers[0x5F] = {.name = "LD E, A", .handler = ExecuteLoad_E_A};

    handlers[0x60] = {.name = "LD H, B", .handler = ExecuteLoad_H_B};
    handlers[0x61] = {.name = "LD H, C", .handler = ExecuteLoad_H_C};
    handlers[0x62] = {.name = "LD H, D", .handler = ExecuteLoad_H_D};
    handlers[0x63] = {.name = "LD H, E", .handler = ExecuteLoad_H_E};
    handlers[0x64] = {.name = "LD H, H", .handler = ExecuteLoad_H_H};
    handlers[0x65] = {.name = "LD H, L", .handler = ExecuteLoad_H_L};
    handlers[0x66] = {.name = "LD H, (HL)", .handler = ExecuteLoadIndirectHFromHL};
    handlers[0x67] = {.name = "LD H, A", .handler = ExecuteLoad_H_A};
    handlers[0x68] = {.name = "LD L, B", .handler = ExecuteLoad_L_B};
    handlers[0x69] = {.name = "LD L, C", .handler = ExecuteLoad_L_C};
    handlers[0x6A] = {.name = "LD L, D", .handler = ExecuteLoad_L_D};
    handlers[0x6B] = {.name = "LD L, E", .handler = ExecuteLoad_L_E};
    handlers[0x6C] = {.name = "LD L, H", .handler = ExecuteLoad_L_H};
    handlers[0x6D] = {.name = "LD L, L", .handler = ExecuteLoad_L_L};
    handlers[0x6E] = {.name = "LD L, (HL)", .handler = ExecuteLoadIndirectLFromHL};
    handlers[0x6F] = {.name = "LD L, A", .handler = ExecuteLoad_L_A};

    handlers[0x70] = {.name = "LD (HL), B", .handler = ExecuteLoadIndirectHLFromB};
    handlers[0x71] = {.name = "LD (HL), C", .handler = ExecuteLoadIndirectHLFromC};
    handlers[0x72] = {.name = "LD (HL), D", .handler = ExecuteLoadIndirectHLFromD};
    handlers[0x73] = {.name = "LD (HL), E", .handler = ExecuteLoadIndirectHLFromE};
    handlers[0x74] = {.name = "LD (HL), H", .handler = ExecuteLoadIndirectHLFromH};
    handlers[0x75] = {.name = "LD (HL), L", .handler = ExecuteLoadIndirectHLFromL};
    handlers[0x76] = {.name = "HALT", .handler = UnimplementedOpcode};
    handlers[0x77] = {.name = "LD (HL), A", .handler = ExecuteLoadIndirectHLFromA};
    handlers[0x78] = {.name = "LD A, B", .handler = ExecuteLoad_A_B};
    handlers[0x79] = {.name = "LD A, C", .handler = ExecuteLoad_A_C};
    handlers[0x7A] = {.name = "LD A, D", .handler = ExecuteLoad_A_D};
    handlers[0x7B] = {.name = "LD A, E", .handler = ExecuteLoad_A_E};
    handlers[0x7C] = {.name = "LD A, H", .handler = ExecuteLoad_A_H};
    handlers[0x7D] = {.name = "LD A, L", .handler = ExecuteLoad_A_L};
    handlers[0x7E] = {.name = "LD A, (HL)", .handler = ExecuteLoadIndirectAFromHL};
    handlers[0x7F] = {.name = "LD A, A", .handler = ExecuteLoad_A_A};

    handlers[0x80] = {.name = "ADD A, B", .handler = ExecuteAddB};
    handlers[0x81] = {.name = "ADD A, C", .handler = ExecuteAddC};
    handlers[0x82] = {.name = "ADD A, D", .handler = ExecuteAddD};
    handlers[0x83] = {.name = "ADD A, E", .handler = ExecuteAddE};
    handlers[0x84] = {.name = "ADD A, H", .handler = ExecuteAddH};
    handlers[0x85] = {.name = "ADD A, L", .handler = ExecuteAddL};
    handlers[0x86] = {.name = "ADD A, (HL)", .handler = ExecuteAddAFromIndirectHL};
    handlers[0x87] = {.name = "ADD A, A", .handler = ExecuteAddA};
    handlers[0x88] = {.name = "ADC A, B", .handler = ExecuteAdcB};
    handlers[0x89] = {.name = "ADC A, C", .handler = ExecuteAdcC};
    handlers[0x8A] = {.name = "ADC A, D", .handler = ExecuteAdcD};
    handlers[0x8B] = {.name = "ADC A, E", .handler = ExecuteAdcE};
    handlers[0x8C] = {.name = "ADC A, H", .handler = ExecuteAdcH};
    handlers[0x8D] = {.name = "ADC A, L", .handler = ExecuteAdcL};
    handlers[0x8E] = {.name = "ADC A, (HL)", .handler = ExecuteAdcAFromIndirectHL};
    handlers[0x8F] = {.name = "ADC A, A", .handler = ExecuteAdcA};

    handlers[0x90] = {.name = "SUB B", .handler = ExecuteSubB};
    handlers[0x91] = {.name = "SUB C", .handler = ExecuteSubC};
    handlers[0x92] = {.name = "SUB D", .handler = ExecuteSubD};
    handlers[0x93] = {.name = "SUB E", .handler = ExecuteSubE};
    handlers[0x94] = {.name = "SUB H", .handler = ExecuteSubH};
    handlers[0x95] = {.name = "SUB L", .handler = ExecuteSubL};
    handlers[0x96] = {.name = "SUB (HL)", .handler = ExecuteSubAFromIndirectHL};
    handlers[0x97] = {.name = "SUB A", .handler = ExecuteSubA};
    handlers[0x98] = {.name = "SBC A, B", .handler = ExecuteSbcB};
    handlers[0x99] = {.name = "SBC A, C", .handler = ExecuteSbcC};
    handlers[0x9A] = {.name = "SBC A, D", .handler = ExecuteSbcD};
    handlers[0x9B] = {.name = "SBC A, E", .handler = ExecuteSbcE};
    handlers[0x9C] = {.name = "SBC A, H", .handler = ExecuteSbcH};
    handlers[0x9D] = {.name = "SBC A, L", .handler = ExecuteSbcL};
    handlers[0x9E] = {.name = "SBC A, (HL)", .handler = ExecuteSbcAFromIndirectHL};
    handlers[0x9F] = {.name = "SBC A, A", .handler = ExecuteSbcA};

    handlers[0xA0] = {.name = "AND B", .handler = ExecuteAndB};
    handlers[0xA1] = {.name = "AND C", .handler = ExecuteAndC};
    handlers[0xA2] = {.name = "AND D", .handler = ExecuteAndD};
    handlers[0xA3] = {.name = "AND E", .handler = ExecuteAndE};
    handlers[0xA4] = {.name = "AND H", .handler = ExecuteAndH};
    handlers[0xA5] = {.name = "AND L", .handler = ExecuteAndL};
    handlers[0xA6] = {.name = "AND (HL)", .handler = ExecuteAndAFromIndirectHL};
    handlers[0xA7] = {.name = "AND A", .handler = ExecuteAndA};
    handlers[0xA8] = {.name = "XOR B", .handler = ExecuteXorB};
    handlers[0xA9] = {.name = "XOR C", .handler = ExecuteXorC};
    handlers[0xAA] = {.name = "XOR D", .handler = ExecuteXorD};
    handlers[0xAB] = {.name = "XOR E", .handler = ExecuteXorE};
    handlers[0xAC] = {.name = "XOR H", .handler = ExecuteXorH};
    handlers[0xAD] = {.name = "XOR L", .handler = ExecuteXorL};
    handlers[0xAE] = {.name = "XOR (HL)", .handler = ExecuteXorAFromIndirectHL};
    handlers[0xAF] = {.name = "XOR A", .handler = ExecuteXorA};

    handlers[0xB0] = {.name = "OR B", .handler = ExecuteOrB};
    handlers[0xB1] = {.name = "OR C", .handler = ExecuteOrC};
    handlers[0xB2] = {.name = "OR D", .handler = ExecuteOrD};
    handlers[0xB3] = {.name = "OR E", .handler = ExecuteOrE};
    handlers[0xB4] = {.name = "OR H", .handler = ExecuteOrH};
    handlers[0xB5] = {.name = "OR L", .handler = ExecuteOrL};
    handlers[0xB6] = {.name = "OR (HL)", .handler = ExecuteOrAFromIndirectHL};
    handlers[0xB7] = {.name = "OR A", .handler = ExecuteOrA};
    handlers[0xB8] = {.name = "CP B", .handler = ExecuteCpB};
    handlers[0xB9] = {.name = "CP C", .handler = ExecuteCpC};
    handlers[0xBA] = {.name = "CP D", .handler = ExecuteCpD};
    handlers[0xBB] = {.name = "CP E", .handler = ExecuteCpE};
    handlers[0xBC] = {.name = "CP H", .handler = ExecuteCpH};
    handlers[0xBD] = {.name = "CP L", .handler = ExecuteCpL};
    handlers[0xBE] = {.name = "CP (HL)", .handler = ExecuteCpAFromIndirectHL};
    handlers[0xBF] = {.name = "CP A", .handler = ExecuteCpA};

    handlers[0xC0] = {.name = "RET NZ", .handler = ExecuteRetNz};
    handlers[0xC1] = {.name = "POP BC", .handler = ExecutePopBC};
    handlers[0xC2] = {.name = "JP NZ, a16", .handler = ExecuteJumpNzA16};
    handlers[0xC3] = {.name = "JP a16", .handler = ExecuteJumpA16};
    handlers[0xC4] = {.name = "CALL NZ, a16", .handler = ExecuteCallNzA16};
    handlers[0xC5] = {.name = "PUSH BC", .handler = ExecutePushBC};
    handlers[0xC6] = {.name = "ADD A, n", .handler = ExecuteAddAn8};
    handlers[0xC7] = {.name = "RST $00", .handler = ExecuteRst00};
    handlers[0xC8] = {.name = "RET Z", .handler = ExecuteRetZ};
    handlers[0xC9] = {.name = "RET", .handler = ExecuteRet};
    handlers[0xCA] = {.name = "JP Z, a16", .handler = ExecuteJumpZA16};
    handlers[0xCB] = {.name = "PREFIX CB", .handler = UnimplementedOpcode};
    handlers[0xCC] = {.name = "CALL Z, a16", .handler = ExecuteCallZA16};
    handlers[0xCD] = {.name = "CALL a16", .handler = ExecuteCallA16};
    handlers[0xCE] = {.name = "ADC A, n", .handler = ExecuteAdcAn8};
    handlers[0xCF] = {.name = "RST $08", .handler = ExecuteRst08};

    handlers[0xD0] = {.name = "RET NC", .handler = ExecuteRetNc};
    handlers[0xD1] = {.name = "POP DE", .handler = ExecutePopDE};
    handlers[0xD2] = {.name = "JP NC, a16", .handler = ExecuteJumpNcA16};
    handlers[0xD3] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xD4] = {.name = "CALL NC, a16", .handler = ExecuteCallNcA16};
    handlers[0xD5] = {.name = "PUSH DE", .handler = ExecutePushDE};
    handlers[0xD6] = {.name = "SUB n", .handler = ExecuteSubAn8};
    handlers[0xD7] = {.name = "RST $10", .handler = ExecuteRst10};
    handlers[0xD8] = {.name = "RET C", .handler = ExecuteRetC};
    handlers[0xD9] = {.name = "RETI", .handler = ExecuteRetI};
    handlers[0xDA] = {.name = "JP C, a16", .handler = ExecuteJumpCA16};
    handlers[0xDB] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xDC] = {.name = "CALL C, a16", .handler = ExecuteCallCA16};
    handlers[0xDD] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xDE] = {.name = "SBC A, n", .handler = ExecuteSbcAn8};
    handlers[0xDF] = {.name = "RST $18", .handler = ExecuteRst18};

    handlers[0xE0] = {.name = "LDH (a8), A", .handler = ExecuteLoadIndirectA8FromA};
    handlers[0xE1] = {.name = "POP HL", .handler = ExecutePopHL};
    handlers[0xE2] = {.name = "LD (C), A", .handler = ExecuteLoadIndirectCFromA};
    handlers[0xE3] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xE4] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xE5] = {.name = "PUSH HL", .handler = ExecutePushHL};
    handlers[0xE6] = {.name = "AND n", .handler = ExecuteAndAn8};
    handlers[0xE7] = {.name = "RST $20", .handler = ExecuteRst20};
    handlers[0xE8] = {.name = "ADD SP, e8", .handler = UnimplementedOpcode};
    handlers[0xE9] = {.name = "JP HL", .handler = ExecuteJumpHl};
    handlers[0xEA] = {.name = "LD (a16), A", .handler = ExecuteLoadIndirectA16FromA};
    handlers[0xEB] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xEC] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xED] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xEE] = {.name = "XOR n", .handler = ExecuteXorAn8};
    handlers[0xEF] = {.name = "RST $28", .handler = ExecuteRst28};

    handlers[0xF0] = {.name = "LDH A, (a8)", .handler = ExecuteLoadAFromIndirectA8};
    handlers[0xF1] = {.name = "POP AF", .handler = ExecutePopAF};
    handlers[0xF2] = {.name = "LD A, (C)", .handler = ExecuteLoadAFromIndirectC};
    handlers[0xF3] = {.name = "DI", .handler = ExecuteDi};
    handlers[0xF4] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xF5] = {.name = "PUSH AF", .handler = ExecutePushAF};
    handlers[0xF6] = {.name = "OR n", .handler = ExecuteOrAn8};
    handlers[0xF7] = {.name = "RST $30", .handler = ExecuteRst30};
    handlers[0xF8] = {.name = "LD HL, SP + e8", .handler = ExecuteLoadSPFromHlPlusE8};
    handlers[0xF9] = {.name = "LD SP, HL", .handler = ExecuteLoad_SP_HL};
    handlers[0xFA] = {.name = "LD A, (a16)", .handler = ExecuteLoadAFromIndirectA16};
    handlers[0xFB] = {.name = "EI", .handler = ExecuteEi};
    handlers[0xFC] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xFD] = {.name = "ILLEGAL", .handler = IllegalOpcode};
    handlers[0xFE] = {.name = "CP n", .handler = ExecuteCpAn8};
    handlers[0xFF] = {.name = "RST $38", .handler = ExecuteRst38};

    return handlers;
}

} // namespace EmulatorLib
