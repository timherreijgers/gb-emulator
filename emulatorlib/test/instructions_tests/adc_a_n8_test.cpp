/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/instruction_translation.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class AdcNTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(AdcNTest, ExecutingOpCode)
{
    m_program.WriteProgram({0xCE_b, 0x05_b});

    m_cpu.Registers().accumulator = 0x02_b;
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xCE_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x05_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x07_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_F(AdcNTest, ExecutingOpCode_WithCarryFlag)
{
    m_program.WriteProgram({0xCE_b, 0x05_b});

    m_cpu.Registers().accumulator = 0x02_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x08_b));
}

TEST_F(AdcNTest, ExecutingOpCode_ClearsSubtractionFlag)
{
    m_program.WriteProgram({0xCE_b, 0x05_b});

    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte();
    m_cpu.Registers().accumulator = 0x02_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_F(AdcNTest, ExecutingOpCode_SetsZeroFlagTo1)
{
    m_program.WriteProgram({0xCE_b, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_F(AdcNTest, ExecutingOpCode_SetsZeroFlagTo0)
{
    m_program.WriteProgram({0xCE_b, 0x03_b});

    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();
    m_cpu.Registers().accumulator = 0x01_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_F(AdcNTest, ExecutingOpCode_SetsHalfCarryTo0)
{
    m_program.WriteProgram({0xCE_b, 0x02_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    m_cpu.Registers().accumulator = 0x01_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_F(AdcNTest, ExecutingOpCode_SetsHalfCarryTo1)
{
    m_program.WriteProgram({0xCE_b, 0x01_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x0F_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AdcNTest, ExecutingOpCode_CarryInSetsHalfCarry)
{
    m_program.WriteProgram({0xCE_b, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0x0F_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x10_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AdcNTest, ExecutingOpCode_CarryInSetsZeroHalfCarryAndCarry)
{
    m_program.WriteProgram({0xCE_b, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0xFF_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Zero.AsByte() | CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));
}

TEST_F(AdcNTest, ExecutingOpCode_CarryInPreservesHalfCarryAndCarry)
{
    m_program.WriteProgram({0xCE_b, 0xFF_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0xFF_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xFF_b));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));
}

TEST_F(AdcNTest, ExecutingOpCode_SetsCarryTo0)
{
    m_program.WriteProgram({0xCE_b, 0x10_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0x10_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags.value, ::testing::Eq(0x00_b));
}

TEST_F(AdcNTest, ExecutingOpCode_SetsCarryTo1)
{
    m_program.WriteProgram({0xCE_b, 0x20_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0xF0_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

} // namespace EmulatorLib::Test
