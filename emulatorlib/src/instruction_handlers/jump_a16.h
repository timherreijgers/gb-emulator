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

template <CpuRegisterPredicate Predicate>
constexpr auto ExecuteJumpRelative = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    if (!Predicate{}(cpuRegisters))
    {
        co_return;
    }

    const auto zSign = static_cast<bool>(cpuRegisters.zRegister.value & UtilityLib::BitMask<7>);
    const auto [result, carryPerBit] =
        UtilityLib::AddWithCarry(cpuRegisters.zRegister.value,
                                 static_cast<std::byte>(0xFF & cpuRegisters.programCounter.value));
    cpuRegisters.zRegister = result;

    const auto adj = [&] noexcept {
        if ((carryPerBit & UtilityLib::BitMask<7>) > 0x00_b && !zSign)
            return 0x01_b;

        if ((carryPerBit & UtilityLib::BitMask<7>) == 0x00_b && zSign)
            return static_cast<std::byte>(-1);

        return 0x00_b;
    }();

    cpuRegisters.wRegister = static_cast<std::byte>(0xFF & (cpuRegisters.programCounter.value >> 8)) + adj;
    co_yield std::monostate{};

    cpuRegisters.programCounter = cpuRegisters.wzRegister.value;
    co_return;
};

constexpr auto ExecuteJumpRelativeE8 = ExecuteJumpRelative<decltype(AlwaysTrue)>;
constexpr auto ExecuteJumpRelativeNzE8 = ExecuteJumpRelative<decltype(NonZero)>;
constexpr auto ExecuteJumpRelativeNcE8 = ExecuteJumpRelative<decltype(NonCarry)>;
constexpr auto ExecuteJumpRelativeZE8 = ExecuteJumpRelative<decltype(Zero)>;
constexpr auto ExecuteJumpRelativeCE8 = ExecuteJumpRelative<decltype(Carry)>;

} // namespace EmulatorLib
