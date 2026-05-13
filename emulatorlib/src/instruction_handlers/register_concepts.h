/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/cpu_registers.h"

#include <concepts>

namespace EmulatorLib
{

template <typename T>
concept ReturnsRegister8BitNonRef = requires(T reg, CpuRegisters& registers) {
    { reg(registers) } -> std::same_as<Register8Bit&>;
};

template <typename T>
concept ReturnsRegister8BitRef = requires(T reg, CpuRegisters& registers) {
    { reg(registers) } -> std::same_as<Register8BitRef&>;
};

template <typename T>
concept ReturnsRegister16Bit = requires(T reg, CpuRegisters& registers) {
    { reg(registers) } -> std::same_as<Register16Bit&>;
};

template <typename T>
concept ReturnsRegister8Bit = ReturnsRegister8BitNonRef<T> || ReturnsRegister8BitRef<T>;

} // namespace EmulatorLib