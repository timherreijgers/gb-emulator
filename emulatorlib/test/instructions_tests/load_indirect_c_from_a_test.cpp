/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadIndirectCTestFromA : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xE2_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadIndirectCTestFromA, ExecutingOpCode)
{
    m_cpu.Registers().accumulator = 0x88_b;
    m_cpu.Registers().cRegister = 0x66_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xE2_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0xFF66, 0x88_b)).Times(1);
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test