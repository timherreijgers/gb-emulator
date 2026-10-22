/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module emulatorlib.cpu:load_indirect_c_from_a;

import std;

import emulatorlib.cpu;
import emulatorlib.address_bus;

using std::uint8_t;

namespace EmulatorLib
{

[[nodiscard]] inline auto ExecuteLoadIndirectCFromA(AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler
{
    addressBus.WriteToAddress(0xFF00 | static_cast<uint8_t>(cpuRegisters.cRegister.value), cpuRegisters.accumulator.value);
    co_yield std::monostate{};

    // We need this co_return here to make sure the execution of the opcode takes 3 M-cycles. We cannot read the next instruction at the same
    // time we write something on the data bus.
    co_return;
};


} // namespace EmulatorLib
