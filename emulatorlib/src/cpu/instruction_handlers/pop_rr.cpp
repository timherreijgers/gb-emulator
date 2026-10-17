/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:pop_rr;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

template <ReturnsRegister16Bit WriteLocation>
constexpr auto ExecutePopRr = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.stackPointer++);
    co_yield std::monostate{};

    WriteLocation{}(cpuRegisters) = cpuRegisters.wzRegister.value;
    co_return;
};

constexpr auto ExecutePopBC = ExecutePopRr<RegisterBC>;
constexpr auto ExecutePopDE = ExecutePopRr<RegisterDE>;
constexpr auto ExecutePopHL = ExecutePopRr<RegisterHL>;
constexpr auto ExecutePopAF = ExecutePopRr<RegisterAF>;

} // namespace EmulatorLib
