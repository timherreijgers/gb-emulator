/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once
#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

namespace EmulatorLib
{

inline auto ExecuteDi(const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) -> InstructionHandler
{
    cpuRegisters.masterInterruptState = MasterInterruptState::DISABLED;
    co_return;
}

} // namespace EmulatorLib
