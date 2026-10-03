/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class DiTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xF3_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(DiTest, DisablesMasterInterruptFlag)
{
    m_cpu.Registers().masterInterruptState = MasterInterruptState::ENABLED;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF3_b));
    ASSERT_THAT(m_cpu.Registers().masterInterruptState, ::testing::Eq(MasterInterruptState::ENABLED));


    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(00_b));
    ASSERT_THAT(m_cpu.Registers().masterInterruptState, ::testing::Eq(MasterInterruptState::DISABLED));
}

TEST_F(DiTest, ExecutingOpCode_CancelsEffectOfEiInstruction)
{
    m_program.WriteProgram({0xFB_b, 0xF3_b});

    m_cpu.Registers().masterInterruptState = MasterInterruptState::DISABLED;

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().masterInterruptState, ::testing::Eq(MasterInterruptState::DISABLED));
}

} // namespace EmulatorLib::Test