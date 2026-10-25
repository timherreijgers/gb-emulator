/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadIndirectA8FromATest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xE0_b, 0xAA_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadIndirectA8FromATest, ExecutingOpCode)
{
    m_cpu.Registers().accumulator = 0xEA_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xE0_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xAA_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFFAA, 0xEA_b)).Times(1);
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test