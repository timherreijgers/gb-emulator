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

namespace EmulatorLib
{

template <ReturnsRegister8Bit TargetRegister>
constexpr auto ExecuteRrc = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto newBit7 = static_cast<bool>(targetRegister & 0x01_b) ? 0x80_b : 0x00_b;
    targetRegister = (targetRegister >> 1) | newBit7;
    cpuRegisters.flags = static_cast<bool>(newBit7) ? CpuFlags::Carry.AsByte() : 0x00_b;

    co_return;
};

constexpr auto ExecuteRrcA = ExecuteRrc<RegisterA>;
constexpr auto ExecuteRrcB = ExecuteRrc<RegisterB>;
constexpr auto ExecuteRrcC = ExecuteRrc<RegisterC>;
constexpr auto ExecuteRrcD = ExecuteRrc<RegisterD>;
constexpr auto ExecuteRrcE = ExecuteRrc<RegisterE>;
constexpr auto ExecuteRrcH = ExecuteRrc<RegisterH>;
constexpr auto ExecuteRrcL = ExecuteRrc<RegisterL>;

inline auto ExecuteRrcIndirectHL(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto newBit7 = static_cast<bool>(cpuRegisters.zRegister & 0x01_b) ? 0x80_b : 0x00_b;
    cpuRegisters.zRegister = (cpuRegisters.zRegister >> 1) | newBit7;
    cpuRegisters.flags = static_cast<bool>(newBit7) ? CpuFlags::Carry.AsByte() : 0x00_b;
    addressBus.WriteToAddress(cpuRegisters.hlRegister, cpuRegisters.zRegister);

    co_return;
}

} // namespace EmulatorLib
