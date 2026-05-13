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
    names[0x0E] = "LoadCn8";

    names[0x11] = "LoadDEn16";
    names[0x12] = "LoadIndirectDEFromA";
    names[0x16] = "LoadDn8";
    names[0x1E] = "LoadEn8";

    names[0x21] = "LoadHLn16";
    names[0x22] = "LoadIndirectHLIncrementFromA";
    names[0x26] = "LoadHn8";
    names[0x2E] = "LoadLn8";

    names[0x31] = "LoadSPn16";
    names[0x32] = "LoadIndirectHLDecrementFromA";
    names[0x3E] = "LoadAn8";

    names[0x77] = "LoadIndirectHLFromA";

    return names;
}

} // namespace

auto OpCodeToInstructionName(std::byte opcode) -> std::string_view
{
    static constexpr auto opCodeLut = CreateOpCodeLut();
    return opCodeLut[static_cast<size_t>(opcode)];
}

} // namespace EmulatorLib