/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/flag_helpers.h"

namespace EmulatorLib
{

constexpr auto ExecuteAndAFromIndirectHL = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    const auto result = static_cast<std::byte>(static_cast<uint8_t>(cpuRegisters.accumulator.value) & static_cast<uint8_t>(cpuRegisters.zRegister.value));
    cpuRegisters.accumulator = result;

    ApplyAndFlags(cpuRegisters, result, 0x00_b);
    co_return;
};

} // namespace EmulatorLib
