/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class SbcAFromIndirectHlTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(SbcAFromIndirectHlTest, ExecutingOpCode)
{
    m_program.WriteProgram({0x9E_b});

    m_cpu.Registers().accumulator = 0x05_b;
    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x9E_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x05_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).WillOnce(testing::Return(0x03_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x05_b));
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x03_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x02_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SbcAFromIndirectHlTest, ExecutingOpCode_WithCarryFlag)
{
    m_program.WriteProgram({0x9E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).WillOnce(testing::Return(0x03_b));

    m_cpu.Registers().accumulator = 0x05_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x01_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SbcAFromIndirectHlTest, ExecutingOpCode_SetsZeroFlag)
{
    m_program.WriteProgram({0x9E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).WillOnce(testing::Return(0x05_b));

    m_cpu.Registers().accumulator = 0x05_b;
    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::Zero.AsByte()));
}

TEST_F(SbcAFromIndirectHlTest, ExecutingOpCode_ClearsZeroFlag)
{
    m_program.WriteProgram({0x9E_b});

    m_cpu.Registers().accumulator = 0x01_b;
    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SbcAFromIndirectHlTest, ExecutingOpCode_SetsHalfCarry)
{
    m_program.WriteProgram({0x9E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).WillOnce(testing::Return(0x01_b));

    m_cpu.Registers().accumulator = 0x10_b;
    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

TEST_F(SbcAFromIndirectHlTest, ExecutingOpCode_CarryInSetsHalfCarry)
{
    m_program.WriteProgram({0x9E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).WillOnce(testing::Return(0x00_b));

    m_cpu.Registers().accumulator = 0x10_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x0F_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

TEST_F(SbcAFromIndirectHlTest, ExecutingOpCode_ClearsHalfCarryAndCarryFlags)
{
    m_program.WriteProgram({0x9E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).WillOnce(testing::Return(0x10_b));

    m_cpu.Registers().accumulator = 0x22_b;
    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte();
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SbcAFromIndirectHlTest, ExecutingOpCode_SetsCarry)
{
    m_program.WriteProgram({0x9E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).WillOnce(testing::Return(0x20_b));

    m_cpu.Registers().accumulator = 0x10_b;
    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::Carry.AsByte()));
}

TEST_F(SbcAFromIndirectHlTest, ExecutingOpCode_CarryInSetsHalfCarryAndCarry)
{
    m_program.WriteProgram({0x9E_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).WillOnce(testing::Return(0x00_b));

    m_cpu.Registers().accumulator = 0x00_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xFF_b));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));
}

} // namespace EmulatorLib::Test
