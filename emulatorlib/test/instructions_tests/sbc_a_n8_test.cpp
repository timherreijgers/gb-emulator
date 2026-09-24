/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu_flags.h"
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class SbcAN8Test : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(SbcAN8Test, ExecutingOpCode)
{
    m_program.WriteProgram({0xDE_b, 0x03_b});
    m_cpu.Registers().accumulator = 0x05_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xDE_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x03_b));
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x02_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SbcAN8Test, SubtractsCarryIn)
{
    m_program.WriteProgram({0xDE_b, 0x03_b});
    m_cpu.Registers().accumulator = 0x05_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x01_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte()));
}

TEST_F(SbcAN8Test, CarryInSetsHalfCarryAndCarryFlags)
{
    m_program.WriteProgram({0xDE_b, 0x00_b});
    m_cpu.Registers().accumulator = 0x00_b;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0xFF_b));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));
}

} // namespace EmulatorLib::Test