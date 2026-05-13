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

using RegisterBC = decltype([](CpuRegisters& registers) -> Register16Bit& {
    return registers.bcRegister;
});

using RegisterDE = decltype([](CpuRegisters& registers) -> Register16Bit& {
    return registers.deRegister;
});

using RegisterHL = decltype([](CpuRegisters& registers) -> Register16Bit& {
    return registers.hlRegister;
});

using RegisterSP = decltype([](CpuRegisters& registers) -> Register16Bit& {
    return registers.stackPointer;
});

using Register16BitIncrement = decltype([](Register16Bit& reg) {
    reg.value++;
});

using Register16BitDecrement = decltype([](Register16Bit& reg) {
    reg.value--;
});

using RegisterNoModifier = decltype([](Register16Bit& /*reg*/) {});

} // namespace EmulatorLib