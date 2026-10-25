/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class RetTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xC9_b});
        m_cpu.Registers().stackPointer = 0xFFF0;
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(RetTest, ExecutingOpCode)
{
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xC9_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xFFF0)).WillOnce(::testing::Return(0x01_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFF1));
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x01_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xFFF1)).WillOnce(::testing::Return(0x02_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xFFF2));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x02_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x0201));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test