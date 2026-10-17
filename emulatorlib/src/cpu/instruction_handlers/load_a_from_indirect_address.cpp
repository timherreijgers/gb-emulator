/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:load_a_from_indirect_address;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

[[nodiscard]] inline auto ExecuteLoadAFromIndirectA8(AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    cpuRegisters.zRegister = addressBus.ReadFromAddress(0xFF00 | static_cast<uint8_t>(cpuRegisters.zRegister.value));
    co_yield std::monostate{};

    cpuRegisters.accumulator = cpuRegisters.zRegister.value;
    co_return;
}

[[nodiscard]] inline auto ExecuteLoadAFromIndirectA16(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.wzRegister.value);
    co_yield std::monostate{};

    cpuRegisters.accumulator = cpuRegisters.zRegister.value;
    co_return;
}

} // namespace EmulatorLib
