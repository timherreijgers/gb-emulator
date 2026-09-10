/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/instruction_translation.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class AdcAFromIndirectHlTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode)
{
    m_program.WriteProgram({0x8E_b});

    m_cpu.Registers().accumulator = 0x02_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x8E_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x02_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0xAB_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x02_b));
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xAB_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xAD_b));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_WithCarryFlag)
{
    m_program.WriteProgram({0x8E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x05_b));

    m_cpu.Registers().accumulator = 0x02_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x08_b));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_ClearsSubtractionFlag)
{
    m_program.WriteProgram({0x8E_b});

    m_cpu.Registers().flags = CpuFlags::Subtract.AsByte();
    m_cpu.Registers().accumulator = 0x01_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_SetsZeroFlagTo1)
{
    m_program.WriteProgram({0x8E_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x00_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte()));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_SetsZeroFlagTo0)
{
    m_program.WriteProgram({0x8E_b});

    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();
    m_cpu.Registers().accumulator = 0x01_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_SetsHalfCarryTo0)
{
    m_program.WriteProgram({0x8E_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    m_cpu.Registers().accumulator = 0x01_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_SetsHalfCarryTo1)
{
    m_program.WriteProgram({0x8E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x08_b));

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x08_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags.value, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_CarryInSetsHalfCarry)
{
    m_program.WriteProgram({0x8E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x08_b));

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0x08_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x11_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::HalfCarry.AsByte()));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_SetsCarryTo0)
{
    m_program.WriteProgram({0x8E_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x40_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags.value, ::testing::Eq(0x00_b));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_SetsCarryTo1)
{
    m_program.WriteProgram({0x8E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x80_b));

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x80_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags.value, ::testing::Eq(CpuFlags::Zero.AsByte() | CpuFlags::Carry.AsByte()));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_SetsCarryTo1_WithCarryFlag)
{
    m_program.WriteProgram({0x8E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x80_b));

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0x80_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags.value, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

TEST_F(AdcAFromIndirectHlTest, ExecutingOpCode_CarryInSetsCarry)
{
    m_program.WriteProgram({0x8E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x80_b));

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0x80_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x01_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

} // namespace EmulatorLib::Test
