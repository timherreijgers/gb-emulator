/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <coroutine>
#include <cstddef>
#include <variant>

module emulatorlib.cpu:load_r_n8;

import emulatorlib.cpu;
import emulatorlib.address_bus;

namespace EmulatorLib
{

template <ReturnsRegister8Bit WriteLocation>
constexpr auto ExecuteLoadRegN8 = [](const AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    co_yield std::monostate{};

    WriteLocation{}(cpuRegisters) = cpuRegisters.zRegister.value;
    co_return;
};

constexpr auto ExecuteLoadAn8 = ExecuteLoadRegN8<RegisterA>;
constexpr auto ExecuteLoadBn8 = ExecuteLoadRegN8<RegisterB>;
constexpr auto ExecuteLoadCn8 = ExecuteLoadRegN8<RegisterC>;
constexpr auto ExecuteLoadDn8 = ExecuteLoadRegN8<RegisterD>;
constexpr auto ExecuteLoadEn8 = ExecuteLoadRegN8<RegisterE>;
constexpr auto ExecuteLoadHn8 = ExecuteLoadRegN8<RegisterH>;
constexpr auto ExecuteLoadLn8 = ExecuteLoadRegN8<RegisterL>;

} // namespace EmulatorLib
