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

template <uint8_t Bit>
    requires UtilityLib::ValidBitMask<Bit>
constexpr auto ExecuteBitIndirectHL = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    cpuRegisters.flags &= ~(CpuFlags::Zero | CpuFlags::Subtract).AsByte();
    cpuRegisters.flags |= CpuFlags::HalfCarry.AsByte();
    cpuRegisters.flags |= static_cast<bool>(cpuRegisters.zRegister & UtilityLib::BitMask<Bit>) ? 0x00_b : CpuFlags::Zero.AsByte();

    co_return;
};

constexpr auto ExecuteBit0IndirectHL = ExecuteBitIndirectHL<0>;
constexpr auto ExecuteBit1IndirectHL = ExecuteBitIndirectHL<1>;
constexpr auto ExecuteBit2IndirectHL = ExecuteBitIndirectHL<2>;
constexpr auto ExecuteBit3IndirectHL = ExecuteBitIndirectHL<3>;
constexpr auto ExecuteBit4IndirectHL = ExecuteBitIndirectHL<4>;
constexpr auto ExecuteBit5IndirectHL = ExecuteBitIndirectHL<5>;
constexpr auto ExecuteBit6IndirectHL = ExecuteBitIndirectHL<6>;
constexpr auto ExecuteBit7IndirectHL = ExecuteBitIndirectHL<7>;

} // namespace EmulatorLib
