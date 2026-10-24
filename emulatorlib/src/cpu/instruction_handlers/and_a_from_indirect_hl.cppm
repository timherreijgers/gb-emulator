/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <variant>

module emulatorlib.cpu:and_a_from_indirect_hl;

import emulatorlib.cpu;
import emulatorlib.address_bus;

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
