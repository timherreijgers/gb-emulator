/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadAFromIndirectA8Test : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xF0_b, 0xAA_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadAFromIndirectA8Test, ExecutingOpCode)
{
    const auto initialValue = m_cpu.Registers().accumulator.value;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF0_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xAA_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xFFAA)).Times(1).WillOnce(::testing::Return(0x88_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x88_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x88_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test