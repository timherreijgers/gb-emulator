/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/cpu_registers.h"
#include "instruction_handlers/flag_helpers.h"

#include <gmock/gmock.h>

namespace EmulatorLib::Test
{

TEST(FlagHelpersTest, ApplyingAdditionFlags_SetsZeroHalfCarryAndCarryAndClearsSubtract)
{
    CpuRegisters registers;
    registers.flags = CpuFlags::Subtract.AsByte();

    ApplyAdditionFlags(registers, 0x00_b, 0x88_b);

    ASSERT_THAT(registers.flags,
                ::testing::Eq(CpuFlags::Zero.AsByte() | CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));
}

} // namespace EmulatorLib::Test