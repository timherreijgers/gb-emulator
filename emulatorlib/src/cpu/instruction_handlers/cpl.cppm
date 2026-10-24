/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:cpl;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

inline auto ExecuteCpl(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    cpuRegisters.accumulator = ~cpuRegisters.accumulator;
    cpuRegisters.flags |= (CpuFlags::Subtract | CpuFlags::HalfCarry).AsByte();
    co_return;
}

} // namespace EmulatorLib
