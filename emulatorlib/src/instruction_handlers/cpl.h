/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

namespace EmulatorLib
{

inline auto ExecuteCpl(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    cpuRegisters.accumulator = ~cpuRegisters.accumulator;
    cpuRegisters.flags = (CpuFlags::Subtract | CpuFlags::HalfCarry).AsByte();
    co_return;
}

} // namespace EmulatorLib
