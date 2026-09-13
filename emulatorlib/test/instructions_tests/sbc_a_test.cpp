/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class SbcATest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(SbcATest, ExecutingOpCode)
{
    m_program.WriteProgram({0x9F_b, 0x00_b});

    m_cpu.Registers().accumulator = 0x02_b;
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x9F_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x02_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_F(SbcATest, ExecutingOpCode_WithCarryFlag)
{
    m_program.WriteProgram({0x9F_b, 0x00_b});

    m_cpu.Registers().accumulator = 0x02_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xFF_b));
}

TEST_F(SbcATest, ExecutingOpCode_SetsZeroAndSubtractionFlags)
{
    m_program.WriteProgram({0x9F_b, 0x00_b});

    m_cpu.Registers().accumulator = 0x42_b;
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::Zero.AsByte()));
}

TEST_F(SbcATest, ExecutingOpCode_CarryInSetsHalfCarryAndCarry)
{
    m_program.WriteProgram({0x9F_b, 0x00_b});

    m_cpu.Registers().accumulator = 0x00_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xFF_b));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));
}

} // namespace EmulatorLib::Test
