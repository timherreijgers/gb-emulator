/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:enable_interrupts;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

inline auto ExecuteEi(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    if (cpuRegisters.masterInterruptState == MasterInterruptState::DISABLED)
    {
        cpuRegisters.masterInterruptState = MasterInterruptState::PENDING;
    }
    co_return;
}

} // namespace EmulatorLib
