/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:inc_rr;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

template <ReturnsRegister16Bit TargetRegister>
constexpr auto ExecuteIncRR = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    TargetRegister{}(cpuRegisters) += 1;
    co_yield std::monostate{};

    co_return;
};

constexpr auto ExecuteIncBC = ExecuteIncRR<RegisterBC>;
constexpr auto ExecuteIncDE = ExecuteIncRR<RegisterDE>;
constexpr auto ExecuteIncHL = ExecuteIncRR<RegisterHL>;
constexpr auto ExecuteIncSP = ExecuteIncRR<RegisterSP>;

} // namespace EmulatorLib
