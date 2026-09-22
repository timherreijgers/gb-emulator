/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/instruction_translation.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class OrAIndirectHlTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(OrAIndirectHlTest, ExecutingOpCode)
{
    m_program.WriteProgram({0xB6_b});

    m_cpu.Registers().accumulator = 0xF0_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xB6_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xF0_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x0F_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xF0_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xFF_b));
}

TEST_F(OrAIndirectHlTest, ExecutingOpCode_ClearsSubtractionFlag)
{
    m_program.WriteProgram({0xB6_b});

    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte();
    m_cpu.Registers().accumulator = 0xFF_b;
    m_cpu.Registers().hlRegister = 0x8080;

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x01_b));
    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_F(OrAIndirectHlTest, ExecutingOpCode_SetsHalfCarry)
{
    m_program.WriteProgram({0xB6_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    m_cpu.Registers().accumulator = 0x00_b;
    m_cpu.Registers().hlRegister = 0x8080;

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x00_b));
    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_F(OrAIndirectHlTest, ExecutingOpCode_ClearsCarryFlag)
{
    m_program.WriteProgram({0xB6_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0xF0_b;
    m_cpu.Registers().hlRegister = 0x8080;

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x0F_b));
    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_F(OrAIndirectHlTest, ExecutingOpCode_SetsZeroFlag)
{
    m_program.WriteProgram({0xB6_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x00_b;
    m_cpu.Registers().hlRegister = 0x8080;

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x00_b));
    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_F(OrAIndirectHlTest, ExecutingOpCode_ClearsZeroFlag)
{
    m_program.WriteProgram({0xB6_b});

    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();
    m_cpu.Registers().accumulator = 0xFF_b;
    m_cpu.Registers().hlRegister = 0x8080;

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0xFF_b));
    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test
