/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/cpu_registers.h"

namespace EmulatorLib
{

using RegisterA = decltype([](CpuRegisters& registers) -> Register8Bit& {
    return registers.accumulator;
});

using RegisterB = decltype([](CpuRegisters& registers) -> Register8BitRef& {
    return registers.bRegister;
});

using RegisterC = decltype([](CpuRegisters& registers) -> Register8BitRef& {
    return registers.cRegister;
});

using RegisterD = decltype([](CpuRegisters& registers) -> Register8BitRef& {
    return registers.dRegister;
});

using RegisterE = decltype([](CpuRegisters& registers) -> Register8BitRef& {
    return registers.eRegister;
});

using RegisterH = decltype([](CpuRegisters& registers) -> Register8BitRef& {
    return registers.hRegister;
});

using RegisterL = decltype([](CpuRegisters& registers) -> Register8BitRef& {
    return registers.lRegister;
});

} // namespace EmulatorLib