/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"

#include "emulatorlib/cpu_flags.h"

namespace EmulatorLib::Test
{

class DecIndirectHLTest : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0x35_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(DecIndirectHLTest, ExecutingOpcode_DecrementsMemoryAndUpdatesFlags)
{
    constexpr auto address = static_cast<uint16_t>(0x1234);
    m_cpu.Registers().hlRegister = address;
    m_cpu.Registers().flags = CpuFlags::Carry.AsByte() | CpuFlags::Zero.AsByte();
    EXPECT_CALL(m_addressableMock, ReadFromAddress(address)).WillOnce(::testing::Return(0x00_b));
    EXPECT_CALL(m_addressableMock, WriteToAddress(address, 0xFF_b)).Times(1);

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x35_b));
    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(address));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(address));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().flags,
                ::testing::Eq(CpuFlags::Carry.AsByte() | CpuFlags::Subtract.AsByte() | CpuFlags::HalfCarry.AsByte()));
    ASSERT_THAT(m_cpu.Registers().hlRegister, ::testing::Eq(address));
}

} // namespace EmulatorLib::Test
