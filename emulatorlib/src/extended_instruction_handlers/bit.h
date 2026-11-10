/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/register_concepts.h"
#include "instruction_handlers/register_io_helpers.h"
#include "utilitylib/bit_mask.h"

namespace EmulatorLib
{

template <ReturnsRegister8Bit SourceRegister, uint8_t Bit>
    requires UtilityLib::ValidBitMask<Bit>
constexpr auto ExecuteBit = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.flags &= ~(CpuFlags::Zero | CpuFlags::Subtract).AsByte();
    cpuRegisters.flags |= (CpuFlags::HalfCarry).AsByte();
    cpuRegisters.flags |= static_cast<bool>(SourceRegister{}(cpuRegisters)&UtilityLib::BitMask<Bit>) ? 0x00_b : CpuFlags::Zero.AsByte();

    co_return;
};

constexpr auto ExecuteBit0A = ExecuteBit<RegisterA, 0>;
constexpr auto ExecuteBit0B = ExecuteBit<RegisterB, 0>;
constexpr auto ExecuteBit0C = ExecuteBit<RegisterC, 0>;
constexpr auto ExecuteBit0D = ExecuteBit<RegisterD, 0>;
constexpr auto ExecuteBit0E = ExecuteBit<RegisterE, 0>;
constexpr auto ExecuteBit0H = ExecuteBit<RegisterH, 0>;
constexpr auto ExecuteBit0L = ExecuteBit<RegisterL, 0>;

constexpr auto ExecuteBit1A = ExecuteBit<RegisterA, 1>;
constexpr auto ExecuteBit1B = ExecuteBit<RegisterB, 1>;
constexpr auto ExecuteBit1C = ExecuteBit<RegisterC, 1>;
constexpr auto ExecuteBit1D = ExecuteBit<RegisterD, 1>;
constexpr auto ExecuteBit1E = ExecuteBit<RegisterE, 1>;
constexpr auto ExecuteBit1H = ExecuteBit<RegisterH, 1>;
constexpr auto ExecuteBit1L = ExecuteBit<RegisterL, 1>;

constexpr auto ExecuteBit2A = ExecuteBit<RegisterA, 2>;
constexpr auto ExecuteBit2B = ExecuteBit<RegisterB, 2>;
constexpr auto ExecuteBit2C = ExecuteBit<RegisterC, 2>;
constexpr auto ExecuteBit2D = ExecuteBit<RegisterD, 2>;
constexpr auto ExecuteBit2E = ExecuteBit<RegisterE, 2>;
constexpr auto ExecuteBit2H = ExecuteBit<RegisterH, 2>;
constexpr auto ExecuteBit2L = ExecuteBit<RegisterL, 2>;

constexpr auto ExecuteBit3A = ExecuteBit<RegisterA, 3>;
constexpr auto ExecuteBit3B = ExecuteBit<RegisterB, 3>;
constexpr auto ExecuteBit3C = ExecuteBit<RegisterC, 3>;
constexpr auto ExecuteBit3D = ExecuteBit<RegisterD, 3>;
constexpr auto ExecuteBit3E = ExecuteBit<RegisterE, 3>;
constexpr auto ExecuteBit3H = ExecuteBit<RegisterH, 3>;
constexpr auto ExecuteBit3L = ExecuteBit<RegisterL, 3>;

constexpr auto ExecuteBit4A = ExecuteBit<RegisterA, 4>;
constexpr auto ExecuteBit4B = ExecuteBit<RegisterB, 4>;
constexpr auto ExecuteBit4C = ExecuteBit<RegisterC, 4>;
constexpr auto ExecuteBit4D = ExecuteBit<RegisterD, 4>;
constexpr auto ExecuteBit4E = ExecuteBit<RegisterE, 4>;
constexpr auto ExecuteBit4H = ExecuteBit<RegisterH, 4>;
constexpr auto ExecuteBit4L = ExecuteBit<RegisterL, 4>;

constexpr auto ExecuteBit5A = ExecuteBit<RegisterA, 5>;
constexpr auto ExecuteBit5B = ExecuteBit<RegisterB, 5>;
constexpr auto ExecuteBit5C = ExecuteBit<RegisterC, 5>;
constexpr auto ExecuteBit5D = ExecuteBit<RegisterD, 5>;
constexpr auto ExecuteBit5E = ExecuteBit<RegisterE, 5>;
constexpr auto ExecuteBit5H = ExecuteBit<RegisterH, 5>;
constexpr auto ExecuteBit5L = ExecuteBit<RegisterL, 5>;

constexpr auto ExecuteBit6A = ExecuteBit<RegisterA, 6>;
constexpr auto ExecuteBit6B = ExecuteBit<RegisterB, 6>;
constexpr auto ExecuteBit6C = ExecuteBit<RegisterC, 6>;
constexpr auto ExecuteBit6D = ExecuteBit<RegisterD, 6>;
constexpr auto ExecuteBit6E = ExecuteBit<RegisterE, 6>;
constexpr auto ExecuteBit6H = ExecuteBit<RegisterH, 6>;
constexpr auto ExecuteBit6L = ExecuteBit<RegisterL, 6>;

constexpr auto ExecuteBit7A = ExecuteBit<RegisterA, 7>;
constexpr auto ExecuteBit7B = ExecuteBit<RegisterB, 7>;
constexpr auto ExecuteBit7C = ExecuteBit<RegisterC, 7>;
constexpr auto ExecuteBit7D = ExecuteBit<RegisterD, 7>;
constexpr auto ExecuteBit7E = ExecuteBit<RegisterE, 7>;
constexpr auto ExecuteBit7H = ExecuteBit<RegisterH, 7>;
constexpr auto ExecuteBit7L = ExecuteBit<RegisterL, 7>;

} // namespace EmulatorLib