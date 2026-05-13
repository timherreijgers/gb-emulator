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
concept ReturnsRegister8Bit = requires(T reg, CpuRegisters& registers) {
    { reg(registers) } -> std::same_as<Register8Bit&>;
};

template <typename T>
concept ReturnsRegister16Bit = requires(T reg, CpuRegisters& registers) {
    { reg(registers) } -> std::same_as<Register16Bit&>;
};

template <typename T>
concept RegisterModifier8Bit = requires(T t, Register8Bit reg) {
    { t(reg) } -> std::same_as<void>;
};

template <typename T>
concept RegisterModifier16Bit = requires(T t, Register16Bit reg) {
    { t(reg) } -> std::same_as<void>;
};

template <typename T>
concept RegisterModifier = RegisterModifier8Bit<T> || RegisterModifier16Bit<T>;

} // namespace EmulatorLib