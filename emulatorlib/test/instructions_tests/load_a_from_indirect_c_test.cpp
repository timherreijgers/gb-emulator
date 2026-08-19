/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadAFromIndirectCTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xF2_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadAFromIndirectCTest, ExecutingOpCode)
{
    const auto initialValue = m_cpu.Registers().accumulator.value;
    m_cpu.Registers().cRegister = 0x77_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF2_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xFF77)).Times(1).WillOnce(::testing::Return(0x4A_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x4A_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x4A_b));
}

} // namespace EmulatorLib::Test