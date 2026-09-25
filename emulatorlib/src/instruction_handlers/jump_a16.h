/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include <concepts>

namespace EmulatorLib
{

constexpr auto AlwaysTrue = [](const CpuRegisters&) -> bool {
    return true;
};

constexpr auto NonZero = [](const CpuRegisters& cpuRegisters) -> bool {
    return (cpuRegisters.flags.value & CpuFlags::Zero.AsByte()) == 0x00_b;
};

constexpr auto Zero = [](const CpuRegisters& cpuRegisters) -> bool {
    return (cpuRegisters.flags.value & CpuFlags::Zero.AsByte()) > 0x00_b;
};

constexpr auto NonCarry = [](const CpuRegisters& cpuRegisters) -> bool {
    return (cpuRegisters.flags.value & CpuFlags::Carry.AsByte()) == 0x00_b;
};

constexpr auto Carry = [](const CpuRegisters& cpuRegisters) -> bool {
    return (cpuRegisters.flags.value & CpuFlags::Carry.AsByte()) > 0x00_b;
};

template <typename T>
concept CpuRegisterPredicate = std::invocable<T, const CpuRegisters&> &&
                               std::same_as<std::invoke_result_t<T, const CpuRegisters&>, bool>;

template <CpuRegisterPredicate Predicate>
constexpr auto ExecuteJump = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    if (!Predicate{}(cpuRegisters))
    {
        co_return;
    }

    cpuRegisters.programCounter = cpuRegisters.wzRegister.value;
    co_yield std::monostate{};

    co_return;
};

constexpr auto ExecuteJumpA16 = ExecuteJump<decltype(AlwaysTrue)>;
constexpr auto ExecuteJumpNzA16 = ExecuteJump<decltype(NonZero)>;
constexpr auto ExecuteJumpNcA16 = ExecuteJump<decltype(NonCarry)>;
constexpr auto ExecuteJumpZA16 = ExecuteJump<decltype(Zero)>;
constexpr auto ExecuteJumpCA16 = ExecuteJump<decltype(Carry)>;

} // namespace EmulatorLib