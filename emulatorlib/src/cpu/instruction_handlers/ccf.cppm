/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module emulatorlib.cpu:ccf;

import std;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

inline auto ExecuteCcf(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    cpuRegisters.flags &= (CpuFlags::Carry | CpuFlags::Zero).AsByte();
    cpuRegisters.flags ^= CpuFlags::Carry.AsByte();
    co_return;
}

} // namespace EmulatorLib
