/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "utilitylib/byte_utils.h"

namespace EmulatorLib::Test
{

class JumpA16Test : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xC3_b, 0x50_b, 0x040_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(JumpA16Test, ExecutingOpCode)
{
    constexpr auto opCode = 0xC3_b;

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

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x4050));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(opCode));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x4051));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test