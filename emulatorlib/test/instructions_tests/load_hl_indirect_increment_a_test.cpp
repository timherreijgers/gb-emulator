/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadHlIndirectIncrementATest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x22_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadHlIndirectIncrementATest, ExecuteOpCode_ExecutesCorrectly)
{
    constexpr auto instruction = 0x22_b;
    m_cpu.Registers().hlRegister = 0x1234;
    m_cpu.Registers().accumulator = 0xFF_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x1234, 0xFF_b)).Times(1);
    m_cpu.Step();
    ASSERT_EQ(m_cpu.Registers().hlRegister, 0x1235);

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test