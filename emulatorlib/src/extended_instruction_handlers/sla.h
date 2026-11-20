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
constexpr auto ExecuteSla = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    auto& targetRegister = TargetRegister{}(cpuRegisters);
    const auto newCarry = static_cast<bool>(targetRegister & 0x80_b);
    const auto result = targetRegister << 1;
    targetRegister = result;
    cpuRegisters.flags = (static_cast<bool>(result) ? 0x00_b : CpuFlags::Zero.AsByte()) | (newCarry ? CpuFlags::Carry.AsByte() : 0x00_b);

    co_return;
};

constexpr auto ExecuteSlaA = ExecuteSla<RegisterA>;
constexpr auto ExecuteSlaB = ExecuteSla<RegisterB>;
constexpr auto ExecuteSlaC = ExecuteSla<RegisterC>;
constexpr auto ExecuteSlaD = ExecuteSla<RegisterD>;
constexpr auto ExecuteSlaE = ExecuteSla<RegisterE>;
constexpr auto ExecuteSlaH = ExecuteSla<RegisterH>;
constexpr auto ExecuteSlaL = ExecuteSla<RegisterL>;

inline auto ExecuteSlaIndirectHL(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto newCarry = static_cast<bool>(cpuRegisters.zRegister & 0x80_b);
    const auto result = cpuRegisters.zRegister << 1;
    cpuRegisters.zRegister = result;
    cpuRegisters.flags = (static_cast<bool>(result) ? 0x00_b : CpuFlags::Zero.AsByte()) | (newCarry ? CpuFlags::Carry.AsByte() : 0x00_b);
    addressBus.WriteToAddress(cpuRegisters.hlRegister, cpuRegisters.zRegister);

    co_return;
}

} // namespace EmulatorLib
