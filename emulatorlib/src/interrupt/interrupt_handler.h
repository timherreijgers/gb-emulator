/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

namespace EmulatorLib
{
template <uint16_t HandlerAddress>
constexpr auto InterruptHandler = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.programCounter--;
    co_yield std::monostate{};

    cpuRegisters.stackPointer--;
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.stackPointer, static_cast<std::byte>(cpuRegisters.programCounter.value >> 8));
    cpuRegisters.stackPointer--;
    co_yield std::monostate{};

    // TODO: Add support for changing interrupt during handler execution
    addressBus.WriteToAddress(cpuRegisters.stackPointer, static_cast<std::byte>(cpuRegisters.programCounter.value));
    cpuRegisters.programCounter = HandlerAddress;
    co_yield std::monostate{};

    co_return;
};

constexpr auto VBlankInterruptHandler = InterruptHandler<0x40>;
constexpr auto LcdInterruptHandler = InterruptHandler<0x48>;
constexpr auto TimerInterruptHandler = InterruptHandler<0x50>;
constexpr auto SerialInterruptHandler = InterruptHandler<0x58>;
constexpr auto JoypadInterruptHandler = InterruptHandler<0x60>;

} // namespace EmulatorLib