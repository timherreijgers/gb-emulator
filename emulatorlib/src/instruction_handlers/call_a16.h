/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"
#include "instruction_handlers/flags_based_predicates.h"

namespace EmulatorLib
{

template <CpuRegisterPredicate Predicate>
constexpr auto ExecuteCall = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    if (!Predicate{}(cpuRegisters))
    {
        co_return;
    }

    cpuRegisters.stackPointer--;
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.stackPointer, static_cast<std::byte>(cpuRegisters.programCounter.value >> 8));
    cpuRegisters.stackPointer--;
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.stackPointer, static_cast<std::byte>(cpuRegisters.programCounter.value));
    cpuRegisters.programCounter = cpuRegisters.wzRegister.value;
    co_yield std::monostate{};

    co_return;
};

constexpr auto ExecuteCallA16 = ExecuteCall<decltype(AlwaysTrue)>;
constexpr auto ExecuteCallNzA16 = ExecuteCall<decltype(NonZero)>;
constexpr auto ExecuteCallNcA16 = ExecuteCall<decltype(NonCarry)>;
constexpr auto ExecuteCallZA16 = ExecuteCall<decltype(Zero)>;
constexpr auto ExecuteCallCA16 = ExecuteCall<decltype(Carry)>;

} // namespace EmulatorLib
