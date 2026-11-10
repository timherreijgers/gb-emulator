/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "emulatorlib/cpu_flags.h"

namespace EmulatorLib::Test
{

class AddHLSPTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x39_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(AddHLSPTest, ExecutingOpCode_AddsStackPointerToHlAndPreservesZeroFlag)
{
    m_cpu.Registers().hlRegister = 0x0FFF;
    m_cpu.Registers().stackPointer = 0x0001;
    m_cpu.Registers().flags = CpuFlags::Zero.AsByte();

    m_cpu.Step();
    m_cpu.Step();
    m_cpu.Step();

    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(0x1000));
    ASSERT_THAT(m_cpu.Registers().flags, ::testing::Eq(CpuFlags::Zero.AsByte() | CpuFlags::HalfCarry.AsByte()));
}

} // namespace EmulatorLib::Test
