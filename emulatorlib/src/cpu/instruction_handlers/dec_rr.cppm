/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module emulatorlib.cpu:dec_rr;

import std;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

template <ReturnsRegister16Bit TargetRegister>
constexpr auto ExecuteDecRR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    TargetRegister{}(cpuRegisters) -= 1;
    co_yield std::monostate{};

    co_return;
};

constexpr auto ExecuteDecBC = ExecuteDecRR<RegisterBC>;
constexpr auto ExecuteDecDE = ExecuteDecRR<RegisterDE>;
constexpr auto ExecuteDecHL = ExecuteDecRR<RegisterHL>;
constexpr auto ExecuteDecSP = ExecuteDecRR<RegisterSP>;

} // namespace EmulatorLib
