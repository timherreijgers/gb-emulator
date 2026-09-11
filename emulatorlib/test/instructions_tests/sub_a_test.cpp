/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/instruction_translation.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class SubATest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(SubATest, ExecutingOpCode)
{
    m_program.WriteProgram({0x97_b, 0x00_b});

    m_cpu.Registers().accumulator = 0x02_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x97_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x02_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_F(SubATest, ExecutingOpCode_SetsSubtractionFlag)
{
    m_program.WriteProgram({0x97_b, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x01_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::Zero.AsByte()));
}

TEST_F(SubATest, ExecutingOpCode_SetsZeroFlagTo1)
{
    m_program.WriteProgram({0x97_b, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x42_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::Zero.AsByte()));
}

TEST_F(SubATest, ExecutingOpCode_SetsHalfCarryTo0)
{
    m_program.WriteProgram({0x97_b, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    m_cpu.Registers().accumulator = 0x08_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::Zero.AsByte()));
}

TEST_F(SubATest, ExecutingOpCode_SetsCarryTo0)
{
    m_program.WriteProgram({0x97_b, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0x80_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::Zero.AsByte()));
}

} // namespace EmulatorLib::Test
