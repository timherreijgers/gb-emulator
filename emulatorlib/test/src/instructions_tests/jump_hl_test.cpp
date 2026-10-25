/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class JumpHlTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xE9_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(JumpHlTest, ExecutingOpCode)
{
    m_cpu.Registers().hlRegister = 0x4554;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xE9_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().programCounter, ::testing::Eq(0x4555));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test