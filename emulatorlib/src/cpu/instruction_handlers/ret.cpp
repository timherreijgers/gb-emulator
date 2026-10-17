/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:ret;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

inline auto ExecuteRet(const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    cpuRegisters.programCounter = cpuRegisters.wzRegister.value;
    co_yield std::monostate{};

    co_return;
}

} // namespace EmulatorLib
