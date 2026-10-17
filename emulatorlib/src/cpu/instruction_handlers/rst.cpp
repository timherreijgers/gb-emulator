/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <variant>

module emulatorlib.cpu:rst;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

template <uint16_t TargetAddress>
constexpr auto ExecuteRst = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.stackPointer--;
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.stackPointer, static_cast<std::byte>(cpuRegisters.programCounter.value >> 8));
    cpuRegisters.stackPointer--;
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.stackPointer, static_cast<std::byte>(cpuRegisters.programCounter.value));
    cpuRegisters.programCounter = TargetAddress;
    co_yield std::monostate{};

    co_return;
};

constexpr auto ExecuteRst00 = ExecuteRst<0x00>;
constexpr auto ExecuteRst08 = ExecuteRst<0x08>;
constexpr auto ExecuteRst10 = ExecuteRst<0x10>;
constexpr auto ExecuteRst18 = ExecuteRst<0x18>;
constexpr auto ExecuteRst20 = ExecuteRst<0x20>;
constexpr auto ExecuteRst28 = ExecuteRst<0x28>;
constexpr auto ExecuteRst30 = ExecuteRst<0x30>;
constexpr auto ExecuteRst38 = ExecuteRst<0x38>;

} // namespace EmulatorLib
