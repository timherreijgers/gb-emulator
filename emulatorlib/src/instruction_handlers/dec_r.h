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
#include "utilitylib/add_with_carry.h"
#include "utilitylib/bit_mask.h"
#include "utilitylib/sub_with_borrow.h"

namespace EmulatorLib
{

template <ReturnsRegister8Bit TargetRegister>
constexpr auto ExecuteDecR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto [result, borrow] = UtilityLib::SubWithBorrow(targetRegister.value, 0x01_b);
    targetRegister = result;

    cpuRegisters.flags.value &= CpuFlags::Carry.AsByte();
    cpuRegisters.flags.value |= CpuFlags::Subtract.AsByte();
    cpuRegisters.flags.value |= result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b;
    cpuRegisters.flags.value |= (borrow & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b;

    co_return;
};

constexpr auto ExecuteDecB = ExecuteDecR<RegisterB>;
constexpr auto ExecuteDecC = ExecuteDecR<RegisterC>;
constexpr auto ExecuteDecD = ExecuteDecR<RegisterD>;
constexpr auto ExecuteDecE = ExecuteDecR<RegisterE>;
constexpr auto ExecuteDecH = ExecuteDecR<RegisterH>;
constexpr auto ExecuteDecL = ExecuteDecR<RegisterL>;
constexpr auto ExecuteDecA = ExecuteDecR<RegisterA>;

} // namespace EmulatorLib