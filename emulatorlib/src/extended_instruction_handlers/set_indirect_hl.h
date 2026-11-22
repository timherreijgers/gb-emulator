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

template <uint8_t Bit>
    requires UtilityLib::ValidBitMask<Bit>
constexpr auto ExecuteSetIndirectHL = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    cpuRegisters.zRegister |= UtilityLib::BitMask<Bit>;
    addressBus.WriteToAddress(cpuRegisters.hlRegister, cpuRegisters.zRegister);
    co_yield std::monostate{};

    co_return;
};

constexpr auto ExecuteSet0IndirectHL = ExecuteSetIndirectHL<0>;
constexpr auto ExecuteSet1IndirectHL = ExecuteSetIndirectHL<1>;
constexpr auto ExecuteSet2IndirectHL = ExecuteSetIndirectHL<2>;
constexpr auto ExecuteSet3IndirectHL = ExecuteSetIndirectHL<3>;
constexpr auto ExecuteSet4IndirectHL = ExecuteSetIndirectHL<4>;
constexpr auto ExecuteSet5IndirectHL = ExecuteSetIndirectHL<5>;
constexpr auto ExecuteSet6IndirectHL = ExecuteSetIndirectHL<6>;
constexpr auto ExecuteSet7IndirectHL = ExecuteSetIndirectHL<7>;

} // namespace EmulatorLib
