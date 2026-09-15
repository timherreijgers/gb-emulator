/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/instruction_translation.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class AndATest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(AndATest, ExecutingOpCode)
{
    m_program.WriteProgram({0xA7_b, 0x00_b});

    m_cpu.Registers().accumulator = 0xF0_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xA7_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xF0_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xF0_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_F(AndATest, ExecutingOpCode_ClearsSubtractionFlag)
{
    m_program.WriteProgram({0xA7_b, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte();
    m_cpu.Registers().accumulator = 0xFF_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AndATest, ExecutingOpCode_SetsHalfCarry)
{
    m_program.WriteProgram({0xA7_b, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x08_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AndATest, ExecutingOpCode_ClearsHalfCarry)
{
    m_program.WriteProgram({0xA7_b, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    m_cpu.Registers().accumulator = 0xF0_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AndATest, ExecutingOpCode_ClearsCarryFlag)
{
    m_program.WriteProgram({0xA7_b, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0xF0_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AndATest, ExecutingOpCode_SetsZeroFlag)
{
    m_program.WriteProgram({0xA7_b, 0x00_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Zero.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AndATest, ExecutingOpCode_ClearsZeroFlag)
{
    m_program.WriteProgram({0xA7_b, 0x00_b});

    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();
    m_cpu.Registers().accumulator = 0xFF_b;

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

} // namespace EmulatorLib::Test
