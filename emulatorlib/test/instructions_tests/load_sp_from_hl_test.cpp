/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadSpFromHlTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xF9_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadSpFromHlTest, ExecutingOpCode)
{
    m_cpu.Registers().hlRegister = 0xAABB;
    m_cpu.Registers().stackPointer = 0x0000;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF9_b));
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0x00));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().stackPointer, ::testing::Eq(0xAABB));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF9_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test