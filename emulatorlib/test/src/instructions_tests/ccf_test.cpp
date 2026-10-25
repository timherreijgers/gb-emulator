/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "register_function_wrappers.h"

#include "instruction_test_base.h"
#include "instruction_translation.h"

#include "emulatorlib/cpu_flags.h"
#include "instruction_handlers/register_io_helpers.h"

namespace EmulatorLib::Test
{

class CcfTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x3F_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(CcfTest, ExecuteOpCode_FlipsFrom0To1)
{
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x3F_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_F(CcfTest, ExecuteOpCode_FlipsFrom1To0)
{
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x3F_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_F(CcfTest, ExecuteOpCode_OnlyFlipsCarryAndClearSubtractAndHalfCarry)
{
    m_cpu.Registers().flags = (CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry | CpuFlags::Zero).AsByte();

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::Zero).AsByte()));
}

} // namespace EmulatorLib::Test
