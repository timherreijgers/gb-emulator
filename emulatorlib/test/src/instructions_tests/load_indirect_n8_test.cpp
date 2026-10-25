/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadIndirectN8Test : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x36_b, 0xEF_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadIndirectN8Test, ExecutingOpCode)
{
    m_cpu.Registers().hlRegister = 0x3456;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x36_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xEF_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x3456, 0xEF_b)).Times(1);
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test