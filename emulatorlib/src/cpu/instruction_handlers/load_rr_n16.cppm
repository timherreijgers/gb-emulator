/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:load_rr_n16;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

template <ReturnsRegister16Bit WriteLocation>
constexpr auto ExecuteLoadRegN16 = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    cpuRegisters.wRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    WriteLocation{}(cpuRegisters) = cpuRegisters.wzRegister.value;
    co_return;
};

constexpr auto ExecuteLoadBCn16 = ExecuteLoadRegN16<RegisterBC>;
constexpr auto ExecuteLoadDEn16 = ExecuteLoadRegN16<RegisterDE>;
constexpr auto ExecuteLoadHLn16 = ExecuteLoadRegN16<RegisterHL>;
constexpr auto ExecuteLoadSPn16 = ExecuteLoadRegN16<RegisterSP>;

} // namespace EmulatorLib
