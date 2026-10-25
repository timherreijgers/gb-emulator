/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadIndirectA16FromATest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xEA_b, 0xAA_b, 0xBB_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadIndirectA16FromATest, ExecutingOpCode)
{
    m_cpu.Registers().accumulator = 0x0F_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xEA_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xAA_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xAA_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0xBB_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xBBAA, 0x0F_b)).Times(1);
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test