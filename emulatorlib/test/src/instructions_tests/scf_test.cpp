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

class ScfTest : public InstructionTestBase, public ::testing::WithParamInterface<std::byte>
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x37_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(ScfTest, ExecutingOpcode)
{
    m_cpu.Registers().flags = GetParam();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x37_b));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Carry.AsByte()));
}

TEST_P(ScfTest, ExecutingOpcode_LeavesZeroWhenSet)
{
    m_cpu.Registers().flags = CpuFlags::Zero.AsByte() | GetParam();

    m_cpu.Step();
    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq((CpuFlags::Zero | CpuFlags::Carry).AsByte()));
}


INSTANTIATE_TEST_SUITE_P(ScfTest, ScfTest,
                         ::testing::Values(
                             0x00_b,
                             CpuFlags::Carry.AsByte(),
                             (CpuFlags::HalfCarry | CpuFlags::Carry | CpuFlags::Subtract).AsByte(),
                             (CpuFlags::HalfCarry | CpuFlags::Subtract).AsByte()));

} // namespace EmulatorLib::Test
