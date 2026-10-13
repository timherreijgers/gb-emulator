#pragma once

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"

#include <concepts>

namespace EmulatorLib
{

constexpr auto AlwaysTrue = [](const CpuRegisters&) -> bool {
    return true;
};

constexpr auto NonZero = [](const CpuRegisters& cpuRegisters) -> bool {
    return (cpuRegisters.flags.value & CpuFlags::Zero.AsByte()) == 0x00_b;
};

constexpr auto Zero = [](const CpuRegisters& cpuRegisters) -> bool {
    return (cpuRegisters.flags.value & CpuFlags::Zero.AsByte()) > 0x00_b;
};

constexpr auto NonCarry = [](const CpuRegisters& cpuRegisters) -> bool {
    return (cpuRegisters.flags.value & CpuFlags::Carry.AsByte()) == 0x00_b;
};

constexpr auto Carry = [](const CpuRegisters& cpuRegisters) -> bool {
    return (cpuRegisters.flags.value & CpuFlags::Carry.AsByte()) > 0x00_b;
};

template <typename T>
concept CpuRegisterPredicate = std::invocable<T, const CpuRegisters&> &&
                               std::same_as<std::invoke_result_t<T, const CpuRegisters&>, bool>;

} // namespace EmulatorLib