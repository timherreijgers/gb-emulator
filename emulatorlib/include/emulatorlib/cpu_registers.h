/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/register.h"

namespace EmulatorLib
{

struct CpuRegisters
{
    Register8Bit instructionRegister{};
    Register8Bit interruptEnableRegister{};
    Register8Bit accumulator{};
    Register8Bit flags{};

    Register16Bit bcRegister{};
    Register8BitRef bRegister = bcRegister.UpperByteAsRegister();
    Register8BitRef cRegister = bcRegister.LowerByteAsRegister();

    Register16Bit deRegister{};
    Register8BitRef dRegister = deRegister.UpperByteAsRegister();
    Register8BitRef eRegister = deRegister.LowerByteAsRegister();

    Register16Bit hlRegister{};
    Register8BitRef hRegister = hlRegister.UpperByteAsRegister();
    Register8BitRef lRegister = hlRegister.LowerByteAsRegister();

    Register16Bit wzRegister{};
    Register8BitRef wRegister = wzRegister.UpperByteAsRegister();
    Register8BitRef zRegister = wzRegister.LowerByteAsRegister();

    Register16Bit programCounter{};
    Register16Bit stackPointer{};
};

} // namespace EmulatorLib