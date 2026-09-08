/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/register.h"

#include <functional>

namespace EmulatorLib::Test
{

template <typename T>
[[nodiscard]] auto FunctionWrapper8Bit() -> std::function<Register8Bit&(CpuRegisters&)>
{
    return T{};
}

template <typename T>
[[nodiscard]] auto FunctionWrapper16Bit() -> std::function<Register16Bit&(CpuRegisters&)>
{
    return T{};
}

} // namespace EmulatorLib::Test