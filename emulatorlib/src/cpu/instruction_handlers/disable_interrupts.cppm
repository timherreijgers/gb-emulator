/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:disable_interrupts;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

inline auto ExecuteDi(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    cpuRegisters.masterInterruptState = MasterInterruptState::DISABLED;
    co_return;
}

} // namespace EmulatorLib
