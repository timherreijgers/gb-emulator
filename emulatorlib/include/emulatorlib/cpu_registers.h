/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/register.h"
#include "utilitylib/integral_suffixes.h"

namespace EmulatorLib
{

struct CpuRegisters
{
    CpuRegisters() = default;
    ~CpuRegisters() = default;
    CpuRegisters(CpuRegisters&) = delete;
    CpuRegisters operator=(const CpuRegisters&) = delete;
    CpuRegisters(CpuRegisters&&) = delete;
    CpuRegisters& operator=(const CpuRegisters&&) noexcept = delete;

    Register8Bit instructionRegister{backingInstructionRegister};
    Register8Bit interruptEnableRegister{backingInterruptEnableRegister};

    Register16Bit afRegister{backingAfRegister, 0xFFF0_u16};
    Register8Bit accumulator{afRegister.UpperByteAsRegister()};
    Register8Bit flags{afRegister.LowerByteAsRegister()};

    Register16Bit bcRegister{backingBcRegister};
    Register8Bit bRegister = bcRegister.UpperByteAsRegister();
    Register8Bit cRegister = bcRegister.LowerByteAsRegister();

    Register16Bit deRegister{backingDeRegister};
    Register8Bit dRegister = deRegister.UpperByteAsRegister();
    Register8Bit eRegister = deRegister.LowerByteAsRegister();

    Register16Bit hlRegister{backingHlRegister};
    Register8Bit hRegister = hlRegister.UpperByteAsRegister();
    Register8Bit lRegister = hlRegister.LowerByteAsRegister();

    Register16Bit wzRegister{backingWzRegister};
    Register8Bit wRegister = wzRegister.UpperByteAsRegister();
    Register8Bit zRegister = wzRegister.LowerByteAsRegister();

    Register16Bit programCounter{backingProgramCounterRegister};
    Register16Bit stackPointer{backingStackPointerRegister};

private:
    std::byte backingInstructionRegister{};
    std::byte backingInterruptEnableRegister{};
    uint16_t backingAfRegister{};
    uint16_t backingBcRegister{};
    uint16_t backingDeRegister{};
    uint16_t backingHlRegister{};
    uint16_t backingWzRegister{};
    uint16_t backingProgramCounterRegister{};
    uint16_t backingStackPointerRegister{};
};

} // namespace EmulatorLib