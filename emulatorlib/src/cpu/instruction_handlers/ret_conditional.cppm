/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module emulatorlib.cpu:ret_conditional;

import std;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

template <CpuRegisterPredicate Predicate>
constexpr auto ExecuteRetConditional = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    if (!Predicate{}(cpuRegisters))
    {
        co_yield std::monostate{};
        co_return;
    }

    co_yield std::monostate{};

    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    cpuRegisters.programCounter = cpuRegisters.wzRegister.value;
    co_yield std::monostate{};

    co_return;
};

constexpr auto ExecuteRetNz = ExecuteRetConditional<decltype(NonZero)>;
constexpr auto ExecuteRetNc = ExecuteRetConditional<decltype(NonCarry)>;
constexpr auto ExecuteRetZ = ExecuteRetConditional<decltype(Zero)>;
constexpr auto ExecuteRetC = ExecuteRetConditional<decltype(Carry)>;

} // namespace EmulatorLib
