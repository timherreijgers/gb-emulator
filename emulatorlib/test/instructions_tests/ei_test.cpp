/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class EiTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xFB_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(EiTest, EnablesMasterInterruptFlag_AfterNextInstruction)
{
    m_cpu.Registers().masterInterruptState = MasterInterruptState::DISABLED;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xFB_b));
    ASSERT_THAT(m_cpu.Registers().masterInterruptState, ::testing::Eq(MasterInterruptState::DISABLED));


    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(00_b));
    ASSERT_THAT(m_cpu.Registers().masterInterruptState, ::testing::Eq(MasterInterruptState::PENDING));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(00_b));
    ASSERT_THAT(m_cpu.Registers().masterInterruptState, ::testing::Eq(MasterInterruptState::ENABLED));
}

TEST_F(EiTest, EnablesMasterInterruptFlag_AfterNextMCylce_WithMultiCycleInstruction)
{
    m_program.WriteProgram({0xFB_b, 0x06_b, 0xFF_b});

    m_cpu.Registers().masterInterruptState = MasterInterruptState::DISABLED;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xFB_b));
    ASSERT_THAT(m_cpu.Registers().masterInterruptState, ::testing::Eq(MasterInterruptState::DISABLED));


    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().masterInterruptState, ::testing::Eq(MasterInterruptState::PENDING));


    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().masterInterruptState, ::testing::Eq(MasterInterruptState::ENABLED));
}

} // namespace EmulatorLib::Test