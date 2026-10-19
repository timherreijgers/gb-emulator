/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <concepts>
#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:jump_hl;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

[[nodiscard]] inline auto ExecuteJumpHl(AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.programCounter = cpuRegisters.hlRegister.value;
    co_return;
}

} // namespace EmulatorLib
