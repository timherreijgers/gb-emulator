/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/instruction_translation.h"

#include <array>

namespace EmulatorLib
{

namespace
{

[[nodiscard]] consteval auto CreateOpCodeLut() noexcept -> std::array<std::string_view, 256>
{
    std::array<std::string_view, 256> names{};
    names.fill("Not_implemented_yet");

    names[0x00] = "Noop";
    names[0x01] = "LoadBCn16";
    names[0x02] = "LoadIndirectBCFromA";
    names[0x06] = "LoadBn8";
    names[0x08] = "LoadA16SP";
    names[0x0A] = "LoadAFromIndirectBC";
    names[0x0E] = "LoadCn8";

    names[0x11] = "LoadDEn16";
    names[0x12] = "LoadIndirectDEFromA";
    names[0x16] = "LoadDn8";
    names[0x1A] = "LoadAFromIndirectDE";
    names[0x1E] = "LoadEn8";

    names[0x21] = "LoadHLn16";
    names[0x22] = "LoadIndirectHLIncrementFromA";
    names[0x26] = "LoadHn8";
    names[0x2A] = "LoadAFromIndirectHLIncrement";
    names[0x2E] = "LoadLn8";

    names[0x31] = "LoadSPn16";
    names[0x32] = "LoadIndirectHLDecrementFromA";
    names[0x36] = "LoadHlN8";
    names[0x3A] = "LoadAFromIndirectHLDecrement";
    names[0x3E] = "LoadAn8";

    names[0x40] = "Load_B_B";
    names[0x41] = "Load_B_C";
    names[0x42] = "Load_B_D";
    names[0x43] = "Load_B_E";
    names[0x44] = "Load_B_H";
    names[0x45] = "Load_B_L";
    names[0x46] = "LoadBFromIndirectHL";
    names[0x47] = "Load_B_A";
    names[0x48] = "Load_C_B";
    names[0x49] = "Load_C_C";
    names[0x4A] = "Load_C_D";
    names[0x4B] = "Load_C_E";
    names[0x4C] = "Load_C_H";
    names[0x4D] = "Load_C_L";
    names[0x4E] = "LoadCFromIndirectHL";
    names[0x4F] = "Load_C_A";

    names[0x50] = "Load_D_B";
    names[0x51] = "Load_D_C";
    names[0x52] = "Load_D_D";
    names[0x53] = "Load_D_E";
    names[0x54] = "Load_D_H";
    names[0x55] = "Load_D_L";
    names[0x56] = "LoadDFromIndirectHL";
    names[0x57] = "Load_D_A";
    names[0x58] = "Load_E_B";
    names[0x59] = "Load_E_C";
    names[0x5A] = "Load_E_D";
    names[0x5B] = "Load_E_E";
    names[0x5C] = "Load_E_H";
    names[0x5D] = "Load_E_L";
    names[0x5E] = "LoadEFromIndirectHL";
    names[0x5F] = "Load_E_A";

    names[0x60] = "Load_H_B";
    names[0x61] = "Load_H_C";
    names[0x62] = "Load_H_D";
    names[0x63] = "Load_H_E";
    names[0x64] = "Load_H_H";
    names[0x65] = "Load_H_L";
    names[0x66] = "LoadHFromIndirectHL";
    names[0x67] = "Load_H_A";
    names[0x68] = "Load_L_B";
    names[0x69] = "Load_L_C";
    names[0x6A] = "Load_L_D";
    names[0x6B] = "Load_L_E";
    names[0x6C] = "Load_L_H";
    names[0x6D] = "Load_L_L";
    names[0x6E] = "LoadLFromIndirectHL";
    names[0x6F] = "Load_L_A";

    names[0x70] = "LoadIndirectHLFromB";
    names[0x71] = "LoadIndirectHLFromC";
    names[0x72] = "LoadIndirectHLFromD";
    names[0x73] = "LoadIndirectHLFromE";
    names[0x74] = "LoadIndirectHLFromH";
    names[0x75] = "LoadIndirectHLFromL";
    names[0x77] = "LoadIndirectHLFromA";
    names[0x78] = "Load_A_B";
    names[0x79] = "Load_A_C";
    names[0x7A] = "Load_A_D";
    names[0x7B] = "Load_A_E";
    names[0x7C] = "Load_A_H";
    names[0x7D] = "Load_A_L";
    names[0x7E] = "LoadAFromIndirectHL";
    names[0x7F] = "Load_A_A";

    names[0xC1] = "POP_BC";

    names[0xD1] = "POP_DE";

    names[0xE0] = "LoadIndirectA8FromA";
    names[0xE1] = "POP_HL";
    names[0xE2] = "LoadIndirectCFromA";
    names[0xEA] = "LoadIndirectA16FromA";

    names[0xF0] = "LoadAFromIndirectA8";
    names[0xF1] = "POP_AF";
    names[0xF2] = "LoadAFromIndirectC";
    names[0xF8] = "LoadHLFromSPPlusE";
    names[0xF9] = "Load_SP_HL";
    names[0xFA] = "LoadAFromIndirectA16";

    return names;
}

} // namespace

auto OpCodeToInstructionName(std::byte opcode) -> std::string_view
{
    static constexpr auto opCodeLut = CreateOpCodeLut();
    return opCodeLut[static_cast<size_t>(opcode)];
}

} // namespace EmulatorLib