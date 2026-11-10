/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "utilitylib/bit_mask.h"
#include "utilitylib/sub_with_borrow.h"

namespace EmulatorLib
{

[[nodiscard]] inline auto ExecuteDecIndirectHL(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept
    -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto [result, borrow] = UtilityLib::SubWithBorrow(cpuRegisters.zRegister.value, 0x01_b);
    cpuRegisters.flags.value &= CpuFlags::Carry.AsByte();
    cpuRegisters.flags.value |= CpuFlags::Subtract.AsByte();
    cpuRegisters.flags.value |= result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b;
    cpuRegisters.flags.value |= (borrow & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b;
    addressBus.WriteToAddress(cpuRegisters.hlRegister, result);
    co_return;
}

} // namespace EmulatorLib
