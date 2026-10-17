/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "register_function_wrappers.h"

#include "instruction_test_base.h"
#include "instruction_translation.h"

import emulatorlib.cpu;

namespace EmulatorLib::Test
{

class CplTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x2F_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(CplTest, ExecuteOpCode)
{
    m_cpu.Registers().accumulator = 0xF0_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x2F_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(0x0F_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_F(CplTest, ExecuteOpCode_SetsCorrectFlags)
{
    m_cpu.Registers().flags = 0x00_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(0x00_b));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

TEST_F(CplTest, ExecuteOpCode_ClearsCorrectFlags_LeavesCarryAndZeroSet)
{
    const auto initialFlags = (CpuFlags::Carry | CpuFlags::Zero).AsByte();

    m_cpu.Registers().flags = initialFlags;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(initialFlags));
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::Subtract | CpuFlags::HalfCarry | CpuFlags::Carry | CpuFlags::Zero).AsByte()));
}

} // namespace EmulatorLib::Test
