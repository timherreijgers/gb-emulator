/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/register_concepts.h"
#include "instruction_handlers/register_io_helpers.h"

namespace EmulatorLib
{

template <ReturnsRegister16Bit ReadLocation, uint16_t ValueMask = 0xFFFF>
constexpr auto ExecutePushRr = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    const auto& registerSource = ReadLocation{}(cpuRegisters);

    cpuRegisters.stackPointer--;
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.stackPointer--, static_cast<std::byte>(registerSource.value >> 8) & static_cast<std::byte>(ValueMask >> 8));
    co_yield std::monostate{};

    addressBus.WriteToAddress(cpuRegisters.stackPointer, static_cast<std::byte>(registerSource.value & 0xFF) & static_cast<std::byte>(ValueMask));
    co_yield std::monostate{};

    // We need this co_return here to make sure the execution of the opcode takes 4 M-cycles. We cannot read the next instruction at the same
    // time we write something on the data bus
    co_return;
};

constexpr auto ExecutePushBC = ExecutePushRr<RegisterBC>;
constexpr auto ExecutePushDE = ExecutePushRr<RegisterDE>;
constexpr auto ExecutePushHL = ExecutePushRr<RegisterHL>;
constexpr auto ExecutePushAF = ExecutePushRr<RegisterAF, 0xFFF0>;

} // namespace EmulatorLib
