/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include <concepts>

namespace EmulatorLib
{

[[nodiscard]] inline auto ExecuteJumpHl(AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    cpuRegisters.programCounter = cpuRegisters.hlRegister.value;
    co_return;
}

} // namespace EmulatorLib