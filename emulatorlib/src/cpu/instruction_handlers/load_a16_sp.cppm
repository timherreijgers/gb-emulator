/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:load_a16_sp;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

[[nodiscard]] inline auto ExecuteLoadA16SP(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.wzRegister.value, static_cast<std::byte>(cpuRegisters.stackPointer.value & 0xFF));
    cpuRegisters.wzRegister++;
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.wzRegister.value, static_cast<std::byte>(cpuRegisters.stackPointer.value >> 8));
    co_yield std::monostate{};

    // We need this co_return here to make sure the execution of the opcode takes 5 M-cycles. We cannot read the next instruction at the same
    // time we write something on the data bus.
    co_return;
};

} // namespace EmulatorLib
