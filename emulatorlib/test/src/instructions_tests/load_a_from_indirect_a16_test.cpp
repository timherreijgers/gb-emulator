/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadAFromIndirectA16Test : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xFA_b, 0xBB_b, 0xAA_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadAFromIndirectA16Test, ExecutingOpCode)
{
    const auto initialValue = m_cpu.Registers().accumulator.value;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xFA_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xBB_b));

    EXPECT_CALL(m_addressableMock, ReadFromAddress(0xAABB)).Times(1).WillOnce(::testing::Return(0x0F_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xBB_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0xAA_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x0F_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x0F_b));
}

} // namespace EmulatorLib::Test