/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class AndAN8Test : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(AndAN8Test, ExecutingOpCode)
{
    m_program.WriteProgram({0xE6_b, 0x0F_b});
    m_cpu.Registers().accumulator = 0xF3_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xE6_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x0F_b));
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x03_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AndAN8Test, SetsZeroAndHalfCarryFlagsAndClearsOtherFlags)
{
    m_program.WriteProgram({0xE6_b, 0x0F_b});
    m_cpu.Registers().accumulator = 0xF0_b;
    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte() | CpuFlags::Carry.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Zero.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

} // namespace EmulatorLib::Test