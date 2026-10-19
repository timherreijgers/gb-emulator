/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:reti;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

inline auto ExecuteRetI(const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    cpuRegisters.programCounter = cpuRegisters.wzRegister.value;
    cpuRegisters.masterInterruptState = MasterInterruptState::ENABLED;
    co_yield std::monostate{};

    co_return;
}

} // namespace EmulatorLib
