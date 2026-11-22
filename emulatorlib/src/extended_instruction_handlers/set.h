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
#include "utilitylib/bit_mask.h"

namespace EmulatorLib
{

template <ReturnsRegister8Bit TargetRegister, uint8_t Bit>
    requires UtilityLib::ValidBitMask<Bit>
constexpr auto ExecuteSet = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    TargetRegister{}(cpuRegisters) |= UtilityLib::BitMask<Bit>;

    co_return;
};

constexpr auto ExecuteSet0B = ExecuteSet<RegisterB, 0>;
constexpr auto ExecuteSet0C = ExecuteSet<RegisterC, 0>;
constexpr auto ExecuteSet0D = ExecuteSet<RegisterD, 0>;
constexpr auto ExecuteSet0E = ExecuteSet<RegisterE, 0>;
constexpr auto ExecuteSet0H = ExecuteSet<RegisterH, 0>;
constexpr auto ExecuteSet0L = ExecuteSet<RegisterL, 0>;
constexpr auto ExecuteSet0A = ExecuteSet<RegisterA, 0>;

constexpr auto ExecuteSet1B = ExecuteSet<RegisterB, 1>;
constexpr auto ExecuteSet1C = ExecuteSet<RegisterC, 1>;
constexpr auto ExecuteSet1D = ExecuteSet<RegisterD, 1>;
constexpr auto ExecuteSet1E = ExecuteSet<RegisterE, 1>;
constexpr auto ExecuteSet1H = ExecuteSet<RegisterH, 1>;
constexpr auto ExecuteSet1L = ExecuteSet<RegisterL, 1>;
constexpr auto ExecuteSet1A = ExecuteSet<RegisterA, 1>;

constexpr auto ExecuteSet2B = ExecuteSet<RegisterB, 2>;
constexpr auto ExecuteSet2C = ExecuteSet<RegisterC, 2>;
constexpr auto ExecuteSet2D = ExecuteSet<RegisterD, 2>;
constexpr auto ExecuteSet2E = ExecuteSet<RegisterE, 2>;
constexpr auto ExecuteSet2H = ExecuteSet<RegisterH, 2>;
constexpr auto ExecuteSet2L = ExecuteSet<RegisterL, 2>;
constexpr auto ExecuteSet2A = ExecuteSet<RegisterA, 2>;

constexpr auto ExecuteSet3B = ExecuteSet<RegisterB, 3>;
constexpr auto ExecuteSet3C = ExecuteSet<RegisterC, 3>;
constexpr auto ExecuteSet3D = ExecuteSet<RegisterD, 3>;
constexpr auto ExecuteSet3E = ExecuteSet<RegisterE, 3>;
constexpr auto ExecuteSet3H = ExecuteSet<RegisterH, 3>;
constexpr auto ExecuteSet3L = ExecuteSet<RegisterL, 3>;
constexpr auto ExecuteSet3A = ExecuteSet<RegisterA, 3>;

constexpr auto ExecuteSet4B = ExecuteSet<RegisterB, 4>;
constexpr auto ExecuteSet4C = ExecuteSet<RegisterC, 4>;
constexpr auto ExecuteSet4D = ExecuteSet<RegisterD, 4>;
constexpr auto ExecuteSet4E = ExecuteSet<RegisterE, 4>;
constexpr auto ExecuteSet4H = ExecuteSet<RegisterH, 4>;
constexpr auto ExecuteSet4L = ExecuteSet<RegisterL, 4>;
constexpr auto ExecuteSet4A = ExecuteSet<RegisterA, 4>;

constexpr auto ExecuteSet5B = ExecuteSet<RegisterB, 5>;
constexpr auto ExecuteSet5C = ExecuteSet<RegisterC, 5>;
constexpr auto ExecuteSet5D = ExecuteSet<RegisterD, 5>;
constexpr auto ExecuteSet5E = ExecuteSet<RegisterE, 5>;
constexpr auto ExecuteSet5H = ExecuteSet<RegisterH, 5>;
constexpr auto ExecuteSet5L = ExecuteSet<RegisterL, 5>;
constexpr auto ExecuteSet5A = ExecuteSet<RegisterA, 5>;

constexpr auto ExecuteSet6B = ExecuteSet<RegisterB, 6>;
constexpr auto ExecuteSet6C = ExecuteSet<RegisterC, 6>;
constexpr auto ExecuteSet6D = ExecuteSet<RegisterD, 6>;
constexpr auto ExecuteSet6E = ExecuteSet<RegisterE, 6>;
constexpr auto ExecuteSet6H = ExecuteSet<RegisterH, 6>;
constexpr auto ExecuteSet6L = ExecuteSet<RegisterL, 6>;
constexpr auto ExecuteSet6A = ExecuteSet<RegisterA, 6>;

constexpr auto ExecuteSet7B = ExecuteSet<RegisterB, 7>;
constexpr auto ExecuteSet7C = ExecuteSet<RegisterC, 7>;
constexpr auto ExecuteSet7D = ExecuteSet<RegisterD, 7>;
constexpr auto ExecuteSet7E = ExecuteSet<RegisterE, 7>;
constexpr auto ExecuteSet7H = ExecuteSet<RegisterH, 7>;
constexpr auto ExecuteSet7L = ExecuteSet<RegisterL, 7>;
constexpr auto ExecuteSet7A = ExecuteSet<RegisterA, 7>;

} // namespace EmulatorLib
