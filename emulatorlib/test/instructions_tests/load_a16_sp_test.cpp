/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "utilitylib/byte_utils.h"

namespace EmulatorLib::Test
{

class LoadA16SPTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x08_b, 0x33_b, 0x44_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadA16SPTest, ExecutingCommand_WithValueToLoadAs0x1234_BehavesCorrectly)
{
    constexpr auto opCode = 0x08_b;

    // TODO: Registers getter should be const. If we validate that LoadN16SP works, we can use that to load data into the stack pointer.
    // For this we first need to make sure that we have a better way of creating "programs" than setting up mocks
    m_cpu.Registers().stackPointer = 0x1234;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(opCode));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x33_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x00_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x33_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x44_b));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x4433, 0x34_b)).Times(1);
    m_cpu.Step();

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x4434, 0x12_b)).Times(1);
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

} // namespace EmulatorLib::Test