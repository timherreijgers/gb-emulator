/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"
#include "instruction_translation.h"

#include "emulatorlib/cpu_flags.h"

namespace EmulatorLib::Test
{

class AddSpE8Test : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xE8_b, 0xF0_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(AddSpE8Test, ExecutingOpcode_AddsSignedOffsetAndClearsFlags)
{
    m_cpu.Registers().stackPointer = 0x0008;
    m_cpu.Registers().flags = (CpuFlags::Zero | CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry).AsByte();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xE8_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0101));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0x0008));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xE8_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0102));
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xF0_b));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0x0008));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xE8_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0102));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0x0008));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0103));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFF8));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
}

TEST_F(AddSpE8Test, ExecutingOpcode_AddsPositiveOffsetAcrossSixteenBitWraparound)
{
    m_program.WriteProgram({0xE8_b, 0x7F_b, 0x00_b});
    m_cpu.Registers().stackPointer = 0xFF81;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0x0000));
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::HalfCarry.AsByte() | CpuFlags::Carry.AsByte()));
}

} // namespace EmulatorLib::Test
