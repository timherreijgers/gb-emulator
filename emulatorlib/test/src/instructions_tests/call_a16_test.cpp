/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "utilitylib/byte_utils.h"

namespace EmulatorLib::Test
{

class CallA16Test : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xCD_b, 0x50_b, 0x040_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(CallA16Test, ExecutingOpCode)
{
    constexpr auto opCode = 0xCD_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(opCode));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x50_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(opCode));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x50_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x40_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(opCode));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFE));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFD));
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x50_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x40_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(opCode));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFFD, 0x01_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFFC));
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x50_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x40_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(opCode));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFFC, 0x03_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x4050));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(opCode));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x4051));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test