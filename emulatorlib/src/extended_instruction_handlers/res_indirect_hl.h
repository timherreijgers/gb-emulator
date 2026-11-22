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
constexpr auto ExecuteResIndirectHL = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    cpuRegisters.zRegister &= ~UtilityLib::BitMask<Bit>;
    addressBus.WriteToAddress(cpuRegisters.hlRegister, cpuRegisters.zRegister);
    co_yield std::monostate{};

    co_return;
};

constexpr auto ExecuteRes0IndirectHL = ExecuteResIndirectHL<0>;
constexpr auto ExecuteRes1IndirectHL = ExecuteResIndirectHL<1>;
constexpr auto ExecuteRes2IndirectHL = ExecuteResIndirectHL<2>;
constexpr auto ExecuteRes3IndirectHL = ExecuteResIndirectHL<3>;
constexpr auto ExecuteRes4IndirectHL = ExecuteResIndirectHL<4>;
constexpr auto ExecuteRes5IndirectHL = ExecuteResIndirectHL<5>;
constexpr auto ExecuteRes6IndirectHL = ExecuteResIndirectHL<6>;
constexpr auto ExecuteRes7IndirectHL = ExecuteResIndirectHL<7>;

} // namespace EmulatorLib
