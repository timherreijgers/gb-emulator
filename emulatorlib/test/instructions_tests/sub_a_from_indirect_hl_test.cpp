/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/instruction_translation.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class SubAIndirectTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(SubAIndirectTest, ExecutingOpCode)
{
    m_program.WriteProgram({0x96_b});

    m_cpu.Registers().accumulator = 0x05_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x96_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x05_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x03_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x05_b));
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x03_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x02_b));
}

TEST_F(SubAIndirectTest, ExecutingOpCode_SetsSubtractionFlag)
{
    m_program.WriteProgram({0x96_b});

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x05_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SubAIndirectTest, ExecutingOpCode_SetsZeroFlagTo1)
{
    m_program.WriteProgram({0x96_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x05_b));

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x05_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::Zero.AsByte()));
}

TEST_F(SubAIndirectTest, ExecutingOpCode_SetsZeroFlagTo0)
{
    m_program.WriteProgram({0x96_b});

    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();
    m_cpu.Registers().accumulator = 0x05_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SubAIndirectTest, ExecutingOpCode_SetsHalfCarryTo0)
{
    m_program.WriteProgram({0x96_b});

    m_cpu.Registers().flags = CpuFlags::HalfCarry.AsByte();
    m_cpu.Registers().accumulator = 0x0F_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SubAIndirectTest, ExecutingOpCode_SetsHalfCarryTo1)
{
    m_program.WriteProgram({0x96_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x01_b));

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x10_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

TEST_F(SubAIndirectTest, ExecutingOpCode_SetsCarryTo0)
{
    m_program.WriteProgram({0x96_b});

    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();
    m_cpu.Registers().accumulator = 0x20_b;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SubAIndirectTest, ExecutingOpCode_SetsCarryTo1)
{
    m_program.WriteProgram({0x96_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x20_b));

    m_cpu.Registers().flags = 0x00_b;
    m_cpu.Registers().accumulator = 0x10_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::Carry.AsByte()));
}

TEST_F(SubAIndirectTest, ExecutingOpCode_SetsHalfCarryAndCarry)
{
    m_program.WriteProgram({0x96_b});
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x8080)).Times(1).WillOnce(testing::Return(0x01_b));

    m_cpu.Registers().accumulator = 0x00_b;
    m_cpu.Registers().hlRegister = 0x8080;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xFF_b));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));
}

} // namespace EmulatorLib::Test
