/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_handler.h"

#include "extended_instruction_handlers/bit.h"

#include <array>
#include <format>
#include <stdexcept>

namespace EmulatorLib
{

inline InstructionHandler UnimplementedExtendedOpcode(AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters)
{
    throw std::runtime_error(std::format("Unimplemented extended opcode: {}", cpuRegisters.instructionRegister.value));
}

[[nodiscard]] inline auto CreateExtendedOpcodeHandlers() noexcept -> std::array<OpcodeHandler, 256>
{
    std::array<OpcodeHandler, 256> handlers{};

    handlers[0x00] = {.name = "RlcB", .handler = UnimplementedExtendedOpcode};
    handlers[0x01] = {.name = "RlcC", .handler = UnimplementedExtendedOpcode};
    handlers[0x02] = {.name = "RlcD", .handler = UnimplementedExtendedOpcode};
    handlers[0x03] = {.name = "RlcE", .handler = UnimplementedExtendedOpcode};
    handlers[0x04] = {.name = "RlcH", .handler = UnimplementedExtendedOpcode};
    handlers[0x05] = {.name = "RlcL", .handler = UnimplementedExtendedOpcode};
    handlers[0x06] = {.name = "RlcHL", .handler = UnimplementedExtendedOpcode};
    handlers[0x07] = {.name = "RlcA", .handler = UnimplementedExtendedOpcode};

    handlers[0x08] = {.name = "RrcB", .handler = UnimplementedExtendedOpcode};
    handlers[0x09] = {.name = "RrcC", .handler = UnimplementedExtendedOpcode};
    handlers[0x0A] = {.name = "RrcD", .handler = UnimplementedExtendedOpcode};
    handlers[0x0B] = {.name = "RrcE", .handler = UnimplementedExtendedOpcode};
    handlers[0x0C] = {.name = "RrcH", .handler = UnimplementedExtendedOpcode};
    handlers[0x0D] = {.name = "RrcL", .handler = UnimplementedExtendedOpcode};
    handlers[0x0E] = {.name = "RrcHL", .handler = UnimplementedExtendedOpcode};
    handlers[0x0F] = {.name = "RrcA", .handler = UnimplementedExtendedOpcode};

    handlers[0x10] = {.name = "RlB", .handler = UnimplementedExtendedOpcode};
    handlers[0x11] = {.name = "RlC", .handler = UnimplementedExtendedOpcode};
    handlers[0x12] = {.name = "RlD", .handler = UnimplementedExtendedOpcode};
    handlers[0x13] = {.name = "RlE", .handler = UnimplementedExtendedOpcode};
    handlers[0x14] = {.name = "RlH", .handler = UnimplementedExtendedOpcode};
    handlers[0x15] = {.name = "RlL", .handler = UnimplementedExtendedOpcode};
    handlers[0x16] = {.name = "RlHL", .handler = UnimplementedExtendedOpcode};
    handlers[0x17] = {.name = "RlA", .handler = UnimplementedExtendedOpcode};

    handlers[0x18] = {.name = "RrB", .handler = UnimplementedExtendedOpcode};
    handlers[0x19] = {.name = "RrC", .handler = UnimplementedExtendedOpcode};
    handlers[0x1A] = {.name = "RrD", .handler = UnimplementedExtendedOpcode};
    handlers[0x1B] = {.name = "RrE", .handler = UnimplementedExtendedOpcode};
    handlers[0x1C] = {.name = "RrH", .handler = UnimplementedExtendedOpcode};
    handlers[0x1D] = {.name = "RrL", .handler = UnimplementedExtendedOpcode};
    handlers[0x1E] = {.name = "RrHL", .handler = UnimplementedExtendedOpcode};
    handlers[0x1F] = {.name = "RrA", .handler = UnimplementedExtendedOpcode};

    handlers[0x20] = {.name = "SlaB", .handler = UnimplementedExtendedOpcode};
    handlers[0x21] = {.name = "SlaC", .handler = UnimplementedExtendedOpcode};
    handlers[0x22] = {.name = "SlaD", .handler = UnimplementedExtendedOpcode};
    handlers[0x23] = {.name = "SlaE", .handler = UnimplementedExtendedOpcode};
    handlers[0x24] = {.name = "SlaH", .handler = UnimplementedExtendedOpcode};
    handlers[0x25] = {.name = "SlaL", .handler = UnimplementedExtendedOpcode};
    handlers[0x26] = {.name = "SlaHL", .handler = UnimplementedExtendedOpcode};
    handlers[0x27] = {.name = "SlaA", .handler = UnimplementedExtendedOpcode};

    handlers[0x28] = {.name = "SraB", .handler = UnimplementedExtendedOpcode};
    handlers[0x29] = {.name = "SraC", .handler = UnimplementedExtendedOpcode};
    handlers[0x2A] = {.name = "SraD", .handler = UnimplementedExtendedOpcode};
    handlers[0x2B] = {.name = "SraE", .handler = UnimplementedExtendedOpcode};
    handlers[0x2C] = {.name = "SraH", .handler = UnimplementedExtendedOpcode};
    handlers[0x2D] = {.name = "SraL", .handler = UnimplementedExtendedOpcode};
    handlers[0x2E] = {.name = "SraHL", .handler = UnimplementedExtendedOpcode};
    handlers[0x2F] = {.name = "SraA", .handler = UnimplementedExtendedOpcode};

    handlers[0x30] = {.name = "SwapB", .handler = UnimplementedExtendedOpcode};
    handlers[0x31] = {.name = "SwapC", .handler = UnimplementedExtendedOpcode};
    handlers[0x32] = {.name = "SwapD", .handler = UnimplementedExtendedOpcode};
    handlers[0x33] = {.name = "SwapE", .handler = UnimplementedExtendedOpcode};
    handlers[0x34] = {.name = "SwapH", .handler = UnimplementedExtendedOpcode};
    handlers[0x35] = {.name = "SwapL", .handler = UnimplementedExtendedOpcode};
    handlers[0x36] = {.name = "SwapHL", .handler = UnimplementedExtendedOpcode};
    handlers[0x37] = {.name = "SwapA", .handler = UnimplementedExtendedOpcode};

    handlers[0x38] = {.name = "SrlB", .handler = UnimplementedExtendedOpcode};
    handlers[0x39] = {.name = "SrlC", .handler = UnimplementedExtendedOpcode};
    handlers[0x3A] = {.name = "SrlD", .handler = UnimplementedExtendedOpcode};
    handlers[0x3B] = {.name = "SrlE", .handler = UnimplementedExtendedOpcode};
    handlers[0x3C] = {.name = "SrlH", .handler = UnimplementedExtendedOpcode};
    handlers[0x3D] = {.name = "SrlL", .handler = UnimplementedExtendedOpcode};
    handlers[0x3E] = {.name = "SrlHL", .handler = UnimplementedExtendedOpcode};
    handlers[0x3F] = {.name = "SrlA", .handler = UnimplementedExtendedOpcode};

    handlers[0x40] = {.name = "Bit0B", .handler = ExecuteBit0B};
    handlers[0x41] = {.name = "Bit0C", .handler = ExecuteBit0C};
    handlers[0x42] = {.name = "Bit0D", .handler = ExecuteBit0D};
    handlers[0x43] = {.name = "Bit0E", .handler = ExecuteBit0E};
    handlers[0x44] = {.name = "Bit0H", .handler = ExecuteBit0H};
    handlers[0x45] = {.name = "Bit0L", .handler = ExecuteBit0L};
    handlers[0x46] = {.name = "Bit0HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x47] = {.name = "Bit0A", .handler = ExecuteBit0A};

    handlers[0x48] = {.name = "Bit1B", .handler = ExecuteBit1B};
    handlers[0x49] = {.name = "Bit1C", .handler = ExecuteBit1C};
    handlers[0x4A] = {.name = "Bit1D", .handler = ExecuteBit1D};
    handlers[0x4B] = {.name = "Bit1E", .handler = ExecuteBit1E};
    handlers[0x4C] = {.name = "Bit1H", .handler = ExecuteBit1H};
    handlers[0x4D] = {.name = "Bit1L", .handler = ExecuteBit1L};
    handlers[0x4E] = {.name = "Bit1HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x4F] = {.name = "Bit1A", .handler = ExecuteBit1A};

    handlers[0x50] = {.name = "Bit2B", .handler = ExecuteBit2B};
    handlers[0x51] = {.name = "Bit2C", .handler = ExecuteBit2C};
    handlers[0x52] = {.name = "Bit2D", .handler = ExecuteBit2D};
    handlers[0x53] = {.name = "Bit2E", .handler = ExecuteBit2E};
    handlers[0x54] = {.name = "Bit2H", .handler = ExecuteBit2H};
    handlers[0x55] = {.name = "Bit2L", .handler = ExecuteBit2L};
    handlers[0x56] = {.name = "Bit2HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x57] = {.name = "Bit2A", .handler = ExecuteBit2A};

    handlers[0x58] = {.name = "Bit3B", .handler = ExecuteBit3B};
    handlers[0x59] = {.name = "Bit3C", .handler = ExecuteBit3C};
    handlers[0x5A] = {.name = "Bit3D", .handler = ExecuteBit3D};
    handlers[0x5B] = {.name = "Bit3E", .handler = ExecuteBit3E};
    handlers[0x5C] = {.name = "Bit3H", .handler = ExecuteBit3H};
    handlers[0x5D] = {.name = "Bit3L", .handler = ExecuteBit3L};
    handlers[0x5E] = {.name = "Bit3HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x5F] = {.name = "Bit3A", .handler = ExecuteBit3A};

    handlers[0x60] = {.name = "Bit4B", .handler = ExecuteBit4B};
    handlers[0x61] = {.name = "Bit4C", .handler = ExecuteBit4C};
    handlers[0x62] = {.name = "Bit4D", .handler = ExecuteBit4D};
    handlers[0x63] = {.name = "Bit4E", .handler = ExecuteBit4E};
    handlers[0x64] = {.name = "Bit4H", .handler = ExecuteBit4H};
    handlers[0x65] = {.name = "Bit4L", .handler = ExecuteBit4L};
    handlers[0x66] = {.name = "Bit4HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x67] = {.name = "Bit4A", .handler = ExecuteBit4A};

    handlers[0x68] = {.name = "Bit5B", .handler = ExecuteBit5B};
    handlers[0x69] = {.name = "Bit5C", .handler = ExecuteBit5C};
    handlers[0x6A] = {.name = "Bit5D", .handler = ExecuteBit5D};
    handlers[0x6B] = {.name = "Bit5E", .handler = ExecuteBit5E};
    handlers[0x6C] = {.name = "Bit5H", .handler = ExecuteBit5H};
    handlers[0x6D] = {.name = "Bit5L", .handler = ExecuteBit5L};
    handlers[0x6E] = {.name = "Bit5HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x6F] = {.name = "Bit5A", .handler = ExecuteBit5A};

    handlers[0x70] = {.name = "Bit6B", .handler = ExecuteBit6B};
    handlers[0x71] = {.name = "Bit6C", .handler = ExecuteBit6C};
    handlers[0x72] = {.name = "Bit6D", .handler = ExecuteBit6D};
    handlers[0x73] = {.name = "Bit6E", .handler = ExecuteBit6E};
    handlers[0x74] = {.name = "Bit6H", .handler = ExecuteBit6H};
    handlers[0x75] = {.name = "Bit6L", .handler = ExecuteBit6L};
    handlers[0x76] = {.name = "Bit6HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x77] = {.name = "Bit6A", .handler = ExecuteBit6A};

    handlers[0x78] = {.name = "Bit7B", .handler = ExecuteBit7B};
    handlers[0x79] = {.name = "Bit7C", .handler = ExecuteBit7C};
    handlers[0x7A] = {.name = "Bit7D", .handler = ExecuteBit7D};
    handlers[0x7B] = {.name = "Bit7E", .handler = ExecuteBit7E};
    handlers[0x7C] = {.name = "Bit7H", .handler = ExecuteBit7H};
    handlers[0x7D] = {.name = "Bit7L", .handler = ExecuteBit7L};
    handlers[0x7E] = {.name = "Bit7HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x7F] = {.name = "Bit7A", .handler = ExecuteBit7A};

    handlers[0x80] = {.name = "Res0B", .handler = UnimplementedExtendedOpcode};
    handlers[0x81] = {.name = "Res0C", .handler = UnimplementedExtendedOpcode};
    handlers[0x82] = {.name = "Res0D", .handler = UnimplementedExtendedOpcode};
    handlers[0x83] = {.name = "Res0E", .handler = UnimplementedExtendedOpcode};
    handlers[0x84] = {.name = "Res0H", .handler = UnimplementedExtendedOpcode};
    handlers[0x85] = {.name = "Res0L", .handler = UnimplementedExtendedOpcode};
    handlers[0x86] = {.name = "Res0HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x87] = {.name = "Res0A", .handler = UnimplementedExtendedOpcode};

    handlers[0x88] = {.name = "Res1B", .handler = UnimplementedExtendedOpcode};
    handlers[0x89] = {.name = "Res1C", .handler = UnimplementedExtendedOpcode};
    handlers[0x8A] = {.name = "Res1D", .handler = UnimplementedExtendedOpcode};
    handlers[0x8B] = {.name = "Res1E", .handler = UnimplementedExtendedOpcode};
    handlers[0x8C] = {.name = "Res1H", .handler = UnimplementedExtendedOpcode};
    handlers[0x8D] = {.name = "Res1L", .handler = UnimplementedExtendedOpcode};
    handlers[0x8E] = {.name = "Res1HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x8F] = {.name = "Res1A", .handler = UnimplementedExtendedOpcode};

    handlers[0x90] = {.name = "Res2B", .handler = UnimplementedExtendedOpcode};
    handlers[0x91] = {.name = "Res2C", .handler = UnimplementedExtendedOpcode};
    handlers[0x92] = {.name = "Res2D", .handler = UnimplementedExtendedOpcode};
    handlers[0x93] = {.name = "Res2E", .handler = UnimplementedExtendedOpcode};
    handlers[0x94] = {.name = "Res2H", .handler = UnimplementedExtendedOpcode};
    handlers[0x95] = {.name = "Res2L", .handler = UnimplementedExtendedOpcode};
    handlers[0x96] = {.name = "Res2HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x97] = {.name = "Res2A", .handler = UnimplementedExtendedOpcode};

    handlers[0x98] = {.name = "Res3B", .handler = UnimplementedExtendedOpcode};
    handlers[0x99] = {.name = "Res3C", .handler = UnimplementedExtendedOpcode};
    handlers[0x9A] = {.name = "Res3D", .handler = UnimplementedExtendedOpcode};
    handlers[0x9B] = {.name = "Res3E", .handler = UnimplementedExtendedOpcode};
    handlers[0x9C] = {.name = "Res3H", .handler = UnimplementedExtendedOpcode};
    handlers[0x9D] = {.name = "Res3L", .handler = UnimplementedExtendedOpcode};
    handlers[0x9E] = {.name = "Res3HL", .handler = UnimplementedExtendedOpcode};
    handlers[0x9F] = {.name = "Res3A", .handler = UnimplementedExtendedOpcode};

    handlers[0xA0] = {.name = "Res4B", .handler = UnimplementedExtendedOpcode};
    handlers[0xA1] = {.name = "Res4C", .handler = UnimplementedExtendedOpcode};
    handlers[0xA2] = {.name = "Res4D", .handler = UnimplementedExtendedOpcode};
    handlers[0xA3] = {.name = "Res4E", .handler = UnimplementedExtendedOpcode};
    handlers[0xA4] = {.name = "Res4H", .handler = UnimplementedExtendedOpcode};
    handlers[0xA5] = {.name = "Res4L", .handler = UnimplementedExtendedOpcode};
    handlers[0xA6] = {.name = "Res4HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xA7] = {.name = "Res4A", .handler = UnimplementedExtendedOpcode};

    handlers[0xA8] = {.name = "Res5B", .handler = UnimplementedExtendedOpcode};
    handlers[0xA9] = {.name = "Res5C", .handler = UnimplementedExtendedOpcode};
    handlers[0xAA] = {.name = "Res5D", .handler = UnimplementedExtendedOpcode};
    handlers[0xAB] = {.name = "Res5E", .handler = UnimplementedExtendedOpcode};
    handlers[0xAC] = {.name = "Res5H", .handler = UnimplementedExtendedOpcode};
    handlers[0xAD] = {.name = "Res5L", .handler = UnimplementedExtendedOpcode};
    handlers[0xAE] = {.name = "Res5HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xAF] = {.name = "Res5A", .handler = UnimplementedExtendedOpcode};

    handlers[0xB0] = {.name = "Res6B", .handler = UnimplementedExtendedOpcode};
    handlers[0xB1] = {.name = "Res6C", .handler = UnimplementedExtendedOpcode};
    handlers[0xB2] = {.name = "Res6D", .handler = UnimplementedExtendedOpcode};
    handlers[0xB3] = {.name = "Res6E", .handler = UnimplementedExtendedOpcode};
    handlers[0xB4] = {.name = "Res6H", .handler = UnimplementedExtendedOpcode};
    handlers[0xB5] = {.name = "Res6L", .handler = UnimplementedExtendedOpcode};
    handlers[0xB6] = {.name = "Res6HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xB7] = {.name = "Res6A", .handler = UnimplementedExtendedOpcode};

    handlers[0xB8] = {.name = "Res7B", .handler = UnimplementedExtendedOpcode};
    handlers[0xB9] = {.name = "Res7C", .handler = UnimplementedExtendedOpcode};
    handlers[0xBA] = {.name = "Res7D", .handler = UnimplementedExtendedOpcode};
    handlers[0xBB] = {.name = "Res7E", .handler = UnimplementedExtendedOpcode};
    handlers[0xBC] = {.name = "Res7H", .handler = UnimplementedExtendedOpcode};
    handlers[0xBD] = {.name = "Res7L", .handler = UnimplementedExtendedOpcode};
    handlers[0xBE] = {.name = "Res7HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xBF] = {.name = "Res7A", .handler = UnimplementedExtendedOpcode};

    handlers[0xC0] = {.name = "Set0B", .handler = UnimplementedExtendedOpcode};
    handlers[0xC1] = {.name = "Set0C", .handler = UnimplementedExtendedOpcode};
    handlers[0xC2] = {.name = "Set0D", .handler = UnimplementedExtendedOpcode};
    handlers[0xC3] = {.name = "Set0E", .handler = UnimplementedExtendedOpcode};
    handlers[0xC4] = {.name = "Set0H", .handler = UnimplementedExtendedOpcode};
    handlers[0xC5] = {.name = "Set0L", .handler = UnimplementedExtendedOpcode};
    handlers[0xC6] = {.name = "Set0HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xC7] = {.name = "Set0A", .handler = UnimplementedExtendedOpcode};

    handlers[0xC8] = {.name = "Set1B", .handler = UnimplementedExtendedOpcode};
    handlers[0xC9] = {.name = "Set1C", .handler = UnimplementedExtendedOpcode};
    handlers[0xCA] = {.name = "Set1D", .handler = UnimplementedExtendedOpcode};
    handlers[0xCB] = {.name = "Set1E", .handler = UnimplementedExtendedOpcode};
    handlers[0xCC] = {.name = "Set1H", .handler = UnimplementedExtendedOpcode};
    handlers[0xCD] = {.name = "Set1L", .handler = UnimplementedExtendedOpcode};
    handlers[0xCE] = {.name = "Set1HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xCF] = {.name = "Set1A", .handler = UnimplementedExtendedOpcode};

    handlers[0xD0] = {.name = "Set2B", .handler = UnimplementedExtendedOpcode};
    handlers[0xD1] = {.name = "Set2C", .handler = UnimplementedExtendedOpcode};
    handlers[0xD2] = {.name = "Set2D", .handler = UnimplementedExtendedOpcode};
    handlers[0xD3] = {.name = "Set2E", .handler = UnimplementedExtendedOpcode};
    handlers[0xD4] = {.name = "Set2H", .handler = UnimplementedExtendedOpcode};
    handlers[0xD5] = {.name = "Set2L", .handler = UnimplementedExtendedOpcode};
    handlers[0xD6] = {.name = "Set2HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xD7] = {.name = "Set2A", .handler = UnimplementedExtendedOpcode};

    handlers[0xD8] = {.name = "Set3B", .handler = UnimplementedExtendedOpcode};
    handlers[0xD9] = {.name = "Set3C", .handler = UnimplementedExtendedOpcode};
    handlers[0xDA] = {.name = "Set3D", .handler = UnimplementedExtendedOpcode};
    handlers[0xDB] = {.name = "Set3E", .handler = UnimplementedExtendedOpcode};
    handlers[0xDC] = {.name = "Set3H", .handler = UnimplementedExtendedOpcode};
    handlers[0xDD] = {.name = "Set3L", .handler = UnimplementedExtendedOpcode};
    handlers[0xDE] = {.name = "Set3HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xDF] = {.name = "Set3A", .handler = UnimplementedExtendedOpcode};

    handlers[0xE0] = {.name = "Set4B", .handler = UnimplementedExtendedOpcode};
    handlers[0xE1] = {.name = "Set4C", .handler = UnimplementedExtendedOpcode};
    handlers[0xE2] = {.name = "Set4D", .handler = UnimplementedExtendedOpcode};
    handlers[0xE3] = {.name = "Set4E", .handler = UnimplementedExtendedOpcode};
    handlers[0xE4] = {.name = "Set4H", .handler = UnimplementedExtendedOpcode};
    handlers[0xE5] = {.name = "Set4L", .handler = UnimplementedExtendedOpcode};
    handlers[0xE6] = {.name = "Set4HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xE7] = {.name = "Set4A", .handler = UnimplementedExtendedOpcode};

    handlers[0xE8] = {.name = "Set5B", .handler = UnimplementedExtendedOpcode};
    handlers[0xE9] = {.name = "Set5C", .handler = UnimplementedExtendedOpcode};
    handlers[0xEA] = {.name = "Set5D", .handler = UnimplementedExtendedOpcode};
    handlers[0xEB] = {.name = "Set5E", .handler = UnimplementedExtendedOpcode};
    handlers[0xEC] = {.name = "Set5H", .handler = UnimplementedExtendedOpcode};
    handlers[0xED] = {.name = "Set5L", .handler = UnimplementedExtendedOpcode};
    handlers[0xEE] = {.name = "Set5HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xEF] = {.name = "Set5A", .handler = UnimplementedExtendedOpcode};

    handlers[0xF0] = {.name = "Set6B", .handler = UnimplementedExtendedOpcode};
    handlers[0xF1] = {.name = "Set6C", .handler = UnimplementedExtendedOpcode};
    handlers[0xF2] = {.name = "Set6D", .handler = UnimplementedExtendedOpcode};
    handlers[0xF3] = {.name = "Set6E", .handler = UnimplementedExtendedOpcode};
    handlers[0xF4] = {.name = "Set6H", .handler = UnimplementedExtendedOpcode};
    handlers[0xF5] = {.name = "Set6L", .handler = UnimplementedExtendedOpcode};
    handlers[0xF6] = {.name = "Set6HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xF7] = {.name = "Set6A", .handler = UnimplementedExtendedOpcode};

    handlers[0xF8] = {.name = "Set7B", .handler = UnimplementedExtendedOpcode};
    handlers[0xF9] = {.name = "Set7C", .handler = UnimplementedExtendedOpcode};
    handlers[0xFA] = {.name = "Set7D", .handler = UnimplementedExtendedOpcode};
    handlers[0xFB] = {.name = "Set7E", .handler = UnimplementedExtendedOpcode};
    handlers[0xFC] = {.name = "Set7H", .handler = UnimplementedExtendedOpcode};
    handlers[0xFD] = {.name = "Set7L", .handler = UnimplementedExtendedOpcode};
    handlers[0xFE] = {.name = "Set7HL", .handler = UnimplementedExtendedOpcode};
    handlers[0xFF] = {.name = "Set7A", .handler = UnimplementedExtendedOpcode};

    return handlers;
}


} // namespace EmulatorLib
